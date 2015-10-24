#ifndef __ROOMSCENE_H__
#define __ROOMSCENE_H__

#include "Client.h"
#include "SceneLayer.h"

class RoomScene : public SceneLayer {
public:
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(RoomScene);

    void onReadyClick(Ref *);
    void onTeamClick(Ref *);
    void load_layouts(Layout *);
    void set_member_info(int index, room_member *meb);

    inline void update_room_member();

    void onEnter() override;
    void onExit() override;
private:
    //
    Layout *_layout_member[MAX_ROOM_MEMBERS];
    CheckBox *_check_team = nullptr;
};

#endif // __ROOMSCENE_H__
