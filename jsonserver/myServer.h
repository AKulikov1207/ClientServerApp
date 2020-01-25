#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

class myServer : public QTcpServer
{
    Q_OBJECT
public:
    myServer();
    ~myServer();

    QTcpSocket* socket;
    QByteArray Data;
    QByteArray itog;

    QJsonDocument doc;
    QJsonParseError docError;

    QSqlDatabase db;

public slots:
    void startServer();
    void incomingConnection(int socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // MYSERVER_H
