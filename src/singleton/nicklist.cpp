/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QLabel>
#include "channel.h"
#include "core.h"
#include "mainwindow.h"
#include "message.h"
#include "nicklist_widget.h"
#include "settings.h"
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
    }

    return Instance;
}

Nicklist::Nicklist()
{
}

int Nicklist::getUserMaxModes(const QString &strModes)
{
    if (strModes.contains(FLAG_DEV)) { return 64; }
    if (strModes.contains(FLAG_ADMIN)) { return 32; }
    if (strModes.contains(FLAG_OWNER)) { return 16; }
    if (strModes.contains(FLAG_OP)) { return 8; }
    if (strModes.contains(FLAG_HALFOP)) { return 4; }
    if (strModes.contains(FLAG_MOD)) { return 2; }
    if (strModes.contains(FLAG_SCREENER)) { return 1; }
    if (strModes.contains(FLAG_VOICE)) { return 0; }

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

    strEmptyUserAvatar = path+"/images/user_avatar.png";
}

QString Nicklist::getEmptyUserAvatar()
{
    if (strEmptyUserAvatar.isEmpty())
        readEmptyUserAvatar();

    return strEmptyUserAvatar;
}

void Nicklist::addUser(const QString &strNick, const QString &strChannel, const QString &strModes)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    if (!Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
    {
        Core::instance()->tw[strChannel]->pNickListWidget->addUser(strNick, strModes);
        Core::instance()->tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(Core::instance()->tw[strChannel]->pNickListWidget->count())));
    }
}

void Nicklist::delUser(const QString &strNick, const QString &strChannel)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
    {
        Core::instance()->tw[strChannel]->pNickListWidget->delUser(strNick);
        Core::instance()->tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(Core::instance()->tw[strChannel]->pNickListWidget->count())));
    }
}

void Nicklist::renameUser(const QString &strNick, const QString &strNewNick, const QString &strDisplay)
{
    MessageCategory eMessageCategory = MessageMode;

    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
        {
            Message::instance()->showMessage(strChannel, strDisplay, eMessageCategory);
            Core::instance()->tw[strChannel]->pNickListWidget->renameUser(strNick, strNewNick);
        }
    }
}

void Nicklist::quitUser(const QString &strNick, const QString &strDisplay)
{
    MessageCategory eMessageCategory = MessageQuit;

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

    QString strMe = Settings::instance()->get("nick");
    QString strCurrentChannel = Channel::instance()->getCurrent();

    if ((!strCurrentChannel.isEmpty()) && (strCurrentChannel == strChannel) && (strNick == strMe))
        Core::instance()->mainWindow()->refreshToolButtons(strChannel);
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

void Nicklist::setUserAvatar(const QString &strNick, const QString &strValue)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
            Core::instance()->tw[strChannel]->pNickListWidget->setUserAvatar(strNick, strValue);
    }
}

QString Nicklist::getUserAvatar(const QString &strNick)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (Core::instance()->tw[strChannel]->pNickListWidget->existUser(strNick))
            return Core::instance()->tw[strChannel]->pNickListWidget->getUserAvatar(strNick);
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
