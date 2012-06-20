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

#include "offline.h"

Offline * Offline::Instance = 0;

Offline * Offline::instance()
{
    if (!Instance)
    {
        Instance = new Offline();
        Instance->init();
    }

    return Instance;
}

Offline::Offline()
{
    offlineMsgAction = new QAction(QIcon(":/images/oxygen/16x16/mail-mark-unread.png") , tr("Offline messages"), this);
    offlineMsgAction->setShortcut(tr("Ctrl+M"));
}

void Offline::init()
{
    lOfflineNicks.clear();
    lOfflineMsg.clear();
}

void Offline::addMsg(int iTime, const QString &strType, const QString &strNick, const QString &strMessage)
{
    OnetOfflineMessage add;
    add.datetime = iTime;
    add.type = strType;
    add.nick = strNick;
    add.message = strMessage;

    lOfflineMsg.append(add);
}

void Offline::removeMsg(const QString &strNick)
{
    for (int i = 0; i < lOfflineMsg.size(); i++)
    {
        QString strOfflineNick = lOfflineMsg.at(i).nick;
        if (strOfflineNick == strNick)
            lOfflineMsg.takeAt(i);
    }
//    foreach (OfflineMsg msg, lOfflineMsg)
//    {
//        if (msg.nick == strNick)
//            lOfflineMsg.removeAll(msg);
//    }
}

void Offline::clearMsg()
{
    lOfflineNicks.clear();
}

QList<OnetOfflineMessage> Offline::getMsg()
{
    return lOfflineMsg;
}

bool Offline::isEmptyMsg()
{
    return lOfflineMsg.isEmpty();
}

void Offline::addNick(const QString &strNick)
{
    lOfflineNicks.append(strNick);

    // show
    if (!offlineMsgAction->isVisible())
        offlineMsgAction->setVisible(true);
}

void Offline::removeNick(const QString &strNick)
{
    lOfflineNicks.removeAll(strNick);

    // hide if no messages
    if (lOfflineNicks.size() == 0)
        offlineMsgAction->setVisible(false);
}

void Offline::clearNicks()
{
    lOfflineNicks.clear();
    offlineMsgAction->setVisible(false);
}

QList<QString> Offline::getNicks()
{
    return lOfflineNicks;
}
