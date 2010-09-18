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

#ifndef UPDATE_H
#define UPDATE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QThread>
#include "dlg_update.h"
#include "tab_container.h"

class UpdateThread : public QThread
{
    Q_OBJECT
public:
    UpdateThread();
    void run();

private slots:
    void thread_work();

signals:
    void version(QString);
    void stop_thread();

};

class Update : public QObject
{
    Q_OBJECT
public:
    Update(QWidget *, TabContainer *);

private:
    QWidget *myparent;
    TabContainer *tabc;
    UpdateThread *updateThr;

public slots:
    void version(QString);
    void stop_thread();

signals:
    void do_remove_uthread(Update*);

};

#endif // UPDATE_H
