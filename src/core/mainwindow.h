/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defines.h"
class QToolButton;
class QMenuBar;
class QMovie;
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
    void init();
    void createGui();

    // options
    void refreshColors();
    void refreshCSS();
    void refreshToolWidgetValues();
    // for core
    int getCurrentTabIndex();
    void changeCurrentTab(int index);
    void refreshToolButtons(const QString &strChannel);
    void updateChannelIcon(int index, const QString &avatar);
    void setTabColor(const QString &strChannel, MessageCategory eMessageCategory);
    QSystemTrayIcon *getTrayIcon();
    QToolButton *getNotificationToolButton();
    QAction *getNotificationAction();
    void startAnimatedTrayIcon();
    void stopAnimatedTrayIcon();
    void inputLineKeyEvent(QKeyEvent *k);
    void insertTextToInputLine(const QString &strText);

public slots:
    void updateNick(const QString &strNick);

private:
    QToolBar *toolBar;
    QMenu *fileMenu;
    QMenu *optionsMenu;

    QAction *notificationAction;
    QToolButton *notificationToolButton;
    QMenu *helpMenu;
    QAction *sccAction;
    QAction *connectAction;
    QAction *optionsAction;
    QAction *aboutAction;
    QAction *notesAction;
    QMenu *trayIconMenu;
    QSystemTrayIcon *trayIcon;
    QMovie *movieTrayIcon;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    // onet actions
    QMenu *chatMenu;
    QAction *channelListAction;
    QAction *channelHomesAction;
    QAction *channelFavouritesAction;
    QAction *friendsAction;
    QAction *ignoreAction;
    QAction *findNickAction;
    QAction *camsAction;
    QMenu *myMenu;
    QAction *myStatsAction;
    QAction *myProfileAction;
    QAction *myAvatarAction;

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
    void addDefaultTabs();
    void firstRun();
    void setTrayMenuVisible(bool visible);

private slots:
    void showWelcome();
    void buttonConnect();
    void openOptions();
    void openNotes();
    void openAbout();
    void updateTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason activationReason);

    void openChannelList();
    void openChannelHomes();
    void openChannelFavourites();
    void openFriends();
    void openFindNick();
    void openIgnore();
    void openCams();
    void openMyAvatar();
    void openMyStats();
    void openMyProfile();

    void openAwaylog();
    void openInviteList();
    void openOfflineMessages();

    void updateButtons();
    void tabCloseRequested(int index);
    void currentTabChanged(int index);

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
