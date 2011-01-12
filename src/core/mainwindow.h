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
#include <QtGui/QMainWindow>
#include "config.h"
#include "core.h"
#include "crypt.h"
#include "dlg_about.h"
#include "dlg_notes.h"
#include "dlg_options.h"
#include "notify.h"
#include "update.h"

/**
 * Main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void set_debug(bool);

private:
    QList <Core*> coreServers;
    DlgOptions *pOptions;
    Notify *pNotify;
    Update *pUpdate;

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
    QAction *channel_listAct;
    QAction *channel_homesAct;
    QAction *channel_favouritesAct;
    QAction *friendsAct;
    QAction *ignoreAct;
    QAction *camsAct;

    void create_settings();

private slots:
    void refresh_colors();
    void button_connect();
    void button_close();
    void open_options();
    void open_about();
    void open_awaylog();
    void open_notes();
    void button_show();
    void tray_icon(QSystemTrayIcon::ActivationReason);

    // onet dialogs
    void open_channel_list();
    void open_channel_homes();
    void open_channel_favourites();
    void open_friends();
    void open_ignore();
    void open_cams();
};

#endif // MAINWINDOW_H
