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

#include <QAction>
#include <QDateTime>
#include "channel.h"
#include "convert.h"
#include "log.h"
#include "notification.h"
#include "settings.h"
#include "awaylog_model.h"

Awaylog * Awaylog::Instance = 0;

Awaylog * Awaylog::instance()
{
    if (!Instance)
    {
        Instance = new Awaylog();
        Instance->init();
    }

    return Instance;
}

Awaylog::Awaylog()
{
    awaylogAction = new QAction(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Awaylog"), this);
    awaylogAction->setVisible(false);
}

void Awaylog::init()
{
    lAwaylog.clear();
}

void Awaylog::add(qint64 iTime, const QString &strChannel, const QString &strData)
{
    // channel
    QString strAwayLogChannel = strChannel;

    // fix priv
    if ((strAwayLogChannel[0] == '^') && (Channel::instance()->containsAlternativeName(strAwayLogChannel)))
        strAwayLogChannel = Channel::instance()->getAlternativeName(strAwayLogChannel);

    // data
    QString strAwayLogData = strData;

    // fix /me
    QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01")), "(.*)", QString(QByteArray("\x01")));
    if (strAwayLogData.contains(QRegExp(strRegExpMe)))
    {
        strAwayLogData.replace(QRegExp(strRegExpMe), "\\1");
        if (strAwayLogData.contains("<")) strAwayLogData = strAwayLogData.remove(strAwayLogData.indexOf("<"),1);
        if (strAwayLogData.contains(">")) strAwayLogData = strAwayLogData.remove(strAwayLogData.indexOf(">"),1);
        strAwayLogData = "*"+strAwayLogData;
    }

    Convert::simpleConvert(strAwayLogData);

    // save awaylog
    if (Settings::instance()->get("disable_logs") == "false")
    {
        QString strAwaylogFileData = QString("%1 %2 %3").arg(QDateTime::fromMSecsSinceEpoch(iTime).toString("[yyyy-MM-dd] [hh:mm:ss]"), strAwayLogChannel, strData);
        Log::save("awaylog", strAwaylogFileData);
    }

    strAwayLogData = QString("%1\n%2 %3").arg(strAwayLogChannel, QDateTime::fromMSecsSinceEpoch(iTime).toString("[hh:mm:ss]"), strAwayLogData);

    if (!lAwaylog.contains(strAwayLogData))
    {
        lAwaylog.append(strAwayLogData);
        Notification::instance()->refreshAwaylog();
    }
}

QList<QString> Awaylog::get()
{
    return lAwaylog;
}

void Awaylog::clear()
{
    lAwaylog.clear();
    Notification::instance()->refreshAwaylog();
}

bool Awaylog::isEmpty()
{
    return lAwaylog.isEmpty();
}

int Awaylog::count()
{
    return lAwaylog.size();
}
