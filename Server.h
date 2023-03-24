#pragma once

#include <QTcpServer>
#include "Connection.h"


class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject* parent = nullptr);

signals:
    void newConnection(Connection* connection);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};