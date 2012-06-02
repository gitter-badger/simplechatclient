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

#include "awaylog_model.h"
#include "core.h"
#include "convert.h"
#include "highlight.h"
#include "log.h"
#include "mainwindow.h"
#include "nicklist.h"
#include "notify.h"
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

    QStringList lHighlight = Highlight::instance()->get();
    lHighlight.append(strMe);

    foreach (QString strData, lData)
    {
        Convert::simpleConvert(strData);

        foreach (QString strHighlight, lHighlight)
        {
            if (strData.toLower().contains(strHighlight.toLower()))
                return true;
        }
    }

    return false;
}

void Message::saveMessage(const QString &strChannel, const QString &strData, int iTime, QString strNick)
{
    if (Core::instance()->settings.value("disable_logs") == "true")
        return;

    QString strSaveData;
    if (!strNick.isEmpty())
        strSaveData = QString("%1 <%2> %3").arg(QDateTime::fromTime_t(iTime).toString("[hh:mm:ss]"), strNick, strData);
    else
        strSaveData = QString("%1 %2").arg(QDateTime::fromTime_t(iTime).toString("[hh:mm:ss]"), strData);

    // fix /me
    QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01")), "(.*)", QString(QByteArray("\x01")));
    if (strSaveData.contains(QRegExp(strRegExpMe)))
        strSaveData.replace(QRegExp(strRegExpMe), "\\1");

    Log::save(strChannel, strSaveData);
}

bool Message::isHideJoinPart(const QString &strChannel, MessageCategory eMessageCategory)
{
    if ((eMessageCategory == MessageJoin) || (eMessageCategory == MessagePart) || (eMessageCategory == MessageQuit))
    {
        if (Core::instance()->settings.value("hide_join_part") == "true")
            return true;

        int iNickCount = Nicklist::instance()->getUserCount(strChannel);
        if ((Core::instance()->settings.value("hide_join_part_200") == "true") && (iNickCount > 200))
            return true;
    }
    return false;
}

void Message::showMessage(const QString &strChannel, const QString &strData, MessageCategory eMessageCategory, QString strNick, int iTime)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    // hide join part
    if ((isHideJoinPart(strChannel, eMessageCategory)) && (strChannel != DEBUG))
        return;

    // time
    if (iTime == 0)
        iTime = (int)QDateTime::currentDateTime().toTime_t();

    // highlight
    if ((isHighlightMessage(strData)) && (eMessageCategory == MessageDefault) && (strChannel != DEBUG))
    {
        // update message category
        eMessageCategory = MessageHighlight;

        // awaylog
        QString strAwaylogData = strData;
        if (!strNick.isEmpty())
            strAwaylogData = QString("<%1> %2").arg(strNick, strData);

        // awaylog
        Awaylog::instance()->add(iTime, strChannel, strAwaylogData);

        // tray
        Tray::instance()->showMessage(strChannel, strAwaylogData);

        // sound
        if (Core::instance()->settings.value("disable_sounds") == "false")
            Notify::instance()->play(Beep);
    }

    // change tab color
    if (strChannel != DEBUG)
        Core::instance()->mainWindow()->setTabColor(strChannel, eMessageCategory);

    // save message
    saveMessage(strChannel, strData, iTime, strNick);

    // display
    Core::instance()->tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory, iTime, strNick);
}

void Message::showMessageAll(const QString &strData, MessageCategory eMessageCategory)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        showMessage(strChannel, strData, eMessageCategory);
    }
}

void Message::showMessageActive(const QString &strData, MessageCategory eMessageCategory)
{
    QString strChannel = Core::instance()->getCurrentChannelName();

    showMessage(strChannel, strData, eMessageCategory);
}
