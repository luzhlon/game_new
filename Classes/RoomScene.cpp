#include "Soldier.h"
#include "RoomScene.h"
#include "GameScene.h"
#include "NetRoom.h"

string g_room_name;

extern Director *g_director;

Scene *RoomScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoomScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoomScene::init() {
    auto layer = load_layer("room_scene.csb");
    auto layout = get_layout(layer);
        
    _check_team = static_cast<CheckBox *>(Helper::seekWidgetByName(layout, "check_team"));

    setClickCallback(layout, "check_ready_me", CC_CALLBACK_1(RoomScene::onReadyClick, this));
    setClickCallback(layout, "check_team", CC_CALLBACK_1(RoomScene::onTeamClick, this));
    setClickCallback(layout, "button_start", [this](Ref *ref) {
        Client::getInstance()->sendMsg(MESSAGE::start_game);
    });
    setClickCallback(layout, "button_back", [this](Ref *ref) {
        Client::getInstance()->sendMsg(MESSAGE::quit_room);
        Director::getInstance()->popScene();
    });

    //CC_ASSERT(!s_room_name.empty());
    auto text_name = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_name"));
    text_name->setString(g_room_name);

    load_layouts(layout);

    return true;
}

void RoomScene::update_room_member() {
    for (int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        set_member_info(i, &NetRoom::_members[i]);
    }
}

void RoomScene::set_member_info(int index, room_member *meb) {
    CC_ASSERT(index < MAX_ROOM_MEMBERS);

    auto lay = _layout_member[index];
    auto ready = static_cast<CheckBox *>(Helper::seekWidgetByName(lay, "check_ready"));
    auto name = static_cast<Text *>(Helper::seekWidgetByName(lay, "text_member"));
    auto sprite = static_cast<ImageView *>(Helper::seekWidgetByName(lay, "image_sprite"));
    CC_ASSERT(sprite);

    if (meb->is_empty()) {
        ready->setSelectedState(false);
        name->setString("");
        sprite->setVisible(false);
        return;
    }

    const char *role_texture[] = {
        "image/girl.png",
        "image/man.png",
        "image/man2.png"
    };

    ready->setSelectedState(meb->get_ready());
    name->setString(meb->m_name);
    name->setColor(meb->m_room_id % 2 ? Color3B(0, 0, 255) : Color3B(255, 0, 0));
    if (meb->m_role_id < 3) sprite->loadTexture(role_texture[meb->m_role_id]);
    sprite->setVisible(true);
}

void RoomScene::onEnter() {
    Layer::onEnter();
    HANDLER(room_members) = Client::handler([this](net_pkg *pkg) {
        auto *meb = (room_member *)pkg->data;
        memcpy(NetRoom::_members, pkg->data, sizeof(room_member)* MAX_ROOM_MEMBERS);

        NetRoom::_master_id = pkg->arg1; // master id
        NetRoom::_self_id = pkg->arg2; //Self id

        update_room_member();

        _check_team->setSelectedState(NetRoom::_self_id % 2);
        //HANDLER(authentication) = nullptr; // 置空，防止重复调用
    });
    HANDLER(start_game) = Client::handler([this](net_pkg *pkg) {
        for (int i = 0; i < MAX_ROOM_MEMBERS; i++)
            NetRoom::_members[i].m_room_id = i; // 确认一下room id

        NetRoom::_room_name = g_room_name;

        g_director->pushScene(GameScene::createScene());

        HANDLER(start_game) = nullptr; // 防止重入
    });
    Client::getInstance()->sendMsg(MESSAGE::room_members);
}

void RoomScene::onExit() {
    Layer::onExit();
    HANDLER(room_members) = nullptr;
}

void RoomScene::onReadyClick(Ref *ref) {
    auto btn = static_cast<CheckBox *>(ref);
    //Client::getInstance()->sendMsg(MESSAGE::set_ready, (msg_arg)!btn->getSelectedState());
    // on version 3.8, this bug is fixed.
    Client::getInstance()->sendMsg(MESSAGE::set_ready, (msg_arg)btn->getSelectedState());
}

void RoomScene::onTeamClick(Ref *ref) {
    auto btn = static_cast<CheckBox *>(ref);
    //Client::getInstance()->sendMsg(MESSAGE::set_team, (msg_arg)!btn->getSelectedState());
    // on version 3.8, this bug is fixed.
    Client::getInstance()->sendMsg(MESSAGE::set_team, (msg_arg)btn->getSelectedState());
}

void RoomScene::load_layouts(Layout *layout) {
    auto layout_team_red = static_cast<Layout *>(Helper::seekWidgetByName(layout, "layout_team_red"));
    auto layout_team_blue = static_cast<Layout *>(Helper::seekWidgetByName(layout, "layout_team_blue"));
    CC_ASSERT(layout_team_red); //load layout failure
    CC_ASSERT(layout_team_blue); //load layout failure

    auto layout_member = static_cast<Layout *>
        (Helper::seekWidgetByName(layout_team_red, "layout_member"));
    CC_ASSERT(_layout_member);
    layout_member->retain();
    layout_member->removeFromParent();

    Layout *layout_red_blue[2];
    layout_red_blue[0] = layout_team_red;
    layout_red_blue[1] = layout_team_blue;

    for (int i = 0; i < MAX_ROOM_MEMBERS; i ++) {
        auto layout = layout_red_blue[i % 2];

        auto lay = static_cast<Layout *>(layout_member->clone());
        auto name = static_cast<Text *>(Helper::seekWidgetByName(lay, "text_member"));
        auto ready = static_cast<CheckBox *>(Helper::seekWidgetByName(lay, "check_ready"));

        layout->addChild(lay); // 加到相应team的layout里
        auto size = layout->getContentSize();
        lay->setPosition(Vec2(0.f, (1.f - i / 2 * 0.333) * size.height));

        _layout_member[i] = lay;
    }
}
