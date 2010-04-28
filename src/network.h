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
#include <QTcpSocket>
#include <QTimer>
#include "crypt.h"
#include "dlg_channel_favourites.h"
#include "dlg_channel_homes.h"
#include "dlg_channel_settings.h"
#include "dlg_friends.h"
#include "dlg_ignore.h"
#include "dlg_moderation.h"
#include "irc_auth.h"
#include "irc_kernel.h"
#include "tab_container.h"

class network : public QObject
{
    Q_OBJECT
public:
    network(QAction *, QSettings *);
    ~network();
    QTcpSocket* get_socket() { return socket; }
    void set_hostport(QString, int);
    void set_dlg(tab_container *, dlg_channel_settings *, dlg_channel_homes *, dlg_channel_list *, dlg_channel_favourites *, dlg_friends *, dlg_ignore *, dlg_moderation *);
    void set_reconnect(bool);
    bool is_connected();
    bool is_writable();
    void connect();
    void close();
    void send(QString);
    void kernel(QString);

private:
    QTcpSocket *socket;
    tab_container *tabc;
    QAction *connectAct;
    QString strServer;
    int iPort;
    QString strDataRecv;
    QSettings *settings;
    dlg_channel_settings *dlgchannel_settings;
    dlg_channel_homes *dlgchannel_homes;
    dlg_channel_list *dlgchannel_list;
    dlg_channel_favourites *dlgchannel_favourites;
    dlg_friends *dlgfriends;
    dlg_ignore *dlgignore;
    dlg_moderation *dlgmoderation;
    QTimer *timer;
    int iActive;
    irc_auth *pIrc_auth;
    irc_kernel *pIrc_kernel;

private slots:
    void reconnect();
    void recv();
    void disconnected();
    void timeout();

};

#endif // NETWORK_H
