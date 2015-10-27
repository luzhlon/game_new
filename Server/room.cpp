#include <QDebug>
#include "dialog.h"
#include "member.h"
#include "room.h"
#include "handler.h"

QHash<QString, Room *> Room::g_room;

const char *g_check_name(char *name) {
    std::string str = name;
    if(str.empty()) {
        return "Empty name";
    }
    if((str.find(' ') != std::string::npos) ||
            (str.find("　") != std::string::npos)) {
        return "Contain space character";
    }
    return nullptr;
}

Room::Room(char *name) {
    if(strlen(name) >= MAX_ROOM_NAME_LEN) {
        m_err = "room name is too long";
        return;
    } //房间名称太长
    if(Room::getRoom(name)) { //房间已经存在
        m_err = "room existed";
        return;
    }
    m_err = g_check_name(name);
    if(m_err) return;
    strcpy(m_name, name); //
    g_room[m_name] = this; //从哈希表中加入自己

    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) m_members[i] = nullptr; //初始化成员为空
}

Room::~Room() {
    g_room[m_name] = nullptr; //从哈希表中删除自己
}

bool Room::add(Member *meb) {
    int i = 0;
    for(Member *m = nullptr; //找一个空位置加入成员
        i < MAX_ROOM_MEMBERS; i++) {
        m = m_members[i];
        if(!m) {  //success
            m_members[i] = meb;
            meb->m_room_id = i;
            meb->m_room = this; //设置成员的所属房间为自己
            g_dialog->output("[LOG] ROOM %s: %s joined.", m_name, meb->m_name);
            broad_members(); //广播房间中的成员信息
            return true;
        }
    }
    //房间已经满员了
    m_err = "没有空位置咯~~";
    return false;
}

bool Room::start_game() {
    //if(m_started) return false;  //DEBUG

    mini_net_pkg pkg;
    pkg.msg = MESSAGE::start_game;
    broadcast((net_pkg *)&pkg);
    m_started = true;
    return true;
}

bool Room::remove(Member *meb) {
    auto r_id = for_member_id(meb);
    if(error()) {
        m_err = "no this member";
        return false;
    }
    m_members[r_id] = nullptr;

    // 告诉其他成员自己退出房间
    mini_net_pkg pkg;
    pkg.msg = MESSAGE::quit_room;
    broadcast((net_pkg *)&pkg, r_id);

    g_dialog->output("[LOG] ROOM %s: %s quited.", m_name, meb->m_name);
    return true;
}

void Room::broad_members() {
    net_pkg pkg;
    room_member meb[MAX_ROOM_MEMBERS];

    pkg.msg = MESSAGE::room_members;
    pkg.arg1 = -1; //房主Room ID
    pkg.arg2 = -1; //自己的Room ID

    int &master_id = pkg.arg1;
    int &self_id = pkg.arg2;

    //size_t size = sizeof(room_member) * MAX_ROOM_MEMBERS;
    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        auto m = m_members[i] ;
        if(m) {
            pkg.arg1 ++; //成员数
            m->m_room_id = i;
            memcpy(&meb[i], m, sizeof(room_member)); //Copy成员信息
        } else {
            //memset(&meb[i], 0, sizeof(room_member)); //此位置清零
            meb[i].set_empty(); //空成员
        }
        meb[i].m_room_id = i;
    } // 构造数据包

    //broadcast
    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        auto m = m_members[i] ;
        // 检测设置房主ID
        if(master_id < 0 && m && !m->is_empty())
            master_id = i;
        // 设置成员的ID
        self_id = i;
        //pkg.arg2 = i; //对应客户端自己的ID
        if(m) m->handler()->Reply(&pkg, meb, sizeof(meb));
    }
}

int Room::for_member_id(Member *meb) {
    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        if(m_members[i] == meb) {
            m_err = nullptr;
            return i;
        }
    }
    m_err = "no this member";
    return -1;
}

bool Room::check_team_ready() {
    int team_red = 0;  // 准备好的
    int team_blue = 0; //

    int num_red = 0;  // 红队人数
    int num_blue = 0; // 蓝队人数

    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        auto m = m_members[i];
        if(m) {
            if(i%2) {
               num_blue += 1;
               team_blue += m->get_ready();
            } else {
               num_red += 1;
               team_red += m->get_ready();
            }
        }
    }
    if(team_red && team_blue &&
       team_red == num_red &&
       team_blue == num_blue)
        return true;
    else
        return false;
}

bool Room::set_team(Member *meb, int team) {
    int r_id = for_member_id(meb);
    if(error()) {
        return false;
    }
    if(r_id % 2 == team % 2) return true; // 已经在team队伍
    //偶数为红队，奇数为蓝队
    for(int i = team % 2; i < MAX_ROOM_MEMBERS; i += 2) {
        if(!m_members[i]) {
            m_members[i] = m_members[r_id];
            m_members[r_id] = nullptr; //原先的位置置空
            return true;
        }
    }
    m_err = "object team is full";
    return false;
}
