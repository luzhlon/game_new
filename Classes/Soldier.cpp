#include "Soldier.h"
#include "Dialog.h"
#include "World.h"
#include "ManSoldier.h"
#include "Man2Soldier.h"
#include "WomanSoldier.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Soldier *Soldier::s_followed = nullptr;
extern World *g_world;
extern SimpleAudioEngine *g_audio;

Soldier *Soldier::create(int type_id) {
    switch (type_id) {
    case SOLDIER_TYPE_WOMAN:
        return WomanSoldier::create();
        break;
    case SOLDIER_TYPE_MAN:
        return ManSoldier::create();
        break;
    case SOLDIER_TYPE_MAN2:
        return Man2Soldier::create();
        break;
    }
}

void g_play_effect(char *file) {
    char buf[32];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sprintf(buf, "%s.%s", file, "wav");
#else
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sprintf(buf, "%s.%s", file, "ogg");
#endif
    g_audio->playEffect(buf);
}

bool Soldier::do_skill(Skill::Type skill) {
    move_stop();

    switch (skill) {
    case Skill::SKILL_BOXING:
        stopAllActions();
        g_play_effect(m_eff_boxing);
        runAction(m_act_boxing);
        _state = SOLDIER_STATE_SKILL;
        break;
    case Skill::SKILL_KICK:
        stopAllActions();
        g_play_effect(m_eff_kick);
        runAction(m_act_kick);
        _state = SOLDIER_STATE_SKILL;
        break;
    case Skill::SKILL_SPECIAL:
        stopAllActions();
        g_play_effect(m_eff_special);
        runAction(m_act_special);
        _state = SOLDIER_STATE_SKILL;
        break;
    case Skill::SKILL_SPEED:
        _buff = BUFF_SPEED;
        move_by(80.f);
        break;
    }

    return true;
}

Soldier *Soldier::create(room_member *rm) {
    auto sol = create(rm->m_role_id);
    sol->_room_member = rm;
    sol->begin_fight();
    return sol;
}

bool Soldier::init() {
    init_soldier();

    CC_ASSERT(_role_id >= 0);

    setRotation3D(Vec3(180.f, _base_angle + 90.f, 180.f));

    m_act_walk = RepeatForever::create(m_act_walk);
    m_act_idle = RepeatForever::create(m_act_idle);

	m_act_kick->retain();
	m_act_boxing->retain();
	m_act_special->retain();
	m_act_walk->retain();
	m_act_idle->retain();

    setPosition3D(Vec3::ZERO);

    return true;
}

bool Soldier::load_config(char *path) {
    auto str = FileUtils::getInstance()->getStringFromFile(path);
    if (str.empty()) return false;

    String content(str);
    Array *lines = content.componentsSeparatedByString("\n");

    Ref *obj;
    int count = lines->count();
    int cur_i = 0;

    // 加载人物
    char role_file[256];
    float scale, base_angle;
    for (; cur_i < count; cur_i++) {
        obj = lines->getObjectAtIndex(cur_i);
        String *line = (String *)obj;
        if (3 == sscanf(line->_string.c_str(), "%s%f%f", role_file, &scale, &base_angle))
            break;
    }
    CCARRAY_FOREACH(lines, obj) {
        String *line = (String *)obj;
        if (3 == sscanf(line->_string.c_str(), "%s%f%f", role_file, &scale, &base_angle))
            break;
    }
    initWithFile(role_file);
    setScale(scale);
    _base_angle = base_angle;
    setRotation3D(Vec3(180.f, _base_angle, 180.f));

    ActionInterval **anim[5] = {
        &m_act_walk,
        &m_act_boxing,
        &m_act_kick,
        &m_act_special,
        &m_act_idle
    };
    // 加载动作
    char act_file[256]; 
    int start_frame, end_frame;
    float fps;
    for (int i = 0; cur_i < count && i < 5; cur_i++) {
        obj = lines->getObjectAtIndex(cur_i);
        String *line = (String *)obj;
        if (4 == sscanf(line->_string.c_str(), "%s%d%d%f", act_file, &start_frame, &end_frame, &fps)) {
            *anim[i] = Animate3D::createWithFrames(Animation3D::create(act_file), start_frame, end_frame, fps);
            i++;
        }
    }

	return true;
}

void Soldier::set_blood(float blood) {
    _blood = blood;
    if (_blood > _full_blood) _blood = _full_blood;
    if (_blood < 0.f) _blood = 0.f;

    if (0.f == _blood) {
        if (_on_death) _on_death(this);
    }

    blood_bar()->setPercent(_blood / _full_blood * 100.f);
    // 血量条淡出
    auto frame = blood_bar_frame();
    frame->setOpacity(255);
    frame->runAction(FadeOut::create(1.f));
}

void Soldier::load_ui() {
    _billboard = BillBoard::create(BillBoard::Mode::VIEW_PLANE_ORIENTED);

    auto node = CSLoader::createNode("soldier_head_info.csb");
    auto layout = static_cast<Layout *>(node->getChildByTag(1));
    layout->removeFromParent();
    layout->setPositionY(10.f);
    _billboard->addChild(layout);

    _billboard->setPosition3D(Vec3(0.f, 270.f, 0.f));
    add_thing(_billboard);

    blood_bar()->setPercent(_blood * 100.f / _full_blood);
    blood_bar_frame()->setOpacity(0);
    blood_dec_text()->setVisible(false);
}

void Soldier::begin_fight() {
    load_ui();
    scheduleUpdate();
}

//#define AT_STATE(_state) if(atState(_state))
#define AT_STATE(STATE) if(this->_state == STATE)

void Soldier::update(float dt) {
    auto world = World::getInstance();
    auto terrain = world->terrain();

    do {
    AT_STATE(SOLDIER_STATE_MOVE) {
        // 判断有没有超过目标点
        auto pos = getPosition3D();
        auto delta = _target_point - Vec2(pos.x, pos.z);
        struct _angle_quat {         // 量化方向
            int x;
            int y;
            void quatum(Vec2& v) {
                x = v.x < 0.f ? -1 : 1;
                y = v.y < 0.f ? -1 : 1;
            }
        } target, current;
        target.quatum(_target_angle);
        current.quatum(delta);

        if (target.x != current.x ||
            target.y != current.y)
            switch_state(SOLDIER_STATE_IDLE);
        else
            updatePosition(dt);

        break;
    }
    AT_STATE(SOLDIER_STATE_SKILL) {
        if (!getNumberOfRunningActions()) { // 动作已经做完了
            switch_state(SOLDIER_STATE_IDLE);
        }
        break;
    }
    } while (false);

    if (this == s_followed)            // 设置镜头跟随自己
        world->camera_follow(this);
}

void Soldier::updateRotation() {
    Vec3 rota = getRotation3D();
    auto delta = _target_point - Vec2(getPosition3D().x,
                                      getPosition3D().z);
    rota.y = CC_RADIANS_TO_DEGREES(delta.getAngle()) + _base_angle;

    setRotation3D(rota);
}

void Soldier::updatePosition(float dt) {
    auto playerPos = getPosition3D();

    auto delta = _target_point - Vec2(playerPos.x, playerPos.z);
    Vec2 angle(delta.x, delta.y);
    angle.normalize();

    auto spd = _buff == BUFF_SPEED ? 300.f : speed();

    playerPos.x += spd * angle.x * dt;
    playerPos.z += spd * angle.y * dt;

    Vec3 Normal;
    float player_h = World::getInstance()->terrain()->getHeight(playerPos.x, playerPos.z, &Normal);
    // 检测人物是否在地形里
    if (Normal.isZero()) {
        player_h = playerPos.y;
    } else {
        playerPos.y = player_h;
        // 碰撞检测
        if (World::getInstance()->is_collision(playerPos))
            return;
        else
            setPosition3D(playerPos);
    }
}

bool Soldier::on_skill(Skill::Type skill) { // 受到某个技能攻击时
    add_blood(Skill::get_skill(skill)._blood);
    return true;
}

void Soldier::show_blood_decline(float dec) {
    static Vec2 pos_start(0.f, 0.f);

    auto text = blood_dec_text();
    if (pos_start.isZero())       // 获得减血文本框的起始位置
        pos_start = text->getPosition();

    char str[128];
    sprintf(str, "%g", dec);

    text->setVisible(true);
    text->setPosition(pos_start);
    text->setScale(1);
    text->setString(str);
    text->setOpacity(255.f);

    // 减血效果
    auto move = MoveBy::create(1.f, Vec2(0, 100.f));
    auto scale = ScaleBy::create(1.f, 1.5f);
    auto fade = FadeOut::create(1.f);
    text->stopAllActions();
    text->runAction(move);
    text->runAction(scale);
    text->runAction(fade);
}

void Soldier::add_thing(Node *node) {
    node->setCameraMask(getCameraMask());
    addChild(node);
}

void Soldier::add2Billboard(Node *node) {
    node->setCameraMask(_billboard->getCameraMask());
    _billboard->addChild(node);
}

ImageView *Soldier::blood_bar_frame() {
    auto layout = static_cast<Layout *>(_billboard->getChildByTag(1));
    auto frame = static_cast<ImageView *>(Helper::seekWidgetByName(layout, "image_blood"));
    CC_ASSERT(frame);
    return frame;
}

LoadingBar *Soldier::blood_bar() {
    auto layout = static_cast<Layout *>(_billboard->getChildByTag(1));
    auto bar = static_cast<LoadingBar *>(Helper::seekWidgetByName(layout, "loadbar_blood"));
    return bar;
}

Text *Soldier::name_text() {
    auto layout = static_cast<Layout *>(_billboard->getChildByTag(1));
    auto textName = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_name"));
    CC_ASSERT(textName);
    return textName;
}

Text *Soldier::blood_dec_text() {
    auto layout = static_cast<Layout *>(_billboard->getChildByTag(1));
    auto text = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_dec_blood"));
    CC_ASSERT(text);
    return text;
}

void Soldier::switch_state(State state) {
    switch (state) {
    case Soldier::SOLDIER_STATE_MOVE:
    {
        auto cur_pos = getPosition3D();
        _target_angle = _target_point - Vec2(cur_pos.x, cur_pos.z);

        updateRotation();

        if (SOLDIER_STATE_MOVE == this->state())
            break;
        set_state(SOLDIER_STATE_MOVE);

        stopAllActions();
        runAction(m_act_walk);
        break;
    }
    case Soldier::SOLDIER_STATE_IDLE:
    {
        _buff = BUFF_NULL;
        _state = SOLDIER_STATE_IDLE;

        stopAllActions();
        runAction(m_act_idle);
        break;
    }
    case Soldier::SOLDIER_STATE_SKILL:
        _state = SOLDIER_STATE_SKILL;
        break;
    case Soldier::SOLDIER_STATE_DEATH:
        _state = SOLDIER_STATE_DEATH;
        setVisible(false);
        break;
    case SOLDIER_STATE_REVIVE:
        setVisible(true);
        switch_state(SOLDIER_STATE_IDLE);
        break;
    }
}

Vec2 Soldier::get_point(float angle, float distance) {
    auto pos = getPosition3D();
    Vec2 v(distance, 0);
    v.rotate(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angle));

    v += Vec2(pos.x, pos.z);
    return v;
}
