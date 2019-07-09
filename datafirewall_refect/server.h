#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <iostream>


class server : public QObject
{
    Q_OBJECT
public:
    explicit server(quint16 port ,bool debug = false, QObject *parent = Q_NULLPTR);
    ~server();

signals:
    void closed();

public slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_client;
    bool m_debug;

};

#endif // SERVER_H
