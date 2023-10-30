#pragma once

#include <QSslServer>
#include <QList>
#include "../Headers/QClientConnection.h"
#include <QString>
#include <QTextStream>


class QChatlisServer : public QSslServer
{
    Q_OBJECT

public:
    QChatlisServer();
    ~QChatlisServer();

    const static quint16 PORT_NB;

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void clientDisconnected();
    void replicateClientMessage(const QString message);
    void replicateNewUser();
    void replicateClientNewUsername(const QString previousUsername);
    void replicateClientNewComputerName(const QString previousComputerName);
    void getNextPendingConnection();

private:
    QList<QClientConnection*> connectedClients;
    QTextStream cout;

    void serverLog(const QString& message);
};