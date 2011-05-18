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

#include <QDateTime>
#include <QDesktopWidget>
#include <QSettings>
#include "config.h"
#include "core.h"
#include "dlg_about.h"
#include "dlg_awaylog.h"
#include "dlg_channel_favourites.h"
#include "dlg_channel_homes.h"
#include "dlg_channel_list.h"
#include "dlg_channel_settings.h"
#include "dlg_friends.h"
#include "dlg_ignore.h"
#include "dlg_moderation.h"
#include "dlg_my_avatar.h"
#include "dlg_my_profile.h"
#include "dlg_my_stats.h"
#include "dlg_notes.h"
#include "dlg_offlinemsg.h"
#include "dlg_options.h"
#include "dlg_user_profile.h"
#include "inputlinedockwidget.h"
#include "network.h"
#include "nicklistdelegate.h"
#include "nicklistwidget.h"
#include "onet_auth.h"
#include "onet_kernel.h"
#include "tab_container.h"
#include "tab_manager.h"
#include "mainwindow.h"

#ifdef Q_WS_WIN
    #include "kamerzysta.h"
#else
    #include "dlg_cam.h"
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // init
    setWindowTitle("Simple Chat Client");
    setWindowIcon(QIcon(":/images/logo.png"));

    set_geometry();
    create_actions();
    create_menus();

    // init all
    camSocket = new QTcpSocket();
    camSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    camSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    pTabM = new TabManager(this);
    this->setCentralWidget(pTabM);

    pNetwork = new Network("czat-app.onet.pl", 5015);
    pNetwork->start(QThread::InheritPriority);

    // classes
    pTabC = new TabContainer(pNetwork, pTabM, camSocket);

    pDlgChannelSettings = new DlgChannelSettings(this, pNetwork);
    pDlgModeration = new DlgModeration(this);
    pDlgUserProfile = new DlgUserProfile(this, pNetwork);

#ifndef Q_WS_WIN
    pDlgCam = new DlgCam(this, pNetwork, camSocket);
#endif

    pOnetKernel = new OnetKernel(pNetwork, pTabC, pDlgChannelSettings, pDlgModeration, pDlgUserProfile);
    pOnetAuth = new OnetAuth(pTabC);

    pTabC->set_dlg(pDlgUserProfile);
#ifndef Q_WS_WIN
    pTabC->set_dlg_cam(pDlgCam);
#endif

    // auto-away
    Core::instance()->autoAwayTimer = new QTimer();
    Core::instance()->autoAwayTimer->setInterval(1000*60*1); // 1 min

    // gui
    createGui();

    // refresh colors
    refresh_colors();

    // create signals
    create_signals();

    // welcome
    QString strStatus = "Status";
    QString strWelcome = "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%";
    QString strWebsite = "%Fb:courier%%C008100%"+tr("Official website")+" SCC%C3030ce%: http://simplechatclien.sf.net/ %Izaskoczony%";
    pTabC->add_tab(strStatus);
    pTabC->show_msg(strStatus, strWelcome, DefaultMessage);
    pTabC->show_msg(strStatus, strWebsite, DefaultMessage);

    // hide offline messages
    Core::instance()->offlineMsgAct->setVisible(false);

    // hide awaylog
    awaylogAct->setVisible(false);

    // TODO
    // hide my avtar - too many errors
    myAvatarAct->setVisible(false);

    // show options if config not exist
    show_options();
}

MainWindow::~MainWindow()
{
    // clear arrays
    clear_all_nicklist();

    // auto-away
    Core::instance()->autoAwayTimer->stop();

    delete pOnetAuth;
    delete pOnetKernel;
#ifndef Q_WS_WIN
    delete pDlgCam;
#endif
    delete pDlgUserProfile;
    delete pDlgModeration;
    delete pDlgChannelSettings;

    // close network
    QSettings settings;
    settings.setValue("reconnect", "false");
    pNetwork->disconnect();
    pNetwork->quit();
    pNetwork->wait();
    pNetwork->deleteLater();
    pNetwork->QObject::disconnect();
    delete pNetwork;

    delete pTabC;
    delete pTabM;
    delete bottomDockWidget;
    delete rightDockWidget;
    camSocket->deleteLater();

    // hide tray
    trayIcon->hide();

    // delete teray
    delete trayIcon;
    delete trayMenu;
}

void MainWindow::set_geometry()
{
    const int x = QApplication::desktop()->availableGeometry().width();
    const int y = QApplication::desktop()->availableGeometry().height();
    const int xo = 800;
    const int yo = 600;

    this->resize(xo,yo);
    this->setGeometry((x-xo)/2,(y-yo)/2,xo,yo);
}

void MainWindow::createGui()
{
    // inputlinewidget
    bottomDockWidget = new QDockWidget(tr("Typing messages"), this);
    bottomDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    bottomDockWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea); // top and bottom
    this->addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);
    viewMenu->addAction(bottomDockWidget->toggleViewAction());

    // nicklistwidget
    rightDockWidget = new QDockWidget(tr("Users"), this);
    rightDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    rightDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea); // left and right
    this->addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);
    viewMenu->addAction(rightDockWidget->toggleViewAction());

    // inputline
    pInputLineDockWidget = new InputLineDockWidget(bottomDockWidget, pNetwork, pDlgChannelSettings, pDlgModeration);
    bottomDockWidget->setWidget(pInputLineDockWidget);

    // nicklist
    pNickListWidget = new NickListWidget(pNetwork, camSocket, pDlgUserProfile);
#ifndef Q_WS_WIN
    pNickListWidget->set_dlg_cam(pDlgCam);
#endif
    pNickListWidget->setParent(rightDockWidget);
    pNickListWidget->setItemDelegate(new NickListDelegate(pNickListWidget));
    rightDockWidget->setWidget(pNickListWidget);

    // hide on classic
    QSettings settings;
    if (settings.value("style") == "classic")
    {
        toolBar->hide();
        pInputLineDockWidget->hide_toolwidget();
    }

    // maximum size
    QString strDisableAvatars = settings.value("disable_avatars").toString();
    if (strDisableAvatars == "off") // with avatars
        rightDockWidget->setMaximumWidth(260);
    else // without avatars
        rightDockWidget->setMaximumWidth(230);
}

void MainWindow::create_actions()
{
    // action
    showAct = new QAction(QIcon(":/images/logo.png"), tr("Show"), this);
    connectAct = new QAction(QIcon(":/images/oxygen/16x16/network-connect.png"), tr("&Connect"), this);
    closeAct = new QAction(QIcon(":/images/oxygen/16x16/application-exit.png"), tr("Close"), this);
    optionsAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-system.png"), tr("Options"), this);
    aboutAct = new QAction(QIcon(":/images/logo.png"), tr("About SCC ..."), this);
    awaylogAct = new QAction(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Awaylog"), this);
    notesAct = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Notes"), this);

    // onet action
    channelListAct = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"), tr("Channel list"), this);
    channelHomesAct = new QAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("Your channels"), this);
    channelFavouritesAct = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"), tr("Favorite channels"), this);
    friendsAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"), tr("Friends"), this);
    ignoreAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"), tr("Ignored"), this);
    Core::instance()->offlineMsgAct = new QAction(QIcon(":/images/oxygen/16x16/mail-mark-unread.png") , tr("Offline messages"), this);
    camsAct = new QAction(QIcon(":/images/oxygen/16x16/camera-web.png"),tr("Cams"), this);
    Core::instance()->busyAct = new QAction(QIcon(":/images/oxygen/16x16/im-user-offline.png"), tr("Mark as busy"), this);
    Core::instance()->awayAct = new QAction(QIcon(":/images/oxygen/16x16/im-user-away.png"), tr("Mark as away"), this);
    myStatsAct = new QAction(QIcon(":/images/oxygen/16x16/office-chart-bar.png"),tr("My statistics"), this);
    myProfileAct = new QAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"),tr("My profile"), this);
    myAvatarAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-desktop-user.png"),tr("My avatar"), this);

    // checkable
    Core::instance()->busyAct->setCheckable(true);
    Core::instance()->awayAct->setCheckable(true);
    Core::instance()->busyAct->setChecked(false);
    Core::instance()->awayAct->setChecked(false);

    // shortcut
    connectAct->setShortcuts(QKeySequence::New);
    closeAct->setShortcut(tr("Ctrl+Q"));
    optionsAct->setShortcut(tr("Ctrl+O"));

    // onet shortcut
    channelListAct->setShortcut(tr("Ctrl+L"));
    channelHomesAct->setShortcut(tr("Ctrl+M"));
    channelFavouritesAct->setShortcut(tr("Ctrl+U"));
    friendsAct->setShortcut(tr("Ctrl+P"));
    ignoreAct->setShortcut(tr("Ctrl+I"));
    Core::instance()->offlineMsgAct->setShortcut(tr("Ctrl+M"));
    awaylogAct->setShortcut(tr("Ctrl+J"));
    camsAct->setShortcut(tr("Ctrl+K"));
}

void MainWindow::create_menus()
{
    // main menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(connectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);

    // view menu
    viewMenu = menuBar()->addMenu(tr("&View"));

    // options menu
    optionsMenu = menuBar()->addMenu(tr("&Settings"));
    optionsMenu->addAction(optionsAct);

    // onet menu
    chatMenu = menuBar()->addMenu(tr("&Chat"));
    chatMenu->addAction(channelListAct);
    chatMenu->addAction(channelHomesAct);
    chatMenu->addAction(channelFavouritesAct);
    chatMenu->addAction(friendsAct);
    chatMenu->addAction(ignoreAct);
    chatMenu->addAction(Core::instance()->offlineMsgAct);
    chatMenu->addAction(awaylogAct);
    chatMenu->addAction(camsAct);
    chatMenu->addSeparator();
    chatMenu->addAction(Core::instance()->busyAct);
    chatMenu->addAction(Core::instance()->awayAct);

    // onet my menu
    chatMenu = menuBar()->addMenu(tr("&My"));
    chatMenu->addAction(myStatsAct);
    chatMenu->addAction(myProfileAct);
    chatMenu->addAction(myAvatarAct);
    chatMenu->addAction(notesAct);

    // help menu
    helpMenu = menuBar()->addMenu(tr("He&lp"));
    helpMenu->addAction(aboutAct);

    // toolbar
    toolBar = new QToolBar();
    toolBar = addToolBar(tr("Navigation bar"));
    toolBar->setIconSize(QSize(16,16));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(connectAct);
    toolBar->addAction(optionsAct);

    // onet toolbar
    toolBar->addAction(channelListAct);
    toolBar->addAction(channelHomesAct);
    toolBar->addAction(friendsAct);

    // offline messages
    toolBar->addAction(Core::instance()->offlineMsgAct);
    // awaylog
    toolBar->addAction(awaylogAct);
    // onet cams
    toolBar->addAction(camsAct);
    // notes
    toolBar->addAction(notesAct);
    // lag
    toolBar->addSeparator();
    toolBar->addAction(Core::instance()->lagAct);

    // tray
    trayMenu = new QMenu();
    trayMenu->addAction(showAct);
    trayMenu->addAction(closeAct);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/logo.png"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void MainWindow::create_signals()
{
    // signals buttons
    QObject::connect(connectAct, SIGNAL(triggered()), this, SLOT(button_connect()));
    QObject::connect(closeAct, SIGNAL(triggered()), this, SLOT(button_close()));
    QObject::connect(optionsAct, SIGNAL(triggered()), this, SLOT(open_options()));
    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(open_about()));
    QObject::connect(showAct, SIGNAL(triggered()), this, SLOT(button_show()));

    // signals onet dialogs
    QObject::connect(channelListAct, SIGNAL(triggered()), this, SLOT(open_channel_list()));
    QObject::connect(channelHomesAct, SIGNAL(triggered()), this, SLOT(open_channel_homes()));
    QObject::connect(channelFavouritesAct, SIGNAL(triggered()), this, SLOT(open_channel_favourites()));
    QObject::connect(friendsAct, SIGNAL(triggered()), this, SLOT(open_friends()));
    QObject::connect(ignoreAct, SIGNAL(triggered()), this, SLOT(open_ignore()));
    QObject::connect(Core::instance()->busyAct, SIGNAL(triggered()), this, SLOT(button_set_busy()));
    QObject::connect(Core::instance()->awayAct, SIGNAL(triggered()), this, SLOT(button_set_away()));
    QObject::connect(myStatsAct, SIGNAL(triggered()), this, SLOT(open_my_stats()));
    QObject::connect(myProfileAct, SIGNAL(triggered()), this, SLOT(open_my_profile()));
    QObject::connect(myAvatarAct, SIGNAL(triggered()), this, SLOT(open_my_avatar()));

    // offlinemsg
    QObject::connect(Core::instance()->offlineMsgAct, SIGNAL(triggered()), this, SLOT(open_offlinemsg()));
    // awaylog
    QObject::connect(awaylogAct, SIGNAL(triggered()), this, SLOT(open_awaylog()));
    // onet cams
    QObject::connect(camsAct, SIGNAL(triggered()), this, SLOT(open_cams()));
    // notes
    QObject::connect(notesAct, SIGNAL(triggered()), this, SLOT(open_notes()));
    // tray connect
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_icon(QSystemTrayIcon::ActivationReason)));

    // signals from tabc
    QObject::connect(pTabC, SIGNAL(clear_nicklist(QString)), this, SLOT(clear_nicklist(QString)));
    QObject::connect(pTabC, SIGNAL(clear_channel_all_nick_avatars(QString)), this, SLOT(clear_channel_all_nick_avatars(QString)));
    QObject::connect(pTabC, SIGNAL(update_nick_avatar(QString)), this, SLOT(update_nick_avatar(QString)));
    QObject::connect(pTabC, SIGNAL(update_awaylog_status()), this, SLOT(update_awaylog_status()));

    // signals tab
    QObject::connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
    QObject::connect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(pDlgModeration, SIGNAL(display_msg(QString&,QString&,MessageCategory)), pTabC, SLOT(slot_show_msg(QString&,QString&,MessageCategory)));

    // signals pInputLineDockWidget
    QObject::connect(pInputLineDockWidget, SIGNAL(show_msg(QString&,QString&,MessageCategory)), pTabC, SLOT(slot_show_msg(QString&,QString&,MessageCategory)));
    QObject::connect(pInputLineDockWidget, SIGNAL(display_message(QString&,QString&,MessageCategory)), pTabC, SLOT(slot_display_message(QString&,QString&,MessageCategory)));
    QObject::connect(pInputLineDockWidget, SIGNAL(change_font_size(QString)), pTabC, SLOT(slot_change_font_size(QString)));
    QObject::connect(pInputLineDockWidget, SIGNAL(clear_content(QString)), pTabC, SLOT(slot_clear_content(QString)));

    // signals onet kernel
    QObject::connect(pOnetKernel, SIGNAL(clear_nicklist(QString)), this, SLOT(clear_nicklist(QString)));

    // signals from kernel to nicklist
    QObject::connect(pOnetKernel, SIGNAL(add_user(QString,QString,QString,bool)), this, SLOT(add_user(QString,QString,QString,bool)));
    QObject::connect(pOnetKernel, SIGNAL(del_user(QString,QString)), this, SLOT(del_user(QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(nicklist_refresh(QString)), this, SLOT(nicklist_refresh(QString)));
    QObject::connect(pOnetKernel, SIGNAL(quit_user(QString,QString)), this, SLOT(quit_user(QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(change_flag(QString,QString,QString)), this, SLOT(change_flag(QString,QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(change_flag(QString,QString)), this, SLOT(change_flag(QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(clear_channel_all_nick_avatars(QString)), this, SLOT(clear_channel_all_nick_avatars(QString)));

    // signals to network
    QObject::connect(pDlgModeration, SIGNAL(send(QString)), pNetwork, SLOT(send(QString)));
    QObject::connect(pOnetAuth, SIGNAL(send(QString)), pNetwork, SLOT(send(QString)));

    // signals from network
    QObject::connect(pNetwork, SIGNAL(set_connected()), this, SLOT(set_connected()));
    QObject::connect(pNetwork, SIGNAL(set_disconnected()), this, SLOT(set_disconnected()));
    QObject::connect(pNetwork, SIGNAL(set_connect_enabled(bool)), this, SLOT(set_connect_enabled(bool)));
    QObject::connect(pNetwork, SIGNAL(kernel(QString)), pOnetKernel, SLOT(kernel(QString)));
    QObject::connect(pNetwork, SIGNAL(authorize(QString,QString,QString)), pOnetAuth, SLOT(authorize(QString,QString,QString)));
    QObject::connect(pNetwork, SIGNAL(show_msg_active(QString&,MessageCategory)), pTabC, SLOT(slot_show_msg_active(QString&,MessageCategory)));
    QObject::connect(pNetwork, SIGNAL(show_msg_all(QString&,MessageCategory)), pTabC, SLOT(slot_show_msg_all(QString&,MessageCategory)));
    QObject::connect(pNetwork, SIGNAL(update_nick(QString)), pInputLineDockWidget, SLOT(slot_update_nick(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_all_nicklist()), this, SLOT(clear_all_nicklist()));
    QObject::connect(pNetwork, SIGNAL(update_actions()), this, SLOT(update_actions()));
    QObject::connect(pNetwork, SIGNAL(close_cam_socket()), this, SLOT(close_cam_socket()));

    // auto-away
    QObject::connect(Core::instance()->autoAwayTimer, SIGNAL(timeout()), this, SLOT(timeout_autoaway()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    Core::instance()->quit();
}

void MainWindow::show_options()
{
    QSettings settings;
    QString strFirstRun = settings.value("first_run").toString();

    if (strFirstRun == "true")
    {
        Config *pConfig = new Config();
        pConfig->set_value("first_run", "false");
        settings.setValue("first_run", "false");
        delete pConfig;

        QTimer::singleShot(1000*1, this, SLOT(open_options())); // 1 sec
    }
}

// refresh colors
void MainWindow::refresh_colors()
{
    QSettings settings;
    QString strBackgroundColor = settings.value("background_color").toString();
    QString strDefaultFontColor = settings.value("default_font_color").toString();

    if (strBackgroundColor.toLower() != "#ffffff")
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);

    pTabC->refresh_colors();
}

// network
void MainWindow::network_connect()
{
    pNetwork->connect();
}

void MainWindow::network_disconnect()
{
    pNetwork->disconnect();
}

void MainWindow::network_send(QString data)
{
    pNetwork->send(data);
}

bool MainWindow::network_is_connected()
{
    return pNetwork->is_connected();
}

// buttons
void MainWindow::button_connect()
{
    QSettings settings;
    if (!network_is_connected())
    {
        connectAct->setText(tr("&Disconnect"));
        connectAct->setIconText(tr("&Disconnect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
        settings.setValue("reconnect", "true");
        network_connect();
    }
    else
    {
        settings.setValue("reconnect", "false");
        settings.setValue("logged", "off");
        connectAct->setText(tr("&Connect"));
        connectAct->setIconText(tr("&Connect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
        network_disconnect();
    }
}

// refresh background image
void MainWindow::refresh_background_image()
{
    pTabC->refresh_background_image();
}

void MainWindow::set_connected()
{
    connectAct->setText(tr("&Disconnect"));
    connectAct->setIconText(tr("&Disconnect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
}

void MainWindow::set_disconnected()
{
    connectAct->setText(tr("&Connect"));
    connectAct->setIconText(tr("&Connect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
}

void MainWindow::set_connect_enabled(bool bSet)
{
    connectAct->setEnabled(bSet);
}

void MainWindow::update_actions()
{
    QSettings settings;
    QString strNick = settings.value("nick").toString();

    bool bRegistered = false;
    if (strNick[0] != '~')
        bRegistered = true;

    if (bRegistered)
    {
        channelHomesAct->setEnabled(true);
        channelFavouritesAct->setEnabled(true);
        friendsAct->setEnabled(true);
        ignoreAct->setEnabled(true);
        myStatsAct->setEnabled(true);
        myProfileAct->setEnabled(true);
        myAvatarAct->setEnabled(true);
    }
    else
    {
        channelHomesAct->setEnabled(false);
        channelFavouritesAct->setEnabled(false);
        friendsAct->setEnabled(false);
        ignoreAct->setEnabled(false);
        myStatsAct->setEnabled(false);
        myProfileAct->setEnabled(false);
        myAvatarAct->setEnabled(false);
    }
}

void MainWindow::close_cam_socket()
{
    if (camSocket->state() == QAbstractSocket::ConnectedState)
        camSocket->disconnectFromHost();
}

void MainWindow::update_awaylog_status()
{
    if (Core::instance()->lAwaylog.size() == 0)
    {
        if (awaylogAct->isVisible())
            awaylogAct->setVisible(false);
    }
    else
    {
        if (!awaylogAct->isVisible())
            awaylogAct->setVisible(true);
    }
}

void MainWindow::open_options()
{
    DlgOptions(this).exec();
}

void MainWindow::update_users_count()
{
    QString strChannel = pNickListWidget->get_channel();
    rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(Core::instance()->mChannelNicks[strChannel]));
}

// onet dialogs
void MainWindow::open_channel_list()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgChannelList(this, pNetwork).exec();
}

void MainWindow::open_channel_homes()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgChannelHomes(this, pNetwork, pDlgChannelSettings).exec();
}

void MainWindow::open_channel_favourites()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgChannelFavourites(this, pNetwork).exec();
}

void MainWindow::open_friends()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgFriends(this, pNetwork).exec();
}

void MainWindow::open_ignore()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgIgnore(this, pNetwork).exec();
}

void MainWindow::button_set_busy()
{
    // do not change status
    if (Core::instance()->busyAct->isChecked())
        Core::instance()->busyAct->setChecked(false);
    else
        Core::instance()->busyAct->setChecked(true);

    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
    {
        bool bBusy = settings.value("busy").toString() == "on" ? true : false;

        if (bBusy)
            pNetwork->send("BUSY 0");
        else
            pNetwork->send("BUSY 1");
    }
}

void MainWindow::button_set_away()
{
    // do not change status
    if (Core::instance()->awayAct->isChecked())
        Core::instance()->awayAct->setChecked(false);
    else
        Core::instance()->awayAct->setChecked(true);

    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
    {
        bool bAway = settings.value("away").toString() == "on" ? true : false;

        QString strReason;
        if (bAway)
            strReason = "";
        else
            strReason = tr("Not here right now");

        pNetwork->send(QString("AWAY :%1").arg(strReason));
    }
}

void MainWindow::open_offlinemsg()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgOfflineMsg(this, pNetwork).exec();
}

void MainWindow::open_awaylog()
{
    DlgAwaylog(this, awaylogAct).exec();
}

void MainWindow::open_cams()
{
#ifdef Q_WS_WIN
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
    {
        QSettings settings;
        QString strMe = settings.value("nick").toString();
        (new Kamerzysta(camSocket))->show(strMe);
    }
#else
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        pDlgCam->show();
#endif
}

void MainWindow::open_my_stats()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgMyStats(this).exec();
}

void MainWindow::open_my_profile()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgMyProfile(this, pNetwork).exec();
}

void MainWindow::open_my_avatar()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
        DlgMyAvatar(this, pNetwork).exec();
}

void MainWindow::open_notes()
{
    DlgNotes(this).exec();
}

void MainWindow::open_about()
{
    DlgAbout(this).exec();
}

void MainWindow::button_close()
{
    this->close();
}

// tray button
void MainWindow::button_show()
{
    this->showNormal();
}

// tray
void MainWindow::tray_icon(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::Trigger)
    {
        if (this->isVisible())
            this->hide();
        else
            this->showNormal();
    }
}

// part tab
void MainWindow::tab_close_requested(int index)
{
    if (index != 0)
        pTabC->part_tab(index);
}

QString MainWindow::get_current_tab_name(int index)
{
    QList<QString> lOpenChannels = Core::instance()->lOpenChannels;
    lOpenChannels.insert(0, "Status");

    for (int i = 0; i < lOpenChannels.size(); i++)
    {
        QString strChannel = lOpenChannels.at(i);
        if (index == i)
            return strChannel;
    }
    return QString::null;
}

void MainWindow::timeout_autoaway()
{
    QSettings settings;
    if ((pNetwork->is_connected()) && (pNetwork->is_writable()) && (settings.value("logged") == "on"))
    {
        QDateTime cdt = QDateTime::currentDateTime();
        int t = (int)cdt.toTime_t(); // seconds that have passed since 1970

        int iLastActive = settings.value("last_active").toInt();

        bool bAway = settings.value("away").toString() == "on" ? true : false;

        if ((!bAway) && (iLastActive != 0) && (t-iLastActive > 300))
            pNetwork->send(QString("AWAY :%1").arg(tr("Not here right now")));
    }
}

// tab changed
void MainWindow::current_tab_changed(int index)
{
    QString strChannel = get_current_tab_name(index);
    if (strChannel.isEmpty()) return; // something wrong

    QSettings settings;

    // change tab name
    QString strTabText = pTabM->tabText(index);
    this->setWindowTitle(QString("Simple Chat Client - [%1]").arg(strTabText));

    // change tab color
    pTabM->set_color(index, QColor(settings.value("default_font_color").toString()));

    // hide/show Status nicklist
    if (index == 0)
        rightDockWidget->hide();
    else
        rightDockWidget->show();

    // hide/show settings on non channel
    if (strChannel[0] != '#')
        pInputLineDockWidget->hide_channel_settings();
    else
        pInputLineDockWidget->show_channel_settings();

    // set current channel
    pNickListWidget->set_channel(strChannel);

    // clear users
    Core::instance()->mChannelNicks[strChannel] = 0;

    // clear nicklist
    pNickListWidget->clear();

    // nicklist
    if (strChannel != "Status")
        create_nicklist(strChannel);

    // update nick count
    update_users_count();

    // set tab active
    pInputLineDockWidget->set_active(strChannel);

    // moderation
    QString strMe = settings.value("nick").toString();
    QString strModes = Core::instance()->get_user_modes(strMe, strChannel);
    if (strModes.contains("!")) pInputLineDockWidget->enable_moderation();
    else pInputLineDockWidget->disable_moderation();
}

void MainWindow::create_nicklist(QString strChannel)
{
    // create nicklist
    for (int i = 0; i < Core::instance()->lUsers.size(); i++)
    {
        if (Core::instance()->lUsers.at(i).channel == strChannel)
        {
            QString strNick = Core::instance()->lUsers.at(i).nick;

            // add
            pNickListWidget->add(strNick);

            // update nick count for option hide join/part when > 200
            Core::instance()->mChannelNicks[strChannel] = Core::instance()->mChannelNicks[strChannel]++;
        }
    }

    // sort
    pNickListWidget->sortItems(Qt::AscendingOrder);

    // update nick count
    update_users_count();
}

// nick list

void MainWindow::add_user(QString strChannel, QString strNick, QString strModes, bool bFastAdd)
{
    // add
    User add;
    add.channel = strChannel;
    add.nick = strNick;
    add.modes = strModes;

    // add to nick list
    Core::instance()->lUsers.append(add);

    // update widget
    if (pNickListWidget->get_channel() == strChannel)
    {
        // add
        pNickListWidget->add(strNick);

        // fast add
        if (!bFastAdd)
        {
            // sort
            pNickListWidget->sortItems(Qt::AscendingOrder);
        }

        // set inputline users
        if (pInputLineDockWidget->get_active() == strChannel)
            pInputLineDockWidget->update_nick_list();

        // update nick count for option hide join/part when > 200
        Core::instance()->mChannelNicks[strChannel] = Core::instance()->mChannelNicks[strChannel]++;

        // update nick count
        if (pInputLineDockWidget->get_active() == strChannel)
            update_users_count();
    }
}

void MainWindow::del_user(QString strChannel, QString strNick)
{
    // remove from nick list
    for (int i = 0; i < Core::instance()->lUsers.size(); i++)
    {
        if ((Core::instance()->lUsers.at(i).nick == strNick) && (Core::instance()->lUsers.at(i).channel == strChannel))
        {
            Core::instance()->lUsers.removeAt(i);
            i--;
        }
    }

    // update widget
    if (pNickListWidget->get_channel() == strChannel)
    {
        pNickListWidget->remove(strNick);

        // set inputline users
        if (pInputLineDockWidget->get_active() == strChannel)
            pInputLineDockWidget->update_nick_list();

        // update nick count for option hide join/part when > 200
        Core::instance()->mChannelNicks[strChannel] = Core::instance()->mChannelNicks[strChannel]--;

        // update nick count
        if (pInputLineDockWidget->get_active() == strChannel)
            update_users_count();
    }
}

void MainWindow::nicklist_refresh(QString strChannel)
{
    //raw 366: End of /NAMES list.
    if (pNickListWidget->get_channel() == strChannel)
    {
        // sort
        pNickListWidget->sortItems(Qt::AscendingOrder);
    }
}

bool MainWindow::nicklist_exist(QString strChannel, QString strNick)
{
    for (int i = 0; i < Core::instance()->lUsers.size(); i++)
    {
        if ((Core::instance()->lUsers.at(i).nick == strNick) && (Core::instance()->lUsers.at(i).channel == strChannel))
            return true;
    }
    return false;
}

void MainWindow::quit_user(QString strNick, QString strDisplay)
{
    QList<QString> lOpenChannels = Core::instance()->lOpenChannels;
    for (int i = 0; i < lOpenChannels.size(); i++)
    {
        QString strChannel = lOpenChannels.at(i);
        if (nicklist_exist(strChannel, strNick))
        {
            QString strDisplayAll = strDisplay;
            pTabC->show_msg(strChannel, strDisplayAll, QuitMessage);
            del_user(strChannel, strNick);

            if (i+1 != pTabM->currentIndex())
                pTabM->set_alert(i+1, QColor(0, 147, 0, 255)); // green

            // update nick count
            if (pInputLineDockWidget->get_active() == strChannel)
                update_users_count();
        }
    }
}

void MainWindow::change_flag(QString strNick, QString strChannel, QString strNewFlag)
{
    if (!nicklist_exist(strChannel, strNick)) return; // nick not exist

    QString strModes = Core::instance()->get_user_modes(strNick, strChannel);

    QString strConvertFrom = "qaohXYvObrWVx";
    QString strConvertTo = "`&@%!=+ObrWVx";

    for (int i = 0; i < strConvertFrom.size(); i++)
    {
        QString plusminus = strNewFlag.at(0);
        QString strFlag = strNewFlag.at(1);

        if (plusminus == "+")
        {
            if ((strFlag == strConvertFrom.at(i)) && (!strModes.contains(strConvertTo.at(i))))
                strModes.append(strConvertTo.at(i));
        }
        else
        {
            if ((strFlag == strConvertFrom.at(i)) && (strModes.contains(strConvertTo.at(i))))
                strModes.remove(strConvertTo.at(i));
        }
    }

    // change flag
    del_user(strChannel, strNick);
    add_user(strChannel, strNick, strModes, false);

    // me ?
    QSettings settings;
    QString strMe = settings.value("nick").toString();

    if (strNick == strMe)
    {
        if (strNewFlag == "+X") pInputLineDockWidget->enable_moderation();
        else if (strNewFlag == "-X") pInputLineDockWidget->disable_moderation();
    }
}

void MainWindow::change_flag(QString strNick, QString strFlag)
{
    QList<QString> lOpenChannels = Core::instance()->lOpenChannels;
    for (int i = 0; i < lOpenChannels.size(); i++)
    {
        QString strChannel = lOpenChannels.at(i);

        if (nicklist_exist(strChannel, strNick))
            change_flag(strNick, strChannel, strFlag);
    }
}

void MainWindow::clear_nicklist(QString strChannel)
{
    // clear
    for (int i = 0; i < Core::instance()->lUsers.size(); i++)
    {
        if (Core::instance()->lUsers.at(i).channel == strChannel)
        {
            Core::instance()->lUsers.removeAt(i);
            i--;
        }
    }

    if (pNickListWidget->get_channel() == strChannel)
        pNickListWidget->clear();

    // clear nick count for option hide join/part when > 200
    Core::instance()->mChannelNicks[strChannel] = 0;

    // update nick count
    if (pNickListWidget->get_channel() == strChannel)
        update_users_count();
}

void MainWindow::clear_all_nicklist()
{
    Core::instance()->lUsers.clear();
    Core::instance()->mNickAvatar.clear();
    Core::instance()->mChannelAvatar.clear();
    pNickListWidget->clear();

    QList<QString> lOpenChannels = Core::instance()->lOpenChannels;
    for (int i = 0; i < lOpenChannels.size(); i++)
    {
        QString strChannel = lOpenChannels.at(i);

        // clear nick count for option hide join/part when > 200
        Core::instance()->mChannelNicks[strChannel] = 0;

        // update nick count
        if (pNickListWidget->get_channel() == strChannel)
            update_users_count();
    }
}

void MainWindow::update_nick_avatar(QString strNick)
{
    QString strChannel = pNickListWidget->get_channel();
    if (nicklist_exist(strChannel, strNick))
        pNickListWidget->reset(); // like repaint
}

// clear all channel avatars
void MainWindow::clear_channel_all_nick_avatars(QString strChannel)
{
    QList<QString> lNicks = Core::instance()->get_nicks_from_channel(strChannel);

    for (int i = 0; i < lNicks.size(); i++)
    {
        QString strNick = lNicks.at(i);

        // remove nick avatar if nick is only in current channel; must be 1 (current channel)
        if ((Core::instance()->mNickAvatar.contains(strNick)) && (Core::instance()->get_nick_channels(strNick) == 1))
            Core::instance()->mNickAvatar.remove(strNick);
    }
}
