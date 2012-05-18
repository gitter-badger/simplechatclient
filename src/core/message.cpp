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

#include "awaylogs.h"
#include "core.h"
#include "convert.h"
#include "log.h"
#include "nicklist.h"
#include "tray.h"
#include "message.h"

#include <QDateTime>

Message * Message::Instance = 0;

Message * Message::instance()
{
    if (!Instance)
    {
        Instance = new Message();
        Instance->init();
    }

    return Instance;
}

Message::Message()
{
}

void Message::init()
{
}

bool Message::isHighlightMessage(const QString &strMessage)
{
    QString strMe = Core::instance()->settings["nick"];
    QStringList lData = strMessage.split(" ");
    QStringList lHighlight = Core::instance()->settings["highlight"].split(";", QString::SkipEmptyParts);
    lHighlight.append(strMe);

    for (int i = 0; i < lData.size(); i++)
    {
        QString strData = lData[i];
        Convert::simpleConvert(strData);

        QStringListIterator lHighlightIterator(lHighlight);
        while (lHighlightIterator.hasNext())
        {
            if (strData.toLower().contains(lHighlightIterator.next().toLower()))
                return true;
        }
    }
    return false;
}

void Message::saveMessage(const QString &strChannel, const QString &strData, QString strTime, QString strNick)
{
    QDateTime dt;
    if (!strTime.isEmpty())
        dt = QDateTime::fromTime_t(strTime.toInt());
    else
        dt = QDateTime::currentDateTime();

    if (Core::instance()->settings.value("disable_logs") == "false")
    {
        QString strSaveData;
        if (!strNick.isEmpty())
            strSaveData = QString("%1 <%2> %3").arg(dt.toString("[hh:mm:ss]"), strNick, strData);
        else
            strSaveData = QString("%1 %2").arg(dt.toString("[hh:mm:ss]"), strData);

        // fix /me
        QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01")), "(.*)", QString(QByteArray("\x01")));
        if (strSaveData.contains(QRegExp(strRegExpMe)))
            strSaveData.replace(QRegExp(strRegExpMe), "\\1");

        Log::save(strChannel, strSaveData);
    }
}

bool Message::hideJoinPart(const QString &strChannel, MessageCategory eMessageCategory)
{
    if ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage))
    {
        if (Core::instance()->settings.value("hide_join_part") == "true")
            return true;

        int iNickCount = Nicklist::instance()->getUserCount(strChannel);
        if ((Core::instance()->settings.value("hide_join_part_200") == "true") && (iNickCount > 200))
            return true;
    }
    return false;
}

void Message::showMessage(const QString &strChannel, const QString &strData, MessageCategory eMessageCategory, QString strTime, QString strNick)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    // hide join part
    if (hideJoinPart(strChannel, eMessageCategory))
        return;

//    int index = Core::instance()->getIndexFromChannelName(strChannel);

    // highlight
    if ((isHighlightMessage(strData)) && (eMessageCategory == DefaultMessage))
    {
        QString strAwaylogData = strData;
        if (!strNick.isEmpty())
            strAwaylogData = QString("<%1> %2").arg(strNick, strData);

        // awaylog
        Awaylog::instance()->add(strTime, strChannel, strAwaylogData);

        // update awaylog status
//        emit updateAwaylogStatus();

        // update message category
        eMessageCategory = HighlightMessage;

        // highlight
//        pTabM->setAlert(index, ChannelHighlight);

        // tray
        Tray::instance()->showMessage(strChannel, strAwaylogData);
    }

    // set color
//    if (eMessageCategory != DefaultMessage)
//        pTabM->setAlert(index, ChannelGreen);
//    else
//        pTabM->setAlert(index, ChannelRed);

    // save message
    saveMessage(strChannel, strData, strTime, strNick);

    // display
    Core::instance()->tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory, strTime, strNick);
}

void Message::showMessageAll(const QString &strData, MessageCategory eMessageCategory)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        // hide join part
        if (!hideJoinPart(strChannel, eMessageCategory))
        {
//            int index = Core::instance()->getIndexFromChannelName(strChannel);

//            if (eMessageCategory != DefaultMessage)
//                pTabM->setAlert(index, ChannelGreen);
//            else
//                pTabM->setAlert(index, ChannelRed);

            // save message
            saveMessage(strChannel, strData);

            // display
            Core::instance()->tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory);
        }
    }
}

void Message::showMessageActive(const QString &strData, MessageCategory eMessageCategory)
{
    QString strChannel = Core::instance()->getCurrentChannelName();

    if (strChannel.isEmpty())
        return;

    // hide join part
    if (hideJoinPart(strChannel, eMessageCategory))
        return;

    // save message
    saveMessage(strChannel, strData);

    // display
    Core::instance()->tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory);
}
