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

#ifndef NETWORK_H
#define NETWORK_H

#include <QAction>
#include <QObject>
#include <QSettings>
#include "network_thread.h"

/**
 * Network class
 */
class Network : public QObject
{
    Q_OBJECT
public:
    Network(QAction *, QAction *, QString, int);
    ~Network();
    bool is_connected();
    bool is_writable();
    void connect();
    void close();
    void send(QString);
    void clear_queue();

public slots:
    void slot_kernel(QString);
    void slot_request_uo(QString, QString, QString);
    void slot_show_msg_active(QString, int);
    void slot_show_msg_all(QString, int);
    void slot_update_nick(QString);
    void slot_clear_nicklist(QString);
    void slot_clear_all_nicklist();

    void slot_send(QString);

private:
    NetworkThread *networkThr;
    QAction *connectAct;
    QAction *lagAct;
    QString strServer;
    int iPort;

signals:
    void kernel(QString);
    void request_uo(QString, QString, QString);
    void show_msg_active(QString, int);
    void show_msg_all(QString, int);
    void update_nick(QString);
    void clear_nicklist(QString);
    void clear_all_nicklist();

    void sconnect();
    void sclose();
    void ssend(QString);
    void sclear_queue();

};

#endif // NETWORK_H
