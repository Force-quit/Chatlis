#pragma once

#include <QTcpServer>
#include <QMap>
#include "QClientConnection.h"
#include <QTcpSocket>
#include <QString>

class QChatlisServer : public QTcpServer
{
    Q_OBJECT

public:
    QChatlisServer(QObject* parent = nullptr);
    ~QChatlisServer();

    quint16 listeningPort() const;
signals:
    void newOutput(const QString& message);

private slots:
    void incomingConnection();
    void messageReceived(QString message, QClientConnection* sender);
    void clientDisconnected(QClientConnection* disconnectedClient);

private:
    const static quint16 PORT_NB;
    quint16 currentListeningPort;
    QMap<QString, QClientConnection*> connectedClients;
};