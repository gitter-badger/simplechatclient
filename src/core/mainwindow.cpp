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
    showAct = new QAction(QIcon(":/images/logo_64.png"), tr("Show"), this);
    connectAct = new QAction(QIcon(":/images/oxygen/16x16/network-connect.png"), tr("Connect"), this);
    closeAct = new QAction(QIcon(":/images/oxygen/16x16/application-exit.png"), tr("Close"), this);
    optionsAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-system.png"), tr("Options"), this);
    aboutAct = new QAction(QIcon(":/images/logo_64.png"), tr("About SCC ..."), this);
    notesAct = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Notes"), this);

    // onet action
    channel_listAct = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"), tr("Channel list"), this);
    channel_homesAct = new QAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("Channel settings"), this);
    channel_favouritesAct = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"), tr("Favorite channels"), this);
    friendsAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"), tr("Friends"), this);
    ignoreAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"), tr("Ignored"), this);
#ifdef Q_WS_WIN
    camsAct = new QAction(QIcon(":/images/pubcam.png"),tr("Cams"), this);
#endif

    // shortcut
    connectAct->setShortcuts(QKeySequence::New);
    closeAct->setShortcut(tr("Ctrl+E"));
    optionsAct->setShortcut(tr("Ctrl+O"));

    // onet shortcut
    channel_listAct->setShortcut(tr("Ctrl+L"));
    channel_homesAct->setShortcut(tr("Ctrl+M"));
    channel_favouritesAct->setShortcut(tr("Ctrl+U"));
    friendsAct->setShortcut(tr("Ctrl+P"));
    ignoreAct->setShortcut(tr("Ctrl+I"));
#ifdef Q_WS_WIN
    camsAct->setShortcut(tr("Ctrl+K"));
#endif

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
    chatMenu->addAction(channel_listAct);
    chatMenu->addAction(channel_homesAct);
    chatMenu->addAction(channel_favouritesAct);
    chatMenu->addAction(friendsAct);
    chatMenu->addAction(ignoreAct);
#ifdef Q_WS_WIN
    chatMenu->addAction(camsAct);
#endif

    // help menu
    helpMenu = menuBar()->addMenu(tr("He&lp"));
    helpMenu->addAction(aboutAct);

    // toolbar
    toolBar = addToolBar(tr("Navigation bar"));
    toolBar->setIconSize(QSize(16,16));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(connectAct);
    toolBar->addAction(optionsAct);

    // onet toolbar
    toolBar->addAction(channel_listAct);
    toolBar->addAction(channel_homesAct);
    toolBar->addAction(friendsAct);
    toolBar->addAction(channel_favouritesAct);
#ifdef Q_WS_WIN
    toolBar->addAction(camsAct);
#endif

    // notes
    toolBar->addAction(notesAct);

    // signals buttons
    QObject::connect(connectAct, SIGNAL(triggered()), this, SLOT(button_connect()));
    QObject::connect(closeAct, SIGNAL(triggered()), this, SLOT(button_close()));
    QObject::connect(optionsAct, SIGNAL(triggered()), this, SLOT(open_options()));
    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(open_about()));
    QObject::connect(showAct, SIGNAL(triggered()), this, SLOT(button_show()));

    // signals onet dialogs
    QObject::connect(channel_listAct, SIGNAL(triggered()), this, SLOT(open_channel_list()));
    QObject::connect(channel_homesAct, SIGNAL(triggered()), this, SLOT(open_channel_homes()));
    QObject::connect(channel_favouritesAct, SIGNAL(triggered()), this, SLOT(open_channel_favourites()));
    QObject::connect(friendsAct, SIGNAL(triggered()), this, SLOT(open_friends()));
    QObject::connect(ignoreAct, SIGNAL(triggered()), this, SLOT(open_ignore()));
#ifdef Q_WS_WIN
    QObject::connect(camsAct, SIGNAL(triggered()), this, SLOT(open_cams()));
#endif

    // notes
    QObject::connect(notesAct, SIGNAL(triggered()), this, SLOT(open_notes()));

    // tray
    trayMenu = new QMenu();
    trayMenu->addAction(showAct);
    trayMenu->addAction(closeAct);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/logo_64.png"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    // tray connect
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_icon(QSystemTrayIcon::ActivationReason)));

    // show options if config not exist
    QString path;
#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    path = QCoreApplication::applicationDirPath();
#endif
    QString strConfigFile = path+"/scc.conf";
    if (QFile::exists(strConfigFile) == false)
        QTimer::singleShot(1000, this, SLOT(open_options()));

    // create settings
    create_settings();

    // options & notify
    pNotify = new Notify();
    pOptions = new DlgOptions(this, pNotify);

    // refresh colors
    refresh_colors();

    // update
    QTimer::singleShot(1000*5, this, SLOT(check_update()));

    // signal refresh colors
    QObject::connect(pOptions, SIGNAL(refresh_colors()), this, SLOT(refresh_colors()));

    // create basic core
    coreServers.append(new Core(this, "czat-app.onet.pl", 5015, pNotify, connectAct, toolBar, viewMenu));
}

MainWindow::~MainWindow()
{
    // kill core servers
    while(coreServers.isEmpty() == false)
        coreServers.removeFirst();

    // kill update threads
    while(uThreadList.isEmpty() == false)
    {
        uThreadList.first()->kill_thread();
        uThreadList.removeFirst();;
    }

    // hide tray
    trayIcon->hide();

    // delete objects
    delete pNotify;
    delete pOptions;

    // delete teray
    delete trayIcon;
    delete trayMenu;

    // clear settings
    QSettings settings;
    settings.clear();
}


// args
void MainWindow::set_debug(bool param1)
{
    QSettings settings;
    if (param1 == true)
        settings.setValue("debug", "on");
    else
        settings.setValue("debug", "off");
}

// create settings
void MainWindow::create_settings()
{
    // config values
    Config *pConfig = new Config();
    QString strLoginNick = pConfig->get_value("login-nick");
    QString strLoginPass = pConfig->get_value("login-pass");
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
    QString strNicklistNickColor = pConfig->get_value("nicklist_nick_color");
    QString strNicklistSelectedNickColor = pConfig->get_value("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = pConfig->get_value("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = pConfig->get_value("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = pConfig->get_value("nicklist_gradient_2_color");
    QString strSoundBeep = pConfig->get_value("sound_beep");
    QString strSoundQuery = pConfig->get_value("sound_query");
    delete pConfig;

    // fix config values
    if (strStyle == "classic")
        strDisableAvatars = "on";

    // settings
    QSettings settings;
    settings.clear();
    settings.setValue("version", "1.0.9.594");
    settings.setValue("debug", "off");
    settings.setValue("logged", "off");
    settings.setValue("busy", "off");
    settings.setValue("override", "off");
    settings.setValue("autojoin_favourites", "on");
    settings.setValue("nick", strLoginNick);
    settings.setValue("pass", strLoginPass);
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
    settings.setValue("nicklist_nick_color", strNicklistNickColor);
    settings.setValue("nicklist_selected_nick_color", strNicklistSelectedNickColor);
    settings.setValue("nicklist_busy_nick_color", strNicklistBusyNickColor);
    settings.setValue("nicklist_gradient_1_color", strNicklistGradient1Color);
    settings.setValue("nicklist_gradient_2_color", strNicklistGradient2Color);
    settings.setValue("sound_beep", strSoundBeep);
    settings.setValue("sound_query", strSoundQuery);
    settings.setValue("uokey", "");
    settings.setValue("uo_nick", "");
    settings.setValue("onet_ubi", "");
    settings.setValue("onet_cid", "");
    settings.setValue("onet_sid", "");
    settings.setValue("onet_uid", "");
}

void MainWindow::check_update()
{
// update
    uThreadList.append(new Update(this));
    QObject::connect(uThreadList.at(uThreadList.size()-1), SIGNAL(sremove_uthread(Update*)), this, SLOT(remove_uthread(Update*)));

#ifdef Q_WS_X11
    QSettings settings;
    if (settings.value("debug").toString() == "on")
        qDebug() << "Update thread +1 (size: " << uThreadList.size() << ")";
#endif
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

    for (int i = 0; i < coreServers.count(); i++)
        coreServers.at(i)->refresh_colors();
}

// buttons
void MainWindow::button_connect()
{
    QSettings settings;
    if (coreServers.at(0)->network_is_connected() == false)
    {
        connectAct->setText(tr("&Disconnect"));
        connectAct->setIconText(tr("&Disconnect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
        settings.setValue("reconnect", "true");
        coreServers.at(0)->network_connect();

    }
    else
    {
        settings.setValue("reconnect", "false");
        settings.setValue("logged", "off");
        coreServers.at(0)->network_send("QUIT");
        connectAct->setText(tr("&Connect"));
        connectAct->setIconText(tr("&Connect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
        coreServers.at(0)->network_disconnect();
    }
}

void MainWindow::button_close()
{
    this->close();
}

void MainWindow::open_options()
{
    pOptions->show();
}

void MainWindow::open_about()
{
    (new DlgAbout(this))->show();
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

// onet dialogs
void MainWindow::open_channel_list()
{
    coreServers.at(0)->open_channel_list();
}

void MainWindow::open_channel_homes()
{
    coreServers.at(0)->open_channel_homes();
}

void MainWindow::open_channel_favourites()
{
    coreServers.at(0)->open_channel_favourites();
}

void MainWindow::open_friends()
{
    coreServers.at(0)->open_friends();
}

void MainWindow::open_ignore()
{
    coreServers.at(0)->open_ignore();
}

void MainWindow::open_cams()
{
    coreServers.at(0)->open_cams();
}

void MainWindow::open_notes()
{
    (new DlgNotes(this))->show();
}
