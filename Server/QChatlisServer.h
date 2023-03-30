#pragma once

#include <QSslServer>
#include <QList>
#include "QClientConnection.h"
#include <QString>

class QChatlisServer : public QSslServer
{
    Q_OBJECT

public:
    QChatlisServer(QObject* parent = nullptr);
    ~QChatlisServer();

    const static quint16 PORT_NB;

signals:
    void serverLog(const QString& message);

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
};