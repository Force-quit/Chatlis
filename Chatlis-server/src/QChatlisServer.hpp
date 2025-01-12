#pragma once

#include <QSslServer>
#include <QList>
#include "QClientConnection.h"
#include <QString>

class QChatlisServer : public QSslServer
{
    Q_OBJECT

public:
    QChatlisServer();

    void start();

signals:
    void serverLog(const QString& message);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void clientDisconnected();
    void replicateClientMessage(const QString message);
    void replicateNewUser();
    void replicateClientChangedName();
    void getNextPendingConnection();

private:
    inline static constexpr quint16 PORT_NB{ 59532 };
    void displayIpAddresses();
    QList<QClientConnection*> mConnectedClients;
};