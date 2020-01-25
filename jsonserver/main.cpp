#include <QCoreApplication>
#include "myServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    myServer Server;
    Server.startServer();

    return a.exec();
}
