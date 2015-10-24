#include "member.h"
#include "room.h"

QHash<QString, Member *> Member::g_member;

extern const char *g_check_name(char *name);

Member::Member(char *name, MsgHandler *handler) {

    if(strlen(name) >= MAX_MEMBER_NAME_LEN) {
        m_err = "nick name too long";
        return;
    }
    if(g_member[name]) {
        m_err = "nick name existed";
        return;
    }

    m_err = g_check_name(name);
    if(m_err) return;
    strcpy(m_name, name);
    m_handler = handler;
    g_member[name] = this;
}

Member::~Member() {
    g_member[m_name] = nullptr;
}

bool Member::join_room(char *room_name) {
    auto room = Room::getRoom(room_name);
    if(!room) {
        m_err = "no this room";
        return false;
    }
    if(m_room) {
        m_err = "in a room now";
        return false;
    }
    if(!room->add(this)) {
        m_err = room->error();
        return false;
    }
    m_room = room;
    return true;
}

bool Member::quit_room() {
    if(!m_room) {
        m_err = "not in a room";
        return false;
    }
    if(!m_room->remove(this)) {
        m_err = m_room->error();
        return false;
    }
    m_room = nullptr;
    return true;
}
