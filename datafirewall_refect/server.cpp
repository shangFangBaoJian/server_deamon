#include "server.h"
#include "business.h"
using namespace std;

QT_USE_NAMESPACE

server::server(quint16 port ,bool debug, QObject *parent) : QObject(parent),
  m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                          QWebSocketServer::NonSecureMode,this)),
  m_debug(1)
{
  Q_UNUSED(debug);
  if(m_pWebSocketServer->listen(QHostAddress::Any,port)){
      qDebug() << "bing port :" << port;
    connect(m_pWebSocketServer,&QWebSocketServer::newConnection,this,&server::onNewConnection);
    connect(m_pWebSocketServer,&QWebSocketServer::closed,this,&server::closed);

  }
}

server::~server()
{
  m_pWebSocketServer->close();
  qDeleteAll(m_client.begin(),m_client.end());
}

void server::onNewConnection()
{
  QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
  connect(pSocket,&QWebSocket::textMessageReceived,this,&server::processTextMessage);
  connect(pSocket,&QWebSocket::binaryMessageReceived,this,&server::processBinaryMessage);
  connect(pSocket,&QWebSocket::disconnected,this,&server::socketDisconnected);
    qDebug() << "has a new connect";
  m_client << pSocket;
}

void server::processTextMessage(QString message)
{
  QString data;
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
  if (m_debug){
    qDebug() << "Message received:" << message;
    data = business::recv(message);
  }
  if (pClient) {
      qDebug() << "Message send:" << data;
      pClient->sendTextMessage(data);
  }
}

void server::processBinaryMessage(QByteArray message)
{
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
  if (m_debug)
    qDebug() << "Binary Message received:" << message;
  if (pClient) {
    pClient->sendBinaryMessage(message);
  }
}

void server::socketDisconnected()
{
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
  if (m_debug)
    qDebug() << "socketDisconnected:" << pClient;
  if (pClient) {
    m_client.removeAll(pClient);
    pClient->deleteLater();
  }
}

