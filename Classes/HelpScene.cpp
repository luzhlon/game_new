#include "HelpScene.h" 
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

Scene *HelpScene::createScene() {
    auto scene = Scene::create();
    auto layer = HelpScene::create();

    scene->addChild(layer);

    return scene;
}

bool HelpScene::init() {
    auto node = CSLoader::createNode("help_scene.csb");
    CC_ASSERT(node);
    node->removeFromParent();
    addChild(node);

    auto btn_back = static_cast<Button *>(node->getChildByName("button_back"));
    btn_back->addClickEventListener([](Ref *ref) {
        Director::getInstance()->popScene();
    });

    return true;
}
