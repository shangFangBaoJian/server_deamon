#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include "server.h"
#include "tcpserver.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  printf("string....\n");
  /*
     QCommandLineParser parser;
     parser.setApplicationDescription("QtWebSockets example: echoserver");
     parser.addHelpOption();

     QCommandLineOption dbgOption(QStringList() << "d" << "debug",
     QCoreApplication::translate("main", "Debug output [default: off]."));
     parser.addOption(dbgOption);
     QCommandLineOption portOption(QStringList() << "p" << "port",
     QCoreApplication::translate("main", "Port for echoserver [default: 1234]."),
     QCoreApplication::translate("main", "port"), QLatin1Literal("1234"));
     parser.addOption(portOption);
     parser.process(a);
     bool debug = parser.isSet(dbgOption);
     int port = parser.value(portOption).toInt();
     */
/* if (argc < 2){
      printf("parameter less,need input port\n");
      exit(-1);
  }

  QString port = argv[1];
  server *myserver = new server(port.toUInt(), true);
  QObject::connect(myserver, &server::closed, &a, &QCoreApplication::quit);
*/
  TcpServer server;
  return a.exec();
}
