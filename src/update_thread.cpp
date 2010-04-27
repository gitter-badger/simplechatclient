/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#include "update_thread.h"

updateThread::updateThread(QSettings *param1, tab_container *param2)
{
    settings = param1;
    tabc = param2;
    pUpdater = new updater(settings, tabc);
}

updateThread::~updateThread()
{
    delete pUpdater;
}

void updateThread::run()
{
    QTimer::singleShot(0, this, SLOT(doTheWork()));

    exec();
}

void updateThread::doTheWork()
{
    QString strVersion = pUpdater->get_available_version();
    emit set_version(strVersion);
}

void updateThread::check_for_updates(QString param1)
{
    pUpdater->check_for_updates(param1);
}

update_thread::update_thread(QSettings *param1, tab_container *param2)
{
    settings = param1;
    tabc = param2;

    updateThr = new updateThread(settings, tabc);
    QObject::connect(updateThr, SIGNAL(set_version(QString)), this, SLOT(setVersion(QString)));
    updateThr->start();
}

update_thread::~update_thread()
{
    delete updateThr;
}

void update_thread::setVersion(QString param1)
{
    updateThr->check_for_updates(param1);
}
