#ifndef BUSINESS_H
#define BUSINESS_H

#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class business
{
public:
    business();

    static QString recv(QString data);

    void send();

    void setHeatbeatFirst(bool flag);
    bool getHeatbeatFirst();
private:
    bool isHeatbeatFirst;
    static business* m_staticslef;
};

#endif // BUSINESS_H
