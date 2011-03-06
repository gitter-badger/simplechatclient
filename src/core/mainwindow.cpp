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

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QTimer>
#include "config.h"
#include "core.h"
#include "dlg_about.h"
#include "dlg_notes.h"
#include "dlg_options.h"
#include "notify.h"
#include "update.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // init
    toolBar = new QToolBar();
    setWindowTitle("Simple Chat Client");
    setWindowIcon(QIcon(":/images/logo_64.png"));

    // action
    showAct = new QAction(QIcon(":/images/logo_64.png"), tr("Show"), this);
    connectAct = new QAction(QIcon(":/images/oxygen/16x16/network-connect.png"), tr("&Connect"), this);
    closeAct = new QAction(QIcon(":/images/oxygen/16x16/application-exit.png"), tr("Close"), this);
    optionsAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-system.png"), tr("Options"), this);
    aboutAct = new QAction(QIcon(":/images/logo_64.png"), tr("About SCC ..."), this);
    awaylogAct = new QAction(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Awaylog"), this);
    notesAct = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Notes"), this);

    // onet action
    channel_listAct = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"), tr("Channel list"), this);
    channel_homesAct = new QAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("Your channels"), this);
    channel_favouritesAct = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"), tr("Favorite channels"), this);
    friendsAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"), tr("Friends"), this);
    ignoreAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"), tr("Ignored"), this);
    camsAct = new QAction(QIcon(":/images/pubcam.png"),tr("Cams"), this);
    myStatsAct = new QAction(QIcon(":/images/oxygen/16x16/office-chart-bar.png"),tr("My statistics"), this);
    myProfileAct = new QAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"),tr("My profile"), this);

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
    awaylogAct->setShortcut(tr("Ctrl+J"));
    camsAct->setShortcut(tr("Ctrl+K"));

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
    chatMenu->addAction(awaylogAct);
    chatMenu->addAction(camsAct);

    // onet my menu
    chatMenu = menuBar()->addMenu(tr("&My"));
    chatMenu->addAction(myStatsAct);
    chatMenu->addAction(myProfileAct);

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

    // awaylog
    toolBar->addAction(awaylogAct);
    // onet cams
    toolBar->addAction(camsAct);
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
    QObject::connect(myStatsAct, SIGNAL(triggered()), this, SLOT(open_my_stats()));
    QObject::connect(myProfileAct, SIGNAL(triggered()), this, SLOT(open_my_profile()));

    // awaylog
    QObject::connect(awaylogAct, SIGNAL(triggered()), this, SLOT(open_awaylog()));
    // onet cams
    QObject::connect(camsAct, SIGNAL(triggered()), this, SLOT(open_cams()));
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
    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
    settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    path = settings.value("Personal").toString();
    path += "/scc";
#endif
    QString strConfigFile = path+"/scc.conf";
    if (QFile::exists(strConfigFile) == false)
        QTimer::singleShot(1000*1, this, SLOT(open_options())); // 1 sec

    // create settings
    create_settings();

    // options & notify
    pNotify = new Notify();
    pOptions = new DlgOptions(this, pNotify);
    pUpdate = new Update(this);

    // refresh colors
    refresh_colors();

    // hide awaylog
    awaylogAct->setVisible(false);

    // signal refresh colors
    QObject::connect(pOptions, SIGNAL(refresh_colors()), this, SLOT(refresh_colors()));
    // signal refresh background image
    QObject::connect(pOptions, SIGNAL(refresh_background_image()), this, SLOT(refresh_background_image()));

    // create basic core
    coreServers.append(new Core());
    coreServers.at(0)->init(this,"czat-app.onet.pl", 5015, pNotify, toolBar, viewMenu, connectAct, channel_homesAct, channel_favouritesAct, friendsAct, ignoreAct, myStatsAct, myProfileAct, awaylogAct);

    // update
    pUpdate->check_update();
}

MainWindow::~MainWindow()
{
    // kill core servers
    while(coreServers.isEmpty() == false)
    {
        delete coreServers.first();
        coreServers.removeFirst();
    }

    // hide tray
    trayIcon->hide();

    // delete update
    delete pUpdate;

    // delete objects
    delete pOptions;
    delete pNotify;

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
    // default settings
    QSettings settings;
    settings.clear();
    settings.setValue("version", "1.0.10.741");
    settings.setValue("debug", "off");
    settings.setValue("logged", "off");
    settings.setValue("busy", "off");
    settings.setValue("away", "off");
    settings.setValue("override", "off");
    settings.setValue("ignore_raw_141", "off");
    settings.setValue("age_check", "on");
    settings.setValue("uokey", "");
    settings.setValue("uo_nick", "");
    settings.setValue("onet_ubi", "");
    settings.setValue("onet_cid", "");
    settings.setValue("onet_sid", "");
    settings.setValue("onet_uid", "");

    // config values
    Config *pConfig = new Config();
    QMap <QString, QString> mConfigValues = pConfig->read_config();
    delete pConfig;

    // set settings
    QMapIterator <QString, QString> i(mConfigValues);
    while (i.hasNext())
    {
        i.next();
        settings.setValue(i.key(), i.value());
    }

    // fix config values
    if (settings.value("style").toString() == "classic")
        settings.setValue("disable_avatars", "on");
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

void MainWindow::refresh_background_image()
{
    for (int i = 0; i < coreServers.count(); i++)
        coreServers.at(i)->refresh_background_image();
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
    if (pOptions->isHidden())
        pOptions->show();
}

void MainWindow::open_about()
{
    DlgAbout(this).exec();
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

// awaylog
void MainWindow::open_awaylog()
{
    coreServers.at(0)->open_awaylog();
}

// onet cams
void MainWindow::open_cams()
{
    coreServers.at(0)->open_cams();
}

// notes
void MainWindow::open_notes()
{
    DlgNotes(this).exec();
}

// onet my stats
void MainWindow::open_my_stats()
{
    coreServers.at(0)->open_my_stats();
}

// onet my profile
void MainWindow::open_my_profile()
{
    coreServers.at(0)->open_my_profile();
}
