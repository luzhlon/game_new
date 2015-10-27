#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QHash>
#include "message.h"

class MsgHandler;
class Room;

class Member : public room_member
{
public:
    static QHash<QString, Member *> g_member;

    Member(char *name, MsgHandler *handler);
    ~Member();

    inline MsgHandler *handler() {
        return m_handler;
    }
    inline Room *room() {
        return m_room;
    }
    inline char *name() {
        return m_name;
    }
    inline const char *error() {
        return m_err;
    }

    bool join_room(char *room);
    bool quit_room();

private:
    friend class Room;
    friend class MsgHandler;

    Room *m_room = nullptr;
    MsgHandler *m_handler;
    const char *m_err = nullptr;
};

#endif // MEMBER_H
