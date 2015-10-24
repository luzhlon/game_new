#ifndef HANDLER_H
#define HANDLER_H

#include <QTcpSocket>
#include "message.h"
#include "member.h"

class WorkThread;

class MsgHandler : public QObject {
    Q_OBJECT

public:
    typedef void (*Handler)(MsgHandler*, net_pkg *); //

    MsgHandler(QTcpSocket *);
    ~MsgHandler();

    void handle(); //总的 处理消息
    void loopHandle(); //开始循环处理用户消息

    inline bool endHandle() { //结束处理？
        return m_end;
    }
    inline void endHandle(bool end) { //结束处理？
        m_end = end;
    }

    bool Recv(); //接收用户消息
    int _Reply(net_pkg *p, int size); //回复用户
    inline int Reply(net_pkg *p) {
        return _Reply(p, NET_PKG_SIZE_1);
    }
    inline int Reply(net_pkg *p, msg_len len) {
        return _Reply(p, p->len);
    }
    inline int Reply(net_pkg *p, msg_arg arg1) {
        p->arg1 = arg1;
        return _Reply(p, NET_PKG_SIZE_1);
    }
    inline int Reply(net_pkg *p, msg_arg arg1, msg_arg arg2) {
        p->arg1 = arg1;
        p->arg2 = arg2;
        return _Reply(p, NET_PKG_SIZE_2);
    }
    inline int Reply(net_pkg *p, void *data, int data_size) {
        memcpy(p->data, data, data_size);
        return _Reply(p, NET_PKG_SIZE + data_size);
    }
    inline int Reply(net_pkg *p, const char *str) { //回复用户，跟一个字符串
        return Reply(p, (void *)str, strlen(str) + 1);
    }
    inline int Reply(net_pkg *p,
                     unsigned short arg1,
                     const char *str) { //回复用户，跟一个字符串
        p->arg1 = arg1;
        return Reply(p, str);
    }

    inline QTcpSocket *socket() {
        return m_socket;
    }
    inline const char *error() {
        return m_err;
    }
    inline Member* member() {
        return m_member;
    }
    bool setMember(char *name, unsigned short role_id) {
        if(m_member) delete m_member;

        m_member = new Member(name, this);
        if(m_member->error()) {
            m_err = m_member->error();
            delete m_member;
            m_member = nullptr;
            return false;
        }
        m_member->m_role_id = role_id;
        return true;
    }

    void on_package(char *buf, int size);

public slots:
    void onDisconnected();
    void onReadyRead();
    //void onStateChange(QAbstractSocket::SocketState state);

private:
    QTcpSocket *m_socket = nullptr; //此处理器持有的socket
    Handler *m_handlers = nullptr; //处理器向量表
    Member * m_member = nullptr; //此处理器对应的用户
    bool     m_end = false; //表明是否要结束处理
    const char *   m_err = nullptr;

    PackageCache *m_pkg_cache = nullptr;
    net_pkg  m_buf; //接收的数据包缓冲区
};

#define HANDLER(MSG) m_handlers[MESSAGE::MSG] = [](MsgHandler *self, net_pkg *pkg)

#endif // HANDLER_H
