#include "Player.h"
#include "Client.h"
#include "World.h"
#include "GameScene.h"
#include "NetRoom.h"

Soldier::Type Player::s_type = Soldier::SOLDIER_TYPE_WOMAN;

Soldier *g_self = nullptr;

const float Player::s_full_magic = 100.f;

extern Soldier *g_soldiers[MAX_ROOM_MEMBERS];
extern World *g_world;
extern Player *g_player;

Player *Player::getInstance(Soldier *soldier) {
    static Player *player = nullptr;
    if (!player) {
        CC_ASSERT(soldier);
        player = new Player(soldier);
    }
    return player;
}

#define REVIVE_TIME 20.f

extern void g_play_effect(char *file);

Player::Player(Soldier *self) {
    _soldier = self;
    g_self = self;

    self->setCameraFollowed();
    show_circle();

    self->onDeath([this](Soldier *sol) {
        g_play_effect(_soldier->m_eff_death);

        NetRoom::set_state(Soldier::SOLDIER_STATE_DEATH);
        // 复活读条
        //GameScene::Instance->begain_progress(REVIVE_TIME, std::bind(&Player::revive, this));
        GameScene::Instance->begain_progress(REVIVE_TIME, [this](float dt, bool end) {
            if (end)
                revive();
        });
        // 掉1/3的草
        int lost = _soldier->grass() / 3;
        NetRoom::set_grass(_soldier->grass() - lost);
        // 找到一个附近的人加草
        int near_id = NetRoom::get_near_enemy(60.f);
        if (near_id >= 0) {
            NetRoom::set_grass(g_soldiers[near_id]->grass() + lost, near_id);
        }
    });

    schedule(schedule_selector(Player::update_per_second), 1.f);
}

void Player::on_skill(Skill::Type skill) {
    g_play_effect(_soldier->m_eff_aida);
    _soldier->on_skill(skill);
    NetRoom::set_blood(_soldier->blood()); // 在房间里更新自己的血量
}

bool Player::do_skill(Skill::Type skill) {
    auto sk = Skill::get_skill(skill);
    if (g_player->_magic < fabs(sk._magic))
        return false; //魔力不足

    add_magic(sk._magic); // 魔力消耗

    NetRoom::do_skill(skill);
    return true;
}

void Player::show_circle(bool show) {
    static Sprite3D *circle3d = nullptr;
    if (!circle3d) {
        circle3d = Sprite3D::create();
        auto circle = Sprite::create("image/circle.png");

        circle3d->setScale(0.3f);
        circle3d->addChild(circle);
        circle->runAction(RepeatForever::create(RotateBy::create(3.5f, Vec3(0.f, 0.f, 360.f))));

        circle3d->setRotation3D(Vec3(-90.f, 0.f, 0.f));
        auto pos = circle3d->getPosition3D();
        pos.y += 2;
        circle3d->setPosition3D(pos);

        _soldier->add_thing(circle3d);
    }
    circle3d->setVisible(show);
}

void Player::draw_circle(float radius, Color4F& color) {
    if (!_draw) {
        _draw = DrawNode::create();
        _soldier->add_thing(_draw);
    }
    _draw->clear();
    _draw->drawDot(Vec2(0, 0), radius, color);
    auto clr = color;
    clr.a = 0;
    _draw->drawCircle(Vec2(0, 0), radius, CC_DEGREES_TO_RADIANS(90.f), 50, false, clr);
    _draw->setRotation3D(Vec3(90, 0, 0));
    //_draw->setGlobalZOrder(1);
}

void Player::set_magic(float magic) {
    _magic = magic;
    if (_magic > s_full_magic) _magic = s_full_magic;
    if (_magic < 0.f) _magic = 0.f;

    GameScene::s_load_magic->setPercent(_magic / s_full_magic * 100.f);
}

void Player::update_per_second(float dt) {
    do {
    if (Soldier::SOLDIER_STATE_IDLE == _soldier->_state) {
        if (_magic < s_full_magic) add_magic(1);     // 回魔

        if (_soldier->_blood < _soldier->_full_blood) 
            NetRoom::set_blood(_soldier->_blood + _soldier->_vitality); // 回血
        break;
    }
    if (Soldier::SOLDIER_STATE_MOVE == _soldier->_state) { // 每秒同步一下位置, 
        auto p3 = _soldier->getPosition3D();
        Vec2 pos(p3.x, p3.z);
        NetRoom::set_position(pos);
        break;
    }
    } while (false);
}

void Player::revive() {
    _soldier->_state = Soldier::SOLDIER_STATE_IDLE;
    NetRoom::set_position(NetRoom::_house_pos);
    NetRoom::set_blood(_soldier->_full_blood * 0.8f);
    NetRoom::set_state(Soldier::SOLDIER_STATE_REVIVE);
}

void Player::on_get_goods(Goods *good) {
    switch (good->type) {
    case Goods::GRASS:
        NetRoom::set_grass(_soldier->grass() + good->count);
        break;
    case Goods::SHOES:
        NetRoom::set_speed(_soldier->speed() + good->count);
        break;
    case Goods::WEAPON:
        //NetRoom::set_speed(_soldier->speed() + 1);
        // 随机给某个技能加强属性
        // Uncomplete
        /*
        switch (random(0, 3)) {
        case 0:
            skill_boxing->_blood -= good->count;
            break;
        case 1:
            skill_kick->_blood -= good->count;
            break;
        case 2:
            skill_special->_blood -= good->count;
            break;
        case 3:
            break;
        }
        // */
        break;
    }

    // remove this good from world
    NetRoom::dec_goods(good->index);
}

void Player::on_pick_goods() {
    static Goods s_good;
    static Vec2 s_pos;

    auto pos3 = g_self->getPosition3D();
    s_pos.x = pos3.x;
    s_pos.y = pos3.z;
    if (!g_world->get_goods(s_pos, &s_good)) return; // pick failure

    GameScene::Instance->begain_progress(3.f, [this](float dt, bool end) {
        if (!end) { // 读条未结束时
            //必须处于IDLE状态
            if (_soldier->state() != Soldier::SOLDIER_STATE_IDLE)
                GameScene::Instance->break_progress();
		} else { // 读条结束后如果pick成功，则为真正的获取到
            if (!g_world->get_goods(s_pos, &s_good))
                return; // pick failure

            on_get_goods(&s_good);
		}
    });
}

void Player::set_grass(int count) {
    _soldier->grass(count);
    GameScene::set_score(count);
}
