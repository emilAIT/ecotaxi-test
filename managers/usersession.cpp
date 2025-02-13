#include "usersession.h"

userSession &userSession::getInstance()
{
    static userSession instance;
    return instance;
}

userSession::userSession(QObject *parent)
    : QObject{parent}
{
}

bool userSession::logMe(QString password)
{
    if (password == "meow")
    {
        isAdminSession = true;
    }
    else if (Operations::checkLoginUser(HASH::generateHMAC(password)))
    {
        isAdminSession = false;
    }
    else
    {
        return false;
    }
    return true;
}

const int userSession::getSecretKey()
{
    return secretKey;
}

bool userSession::checkIsAdmin()
{
    return isAdminSession;
}
