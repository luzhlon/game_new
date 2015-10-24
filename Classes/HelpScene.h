#ifndef __HELPSCENE_H__
#define __HELPSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class HelpScene : public Layer {
public:
    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelpScene);
};


#endif