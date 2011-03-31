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

#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QTimer>
#include "config.h"
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
#include "dlg_options.h"
#include "dlg_user_profile.h"
#include "inputlinedockwidget.h"
#include "network.h"
#include "nicklistdelegate.h"
#include "nicklistwidget.h"
#include "nicklistdockwidget.h"
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
    setWindowIcon(QIcon(":/images/logo_64.png"));

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
    pTabC = new TabContainer(pNetwork, pTabM, &mChannelAvatar, camSocket, &stlChannelNickStatus, &lAwaylog);

    pDlgChannelSettings = new DlgChannelSettings(this, pNetwork);
    pDlgModeration = new DlgModeration(this);
    pDlgUserProfile = new DlgUserProfile(this, pNetwork);

#ifndef Q_WS_WIN
    pDlgCam = new DlgCam(this, pNetwork, camSocket);
#endif

    pOnetKernel = new OnetKernel(pNetwork, pTabC, &mNickAvatar, &mChannelAvatar, pDlgChannelSettings, &lChannelHomes, &stlChannelList, &lChannelFavourites, &mFriends, &lIgnore, pDlgModeration, &mMyStats, &mMyProfile, pDlgUserProfile);
    pOnetAuth = new OnetAuth(pTabC);

    pTabC->set_dlg(pDlgUserProfile);
#ifndef Q_WS_WIN
    pTabC->set_dlg_cam(pDlgCam);
#endif

    // gui
    createGui();

    // welcome
    pTabC->add_tab("Status");
    pTabC->show_msg("Status", "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%C008100%"+tr("Official website")+" SCC%C3030ce%: http://simplechatclien.sf.net/ %Izaskoczony%", 0);

    // hide awaylog
    awaylogAct->setVisible(false);

    // refresh colors
    refresh_colors();

    // create signals
    create_signals();

    // show options if config not exist
    show_options();
}

MainWindow::~MainWindow()
{
    // clear arrays
    clear_all_nicklist();

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
    pInputLineDockWidget = new InputLineDockWidget(bottomDockWidget, pNetwork, pDlgChannelSettings, pDlgModeration);
    bottomDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    bottomDockWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea); // top and bottom
    bottomDockWidget->setWidget(pInputLineDockWidget);
    this->addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);
    viewMenu->addAction(bottomDockWidget->toggleViewAction());

    // nicklistwidget
    rightDockWidget = new QDockWidget(tr("Users"), this);
    pNickListDockWidget = new NickListDockWidget(rightDockWidget);
    rightDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    rightDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea); // left and right
    rightDockWidget->setWidget(pNickListDockWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);
    viewMenu->addAction(rightDockWidget->toggleViewAction());

    // hide on classic
    QSettings settings;
    if (settings.value("style") == "classic")
    {
        toolBar->hide();
        pInputLineDockWidget->hide_toolwidget();
    }

    // hide nicklist and toolwidget on status
    rightDockWidget->hide();
}

void MainWindow::create_actions()
{
    // action
    showAct = new QAction(QIcon(":/images/logo_64.png"), tr("Show"), this);
    connectAct = new QAction(QIcon(":/images/oxygen/16x16/network-connect.png"), tr("&Connect"), this);
    closeAct = new QAction(QIcon(":/images/oxygen/16x16/application-exit.png"), tr("Close"), this);
    optionsAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-system.png"), tr("Options"), this);
    aboutAct = new QAction(QIcon(":/images/logo_64.png"), tr("About SCC ..."), this);
    awaylogAct = new QAction(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Awaylog"), this);
    notesAct = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Notes"), this);

    // onet action
    channelListAct = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"), tr("Channel list"), this);
    channelHomesAct = new QAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("Your channels"), this);
    channelFavouritesAct = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"), tr("Favorite channels"), this);
    friendsAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"), tr("Friends"), this);
    ignoreAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"), tr("Ignored"), this);
    camsAct = new QAction(QIcon(":/images/oxygen/16x16/camera-web.png"),tr("Cams"), this);
    myStatsAct = new QAction(QIcon(":/images/oxygen/16x16/office-chart-bar.png"),tr("My statistics"), this);
    myProfileAct = new QAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"),tr("My profile"), this);
    myAvatarAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-desktop-user.png"),tr("My avatar"), this);

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
    awaylogAct->setShortcut(tr("Ctrl+J"));
    camsAct->setShortcut(tr("Ctrl+K"));

    // lag
    lagAct = new QAction("Lag: ?", this);
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
    chatMenu->addAction(awaylogAct);
    chatMenu->addAction(camsAct);

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

    // awaylog
    toolBar->addAction(awaylogAct);
    // onet cams
    toolBar->addAction(camsAct);
    // notes
    toolBar->addAction(notesAct);
    // lag
    toolBar->addSeparator();
    toolBar->addAction(lagAct);

    // tray
    trayMenu = new QMenu();
    trayMenu->addAction(showAct);
    trayMenu->addAction(closeAct);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/logo_64.png"));
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
    QObject::connect(myStatsAct, SIGNAL(triggered()), this, SLOT(open_my_stats()));
    QObject::connect(myProfileAct, SIGNAL(triggered()), this, SLOT(open_my_profile()));
    QObject::connect(myAvatarAct, SIGNAL(triggered()), this, SLOT(open_my_avatar()));

    // awaylog
    QObject::connect(awaylogAct, SIGNAL(triggered()), this, SLOT(open_awaylog()));
    // onet cams
    QObject::connect(camsAct, SIGNAL(triggered()), this, SLOT(open_cams()));
    // notes
    QObject::connect(notesAct, SIGNAL(triggered()), this, SLOT(open_notes()));
    // tray connect
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_icon(QSystemTrayIcon::ActivationReason)));

    // signals from tabc
    QObject::connect(pTabC, SIGNAL(create_nicklist(QString)), this, SLOT(create_nicklist(QString)));
    QObject::connect(pTabC, SIGNAL(remove_nicklist(QString)), this, SLOT(remove_nicklist(QString)));
    QObject::connect(pTabC, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(pTabC, SIGNAL(update_nick_avatar(QString)), this, SLOT(update_nick_avatar(QString)));
    QObject::connect(pTabC, SIGNAL(set_open_channels()), this, SLOT(set_open_channels()));
    QObject::connect(pTabC, SIGNAL(update_awaylog_status()), this, SLOT(update_awaylog_status()));

    // signals tab
    QObject::connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
    QObject::connect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(pDlgModeration, SIGNAL(display_msg(QString,QString,int)), pTabC, SLOT(slot_show_msg(QString,QString,int)));

    // signals inputLineWidget
    QObject::connect(pInputLineDockWidget, SIGNAL(show_msg(QString,QString,int)), pTabC, SLOT(slot_show_msg(QString,QString,int)));
    QObject::connect(pInputLineDockWidget, SIGNAL(display_message(QString,QString,int)), pTabC, SLOT(slot_display_message(QString,QString,int)));
    QObject::connect(pInputLineDockWidget, SIGNAL(change_font_size(QString)), pTabC, SLOT(slot_change_font_size(QString)));
    QObject::connect(pInputLineDockWidget, SIGNAL(clear_content(QString)), pTabC, SLOT(slot_clear_content(QString)));

    // signals lag
    QObject::connect(pOnetKernel, SIGNAL(set_lag(QString)), this, SLOT(set_lag(QString)));
    QObject::connect(pOnetKernel, SIGNAL(update_nick(QString)), pInputLineDockWidget, SLOT(slot_update_nick(QString)));
    QObject::connect(pOnetKernel, SIGNAL(clear_nicklist(QString)), this, SLOT(clear_nicklist(QString)));

    // signals from kernel to nicklist
    QObject::connect(pOnetKernel, SIGNAL(add_user(QString,QString,QString,QString)), this, SLOT(add_user(QString,QString,QString,QString)));
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
    QObject::connect(pNetwork, SIGNAL(set_lag(QString)), this, SLOT(set_lag(QString)));
    QObject::connect(pNetwork, SIGNAL(set_connect_enabled(bool)), this, SLOT(set_connect_enabled(bool)));
    QObject::connect(pNetwork, SIGNAL(kernel(QString)), pOnetKernel, SLOT(kernel(QString)));
    QObject::connect(pNetwork, SIGNAL(authorize(QString,QString,QString)), pOnetAuth, SLOT(authorize(QString,QString,QString)));
    QObject::connect(pNetwork, SIGNAL(show_msg_active(QString,int)), pTabC, SLOT(slot_show_msg_active(QString,int)));
    QObject::connect(pNetwork, SIGNAL(show_msg_all(QString,int)), pTabC, SLOT(slot_show_msg_all(QString,int)));
    QObject::connect(pNetwork, SIGNAL(update_nick(QString)), pInputLineDockWidget, SLOT(slot_update_nick(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_nicklist(QString)), this, SLOT(clear_nicklist(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_all_nicklist()), this, SLOT(clear_all_nicklist()));
    QObject::connect(pNetwork, SIGNAL(update_actions()), this, SLOT(update_actions()));
    QObject::connect(pNetwork, SIGNAL(close_cam_socket()), this, SLOT(close_cam_socket()));
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
    if (network_is_connected() == false)
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

    if (bRegistered == true)
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
    if (lAwaylog.size() == 0)
    {
        if (awaylogAct->isVisible() == true)
            awaylogAct->setVisible(false);
    }
    else
    {
        if (awaylogAct->isVisible() == false)
            awaylogAct->setVisible(true);
    }
}

// set lag
void MainWindow::set_lag(QString strValue)
{
    lagAct->setText(strValue);
}

void MainWindow::open_options()
{
    DlgOptions(this).exec();
}

// onet dialogs
void MainWindow::open_channel_list()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgChannelList(this, pNetwork, &stlChannelList).exec();
}

void MainWindow::open_channel_homes()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgChannelHomes(this, pNetwork, &mChannelAvatar, &lChannelHomes, pDlgChannelSettings).exec();
}

void MainWindow::open_channel_favourites()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgChannelFavourites(this, pNetwork, &mChannelAvatar, &lChannelFavourites).exec();
}

void MainWindow::open_friends()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgFriends(this, pNetwork, &mNickAvatar, &mFriends).exec();
}

void MainWindow::open_ignore()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgIgnore(this, pNetwork, &mNickAvatar, &lIgnore).exec();
}

void MainWindow::open_awaylog()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgAwaylog(this, &lAwaylog, awaylogAct).exec();
}

void MainWindow::open_cams()
{
#ifdef Q_WS_WIN
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
    {
        QSettings settings;
        QString strMe = settings.value("nick").toString();
        (new Kamerzysta(camSocket))->show(strMe);
    }
#else
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlgCam->show();
#endif
}

void MainWindow::open_my_stats()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgMyStats(this, &mMyStats).exec();
}

void MainWindow::open_my_profile()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgMyProfile(this, pNetwork, &mNickAvatar, &mMyProfile).exec();
}

void MainWindow::open_my_avatar()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgMyAvatar(this, pNetwork, &mNickAvatar).exec();
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
        if (this->isVisible() == true)
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

// tab changed
void MainWindow::current_tab_changed(int index)
{
    QString strChannel = pTabC->get_name(index);

    // change tab name
    QString strTabText = pTabM->tabText(index);
    this->setWindowTitle(QString("Simple Chat Client - [%1]").arg(strTabText));

    // change tab color
    QSettings settings;
    pTabM->set_color(index, QColor(settings.value("default_font_color").toString()));

    // set tab active
    pInputLineDockWidget->set_active(strChannel);

    // hide/show Status nicklist
    if (index == 0)
        rightDockWidget->hide();
    else
        rightDockWidget->show();

    // hide on classic
    if (settings.value("style") == "classic")
        rightDockWidget->hide();

    // hide/show settings on priv
    if (strChannel[0] == '^')
        pInputLineDockWidget->hide_channel_settings();
    else
        pInputLineDockWidget->show_channel_settings();

    // nicklist
    if (mChannelNickListWidget.contains(strChannel))
    {
        // switch
        NickListWidget *nlw = mChannelNickListWidget.value(strChannel);
        pNickListDockWidget->setCurrentWidget(nlw);
    }
    else
    {
        // create new
        create_nicklist(strChannel);
    }

    // set inputline users
    pInputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

    // update nick count
    rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));

    // moderation
    QString strMe = settings.value("nick").toString();
    QString strPrefix;
    for (int i = 0; i < stlChannelNickStatus.size(); i++)
    {
        if ((stlChannelNickStatus.at(i).nick == strMe) && (stlChannelNickStatus.at(i).channel == strChannel))
        {
            strPrefix = stlChannelNickStatus.at(i).prefix;
            break;
        }
    }
    if (strPrefix.contains("!")) pInputLineDockWidget->enable_moderation();
    else pInputLineDockWidget->disable_moderation();
}

void MainWindow::create_nicklist(QString strChannel)
{
    if (mChannelNickListWidget.contains(strChannel) == false)
    {
        NickListWidget *nicklist = new NickListWidget(pNetwork, strChannel, &mNickAvatar, camSocket, &stlChannelNickStatus, pDlgUserProfile);
#ifndef Q_WS_WIN
        nicklist->set_dlg_cam(pDlgCam);
#endif
        nicklist->setParent(pNickListDockWidget);
        nicklist->setItemDelegate(new NickListDelegate(nicklist));
        nicklist->show();

        mChannelNickListWidget.insert(strChannel, nicklist);
        pNickListDockWidget->addWidget(nicklist);
    }
}

void MainWindow::remove_nicklist(QString strChannel)
{
    if (mChannelNickListWidget.contains(strChannel) == true)
    {
        pNickListDockWidget->removeWidget(mChannelNickListWidget.value(strChannel));
        delete mChannelNickListWidget.value(strChannel);
        mChannelNickListWidget.remove(strChannel);
    }
}

// nick list

bool MainWindow::nicklist_exist(QString strChannel, QString strNick)
{
    if (mChannelNickListWidget.contains(strChannel) == true)
        return mChannelNickListWidget.value(strChannel)->exist(strNick, &stlChannelNickStatus);
    else
        return false;
}

void MainWindow::add_user(QString strChannel, QString strNick, QString strPrefix, QString strSuffix)
{
    if ((nicklist_exist(strChannel, strNick) == false) && (mChannelNickListWidget.contains(strChannel) == true))
    {
        mChannelNickListWidget.value(strChannel)->add(strNick, strPrefix, strSuffix, &stlChannelNickStatus);

        // set inputline users
        if (pInputLineDockWidget->get_active() == strChannel)
            pInputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

        // update nick count for option hide join/part when > 200
        pTabC->add_user(strChannel);

        // update nick count
        if (pInputLineDockWidget->get_active() == strChannel)
            rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
    }
}

void MainWindow::del_user(QString strChannel, QString strNick)
{
    if ((nicklist_exist(strChannel, strNick) == true) && (mChannelNickListWidget.contains(strChannel) == true))
    {
        mChannelNickListWidget.value(strChannel)->remove(strNick, &stlChannelNickStatus);

        // set inputline users
        if (pInputLineDockWidget->get_active() == strChannel)
            pInputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

        // update nick count for option hide join/part when > 200
        pTabC->del_user(strChannel);

        // update nick count
        if (pInputLineDockWidget->get_active() == strChannel)
            rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
    }
}

void MainWindow::nicklist_refresh(QString strChannel)
{
    //raw 366: End of /NAMES list.
    if (mChannelNickListWidget.contains(strChannel) == true)
    {
        mChannelNickListWidget.value(strChannel)->expandAll();
        pInputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));
    }
}

void MainWindow::quit_user(QString strNick, QString strDisplay)
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.size(); i++)
    {
        QString strChannel = strlChannels.at(i);
        if (nicklist_exist(strChannel, strNick) == true)
        {
            int iLevel = 3;
            pTabC->show_msg(strChannel, strDisplay, iLevel);
            del_user(strChannel, strNick);

            if (i+1 != pTabM->currentIndex())
                pTabM->set_alert(i+1, QColor(0, 147, 0, 255)); // green

            // update nick count
            if (pInputLineDockWidget->get_active() == strChannel)
                rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
        }
    }
}

void MainWindow::change_flag(QString strNick, QString strChannel, QString strNewFlag)
{
    if (nicklist_exist(strChannel, strNick) == false) return; // nick not exist

    QString strOldPrefix;
    QString strOldSuffix;

    for (int i = 0; i < stlChannelNickStatus.size(); i++)
    {
        if ((stlChannelNickStatus.at(i).nick == strNick) && (stlChannelNickStatus.at(i).channel == strChannel))
        {
            strOldPrefix = stlChannelNickStatus.at(i).prefix;
            strOldSuffix = stlChannelNickStatus.at(i).suffix;
            break;
        }
    }

    QString strPrefix = strOldPrefix;
    QString strSuffix = strOldSuffix;

    // prefix
    QString strPrefix1 = "qaohXYv";
    QString strPrefix2 = "`&@%!=+";

    for (int i = 0; i < strPrefix1.size(); i++)
    {
        QString plusminus = strNewFlag.at(0);
        QString strFlag = strNewFlag.at(1);

        if (plusminus == "+")
        {
            if ((strFlag == strPrefix1.at(i)) && (strPrefix.indexOf(strPrefix2.at(i)) == -1))
                strPrefix.append(strPrefix2.at(i));
        }
        else
        {
            if ((strFlag == strPrefix1.at(i)) && (strPrefix.indexOf(strPrefix2.at(i)) != -1))
                strPrefix.remove(strPrefix2.at(i));
        }
    }

    // suffix
    QString strSuffix1 = "ObrWVx";
    QString strSuffix2 = "ObrWVx";

    for (int i = 0; i < strSuffix1.size(); i++)
    {
        QString plusminus = strNewFlag.at(0);
        QString strFlag = strNewFlag.at(1);

        if (plusminus == "+")
        {
            if ((strFlag == strSuffix1.at(i)) && (strSuffix.indexOf(strSuffix2.at(i)) == -1))
                strSuffix.append(strSuffix2.at(i));
        }
        else
        {
            if ((strFlag == strSuffix1.at(i)) && (strSuffix.indexOf(strSuffix2.at(i)) != -1))
                strSuffix.remove(strSuffix2.at(i));
        }
    }

    // change flag
    del_user(strChannel, strNick);
    add_user(strChannel, strNick, strPrefix, strSuffix);

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
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.size(); i++)
    {
        QString strChannel = strlChannels.at(i);

        if (nicklist_exist(strChannel, strNick) == true)
            change_flag(strNick, strChannel, strFlag);
    }
}

void MainWindow::clear_nicklist(QString strChannel)
{
    // clear
    for (int i = 0; i < stlChannelNickStatus.size(); i++)
    {
        if (stlChannelNickStatus.at(i).channel == strChannel)
        {
            stlChannelNickStatus.removeAt(i);
            i--;
        }
    }

    // clear nick count for option hide join/part when > 200
    pTabC->clear_users(strChannel);

    // update nick count
    if (pInputLineDockWidget->get_active() == strChannel)
        rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
}

void MainWindow::clear_all_nicklist()
{
    stlChannelNickStatus.clear();
    mNickAvatar.clear();
    mChannelAvatar.clear();

    QStringList strlChannels = pTabC->get_open_channels();

    for (int i = 0; i < strlChannels.size(); i++)
    {
        mChannelNickListWidget.value(strlChannels.at(i))->clear();

        // clear nick count for option hide join/part when > 200
        pTabC->clear_users(strlChannels.at(i));

        // update nick count
        if (pInputLineDockWidget->get_active() == strlChannels.at(i))
            rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strlChannels.at(i))));
    }
}

void MainWindow::update_nick_avatar(QString strNick)
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.size(); i++)
    {
        QString strChannel = strlChannels.at(i);

        // nicklist
        if (mChannelNickListWidget.value(strChannel)->exist(strNick, &stlChannelNickStatus) == true)
            mChannelNickListWidget.value(strChannel)->refresh_avatars();
    }
}

// clear all channel avatars
void MainWindow::clear_channel_all_nick_avatars(QString strChannel)
{
    QStringList strlNicks = mChannelNickListWidget.value(strChannel)->get(&stlChannelNickStatus);

    for (int i = 0; i < strlNicks.size(); i++)
    {
        QString strNick = strlNicks.at(i);

        // remove nick avatar if nick is only in current channel; must be 1 (current channel)
        if ((mNickAvatar.contains(strNick) == true) && (pTabC->get_nick_channels(strNick) == 1))
            mNickAvatar.remove(strNick);
    }
}

void MainWindow::set_open_channels()
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.size(); i++)
    {
        QString strChannel = strlChannels.at(i);

        mChannelNickListWidget.value(strChannel)->set_open_channels(strlChannels);
    }
}
