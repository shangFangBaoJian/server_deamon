#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QStringList>
#include <QTcpSocket>
#include <QDataStream>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
signals:

public slots:
    void sendData();
    void m_newconnect();
private:
    QStringList m_data;
    QTcpServer *server;
    QTcpSocket *client;
};

#endif // TCPSERVER_H
