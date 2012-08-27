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

#include <QAction>
#include "core.h"
#include "away.h"

Away * Away::Instance = 0;

Away * Away::instance()
{
    if (!Instance)
    {
        Instance = new Away();
        Instance->init();
    }

    return Instance;
}

Away::Away()
{
    awayAction = new QAction(this);
    awayAction->setIcon(QIcon(":/images/oxygen/16x16/im-user-away.png"));
    awayAction->setText(tr("Mark as away"));
    awayAction->setCheckable(true);
    awayAction->setChecked(false);
}

void Away::init()
{
    connect(awayAction, SIGNAL(triggered()), this, SLOT(awayTriggered()));
}

void Away::start()
{
    Core::instance()->settings["away"] = "true";
    awayAction->setChecked(true);
}

void Away::stop()
{
    Core::instance()->settings["away"] = "false";
    awayAction->setChecked(false);
}

void Away::awayTriggered()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->settings.value("logged") == "true"))
    {
        bool bAway = Core::instance()->settings.value("away") == "true" ? true : false;

        QString strReason;
        if (bAway)
            strReason = "";
        else
            strReason = tr("Not here right now");

        Core::instance()->pNetwork->send(QString("AWAY :%1").arg(strReason));
    }
}
