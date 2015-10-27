#ifndef __ROLESCENE_H__
#define __ROLESCENE_H__

#include "SceneLayer.h"

class RoleScene : public SceneLayer
{
public:
    static Scene *createScene();

    CREATE_FUNC(RoleScene);

    virtual bool init();

    bool load_pages();

    void onStartClick(Ref *);
    void onLeaveClick(Ref *);
    void onEnter() override;
private:
    TextField *m_editName;
    PageView *m_pageSprite;
};

#endif // __ROLESCENE_H__
