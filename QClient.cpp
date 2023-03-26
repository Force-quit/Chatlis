#include "QClient.h"

QClient::QClient(QObject *parent)
	: QObject(parent), computerName(qEnvironmentVariable("USERDOMAIN")),
    username(qEnvironmentVariable("USERNAME"))
{
    if (username.isEmpty())
        username = qEnvironmentVariable("USER");
}

QString QClient::nickName() const
{
    return username + '@' + computerName;
}

void QClient::setUsername(const QString& newName)
{

}

void QClient::setComputerName(const QString& newComputerName)
{

}

QClient::~QClient() {}