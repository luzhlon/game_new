#ifndef __ROOMLISTSCENE_H__
#define __ROOMLISTSCENE_H__

#include <string>
#include <functional>
#include "SceneLayer.h"

using namespace std;

class RoomListScene : public SceneLayer
{
public:
    static Scene *createScene();

    virtual bool init();

    CREATE_FUNC(RoomListScene);

    void onCreateClick(Ref *);
    void onUpdateClick(Ref *);
    void onBackClick(Ref *);
    void onItemClick(Ref *); //进入相应的房间

    bool addRoomItem(const string&);

    void onEnter() override;
    void onExit() override;

    void updateRoomList();

private:
    Button *m_btnCreate;
    Button *m_btnEnter;
    Button *m_btnLeave;
    ListView *m_listRoom;
};

#endif // ROOMLISTSCENE_H
