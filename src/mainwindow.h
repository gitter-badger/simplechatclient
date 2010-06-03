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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QtGui/QMainWindow>
#include "config.h"
#include "crypt.h"
#include "dlg_about.h"
#include "dlg_channel_favourites.h"
#include "dlg_channel_homes.h"
#include "dlg_channel_list.h"
#include "dlg_channel_settings.h"
#include "dlg_friends.h"
#include "dlg_ignore.h"
#include "dlg_moderation.h"
#include "dlg_options.h"
#include "irc_auth.h"
#include "irc_kernel.h"
#include "network.h"
#include "tab_container.h"
#include "tab_manager.h"
#include "update_thread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void set_debug(bool);

public slots:
    void kernel(QString);
    void request_uo(QString, QString);
    void show_msg_active(QString, int);
    void show_msg_all(QString, int);
    void update_nick(QString);
    void clear_nicklist(QString);

private:
    QSettings settings;
    tab_manager *tabm;
    tab_container *tabc;
    network *pNetwork;
    dlg_channel_favourites *dlgchannel_favourites;
    dlg_channel_homes *dlgchannel_homes;
    dlg_channel_list *dlgchannel_list;
    dlg_channel_settings *dlgchannel_settings;
    dlg_friends *dlgfriends;
    dlg_ignore *dlgignore;
    dlg_moderation *dlgmoderation;
    QList <update_thread*> uThreadList;
    irc_kernel *pIrc_kernel;
    irc_auth *pIrc_auth;
    QMenu *trayMenu;
    QSystemTrayIcon *trayIcon;
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *chatMenu;
    QMenu *helpMenu;
    QToolBar *toolBar;
    QAction *showAct;
    QAction *connectAct;
    QAction *closeAct;
    QAction *optionsAct;
    QAction *channel_list_Act;
    QAction *channel_homes_Act;
    QAction *channel_favourites_Act;
    QAction *friendsAct;
    QAction *ignoreAct;
    QAction *aboutAct;

private slots:
    void button_close();
    void button_connect();
    void options_dlg();
    void channel_list_dlg();
    void channel_homes_dlg();
    void channel_favourites_dlg();
    void friends_dlg();
    void ignore_dlg();
    void about_dlg();
    void tray_icon(QSystemTrayIcon::ActivationReason);
    void button_show();
    void tab_close_requested(int);
    void remove_uthread(update_thread*);

signals:
    void do_kernel(QString);
    void do_request_uo(QString, QString);

};

#endif // MAINWINDOW_H
