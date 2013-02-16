/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include "channel.h"
#include "core.h"
#include "convert.h"
#include "highlight.h"
#include "log.h"
#include "mainwindow.h"
#include "nick.h"
#include "settings.h"
#include "sound_notify.h"
#include "tray.h"
#include "message.h"

#include <QDateTime>

Message * Message::Instance = 0;

Message * Message::instance()
{
    if (!Instance)
    {
        Instance = new Message();
    }

    return Instance;
}

Message::Message()
{
}

bool Message::isHighlightMessage(const QString &strMessage)
{
    QString strMe = Settings::instance()->get("nick");
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

void Message::saveMessage(const QString &strChannel, const QString &strData, qint64 iTime, QString strNick)
{
    QString strSaveData;
    if (!strNick.isEmpty())
        strSaveData = QString("%1 <%2> %3").arg(QDateTime::fromMSecsSinceEpoch(iTime).toString("[hh:mm:ss]"), strNick, strData);
    else
        strSaveData = QString("%1 %2").arg(QDateTime::fromMSecsSinceEpoch(iTime).toString("[hh:mm:ss]"), strData);

    // fix /me
    Convert::fixMeAction(strSaveData);

    Log::save(strChannel, strSaveData);
}

bool Message::isHideJoinPart(const QString &strChannel, MessageCategory eMessageCategory)
{
    if ((eMessageCategory == MessageJoin) || (eMessageCategory == MessagePart) || (eMessageCategory == MessageQuit))
    {
        if (Settings::instance()->get("hide_join_part") == "true")
            return true;

        int iNickCount = Nick::instance()->getFromChannel(strChannel).size();
        if ((Settings::instance()->get("hide_join_part_200") == "true") && (iNickCount > 200))
            return true;
    }
    return false;
}

void Message::showMessage(const QString &strChannel, const QString &strData, MessageCategory eMessageCategory, QString strNick, qint64 iTime)
{
    if (!Channel::instance()->contains(strChannel))
        return;

    // hide join part
    if ((isHideJoinPart(strChannel, eMessageCategory)) && (strChannel != DEBUG_WINDOW))
        return;

    // time
    if (iTime == 0)
        iTime = QDateTime::currentMSecsSinceEpoch();

    // highlight
    if ((eMessageCategory == MessageDefault) && (isHighlightMessage(strData)) && (strChannel != DEBUG_WINDOW))
    {
        // update message category
        eMessageCategory = MessageHighlight;

        // awaylog
        QString strAwaylogData = strData;
        if (!strNick.isEmpty())
            strAwaylogData = QString("<%1> %2").arg(strNick, strData);

        // awaylog
        if (Settings::instance()->get("away") == "true")
            Awaylog::instance()->add(iTime, strChannel, strAwaylogData);

        // tray
        if (Settings::instance()->get("tray_message") == "true")
            Tray::instance()->showMessage(strChannel, strAwaylogData);

        // sound
        if (Settings::instance()->get("disable_sounds") == "false")
            SoundNotify::instance()->play(Beep);
    }

    // change tab color
    if (strChannel != DEBUG_WINDOW)
        Core::instance()->mainWindow()->setTabColor(strChannel, eMessageCategory);

    // save message
    if (Settings::instance()->get("disable_logs") == "false")
        saveMessage(strChannel, strData, iTime, strNick);

    // display
    Channel::instance()->getChatView(strChannel)->displayMessage(strData, eMessageCategory, iTime, strNick);
}

void Message::showMessageAll(const QString &strData, MessageCategory eMessageCategory)
{
    QList<QString> lChannels = Channel::instance()->getList();

    foreach (QString strChannel, lChannels)
    {
        showMessage(strChannel, strData, eMessageCategory);
    }
}

void Message::showMessageActive(const QString &strData, MessageCategory eMessageCategory)
{
    QString strChannel = Channel::instance()->getCurrentName();

    showMessage(strChannel, strData, eMessageCategory);
}
