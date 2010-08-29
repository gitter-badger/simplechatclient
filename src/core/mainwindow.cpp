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
    connectAct = new QAction(QIcon(":/3rdparty/images/connect.png"),tr("Connect"),this);
    closeAct = new QAction(QIcon(":/3rdparty/images/exit.png"),tr("Close"),this);
    optionsAct = new QAction(QIcon(":/3rdparty/images/options.png"),tr("Options"),this);
    channel_listAct = new QAction(QIcon(":/3rdparty/images/list.png"),tr("Channel list"),this);
    channel_homesAct = new QAction(QIcon(":/3rdparty/images/list.png"),tr("Channel settings"),this);
    channel_favouritesAct = new QAction(QIcon(":/3rdparty/images/list.png"),tr("Favorite channels"),this);
    friendsAct = new QAction(QIcon(":/3rdparty/images/people.png"),tr("Friends"),this);
    ignoreAct = new QAction(QIcon(":/3rdparty/images/people.png"),tr("Ignored"),this);
#ifdef Q_WS_WIN
    camsAct = new QAction(QIcon(":/images/cam.png"),tr("Cams"),this);
#endif
    aboutAct = new QAction(QIcon(":/images/logo_64.png"),tr("About SCC ..."),this);

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
    delete pConfig;

// settings
    settings.clear();
    settings.setValue("version", "1.0.7.451");
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
    settings.setValue("background_color", strBackgroundColor);
    settings.setValue("my_bold", strMyBold);
    settings.setValue("my_italic", strMyItalic);
    settings.setValue("my_font", strMyFont);
    settings.setValue("my_color", strMyColor);
    settings.setValue("uokey", "");
    settings.setValue("uo_nick", "");
    settings.setValue("onet_ubi", "");
    settings.setValue("onet_cid", "");
    settings.setValue("onet_sid", "");
    settings.setValue("onet_uid", "");

// init all
    camSocket = new QTcpSocket();

    pTabM = new TabManager(this, &settings);
    setCentralWidget(pTabM);

    pNetwork = new Network(this, connectAct, &settings);
    pNotify = new Notify();
    pTabC = new TabContainer(this, pNetwork, &settings, pTabM, pNotify, &mNickAvatar, &mChannelAvatar, camSocket);

    pDlg_channel_settings = new DlgChannelSettings(this, pNetwork, &settings);
    pDlg_moderation = new DlgModeration(this, &settings);
    pDlg_channel_list = new DlgChannelList(this, &settings);
    pDlg_channel_homes = new DlgChannelHomes(this, pNetwork, &settings, &mChannelAvatar, pDlg_channel_settings);
    pDlg_channel_favourites = new DlgChannelFavourites(this, pNetwork, &settings, &mChannelAvatar);
    pDlg_friends = new DlgFriends(this, pNetwork, &settings, &mNickAvatar);
    pDlg_ignore = new DlgIgnore(this, pNetwork, &settings, &mNickAvatar);

    pOnet_kernel = new OnetKernel(this, pNetwork, &settings, pTabC, &mNickAvatar, &mChannelAvatar, pDlg_channel_settings, pDlg_channel_homes, pDlg_channel_list, pDlg_channel_favourites, pDlg_friends, pDlg_ignore, pDlg_moderation);
    pOnet_auth = new OnetAuth(&settings, pTabC);

    pTabC->set_dlg(pDlg_channel_settings, pDlg_moderation);

    pOptions = new DlgOptions(this, &settings);

// welcome
    pTabC->show_msg("Status", "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%Cff6500%"+tr("Official website")+" SCC%C000000%: http://simplechatclien.sourceforge.net/ %Izaskoczony%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%C008100%"+tr("Official Forum")+" SCC%C000000%: http://simplechatclien.sourceforge.net/forum/ %Ioczko%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%Cff0000%"+tr("Known bugs")+"%C000000%: http://sourceforge.net/apps/trac/simplechatclien/report/6 %Iskwaszony%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%C8800ab%"+tr("Bug reporting")+"%C000000%: http://sourceforge.net/apps/trac/simplechatclien/newticket %Ipanda%", 0);

// update
    uThreadList.append(new UpdateThread(this, &settings, pTabC));
    QObject::connect(uThreadList.at(uThreadList.size()-1), SIGNAL(do_remove_uthread(UpdateThread*)), this, SLOT(remove_uthread(UpdateThread*)));

#ifdef Q_WS_X11
    //if (settings.value("debug").toString() == "on")
        qDebug() << "Update thread +1 (size: " << uThreadList.size() << ")";
#endif

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

// statusbar
    lLag = new QLabel();
    lLag->setText("");
    statusBar()->addWidget(lLag);

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
// signals status bar
    QObject::connect(pOnet_kernel, SIGNAL(set_statusbar(QString)), this, SLOT(set_statusbar(QString)));

// signals to network send
    QObject::connect(pDlg_moderation, SIGNAL(send(QString)), pNetwork, SLOT(send_slot(QString)));
    QObject::connect(pDlg_channel_list, SIGNAL(send(QString)), pNetwork, SLOT(send_slot(QString)));
    QObject::connect(pOnet_auth, SIGNAL(send(QString)), pNetwork, SLOT(send_slot(QString)));

// signals onet kernel, onet auth
    QObject::connect(this, SIGNAL(do_kernel(QString)), pOnet_kernel, SLOT(kernel(QString)));
    QObject::connect(this, SIGNAL(do_request_uo(QString, QString, QString)), pOnet_auth, SLOT(request_uo(QString, QString, QString)));

// signal refresh background color
    QObject::connect(pOptions, SIGNAL(refresh_background_color()), this, SLOT(refresh_background_color()));

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
    lCloseChannelsList.clear();
    mNickAvatar.clear();
    mChannelAvatar.clear();
    trayIcon->hide();

    // close network
    settings.setValue("reconnect", "false");
    pNetwork->send("QUIT");
    pNetwork->close();

    // delete objects
    delete trayIcon;
    delete trayMenu;
    delete lLag;
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
}

void MainWindow::remove_uthread(UpdateThread *thr)
{
    thr->QObject::disconnect();
    uThreadList.removeOne(thr);

#ifdef Q_WS_X11
    //if (settings.value("debug").toString() == "on")
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
    if (param1 == true)
        settings.setValue("debug", "on");
    else
        settings.setValue("debug", "off");
}

// set statusbar
void MainWindow::set_statusbar(QString strValue)
{
    lLag->setText(strValue);
}

void MainWindow::refresh_background_color()
{
    pTabC->refresh_background_color();
}

// force close tab after 10 sec
void MainWindow::force_close_tab()
{
    while (lCloseChannelsList.isEmpty() == false)
    {
        QString strChannel = lCloseChannelsList.takeFirst();
        pTabC->remove_tab(strChannel);
    }
}

// buttons
void MainWindow::button_close()
{
    this->close();
}

void MainWindow::button_connect()
{
    if (pNetwork->is_connected() == false)
    {
        connectAct->setText(tr("&Disconnect"));
        connectAct->setIconText(tr("&Disconnect"));
        settings.setValue("reconnect", "true");
        pNetwork->connect();
    }
    else
    {
        settings.setValue("reconnect", "false");
        pNetwork->send("QUIT");
        connectAct->setText(tr("&Connect"));
        connectAct->setIconText(tr("&Connect"));
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

        QString strUOKey = settings.value("uokey").toString();
        (new Kamerzysta(camSocket))->show(strMe, strUOKey);
    }
#endif
}

void MainWindow::about_dlg()
{
    (new DlgAbout(this, &settings))->show();
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

// close tab
void MainWindow::tab_close_requested(int index)
{
    QString strName = pTabM->tabText(index);
    if (strName != "Status")
    {
        if (strName[0] == '#')
        {
            if (pNetwork->is_connected() == true)
            {
                pNetwork->send(QString("PART %1").arg(strName));
                lCloseChannelsList.append(strName);
                QTimer::singleShot(1000*10, this, SLOT(force_close_tab())); // 10 sec
            }
            else
                pTabC->remove_tab(strName);
        }
        else
        {
            if (pTabC->exist_tab(strName) == false)
            {
                QString strNewName = settings.value("priv"+strName).toString();
                if (strNewName.isEmpty() == false)
                {
                    if (pNetwork->is_connected() == true)
                    {
                        pNetwork->send(QString("PART %1").arg(strNewName));
                        lCloseChannelsList.append(strNewName);
                        QTimer::singleShot(1000*10, this, SLOT(force_close_tab())); // 10 sec
                    }
                    else
                        pTabC->remove_tab(strNewName);
                }
            }
            else
            {
                if (pNetwork->is_connected() == true)
                {
                    pNetwork->send(QString("PART %1").arg(strName));
                    lCloseChannelsList.append(strName);
                    QTimer::singleShot(1000*10, this, SLOT(force_close_tab())); // 10 sec
                }
                else
                    pTabC->remove_tab(strName);
            }
        }
    }
}
