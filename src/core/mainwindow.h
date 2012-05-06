/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
class QMenuBar;
class QToolBar;
class OnetAuth;
class OnetKernel;
class TabContainer;
class TabManager;
class ToolWidget;
#include <QSystemTrayIcon>
#include <QMainWindow>

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

    // options
    void refreshColors();
    void refreshCSS();
    void refreshToolWidgetValues();
    // for core
    int getCurrentTabIndex();
    QSystemTrayIcon *getTrayIcon();

private:
    QToolBar *toolBar;
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QAction *sccAct;
    QAction *restoreMinimalizeAct;
    QAction *connectAct;
    QAction *closeAct;
    QAction *optionsAct;
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
    QMenu *myMenu;
    QAction *myStatsAct;
    QAction *myProfileAct;
    QAction *myAvatarAct;

    TabManager *pTabM;
    TabContainer *pTabC;
    OnetKernel *pOnetKernel;
    OnetAuth *pOnetAuth;

    // gui
    ToolWidget *pToolWidget;

    // other
    void setWindowGeometry();
    void createActions();
    void createMenus();
    void createSignals();
    void showOptions();
    void createTrayMenu();

private slots:
    void showWelcome();
    void buttonConnect();
    void openOptions();
    void openAwaylog();
    void openNotes();
    void openAbout();
    void buttonRestoreMinimalize();
    void trayIconPressed(QSystemTrayIcon::ActivationReason activationReason);
    void ctrlTabPressed();
    void ctrlShiftTabPressed();

    // onet dialogs
    void openChannelList();
    void openChannelHomes();
    void openChannelFavourites();
    void openFriends();
    void openIgnore();
    void openCams();
    void buttonSetBusy();
    void buttonSetAway();
    void openMyAvatar();
    void openMyStats();
    void openMyProfile();
    void openOfflinemsg();

    void setConnected();
    void setDisconnected();
    void updateActions();
    void updateAwaylogStatus();
    void setConnectEnabled(bool bSet);
    void tabCloseRequested(int index);
    void currentTabChanged(int index);
    void tabMoved(int from, int to);
    void timeoutAutoaway();

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
