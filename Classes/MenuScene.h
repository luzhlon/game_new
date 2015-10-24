#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "SceneLayer.h"

class MenuScene : public SceneLayer {
public:
    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(MenuScene);

    void onSingleClick(Ref *);
    void onMultiClick(Ref *);
    void onSettingClick(Ref *);
    void onQuitClick(Ref *);

    void onEnter() override;
};

#endif //__MENUSCENE_H__
