#include "handler.h"
#include "server.h"

void WorkThread::onConnect() {
    qDebug() << "New client:";
    QTcpSocket *sock = m_server->nextPendingConnection();
    qDebug() << sock->peerAddress().toIPv4Address() << "\n";

    /* auto thread = new ConnectThread(sock);
    thread->start(); //运行连接处理线程 // */
    auto h = new MsgHandler(sock);
    h->moveToThread(this); //将MsgHandler的事件循环移到本线程中来
}

void WorkThread::run() {
    qDebug() << "Begain working...\n";
    //新建一个QTcpServer并监听4321端口
    m_server = new QTcpServer();
    m_server->listen(QHostAddress::Any, 4321);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(onConnect()));
    QThread::exec(); //启动QT的事件循环
}

void startWork() {
    static WorkThread* thread = nullptr;
    if(thread) return; //保证只有一个工作线程
    thread = new WorkThread();
    thread->start();
}
