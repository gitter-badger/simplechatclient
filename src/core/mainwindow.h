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
#ifdef Q_WS_WIN
#include "kamerzysta.h"
#endif
#include "network.h"
#include "onet_auth.h"
#include "onet_kernel.h"
#include "notify.h"
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
    void set_lag(QString);
    void kernel(QString);
    void request_uo(QString, QString, QString);
    void show_msg_active(QString, int);
    void show_msg_all(QString, int);
    void update_nick(QString);
    void clear_nicklist(QString);
    void clear_all_nicklist();

private:
    QTcpSocket *camSocket;
    TabManager *pTabM;
    TabContainer *pTabC;
    Network *pNetwork;
    DlgChannelFavourites *pDlg_channel_favourites;
    DlgChannelHomes *pDlg_channel_homes;
    DlgChannelList *pDlg_channel_list;
    DlgChannelSettings *pDlg_channel_settings;
    DlgFriends *pDlg_friends;
    DlgIgnore *pDlg_ignore;
    DlgModeration *pDlg_moderation;
    OnetKernel *pOnet_kernel;
    OnetAuth *pOnet_auth;
    Notify *pNotify;
    DlgOptions *pOptions;
    QList <UpdateThread*> uThreadList;
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
    QAction *channel_listAct;
    QAction *channel_homesAct;
    QAction *channel_favouritesAct;
    QAction *friendsAct;
    QAction *ignoreAct;
#ifdef Q_WS_WIN
    QAction *camsAct;
#endif
    QAction *aboutAct;
    QAction *lagAct;
    QMap <QString, QByteArray> mNickAvatar;
    QMap <QString, QByteArray> mChannelAvatar;

private slots:
    void check_update();
    void button_close();
    void button_connect();
    void options_dlg();
    void channel_list_dlg();
    void channel_homes_dlg();
    void channel_favourites_dlg();
    void friends_dlg();
    void ignore_dlg();
    void cams_clicked();
    void about_dlg();
    void tray_icon(QSystemTrayIcon::ActivationReason);
    void button_show();
    void tab_close_requested(int);
    void remove_uthread(UpdateThread*);

signals:
    void do_kernel(QString);
    void do_request_uo(QString, QString, QString);

};

#endif // MAINWINDOW_H
