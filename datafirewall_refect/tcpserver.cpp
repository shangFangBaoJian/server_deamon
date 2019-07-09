#include "tcpserver.h"
#include "business.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server,&QTcpServer::newConnection,this,&TcpServer::m_newconnect);
    if(!server->listen(QHostAddress::Any,10001)){
        qDebug() << "create socket failed";
        exit(-1);
    }
}

TcpServer::~TcpServer()
{
    delete server;
}

void TcpServer::sendData()
{
    QString data;
    QByteArray sreaddata = client->readAll();
    QByteArray datatmp;
//    qDebug() << "<<<<:"<< endl << sreaddata << endl;
    QString tmp = QString(sreaddata);
    for (int i = 0;i < sreaddata.size() ;i++) {
        if(sreaddata[i] == '\r' && sreaddata[i+1] == '\n' && sreaddata[i+2] == '\r' && sreaddata[i+3] == '\n'){
            datatmp = sreaddata.mid(i+4,sreaddata.size()-i-4);
        }
    }
    qDebug() << "<<<<<:" << QString(datatmp) <<endl ;
    QString mm = QString(datatmp);
    data = business::recv(mm);
    qDebug() << ">>>>:" << data <<endl ;
    client->write(data.toLocal8Bit());
}

void TcpServer::m_newconnect()
{

    qDebug() << "has new connect..." << endl;
    client = server->nextPendingConnection();
    connect(client,SIGNAL(readyRead()),this,SLOT(sendData()));

}

