#ifndef SCENELAYER_H
#define SCENELAYER_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class Dialog;

class SceneLayer : public Layer
{
public:
    SceneLayer();
    enum {
        SCENE_MENU,
        SCENE_SETTING,
        SCENE_ROLE,
        SCENE_ROOMLIST,
        SCENE_ROOM,
        SCENE_GAME
    };

protected:
    Node *load_layer(const std::string& sceneFile,
                     int layerTag = 1);
    Layer *load_layer(Node *scene,                          // 从一个场景(文件)中加载层
                      int layerTag = 1);
    Layout *get_layout(const std::string& nodefile,
                       int tag = 1);
    inline Layout *get_layout(Node *layer,                  // 获取一个Layout
                              const std::string& name = "layout") {
        return layer ?
               static_cast<Layout *>(layer->getChildByName(name))
               : nullptr;
    }
    bool setClickCallback(Layout *layout,                   // 给一个控件设置OnClick事件
                          const std::string& btnName,
                          const Widget::ccWidgetClickCallback& callback);
    bool setTouchCallback(Layout *layout,                   // 给一个控件设置OnTouch事件
                          const std::string& btnName,
                          const Widget::ccWidgetTouchCallback& callback);
};

#endif // SCENELAYER_H
