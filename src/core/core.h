/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef CORE_H
#define CORE_H

#include "defines.h"
#ifndef Q_WS_WIN
    class DlgCam;
#endif
class DlgChannelSettings;
class DlgModeration;
class DlgUserProfile;
class InputLineDockWidget;
class Network;
class NickListDockWidget;
class NickListWidget;
class Notify;
class OnetAuth;
class OnetKernel;
class TabContainer;
class TabManager;
#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QTcpSocket>
#include <QToolBar>

/**
 * Core
 */
class Core : public QObject
{
    Q_OBJECT
public:
    Core();
    ~Core();
    void init(QMainWindow *, QString, int, Notify *, QToolBar *, QMenu *, QAction *, QAction *, QAction *, QAction *, QAction *, QAction *, QAction *, QAction *, QAction *);

    void refresh_colors();
    void refresh_background_image();

    void network_connect();
    void network_disconnect();
    void network_send(QString);
    bool network_is_connected();
    // onet dialogs
    void open_channel_list();
    void open_channel_homes();
    void open_channel_favourites();
    void open_friends();
    void open_ignore();
    void open_awaylog();
    void open_cams();
    void open_my_stats();
    void open_my_profile();
    void open_my_avatar();
    // nicklist
    bool nicklist_exist(QString, QString);

public slots:
    void set_lag(QString);
    void create_nicklist(QString);
    void remove_nicklist(QString);
    void add_user(QString, QString, QString, QString);
    void del_user(QString, QString);
    void nicklist_refresh(QString);
    void quit_user(QString, QString);
    void change_flag(QString, QString, QString);
    void change_flag(QString, QString);
    void clear_nicklist(QString);
    void clear_all_nicklist();
    void update_nick_avatar(QString);
    void clear_channel_all_nick_avatars(QString);
    void set_open_channels();

private:
    // params
    QMainWindow *myparent;
    QString strServer;
    int iPort;
    Notify *pNotify;
    QToolBar *toolBar;
    QMenu *viewMenu;
    QAction *connectAct;
    QAction *channel_homesAct;
    QAction *channel_favouritesAct;
    QAction *friendsAct;
    QAction *ignoreAct;
    QAction *myStatsAct;
    QAction *myProfileAct;
    QAction *myAvatarAct;
    QAction *awaylogAct;

    // require
    QTcpSocket *camSocket;
    TabManager *pTabM;
    TabContainer *pTabC;
    Network *pNetwork;
    OnetKernel *pOnet_kernel;
    OnetAuth *pOnet_auth;
    DlgChannelSettings *pDlg_channel_settings;
    DlgUserProfile *pDlg_user_profile;
    DlgModeration *pDlg_moderation;

    // gui
    InputLineDockWidget *inputLineDockWidget;
    NickListDockWidget *nickListDockWidget;
    QDockWidget *bottomDockWidget;
    QDockWidget *rightDockWidget;
    QMap <QString, NickListWidget*> mChannelNickListWidget;
    sChannelNickStatus stlChannelNickStatus;

    // other
#ifndef Q_WS_WIN
    DlgCam *pDlg_cam;
#endif
    QAction *lagAct;
    QList<QString> lAwaylog;
    QMap<QString, bool> mFriends;
    QList<QString> lIgnore;
    QList<QString> lChannelFavourites;
    sChannelList stlChannelList;
    QMap <QString, QByteArray> mNickAvatar;
    QMap <QString, QByteArray> mChannelAvatar;
    QMap <QString, QString> mMyStats;
    QMap <QString, QString> mMyProfile;
    QList<QString> lChannelHomes;

    void create_signals();

private slots:
    void set_connected();
    void set_disconnected();
    void update_actions();
    void close_cam_socket();
    void update_awaylog_status();
    void set_connect_enabled(bool);
    void tab_close_requested(int);
    void current_tab_changed(int);

signals:
    void skernel(QString);
    void srequest_uo(QString, QString, QString);
};

#endif // CORE_H
