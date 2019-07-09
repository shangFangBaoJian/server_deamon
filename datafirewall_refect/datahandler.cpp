#include "datahandler.h"
#include <QVariantMap>

datahandler::datahandler():
    hostId("5d0b8a06098c98d2428b4569")
{

}

QString datahandler::heatbeat()
{
    QVariantMap json;
    json["Ret"] = 1;
    json["Status"] = 1;

    QVariantList list;
    for(int i = 0;i < list.size();i++){
        QString tmp = list.at(i).toString();
        list.append(tmp);
    }
    QVariantMap svn;
    svn["interval"] = 24;
    svn["path"] = "/opt/tlx/svnrepo";
    svn["repo_list"] = list;

    json["svn_back"] = svn;

    QJsonDocument pjson = QJsonDocument::fromVariant(QVariant(json));
    QString value = pjson.toJson(QJsonDocument::Compact);
    return value;
}

QString datahandler::recvResult(int result)
{
    QVariantMap json;
    json["status"] = 0;
    json["ret"] = result;

    QJsonDocument pjson = QJsonDocument::fromVariant(QVariant(json));
    QString value = pjson.toJson(QJsonDocument::Compact);
    return value;

}

QString datahandler::createHostid()
{
    QVariantMap json;
    json["host_id"] = hostId;
    json["ret"] = 1;

    QJsonDocument pjson = QJsonDocument::fromVariant(QVariant(json));
    QString value = pjson.toJson(QJsonDocument::Compact);
    return value;
}

void datahandler::registered(QString data)
{
    int hostidFlag = 0;
    QJsonDocument pjson=  QJsonDocument::fromJson(data.toLocal8Bit());

    QJsonObject ojson = pjson.object();

    if(ojson.contains("mod") && ("heartbeat" == ojson.take("mod").toString())){
        hostidFlag = 1;
    }

    if(hostidFlag){
        if(ojson.contains("host_id")){
            QJsonValue host_id = ojson.take("host_id");
            QString hostidTmp = host_id.toString();
            if((hostidTmp.isNull() || hostidTmp.isEmpty())){
               hostId = hostidTmp;
            }
        }
    }
}

void datahandler::svnAction(QString data)
{
    bool flag = false;
    QJsonDocument json = QJsonDocument::fromJson(data.toLocal8Bit());
    QJsonObject ojson = json.object();
    if(ojson.contains("mod") && ("svn_action" == ojson.take("mod").toString())){
        flag = true;
    }

    if(flag){
        if(ojson.contains("file_info")){
            if (ojson.take("file_info").isArray()){
                QJsonArray array = ojson.take("file_info").toArray();
                QVariantList alist = array.toVariantList();
                for(int i = 0;i < alist.size();i++){
                    QString tmp = alist.at(i).toString();
                    setRepolist(tmp);
                }
            }
        }

    }
}

void datahandler::setRepolist(QString strname)
{
    repolist.insert(0,strname);
}
