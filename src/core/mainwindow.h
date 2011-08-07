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
class QDockWidget;
class QMenuBar;
class QToolBar;
class DlgAbout;
class DlgChannelSettings;
class DlgModeration;
class DlgNotes;
class DlgUserProfile;
class InputLineDockWidget;
class NickListWidget;
class OnetAuth;
class OnetKernel;
class TabContainer;
class TabManager;
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
    void refreshBackgroundImage();
    // for core
    int getCurrentTabIndex();

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
    QMenu *myMenu;
    QAction *myStatsAct;
    QAction *myProfileAct;

    TabManager *pTabM;
    TabContainer *pTabC;
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
    void setWindowGeometry();
    void createActions();
    void createMenus();
    void createSignals();
    void showOptions();
    // nicklist
    void updateUsersCount();
    bool nicklistExist(QString, QString);

private slots:
    void buttonConnect();
    void buttonClose();
    void openOptions();
    void openAwaylog();
    void openNotes();
    void openAbout();
    void buttonShow();
    void trayIconPressed(QSystemTrayIcon::ActivationReason);
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
    void openMyStats();
    void openMyProfile();
    void openOfflinemsg();

public slots:
    void createNicklist(QString);
    void addUser(QString, QString, QString, bool);
    void delUser(QString, QString);
    void nicklistRefresh(QString);
    void quitUser(QString, QString);
    void changeFlag(QString, QString, QString);
    void changeFlag(QString, QString);
    void clearNicklist(QString);
    void clearAllNicklist();
    void updateNickAvatar(QString);
    void clearChannelAllNickAvatars(QString);

private slots:
    void setConnected();
    void setDisconnected();
    void updateActions();
    void updateAwaylogStatus();
    void setConnectEnabled(bool);
    void tabCloseRequested(int);
    void currentTabChanged(int);
    void timeoutAutoaway();

signals:
    void skernel(QString);
    void srequestUo(QString, QString, QString);
};

#endif // MAINWINDOW_H
