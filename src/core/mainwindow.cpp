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

#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
// init
    toolBar = new QToolBar();
    setWindowTitle("Simple Chat Client");
    setWindowIcon(QIcon(":/images/logo_64.png"));

// action
    showAct = new QAction(QIcon(":/images/logo_64.png"),tr("Show"),this);
    connectAct = new QAction(QIcon(":/images/oxygen/16x16/network-disconnect.png"),tr("Connect"),this);
    closeAct = new QAction(QIcon(":/images/oxygen/16x16/application-exit.png"),tr("Close"),this);
    optionsAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-system.png"),tr("Options"),this);
    channel_listAct = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"),tr("Channel list"),this);
    channel_homesAct = new QAction(QIcon(":/images/oxygen/16x16/configure.png"),tr("Channel settings"),this);
    channel_favouritesAct = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"),tr("Favorite channels"),this);
    friendsAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"),tr("Friends"),this);
    ignoreAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"),tr("Ignored"),this);
#ifdef Q_WS_WIN
    camsAct = new QAction(QIcon(":/images/pubcam.png"),tr("Cams"),this);
#endif
    aboutAct = new QAction(QIcon(":/images/logo_64.png"),tr("About SCC ..."),this);
    lagAct = new QAction("Lag: 0s", this);

// shortcut
    connectAct->setShortcuts(QKeySequence::New);
    closeAct->setShortcut(tr("Ctrl+E"));
    optionsAct->setShortcut(tr("Ctrl+O"));
    channel_listAct->setShortcut(tr("Ctrl+L"));
    channel_homesAct->setShortcut(tr("Ctrl+M"));
    channel_favouritesAct->setShortcut(tr("Ctrl+U"));
    friendsAct->setShortcut(tr("Ctrl+P"));
    ignoreAct->setShortcut(tr("Ctrl+I"));
#ifdef Q_WS_WIN
    camsAct->setShortcut(tr("Ctrl+K"));
#endif

// show options if config not exist
    QString path = QCoreApplication::applicationDirPath();
    QString strConfigFile = path+"/scc.conf";
    if (QFile::exists(strConfigFile) == false)
        QTimer::singleShot(1000, this, SLOT(options_dlg()));

// config values
    Config *pConfig = new Config();
    QString strLanguage = pConfig->get_value("language");
    QString strAutoBusy = pConfig->get_value("auto_busy");
    QString strShowZuo = pConfig->get_value("show_zuo");
    QString strHideFormating = pConfig->get_value("hide_formating");
    QString strHideJoinPart = pConfig->get_value("hide_join_part");
    QString strHideJoinPart200 = pConfig->get_value("hide_join_part_200");
    QString strDisableAvatars = pConfig->get_value("disable_avatars");
    QString strDisableLogs = pConfig->get_value("disable_logs");
    QString strDisableSounds = pConfig->get_value("disable_sounds");
    QString strStyle = pConfig->get_value("style");
    QString strBackgroundColor = pConfig->get_value("background_color");
    QString strMyBold = pConfig->get_value("my_bold");
    QString strMyItalic = pConfig->get_value("my_italic");
    QString strMyFont = pConfig->get_value("my_font");
    QString strMyColor = pConfig->get_value("my_color");
    QString strDefaultFontColor = pConfig->get_value("default_font_color");
    QString strJoinFontColor = pConfig->get_value("font_color_level_1");
    QString strPartFontColor = pConfig->get_value("font_color_level_2");
    QString strQuitFontColor = pConfig->get_value("font_color_level_3");
    QString strKickFontColor = pConfig->get_value("font_color_level_4");
    QString strModeFontColor = pConfig->get_value("font_color_level_5");
    QString strNoticeFontColor = pConfig->get_value("font_color_level_6");
    QString strInfoFontColor = pConfig->get_value("font_color_level_7");
    QString strErrorFontColor = pConfig->get_value("font_color_level_9");
    QString strChannelFontColor = pConfig->get_value("channel_font_color");
    QString strNicklistBackgroundColor = pConfig->get_value("nicklist_background_color");
    QString strNicklistLineColor = pConfig->get_value("nicklist_line_color");
    QString strNicklistSelectedLineColor = pConfig->get_value("nicklist_selected_line_color");
    QString strNicklistNickColor = pConfig->get_value("nicklist_nick_color");
    QString strNicklistSelectedNickColor = pConfig->get_value("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = pConfig->get_value("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = pConfig->get_value("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = pConfig->get_value("nicklist_gradient_2_color");
    delete pConfig;

// settings
    QSettings settings;
    settings.clear();
    settings.setValue("version", "1.0.9.532");
    settings.setValue("debug", "off");
    settings.setValue("logged", "off");
    settings.setValue("busy", "off");
    settings.setValue("override", "off");
    settings.setValue("autojoin_favourites", "on");
    settings.setValue("language", strLanguage);
    settings.setValue("auto_busy", strAutoBusy);
    settings.setValue("show_zuo", strShowZuo);
    settings.setValue("hide_formating", strHideFormating);
    settings.setValue("hide_join_part", strHideJoinPart);
    settings.setValue("hide_join_part_200", strHideJoinPart200);
    settings.setValue("disable_avatars", strDisableAvatars);
    settings.setValue("disable_logs", strDisableLogs);
    settings.setValue("disable_sounds", strDisableSounds);
    settings.setValue("style", strStyle);
    settings.setValue("my_bold", strMyBold);
    settings.setValue("my_italic", strMyItalic);
    settings.setValue("my_font", strMyFont);
    settings.setValue("my_color", strMyColor);
    settings.setValue("background_color", strBackgroundColor);
    settings.setValue("default_font_color", strDefaultFontColor);
    settings.setValue("font_color_level_1", strJoinFontColor);
    settings.setValue("font_color_level_2", strPartFontColor);
    settings.setValue("font_color_level_3", strQuitFontColor);
    settings.setValue("font_color_level_4", strKickFontColor);
    settings.setValue("font_color_level_5", strModeFontColor);
    settings.setValue("font_color_level_6", strNoticeFontColor);
    settings.setValue("font_color_level_7", strInfoFontColor);
    settings.setValue("font_color_level_9", strErrorFontColor);
    settings.setValue("channel_font_color", strChannelFontColor);
    settings.setValue("nicklist_background_color", strNicklistBackgroundColor);
    settings.setValue("nicklist_line_color", strNicklistLineColor);
    settings.setValue("nicklist_selected_line_color", strNicklistSelectedLineColor);
    settings.setValue("nicklist_nick_color", strNicklistNickColor);
    settings.setValue("nicklist_selected_nick_color", strNicklistSelectedNickColor);
    settings.setValue("nicklist_busy_nick_color", strNicklistBusyNickColor);
    settings.setValue("nicklist_gradient_1_color", strNicklistGradient1Color);
    settings.setValue("nicklist_gradient_2_color", strNicklistGradient2Color);
    settings.setValue("uokey", "");
    settings.setValue("uo_nick", "");
    settings.setValue("onet_ubi", "");
    settings.setValue("onet_cid", "");
    settings.setValue("onet_sid", "");
    settings.setValue("onet_uid", "");

// init all
    camSocket = new QTcpSocket();

    pTabM = new TabManager(this);
    setCentralWidget(pTabM);

    pNetwork = new Network(this, connectAct, lagAct);
    pNotify = new Notify();
    pTabC = new TabContainer(this, pNetwork, pTabM, pNotify, &mNickAvatar, &mChannelAvatar, camSocket);

    pDlg_channel_settings = new DlgChannelSettings(this, pNetwork);
    pDlg_moderation = new DlgModeration(this);
    pDlg_channel_list = new DlgChannelList(this);
    pDlg_channel_homes = new DlgChannelHomes(this, pNetwork, &mChannelAvatar, pDlg_channel_settings);
    pDlg_channel_favourites = new DlgChannelFavourites(this, pNetwork, &mChannelAvatar);
    pDlg_friends = new DlgFriends(this, pNetwork, &mNickAvatar);
    pDlg_ignore = new DlgIgnore(this, pNetwork, &mNickAvatar);

    pOnet_kernel = new OnetKernel(this, pNetwork, pTabC, &mNickAvatar, &mChannelAvatar, pDlg_channel_settings, pDlg_channel_homes, pDlg_channel_list, pDlg_channel_favourites, pDlg_friends, pDlg_ignore, pDlg_moderation);
    pOnet_auth = new OnetAuth(pTabC);

    pTabC->set_dlg(pDlg_channel_settings, pDlg_moderation);

    pOptions = new DlgOptions(this);

// welcome
    pTabC->show_msg("Status", "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%Cff6500%"+tr("Official website")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/ %Izaskoczony%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%C008100%"+tr("Official Forum")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/forum/ %Ioczko%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%Cff0000%"+tr("Known bugs")+"%C3030ce%: http://sourceforge.net/apps/trac/simplechatclien/report/6 %Iskwaszony%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%C8800ab%"+tr("Bug reporting")+"%C3030ce%: http://sourceforge.net/apps/trac/simplechatclien/newticket %Ipanda%", 0);

// update
    QTimer::singleShot(1000*5, this, SLOT(check_update()));

// main menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(connectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);

    optionsMenu = menuBar()->addMenu(tr("&Settings"));
    optionsMenu->addAction(optionsAct);

    chatMenu = menuBar()->addMenu(tr("&Chat"));
    chatMenu->addAction(channel_listAct);
    chatMenu->addAction(channel_homesAct);
    chatMenu->addAction(channel_favouritesAct);
    chatMenu->addAction(friendsAct);
    chatMenu->addAction(ignoreAct);
#ifdef Q_WS_WIN
    chatMenu->addAction(camsAct);
#endif

    helpMenu = menuBar()->addMenu(tr("He&lp"));
    helpMenu->addAction(aboutAct);

// toolbar
    toolBar = addToolBar(tr("Navigation bar"));
    toolBar->setIconSize(QSize(16,16));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(connectAct);
    toolBar->addAction(optionsAct);
    toolBar->addAction(channel_listAct);
    toolBar->addAction(channel_homesAct);
    toolBar->addAction(friendsAct);
    toolBar->addAction(channel_favouritesAct);
#ifdef Q_WS_WIN
    toolBar->addAction(camsAct);
#endif
    toolBar->addSeparator();
    toolBar->addAction(lagAct);

// signals buttons
    QObject::connect(connectAct, SIGNAL(triggered()), this, SLOT(button_connect()));
    QObject::connect(optionsAct, SIGNAL(triggered()), this, SLOT(options_dlg()));
    QObject::connect(channel_listAct, SIGNAL(triggered()), this, SLOT(channel_list_dlg()));
    QObject::connect(channel_homesAct, SIGNAL(triggered()), this, SLOT(channel_homes_dlg()));
    QObject::connect(channel_favouritesAct, SIGNAL(triggered()), this, SLOT(channel_favourites_dlg()));
    QObject::connect(friendsAct, SIGNAL(triggered()), this, SLOT(friends_dlg()));
    QObject::connect(ignoreAct, SIGNAL(triggered()), this, SLOT(ignore_dlg()));
#ifdef Q_WS_WIN
    QObject::connect(camsAct, SIGNAL(triggered()), this, SLOT(cams_clicked()));
#endif
    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(about_dlg()));
    QObject::connect(showAct, SIGNAL(triggered()), this, SLOT(button_show()));
    QObject::connect(closeAct, SIGNAL(triggered()), this, SLOT(button_close()));
// signals tab
    QObject::connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
    QObject::connect(pDlg_moderation, SIGNAL(display_msg(QString,QString,int)), pTabC, SLOT(sshow_msg(QString,QString,int)));
// signals lag
    QObject::connect(pOnet_kernel, SIGNAL(set_lag(QString)), this, SLOT(set_lag(QString)));

// signals to network send
    QObject::connect(pDlg_moderation, SIGNAL(send(QString)), pNetwork, SLOT(send_slot(QString)));
    QObject::connect(pDlg_channel_list, SIGNAL(send(QString)), pNetwork, SLOT(send_slot(QString)));
    QObject::connect(pOnet_auth, SIGNAL(send(QString)), pNetwork, SLOT(send_slot(QString)));

// signals onet kernel, onet auth
    QObject::connect(this, SIGNAL(do_kernel(QString)), pOnet_kernel, SLOT(kernel(QString)));
    QObject::connect(this, SIGNAL(do_request_uo(QString, QString, QString)), pOnet_auth, SLOT(request_uo(QString, QString, QString)));

// signal refresh colors
    QObject::connect(pOptions, SIGNAL(refresh_colors()), pTabC, SLOT(refresh_colors()));

// tray
    trayMenu = new QMenu();
    trayMenu->addAction(showAct);
    trayMenu->addAction(closeAct);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/logo_64.png"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_icon(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
    // clear arrays
    mNickAvatar.clear();
    mChannelAvatar.clear();
    trayIcon->hide();

    // close network
    QSettings settings;
    settings.setValue("reconnect", "false");
    pNetwork->send("QUIT");
    pNetwork->close();

    // delete objects
    delete trayIcon;
    delete trayMenu;
    delete pOptions;
    delete pOnet_auth;
    delete pOnet_kernel;
    delete pDlg_ignore;
    delete pDlg_friends;
    delete pDlg_channel_favourites;
    delete pDlg_channel_homes;
    delete pDlg_channel_list;
    delete pDlg_moderation;
    delete pDlg_channel_settings;
    delete pNotify;
    delete pNetwork;
    delete pTabC;
    delete pTabM;
    delete camSocket;

    // clear settings
    settings.clear();
}

void MainWindow::remove_uthread(Update *thr)
{
    thr->QObject::disconnect();
    uThreadList.removeOne(thr);

#ifdef Q_WS_X11
    QSettings settings;
    if (settings.value("debug").toString() == "on")
        qDebug() << "Update thread -1 (size: " << uThreadList.size() << ")";
#endif
}

// slots

void MainWindow::kernel(QString param1) { emit do_kernel(param1); }
void MainWindow::request_uo(QString param1, QString param2, QString param3) { emit do_request_uo(param1, param2, param3); }
void MainWindow::show_msg_active(QString param1, int param2) { pTabC->show_msg_active(param1, param2); }
void MainWindow::show_msg_all(QString param1, int param2) { pTabC->show_msg_all(param1, param2); }
void MainWindow::update_nick(QString param1) { pTabC->update_nick(param1); }
void MainWindow::clear_nicklist(QString param1) { pTabC->clear_nicklist(param1); }
void MainWindow::clear_all_nicklist() { pTabC->clear_all_nicklist(); }

// args
void MainWindow::set_debug(bool param1)
{
    QSettings settings;
    if (param1 == true)
        settings.setValue("debug", "on");
    else
        settings.setValue("debug", "off");
}

// set lag
void MainWindow::set_lag(QString strValue)
{
    lagAct->setText(strValue);
}

void MainWindow::check_update()
{
// update
    uThreadList.append(new Update(this, pTabC));
    QObject::connect(uThreadList.at(uThreadList.size()-1), SIGNAL(do_remove_uthread(Update*)), this, SLOT(remove_uthread(Update*)));

#ifdef Q_WS_X11
    QSettings settings;
    if (settings.value("debug").toString() == "on")
        qDebug() << "Update thread +1 (size: " << uThreadList.size() << ")";
#endif
}

// buttons
void MainWindow::button_close()
{
    this->close();
}

void MainWindow::button_connect()
{
    QSettings settings;
    if (pNetwork->is_connected() == false)
    {
        connectAct->setText(tr("&Disconnect"));
        connectAct->setIconText(tr("&Disconnect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
        settings.setValue("reconnect", "true");
        pNetwork->connect();
    }
    else
    {
        settings.setValue("reconnect", "false");
        pNetwork->send("QUIT");
        connectAct->setText(tr("&Connect"));
        connectAct->setIconText(tr("&Connect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
        pNetwork->close();
    }
}

void MainWindow::options_dlg()
{
    pOptions->show();
}

void MainWindow::channel_list_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_list->show();
}

void MainWindow::channel_homes_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_homes->show();
}

void MainWindow::channel_favourites_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_favourites->show();
}

void MainWindow::friends_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_friends->show();
}

void MainWindow::ignore_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_ignore->show();
}

void MainWindow::cams_clicked()
{
#ifdef Q_WS_WIN
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
    {
        Config *pConfig = new Config();
        QString strMe = pConfig->get_value("login-nick");
        delete pConfig;

        QSettings settings;
        QString strUOKey = settings.value("uokey").toString();
        (new Kamerzysta(camSocket))->show(strMe, strUOKey);
    }
#endif
}

void MainWindow::about_dlg()
{
    (new DlgAbout(this))->show();
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

// tray button
void MainWindow::button_show()
{
    this->showNormal();
}

// part tab
void MainWindow::tab_close_requested(int index)
{
    if (index != 0)
        pTabC->part_tab(index);
}
