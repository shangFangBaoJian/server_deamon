#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>

class datahandler
{
public:
    datahandler();

//send
    QString heatbeat();
    QString recvResult(int result);
    QString createHostid();
//recv
    void registered(QString data);
    void svnAction(QString data);

private:
    void setRepolist(QString strname);

private:
    QString hostId;
    QList<QString> repolist;

};

#endif // DATAHANDLER_H
