#include "myServer.h"

myServer::myServer(){}
myServer::~myServer(){}

void myServer::startServer()
{
    if (this->listen(QHostAddress::Any, 5000))
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:\\Users\\kandr\\Documents\\workers.db");
        if (db.open())
        {
            qDebug() << "Listening and DB is open";
        }
        else
            qDebug() << "DB not open";
    } else {
        qDebug() << "Not listening!";
    }
}

void myServer::incomingConnection(int socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc));

    qDebug() << socketDescriptor << "Client connected";

    socket->write("{\"type\":\"connect\", \"status\":\"yes\"}");      //JSON
    qDebug() << "Send client connect status - YES";
}

void myServer::sockReady()
{
    Data = socket->readAll();
    qDebug() << "Data: " << Data;

    doc = QJsonDocument::fromJson(Data, &docError);

    if (docError.errorString().toInt() == QJsonParseError::NoError)
    {
        if ((doc.object().value("type").toString() == "reSize") && (doc.object().value("resp").toString() == "workers"))
        {
            itog = "{\"type\":\"resultSelect\", \"result\":[";
            // "{\"type\":\"resultSelect\", \"result\":" + fromFile + "}"
            if (db.isOpen())
            {
                QSqlQuery* query = new QSqlQuery(db);
                if (query->exec("SELECT name FROM listWorkers"))
                {
                    while (query->next()) {
                        itog.append("{\"name\":\"" + query->value(0).toString() + "\"},");
                    }

                    itog.remove(itog.length()-1, 1);
                }
                else
                    qDebug() << "Query not success";

                delete query;
            }
            itog.append("]}");

            socket->write("{\"type\":\"size\", \"resp\":\"workers\", \"length\":" + QByteArray::number(itog.size()) + "}");
            socket->waitForBytesWritten(500);
        }
        else if ((doc.object().value("type").toString() == "select") && (doc.object().value("params").toString() == "workers")) {
            socket->write(itog);
            qDebug() << "Size message: " << socket->bytesToWrite();
            socket->waitForBytesWritten(500);
        }
    }
}

void myServer::sockDisc()
{
    qDebug() << "Disconnect";
    socket->deleteLater();
}
