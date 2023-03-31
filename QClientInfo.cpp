#include "QClientInfo.h"

QClientInfo::QClientInfo(QObject *parent)
	: QObject(parent), computerName(qEnvironmentVariable("USERDOMAIN")),
    username(qEnvironmentVariable("USERNAME"))
{
    if (username.isEmpty())
        username = qEnvironmentVariable("USER");
}

void QClientInfo::setUsername(const QString& newName)
{
    username = newName;
    emit usernameChanged(newName);
}

void QClientInfo::setComputerName(const QString& newComputerName)
{
    computerName = newComputerName;
    emit computerNameChanged(newComputerName);
}

QString QClientInfo::getUsername() const
{
    return username;
}

QString QClientInfo::getComputerName() const
{
    return computerName;
}

QClientInfo::~QClientInfo() {}