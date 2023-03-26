#pragma once

#include <QCborStreamReader>
#include <QCborStreamWriter>
#include <QElapsedTimer>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTimer>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    enum class ConnectionState 
    {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };

    enum DataType 
    {
        PlainText,
        Ping,
        Pong,
        Greeting,
        Undefined
    };

    Connection(QObject* parent = nullptr);
    Connection(qintptr socketDescriptor, QObject* parent = nullptr);
    ~Connection();

    QString name() const;
    void setGreetingMessage(const QString& message);
    bool sendMessage(const QString& message);

signals:
    void readyForUse();
    void newMessage(const QString& from, const QString& message);

protected:
    void timerEvent(QTimerEvent* timerEvent) override;

private slots:
    void processReadyRead();
    void sendPing();
    void sendGreetingMessage();

private:
    bool hasEnoughData();
    void processGreeting();
    void processData();

    QCborStreamReader reader;
    QCborStreamWriter writer;
    QString greetingMessage;
    QString username;
    QTimer pingTimer;
    QElapsedTimer pongTime;
    QString buffer;
    ConnectionState state;
    DataType currentDataType;
    int transferTimerId;
    bool isGreetingMessageSent;
};