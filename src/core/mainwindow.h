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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defines.h"
class DlgAbout;
class DlgChannelSettings;
class DlgModeration;
class DlgNotes;
class DlgUserProfile;
class InputLineDockWidget;
class Network;
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
#include <QSystemTrayIcon>
#include <QToolBar>

/**
 * Main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    void createGui();

    void network_connect();
    void network_disconnect();
    void network_send(QString);
    bool network_is_connected();
    // options
    void refresh_colors();
    void refresh_background_image();

private:
    QToolBar *toolBar;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QAction *connectAct;
    QAction *closeAct;
    QAction *optionsAct;
    QAction *showAct;
    QAction *aboutAct;
    QAction *awaylogAct;
    QAction *notesAct;
    QMenu *trayMenu;
    QSystemTrayIcon *trayIcon;

    // onet actions
    QMenu *chatMenu;
    QAction *channelListAct;
    QAction *channelHomesAct;
    QAction *channelFavouritesAct;
    QAction *friendsAct;
    QAction *ignoreAct;
    QAction *camsAct;
    QAction *myStatsAct;
    QAction *myProfileAct;
    QAction *myAvatarAct;

    TabManager *pTabM;
    TabContainer *pTabC;
    Network *pNetwork;
    OnetKernel *pOnetKernel;
    OnetAuth *pOnetAuth;
    DlgChannelSettings *pDlgChannelSettings;
    DlgUserProfile *pDlgUserProfile;
    DlgModeration *pDlgModeration;

    // gui
    InputLineDockWidget *pInputLineDockWidget;
    NickListWidget *pNickListWidget;
    QDockWidget *bottomDockWidget;
    QDockWidget *rightDockWidget;

    // other
    void set_geometry();
    void create_actions();
    void create_menus();
    void create_signals();
    void show_options();
    QString get_current_tab_name(int);
    // nicklist
    void update_users_count();
    bool nicklist_exist(QString, QString);

private slots:
    void button_connect();
    void button_close();
    void open_options();
    void open_awaylog();
    void open_notes();
    void open_about();
    void button_show();
    void tray_icon(QSystemTrayIcon::ActivationReason);

    // onet dialogs
    void open_channel_list();
    void open_channel_homes();
    void open_channel_favourites();
    void open_friends();
    void open_ignore();
    void open_cams();
    void button_set_busy();
    void button_set_away();
    void open_my_stats();
    void open_my_profile();
    void open_my_avatar();
    void open_offlinemsg();

public slots:
    void create_nicklist(QString);
    void add_user(QString, QString, QString, bool);
    void del_user(QString, QString);
    void nicklist_refresh(QString);
    void quit_user(QString, QString);
    void change_flag(QString, QString, QString);
    void change_flag(QString, QString);
    void clear_nicklist(QString);
    void clear_all_nicklist();
    void update_nick_avatar(QString);
    void clear_channel_all_nick_avatars(QString);

private slots:
    void set_connected();
    void set_disconnected();
    void update_actions();
    void update_awaylog_status();
    void set_connect_enabled(bool);
    void tab_close_requested(int);
    void current_tab_changed(int);
    void timeout_autoaway();

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void skernel(QString);
    void srequest_uo(QString, QString, QString);
};

#endif // MAINWINDOW_H
