#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <thread>
#include <condition_variable>
#include <functional>
#include <list>

#include "cocos2d.h"

#include "ODSocket.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "windows.h"
#define sleep Sleep
#else
#include "unistd.h"
#define sleep usleep
#endif

#include "../Server/message.h"

#define MAX_BLOCK_MSG_NUM 16
#define INCREASE(num)  num = (num == MAX_BLOCK_MSG_NUM) ? 0 : num + 1
#define HANDLER(msg) Client::s_handlers[MESSAGE::msg]

class Client : public cocos2d::Ref
{
public:
    typedef std::function<void (net_pkg *)> handler;
    typedef unsigned int data_size_t;
    static Client *getInstance();
    static handler s_handlers[MESSAGE::Max_number];
    static net_pkg s_pkg;
    //static std::list<net_pkg *> s_send_list;
    static std::list<net_pkg *> s_recv_list;
    static bool s_lock; // lock the recv_list
    static std::function<void()> onDisconnect;

    void dispatchMsg(float );

    bool send(char *, int);
    bool recv_data();

    inline bool sendMsg(net_pkg *pkg, int size) {
		pkg->len = size;
        return send((char*)pkg, size);
    }
    inline void copy_data(void *data, int size) {
        if(data)
            memcpy(s_pkg.data, data, size);
    }
    inline bool sendMsg(msg_msg msg,
                        data_size_t data_size, void *data = nullptr) {
        copy_data(data, data_size);
        s_pkg.msg = msg;
        return sendMsg(&s_pkg, data ? NET_PKG_SIZE + data_size : NET_PKG_SIZE_2);
    }
    inline bool sendMsg(msg_msg msg, msg_arg arg1 = 0) {
		s_pkg.arg1 = arg1;
		s_pkg.msg = msg;
        return sendMsg(&s_pkg, NET_PKG_SIZE_1);
	}
    inline bool sendMsg(msg_msg msg, const char *str) {
        return sendMsg(msg,
                       static_cast<data_size_t>(strlen(str) + 1),
                       (void *)str);
    }
    inline bool sendMsg(msg_msg msg, msg_arg arg1, const char *str) {
        s_pkg.arg1 = arg1;
        return sendMsg(msg, str);
    }
    inline bool isConnected() {
        return m_connect;
    }

    bool connect_server();

    void threadRecv();
    void start(); // will start a new thread
private:
    Client();

    std::thread   *m_thread = nullptr;
    ODSocket      *m_sock = nullptr;
    net_pkg        m_pkg_recv;

    bool           m_connect = false;
    int            m_recv_count = 0;
};

#endif // __CLIENT_H__
