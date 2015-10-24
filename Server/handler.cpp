#include <QDebug>
#include <QThread>
#include "room.h"
#include "dialog.h"
#include "handler.h"

extern Dialog *g_dialog;

namespace MESSAGE {
    const char *name[] = {
        "authentication",
        "room_list", //房间列表      1
        "create_room", //创建房间    2
        "join_room ", //加入房间     3
        "quit_room ", //退出房间     4
        "room_members", //房间成员信息    5
        "start_game", //开始游戏     6
        "set_ready", //设置准备状态    7
        "set_team", //设置队伍          8

        "update_state",               // 9
        "update_position",  //          10
        "update_angle", //           11
        "update_speed",   //           12
        "update_blood",      //          13

        "action_move",   //           14
        "action_stop",   //           15

        "do_skill",  //                16
        "on_attacked",  //             17

        "update_grass", //           18
        "add_goods",     //       19
        "dec_goods",        //     20

        "set_master",     // 21
        "game_over",       // 22

        ""
    };
}

MsgHandler::MsgHandler(QTcpSocket *sock) {
    m_socket = sock;
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    m_handlers = new Handler[MESSAGE::Max_number]; //分配处理器Vector
    for(int i = 0; i < MESSAGE::Max_number; i++) { m_handlers[i] = nullptr; }

    m_pkg_cache = new PackageCache([this](char *buf, int size) {
        auto pkg = &m_buf;
        memcpy(pkg, buf, size);
        // 处理包
        if(pkg->msg < MESSAGE::Max_number) {
        auto handler = m_handlers[pkg->msg];

        if(handler) handler(this, pkg); //交由子类处理器处理
        } else {
            g_dialog->output("[Error] Unrecognizied message %d", pkg->msg);
            //break;
        }
    });

    HANDLER(authentication) { //
        if(self->setMember(pkg->data, pkg->arg1)) {
            self->Reply(pkg, 1);
            g_dialog->output("[LOG] New member: %s",
                             self->member()->name());
        } else {
            if(self->member())
                self->Reply(pkg, 0, self->error());
            self->endHandle(true);
        }
    };
    HANDLER(room_list) { //房间列表
        auto p = pkg->data;
        for(auto name : Room::list()) { //枚举房间列表并发送
            pkg->arg1 = 1;
            pkg->arg2 = strlen(p);
            self->Reply(pkg, name.toStdString().c_str());
        }
        pkg->arg1 = 0; //枚举结束
        self->Reply(pkg);
    };
    HANDLER(create_room) { //创建房间
        auto room = new Room(pkg->data);
        if(room->error()) {
            self->Reply(pkg, 0, room->error());
            delete room;
        } else {
            self->Reply(pkg, 1);
            g_dialog->output("[LOG] Room: %s created by %s",
                             room->name(),
                             self->member()->name());
        }
        g_dialog->updateRoomList();
    };
    HANDLER(join_room) { //加入某个房间
        if(self->member()->join_room(pkg->data)) {
            self->Reply(pkg, 1);
        } else {
            self->Reply(pkg, 0, self->member()->error());
        }
    };
    HANDLER(quit_room) { //退出房间
        if(self->member()->quit_room()) {
            self->Reply(pkg, 1);
        } else {
            self->Reply(pkg, 0, self->member()->error());
        }
        g_dialog->updateRoomList();
    };
    HANDLER(room_members) { //获取房间成员列表
        self->member()->room()->broad_members();
    };
    HANDLER(start_game) { //通知各客户端开始游戏
        self->member()->room()->start_game();
    };
    HANDLER(set_ready) {  //设置准备状态
        auto room = self->member()->room();
        if(!room) return;
        if(pkg->arg1) {
            self->member()->set_ready_1();
            if(room->check_team_ready()) {
                //start game
                room->start_game();
            }
        } else {
            self->member()->set_ready_0();
        }
        room->broad_members();
    };
    HANDLER(set_team) { //
        auto room = self->member()->room();
        if(!room) return;
        if(room->set_team(self->member(), pkg->arg1)) {
            self->Reply(pkg, 1);
            room->broad_members();
        } else {
            self->Reply(pkg, 0, room->error());
        }
    };

    auto handler_update = [](MsgHandler *self, net_pkg *pkg) {
        auto m = self->member();
        if(m && m->room()) m->room()->broadcast(pkg, pkg->len);
    };

    m_handlers[MESSAGE::update_state] = handler_update;
    m_handlers[MESSAGE::update_speed] = handler_update;
    m_handlers[MESSAGE::update_blood] = handler_update;

    m_handlers[MESSAGE::update_position] = handler_update;
    m_handlers[MESSAGE::update_angle] = handler_update;

    m_handlers[MESSAGE::action_move] = handler_update;
    m_handlers[MESSAGE::action_stop] = handler_update;

    m_handlers[MESSAGE::do_skill] = handler_update;
    m_handlers[MESSAGE::on_skill] = handler_update;

    m_handlers[MESSAGE::update_grass] = handler_update;
    m_handlers[MESSAGE::add_goods] = handler_update;
    m_handlers[MESSAGE::dec_goods] = handler_update;

    m_handlers[MESSAGE::set_master] = handler_update;
    m_handlers[MESSAGE::game_over] = handler_update;
}

MsgHandler::~MsgHandler() {
    if(m_member) delete m_member;
    delete[] m_handlers;
    delete m_pkg_cache;
}

void MsgHandler::onDisconnected() {
    g_dialog->output("[LOG] Connect: %s disconnected.",
                     (m_member ? member()->name() : m_socket->peerName().toStdString().c_str()));

    if(m_member) m_member->quit_room();
    delete this;
}

int MsgHandler::_Reply(net_pkg *p, int size) {
    p->len = size;
    int ret = m_socket->write((const char *)p, (qint64)size);
    m_socket->flush();
    return ret;
}
//数据到达时调用
void MsgHandler::onReadyRead() {
    if(endHandle()) {
        m_socket->disconnectFromHost();
        return;
    }
    char buf[10240];
    int size = m_socket->read(buf, 10240);
    m_pkg_cache->input_stream(buf, size);
}
