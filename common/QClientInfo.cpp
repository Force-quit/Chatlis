#include "QClientInfo.h"

QClientInfo::QClientInfo(bool isLocalClient)
	: computerName(), username()
{
    if (isLocalClient)
    {
        computerName = qEnvironmentVariable("USERDOMAIN");
        username = qEnvironmentVariable("USERNAME");
        if (username.isEmpty())
            username = qEnvironmentVariable("USER");
    }
}

void QClientInfo::setUsername(const QString& newName)
{
    username = newName;
}

void QClientInfo::setComputerName(const QString& newComputerName)
{
    computerName = newComputerName;
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