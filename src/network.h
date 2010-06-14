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

#ifndef NETWORK_H
#define NETWORK_H

#include <QAction>
#include <QObject>
#include <QSettings>
#include "network_thread.h"

class Network : public QObject
{
    Q_OBJECT
public:
    Network(QWidget *, QAction *, QSettings *);
    ~Network();
    bool is_connected();
    bool is_writable();
    void connect();
    void close();
    void send(QString);

public slots:
    void send_slot(QString);

private:
    QAction *connectAct;
    QSettings *settings;
    NetworkThread *networkThr;

signals:
    void do_connect();
    void do_close();
    void do_send(QString);

};

#endif // NETWORK_H
