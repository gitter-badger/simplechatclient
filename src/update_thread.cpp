/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

UpdateThreadClass::UpdateThreadClass(QSettings *param1, TabContainer *param2)
{
    settings = param1;
    tabc = param2;
    pUpdater = new Updater(settings, tabc);
}

UpdateThreadClass::~UpdateThreadClass()
{
    delete pUpdater;
}

void UpdateThreadClass::run()
{
    QTimer::singleShot(0, this, SLOT(thread_work()));

    exec();
}

void UpdateThreadClass::thread_work()
{
    QString strVersion = pUpdater->get_available_version();

    emit set_version(strVersion);
    emit stop_thread();
}

void UpdateThreadClass::check_for_updates(QString param1)
{
    pUpdater->check_for_updates(param1);
}

UpdateThread::UpdateThread(QSettings *param1, TabContainer *param2)
{
    settings = param1;
    tabc = param2;

    updateThr = new UpdateThreadClass(settings, tabc);
    QObject::connect(updateThr, SIGNAL(set_version(QString)), this, SLOT(setVersion(QString)));
    QObject::connect(updateThr, SIGNAL(stop_thread()), this, SLOT(stop_thread()));
    updateThr->start(QThread::LowPriority);
}

void UpdateThread::setVersion(QString param1)
{
    updateThr->check_for_updates(param1);
}

void UpdateThread::stop_thread()
{
    updateThr->quit();
    updateThr->wait();
    updateThr->deleteLater();
    updateThr->QObject::disconnect();
    delete updateThr;

    emit do_remove_uthread(this);
}
