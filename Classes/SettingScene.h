#ifndef __SETTINGSCENE_H__
#define __SETTINGSCENE_H__

#include "SceneLayer.h"

class SettingScene : public SceneLayer {
public:
    static void load_settings();
    static Scene* createScene();
    CREATE_FUNC(SettingScene);
    virtual bool init();
    void onOkClicked(Ref *);
    void onCancelClicked(Ref *);
    void save_settings();
private:
    TextField *m_textIP;
    Slider    *m_slidVolum;
};

#endif //__MENUSCENE_H__
