#include <QThread>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qtcpserver.h>

class MsgHandler;

//工作处理线程
class WorkThread : public QThread {
    Q_OBJECT
private:
    QTcpServer *m_server; //服务器
public:
    WorkThread() {}
    ~WorkThread() {}
    void run() Q_DECL_OVERRIDE;
public slots:
    void onConnect();
};
