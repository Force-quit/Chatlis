#pragma once

#include <QObject>
#include "Client.h"
#include <QString>
#include <QHostAddress>
#include "Connection.h"
#include <QList>
#include <QUdpSocket>
#include <QTimer>

class PeerManager : public QObject
{
    Q_OBJECT

public:
    PeerManager(Client* client);

    void setServerPort(int port);
    QString userName() const;
    void startBroadcasting();
    bool isLocalHostAddress(const QHostAddress& address) const;

signals:
    void newConnection(Connection* connection);

private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();

private:
    void updateAddresses();

    Client* client;
    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;
    QUdpSocket broadcastSocket;
    QTimer broadcastTimer;
    QString username;
    int serverPort;
};