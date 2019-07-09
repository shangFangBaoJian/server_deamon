#include "business.h"
#include "datahandler.h"
#include <QDebug>

business* business::m_staticslef;

business::business()
{
    m_staticslef = this;
}

QString business::recv(QString data)
{
    datahandler handler;
    QJsonDocument json = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject ojson = json.object();
    QString fun,sendData;
    if(!(ojson.contains("mod"))){
        fun = ojson.take("mod").toString();
        qDebug() << "fromat is error";
        return QString();
    }
    if(!fun.compare("client_register")){
        sendData = handler.createHostid();
    }

    if(!fun.compare("heartbeat")){
        if(!m_staticslef->getHeatbeatFirst()){
            handler.registered(data);
            m_staticslef->setHeatbeatFirst(true);
        }
        else
            sendData = handler.heatbeat();
    }

    return sendData;
}

void business::send()
{

}

void business::setHeatbeatFirst(bool flag)
{
    isHeatbeatFirst = flag;
}

bool business::getHeatbeatFirst()
{
    return isHeatbeatFirst;
}
