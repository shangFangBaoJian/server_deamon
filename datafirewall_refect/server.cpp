#include "server.h"

using namespace std;

server::server(QObject *parent) : QObject(parent)
{

}

bool server::get(QUrl url)
{
    QNetworkRequest request;

    request.setUrl(url);

    QNetworkReply *reply = m_pManager->get(request);

    connect(reply,SIGNAL(finished()),this,SLOT(finished()));

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),

            this,SLOT(downloadProgress(qint64,qint64)));

    return true;
}

bool server::post(QUrl url, QByteArray &bytes)
{
    m_pManager = new QNetworkAccessManager();
    QNetworkRequest request;

    request.setUrl(url);

    QNetworkReply *reply = m_pManager->post(request,bytes);

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),

            this,SLOT(downloadProgress(qint64,qint64)));

    return true;
}

void server::replyFinish(QNetworkReply *reply)
{


    qDebug()<<"对返回信息的JSON包进行解析";
    QString strJsonText = reply->readAll();//这个返回的JSON包所携带的所有信息
    qDebug()<<"所有返回信息"<<strJsonText;
}

void server::downloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    cout<< "\ndownloadProgress done:\n";

    cout << "bytesSent: " << bytesSent

         << "  " << "bytesTocal: " << bytesTotal;
}

void server::finished()
{
    QNetworkReply* reply = (QNetworkReply*)sender();

    reply->deleteLater();
}
