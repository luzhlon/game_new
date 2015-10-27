#include "RoomListScene.h"
#include "RoomScene.h"
#include "GameScene.h"
#include "Dialog.h"
#include "Client.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

extern string g_room_name;

Scene *RoomListScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoomListScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoomListScene::addRoomItem(const string& item_name) {
    auto layout = get_layout("list_item.csb");
    CC_ASSERT(layout);

    auto btn = static_cast<Button *>
            (Helper::seekWidgetByName(layout, "button_item"));
    btn->setTitleText(item_name);
    btn->addClickEventListener(CC_CALLBACK_1(RoomListScene::onItemClick, this));

    layout->setSizePercent(Vec2(0.8f, 0.15f));
    m_listRoom->pushBackCustomItem(layout);

    return true;
}

void RoomListScene::onItemClick(Ref *ref) {
    Button *btn = dynamic_cast<Button *>(ref);
    CC_ASSERT(btn);
    string item = btn->getTitleText();
    g_room_name = item;
    //strncpy(g_room_name, item.c_str(), MAX_ROOM_NAME_LEN);
    Client::getInstance()->sendMsg(MESSAGE::join_room, item.c_str()); // 请求服务器
    log("[Log] item %s clicked", item.c_str());
}

bool RoomListScene::init() {
    auto layer = load_layer("room_list_scene.csb");
    auto layout = get_layout(layer);

    CC_ASSERT(layout); //load layout failure

    setClickCallback(layout, "button_create", CC_CALLBACK_1(RoomListScene::onCreateClick, this));
    setClickCallback(layout, "button_update", CC_CALLBACK_1(RoomListScene::onUpdateClick, this));
    setClickCallback(layout, "button_back", CC_CALLBACK_1(RoomListScene::onBackClick, this));

    m_listRoom = static_cast<ListView *>(Helper::seekWidgetByName(layout, "list_room"));

    return true;
}

void RoomListScene::onEnter() {
	Layer::onEnter();
    //set timer
	/*
    schedule([this](float lt) {
        updateRoomList();
    }, 1.0f, "timer"); // */
    // 房间列表
    HANDLER(join_room) = Client::handler([this](net_pkg *pkg) {
        if (pkg->arg1) {
            Director::getInstance()->pushScene(RoomScene::createScene());
        } else {
            Dialog::getInstance()->Popup_t(this, " 错误 ", " 加入房间失败： " + string(pkg->data));
        }
    });

    updateRoomList();
}

void RoomListScene::onExit() {
    Layer::onExit();
    //unset timer
    //unschedule("timer");
}

void RoomListScene::updateRoomList() {
    auto clt = Client::getInstance();
    m_listRoom->removeAllItems();
    HANDLER(room_list) =
            Client::handler([this](net_pkg *pkg) {
        if (pkg->arg1) {
            //pkg->data[pkg->arg2] = '\0';
            addRoomItem(pkg->data);
        } else { // 房间枚举结束
            HANDLER(room_list) = nullptr;
        }
    });
    clt->sendMsg((msg_msg)MESSAGE::room_list);
}

void RoomListScene::onCreateClick(Ref *ref) {
	auto dlg = Dialog::getInstance();
    dlg->setCallback(Dialog::Callback([this](Dialog *d, bool ok) {
        if (!ok) return; // 点击了取消按钮
        auto name = d->content_e()->getString();
        cocos2d::log("房间名：%s OK:%d",
                         name.c_str(), ok);
        if(name.empty()) {
            Dialog::getInstance()->Popup_t(this, "错误", "房间名不能为空!!");
            return;
        }
        g_room_name = name;
        HANDLER(create_room) = Client::handler(
                         [this](net_pkg *pkg) {
                             if(pkg->arg1) { // 创建成功后立即加入，不留BUG
                                 log("create room success");
                                 Client::getInstance()->sendMsg(MESSAGE::join_room, g_room_name.c_str()); // 请求服务器
                                 //updateRoomList();
                             } else {
                                 Dialog::getInstance()->Popup_t(this, "创建失败", pkg->data);
                             }
                         });
        Client::getInstance()->sendMsg(MESSAGE::create_room, name.c_str());
    }));
    dlg->Popup_e(this, "创建房间", "Room name");
}

void RoomListScene::onUpdateClick(Ref *) {
	updateRoomList();
}

void RoomListScene::onBackClick(Ref *ref) {
    log("leave room list");
    Director::getInstance()->popScene();
}
