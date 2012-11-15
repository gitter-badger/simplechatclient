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

void Nicklist::clearUsers()
{
    QList<CaseIgnoreString> lChannels = Channel::instance()->getListClearedSorted();
    foreach (QString strChannel, lChannels)
    {
        Channel::instance()->getNickListWidget(strChannel)->clear();
        int iUsersCount = Channel::instance()->getNickListWidget(strChannel)->count();
        Channel::instance()->getUsers(strChannel)->setText(QString(tr("Users (%1)").arg(iUsersCount)));
    }
}

void Nicklist::addUser(const QString &strNick, const QString &strChannel, const QString &strModes, int iMaxModes, const QString &strAvatar)
{
    if (!Channel::instance()->contains(strChannel)) return;

    Channel::instance()->getNickListWidget(strChannel)->addUser(strNick, strModes, iMaxModes, strAvatar);
    int iUsersCount = Channel::instance()->getNickListWidget(strChannel)->count();
    Channel::instance()->getUsers(strChannel)->setText(QString(tr("Users (%1)").arg(iUsersCount)));
}

void Nicklist::delUser(const QString &strNick, const QString &strChannel)
{
    if (!Channel::instance()->contains(strChannel)) return;

    Channel::instance()->getNickListWidget(strChannel)->delUser(strNick);
    int iUsersCount = Channel::instance()->getNickListWidget(strChannel)->count();
    Channel::instance()->getUsers(strChannel)->setText(QString(tr("Users (%1)").arg(iUsersCount)));
}

void Nicklist::renameUser(const QString &strNick, const QString &strNewNick, const QList<QString> &lChannels, const QString &strDisplay)
{
    MessageCategory eMessageCategory = MessageMode;

    foreach (QString strChannel, lChannels)
    {
        Message::instance()->showMessage(strChannel, strDisplay, eMessageCategory);
        Channel::instance()->getNickListWidget(strChannel)->renameUser(strNick, strNewNick);
    }
}

void Nicklist::quitUser(const QString &strNick, const QList<QString> &lChannels, const QString &strDisplay)
{
    MessageCategory eMessageCategory = MessageQuit;

    foreach (QString strChannel, lChannels)
    {
        Message::instance()->showMessage(strChannel, strDisplay, eMessageCategory);
        Channel::instance()->getNickListWidget(strChannel)->delUser(strNick);
        int iUsersCount = Channel::instance()->getNickListWidget(strChannel)->count();
        Channel::instance()->getUsers(strChannel)->setText(QString(tr("Users (%1)").arg(iUsersCount)));
    }
}

void Nicklist::setUserModes(const QString &strNick, const QString &strChannel, const QString &strModes, int iMaxModes)
{
    if (!Channel::instance()->contains(strChannel)) return;

    Channel::instance()->getNickListWidget(strChannel)->setUserModes(strNick, strModes, iMaxModes);

    // channel tool buttons
    QString strMe = Settings::instance()->get("nick");
    QString strCurrentChannel = Channel::instance()->getCurrentName();

    if ((strCurrentChannel == strChannel) && (strNick == strMe))
        Core::instance()->mainWindow()->refreshToolButtons(strChannel);
}

void Nicklist::setUserAvatar(const QString &strNick, const QList<QString> &lChannels, const QString &strAvatar)
{
    foreach (QString strChannel, lChannels)
    {
        Channel::instance()->getNickListWidget(strChannel)->setUserAvatar(strNick, strAvatar);
    }
}
