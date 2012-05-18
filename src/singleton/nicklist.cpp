/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 ****************************************************************************/

#include "core.h"
#include "message.h"
#include "nicklist.h"

#ifdef Q_WS_WIN
    #include <QCoreApplication>
#endif

Nicklist * Nicklist::Instance = 0;

Nicklist * Nicklist::instance()
{
    if (!Instance)
    {
        Instance = new Nicklist();
        Instance->init();
    }

    return Instance;
}

Nicklist::Nicklist()
{
}

void Nicklist::init()
{
}

int Nicklist::getUserMaxModes(const QString &strModes)
{
    if (strModes.contains("O")) { return 64; }
    if (strModes.contains("o")) { return 32; }
    if (strModes.contains("`")) { return 16; }
    if (strModes.contains("@")) { return 8; }
    if (strModes.contains("%")) { return 4; }
    if (strModes.contains("!")) { return 2; }
    if (strModes.contains("=")) { return 1; }
    if (strModes.contains("+")) { return 0; }

    return -1;
}

void Nicklist::readEmptyUserAvatar()
{
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = SCC_DATA_DIR;
#endif

    strEmptyUserAvatarPath = path+"/images/user_avatar.png";
}

QString Nicklist::getEmptyUserAvatarPath()
{
    if (strEmptyUserAvatarPath.isEmpty())
        readEmptyUserAvatar();

    return strEmptyUserAvatarPath;
}

void Nicklist::addUser(const QString &strChannel, const QString &strNick, const QString &strModes)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    Core::instance()->tw[strChannel]->pNickListWidget->addUser(strNick, strModes);
    Core::instance()->tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(Core::instance()->tw[strChannel]->pNickListWidget->count())));
}

void Nicklist::delUser(const QString &strChannel, const QString &strNick)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    Core::instance()->tw[strChannel]->pNickListWidget->delUser(strNick);
    Core::instance()->tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(Core::instance()->tw[strChannel]->pNickListWidget->count())));
}

void Nicklist::quitUser(const QString &strNick, const QString &strDisplay)
{
    MessageCategory eMessageCategory = QuitMessage;

    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
        {
            Message::instance()->showMessage(strChannel, strDisplay, eMessageCategory);
            Core::instance()->tw[strChannel]->pNickListWidget->delUser(strNick);
            Core::instance()->tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(Core::instance()->tw[strChannel]->pNickListWidget->count())));
        }
    }
}

void Nicklist::changeFlag(const QString &strNick, const QString &strChannel, const QString &strFlag)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
        Core::instance()->tw[strChannel]->pNickListWidget->changeUserFlag(strNick, strFlag);

    QString strCurrentChannel = Core::instance()->getCurrentChannelName();

    if ((!strCurrentChannel.isEmpty()) && (strCurrentChannel == strChannel))
    {
        QString strMe = Core::instance()->settings.value("nick");
        if (strNick == strMe)
        {
//            if (strFlag == "+X") emit setModeration(true);
//            else if (strFlag == "-X") emit setModeration(false);
        }
    }
}

void Nicklist::changeFlag(const QString &strNick, const QString &strFlag)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
            changeFlag(strNick, strChannel, strFlag);
    }
}

void Nicklist::clearAllNicklist()
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        Core::instance()->tw[strChannel]->pNickListWidget->clear();
        Core::instance()->tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(Core::instance()->tw[strChannel]->pNickListWidget->count())));
    }
}

void Nicklist::setUserAvatarPath(const QString &strNick, const QString &strValue)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
            Core::instance()->tw[strChannel]->pNickListWidget->setUserAvatarPath(strNick, strValue);
    }
}

QString Nicklist::getUserAvatarPath(const QString &strNick)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
            return Core::instance()->tw[strChannel]->pNickListWidget->getUserAvatarPath(strNick);
    }
    return QString::null;
}

int Nicklist::getUserCount(const QString &strChannel)
{
    if (Core::instance()->tw.contains(strChannel))
        return Core::instance()->tw[strChannel]->pNickListWidget->count();
    else
        return 0;
}

QString Nicklist::getUserModes(const QString &strNick, const QString &strChannel)
{
    if ((Core::instance()->tw.contains(strChannel)) && (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick)))
        return Core::instance()->tw[strChannel]->pNickListWidget->getUserModes(strNick);
    else
        return QString::null;
}

QList<QString> Nicklist::getUserList(const QString &strChannel)
{
    QList<QString> userList;

    if (Core::instance()->tw.contains(strChannel))
        userList = Core::instance()->tw[strChannel]->pNickListWidget->getUserList();

    return userList;
}
