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

#ifndef UPDATE_THREAD_H
#define UPDATE_THREAD_H

#include <QObject>
#include <QSettings>
#include <QThread>
#include "tab_container.h"
#include "update.h"

class updateThread : public QThread
{
    Q_OBJECT
public:
    updateThread(QSettings *, tab_container *);
    ~updateThread();
    void run();
    void check_for_updates(QString);

private:
    QSettings *settings;
    tab_container *tabc;
    updater *pUpdater;

private slots:
    void threadWork();

signals:
    void set_version(QString);

};

class update_thread : public QObject
{
    Q_OBJECT
public:
    update_thread(QSettings *, tab_container *);
    ~update_thread();

private:
    QSettings *settings;
    tab_container *tabc;
    updateThread *updateThr;

private slots:
    void setVersion(QString);

};

#endif // UPDATE_THREAD_H
