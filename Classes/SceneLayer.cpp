#include "SceneLayer.h"
#include "Dialog.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

SceneLayer::SceneLayer()
{
}

Node *SceneLayer::load_layer(const std::string& sceneFile, int layerTag) {
    auto node = CSLoader::getInstance()->createNode(sceneFile);
    return node ?
                load_layer(node, layerTag) : nullptr;
}

Layer *SceneLayer::load_layer(Node *scene, int layerTag) {
    auto child = scene->getChildByTag(layerTag);
    if(child) {
        child->removeFromParent();
        addChild(child);
        return static_cast<Layer *>(child);
    } else
        return nullptr;
}

bool SceneLayer::setClickCallback(Layout *layout,
							const std::string& btnName,
							const Widget::ccWidgetClickCallback& callback) {
    auto widget = Helper::seekWidgetByName(layout, btnName);
    if(widget) {
        widget->addClickEventListener(callback);
        return true;
    } else
        return false;
}

bool SceneLayer::setTouchCallback(Layout *layout,
								const std::string& btnName,
								const Widget::ccWidgetTouchCallback& callback) {
    auto widget = Helper::seekWidgetByName(layout, btnName);
    if(widget) {
		widget->addTouchEventListener(callback);
        return true;
    } else
        return false;
}

Layout *SceneLayer::get_layout(const std::string& nodefile, int tag) {
    auto node = CSLoader::createNode(nodefile);
    auto layout = static_cast<Layout *>(node->getChildByTag(tag));

    if (layout) layout->removeFromParent();

    return layout;
}
