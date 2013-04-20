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
#include "core.h"
#include "settings.h"
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
    if (Settings::instance()->get("away") != "true")
    {
        Settings::instance()->set("away", "true");
        awayAction->setChecked(true);
    }
}

void Away::stop()
{
    if (Settings::instance()->get("away") != "false")
    {
        Settings::instance()->set("away", "false");
        awayAction->setChecked(false);
    }
}

void Away::awayTriggered()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
    {
        bool bAway = Settings::instance()->get("away") == "true" ? true : false;

        QString strReason;
        if (bAway)
            strReason = QString::null;
        else
            strReason = tr("Not here right now");

        Core::instance()->network->send(QString("AWAY :%1").arg(strReason));
    }
}
