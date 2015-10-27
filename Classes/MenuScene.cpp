#include "MenuScene.h"
#include "HelpScene.h"
#include "SettingScene.h"
#include "RoleScene.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
using namespace CocosDenshion;

extern SimpleAudioEngine *g_audio;

Scene *MenuScene::createScene() {
    auto scene = Scene::create();
    auto layer = MenuScene::create();

    scene->addChild(layer);

    return scene;
}

void MenuScene::onSettingClick(Ref *ref) {
    log("enter setting");
    Director::getInstance()->pushScene(SettingScene::createScene());
}

void MenuScene::onMultiClick(Ref *ref) {
    log("enter role");
    Director::getInstance()->pushScene(RoleScene::createScene());
}

bool MenuScene::init() {
    auto layer = load_layer("menu_scene.csb");
    auto layout = get_layout(layer);

    CC_ASSERT(layout); //load layout failure

    setClickCallback(layout, "button_multi", CC_CALLBACK_1(MenuScene::onMultiClick, this));
    setClickCallback(layout, "button_setting", CC_CALLBACK_1(MenuScene::onSettingClick, this));
    setClickCallback(layout, "button_quit", CC_CALLBACK_1(MenuScene::onQuitClick, this));

    setClickCallback(layout, "button_help", [](Ref *ref) {
        Director::getInstance()->pushScene(HelpScene::createScene());
    });

    // 循环播放背景音乐
    g_audio->playBackgroundMusic("bg1.mp3", true);

    return true;
}

void MenuScene::onEnter() {
    Layer::onEnter();
    //加载配置
    SettingScene::load_settings();
}

void MenuScene::onQuitClick(Ref *ref) {
	Director::getInstance()->end();
}
