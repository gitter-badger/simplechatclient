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
#include "busy.h"

Busy * Busy::Instance = 0;

Busy * Busy::instance()
{
    if (!Instance)
    {
        Instance = new Busy();
        Instance->init();
    }

    return Instance;
}

Busy::Busy()
{
    busyAction = new QAction(this);
    busyAction->setIcon(QIcon(":/images/oxygen/16x16/im-user-offline.png"));
    busyAction->setText(tr("Mark as busy"));
    busyAction->setCheckable(true);
    busyAction->setChecked(false);
}

void Busy::init()
{
    connect(busyAction, SIGNAL(triggered()), this, SLOT(busyTriggered()));
}

void Busy::start()
{
    Core::instance()->settings["busy"] = "true";
    busyAction->setChecked(true);
}

void Busy::stop()
{
    Core::instance()->settings["busy"] = "false";
    busyAction->setChecked(false);
}

void Busy::busyTriggered()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Settings::instance()->get("logged") == "true"))
    {
        bool bBusy = Settings::instance()->get("busy") == "true" ? true : false;

        if (bBusy)
            Core::instance()->pNetwork->send("BUSY 0");
        else
            Core::instance()->pNetwork->send("BUSY 1");
    }
}
