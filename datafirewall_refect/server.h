#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <iostream>


class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = 0);

    bool get(QUrl url);

    bool post(QUrl url,QByteArray& bytes);

signals:

public slots:
    void replyFinish(QNetworkReply *reply);
    void downloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void finished();

private:
    QNetworkAccessManager *manager;
    QNetworkAccessManager *m_pManager;


};

#endif // SERVER_H
