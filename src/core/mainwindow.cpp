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
#include <QDockWidget>
#include <QMenuBar>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QToolBar>
#include "config.h"
#include "core.h"
#include "about.h"
#include "awaylog.h"
#include "channel_favourites.h"
#include "channel_homes.h"
#include "channel_list.h"
#include "friends.h"
#include "ignore.h"
#include "my_avatar.h"
#include "my_profile.h"
#include "my_stats.h"
#include "notes.h"
#include "offlinemsg.h"
#include "options.h"
#include "onet_auth.h"
#include "onet_kernel.h"
#include "tab_container.h"
#include "tab_manager.h"
#include "tool_widget.h"
#include "mainwindow.h"

#ifdef Q_WS_WIN
    #include "kamerzysta.h"
#else
    #include "webcam.h"
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // init
    setWindowTitle("Simple Chat Client");
    setWindowIcon(QIcon(":/images/logo.png"));

    setWindowGeometry();
    createActions();
    createMenus();

    // classes
    pTabM = new TabManager(this);
    pTabC = new TabContainer(pTabM);

    pOnetKernel = new OnetKernel(pTabC);
    pOnetAuth = new OnetAuth(pTabC);

    // auto-away
    Core::instance()->autoAwayTimer = new QTimer();
    Core::instance()->autoAwayTimer->setInterval(1000*60*1); // 1 min

    // gui
    createGui();

    // refresh colors
    refreshColors();

    // create signals
    createSignals();

    // hide offline messages
    Core::instance()->offlineMsgAct->setVisible(false);

    // hide awaylog
    awaylogAct->setVisible(false);

    // show welcome
    QTimer::singleShot(0, this, SLOT(showWelcome())); // 0 sec

    // show options if config not exist
    showOptions();

    // focus
    pToolWidget->setFocus();
}

MainWindow::~MainWindow()
{
    delete pToolWidget;

    // auto-away
    Core::instance()->autoAwayTimer->stop();

    delete pOnetAuth;
    delete pOnetKernel;

    QObject::disconnect(Core::instance()->pNetwork, SIGNAL(clearAllNicklist()), pTabC, SLOT(clearAllNicklist()));
    QObject::disconnect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    delete pTabC;
    delete pTabM;

    // hide tray
    trayIcon->hide();

    // delete teray
    delete trayIcon;
    delete trayMenu;
}

void MainWindow::setWindowGeometry()
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
    pToolWidget = new ToolWidget(this);
    pToolWidget->show();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(pTabM);
    layout->addWidget(pToolWidget);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    this->setCentralWidget(widget);
}

void MainWindow::createActions()
{
    // action
    sccAct = new QAction(QIcon(":/images/logo.png"), "Simple Chat Client", this);
    sccAct->setFont(QFont(this->font().family(), -1, 75, false));
    sccAct->setEnabled(false);

    restoreMinimalizeAct = new QAction(this);
    connectAct = new QAction(QIcon(":/images/oxygen/16x16/network-connect.png"), tr("&Connect"), this);
    closeAct = new QAction(QIcon(":/images/oxygen/16x16/application-exit.png"), tr("Close"), this);
    optionsAct = new QAction(QIcon(":/images/oxygen/16x16/preferences-system.png"), tr("Options"), this);
    aboutAct = new QAction(QIcon(":/images/logo.png"), tr("About SCC ..."), this);
    awaylogAct = new QAction(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Awaylog"), this);
    notesAct = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Notes"), this);

    // onet action
    channelListAct = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"), tr("Channel list"), this);
    channelHomesAct = new QAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("My channels"), this);
    channelFavouritesAct = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"), tr("Favorite channels"), this);
    friendsAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"), tr("Friends"), this);
    ignoreAct = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"), tr("Ignored"), this);
    Core::instance()->offlineMsgAct = new QAction(QIcon(":/images/oxygen/16x16/mail-mark-unread.png") , tr("Offline messages"), this);
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
    Core::instance()->offlineMsgAct->setShortcut(tr("Ctrl+M"));
    awaylogAct->setShortcut(tr("Ctrl+J"));
    camsAct->setShortcut(tr("Ctrl+K"));
}

void MainWindow::createMenus()
{
    // main menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(connectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);

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
    myMenu = menuBar()->addMenu(tr("&My"));
    myMenu->addAction(myStatsAct);
    myMenu->addAction(myProfileAct);
    //myMenu->addAction(myAvatarAct);
    myMenu->addAction(notesAct);

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
    createTrayMenu();

    trayIcon = new QSystemTrayIcon(QIcon(":/images/logo.png"), this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setToolTip("Simple Chat Client");
    trayIcon->show();
}

void MainWindow::createTrayMenu()
{
    if (this->isHidden())
        restoreMinimalizeAct->setText(tr("Restore"));
    else
        restoreMinimalizeAct->setText(tr("Minimalize"));

    trayMenu->clear();
    trayMenu->addAction(sccAct);
    trayMenu->addSeparator();
    trayMenu->addAction(restoreMinimalizeAct);
    trayMenu->addAction(closeAct);
}

void MainWindow::createSignals()
{
    // signals buttons
    QObject::connect(connectAct, SIGNAL(triggered()), this, SLOT(buttonConnect()));
    QObject::connect(closeAct, SIGNAL(triggered()), this, SLOT(buttonClose()));
    QObject::connect(optionsAct, SIGNAL(triggered()), this, SLOT(openOptions()));
    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(openAbout()));
    QObject::connect(restoreMinimalizeAct, SIGNAL(triggered()), this, SLOT(buttonRestoreMinimalize()));

    // signals onet dialogs
    QObject::connect(channelListAct, SIGNAL(triggered()), this, SLOT(openChannelList()));
    QObject::connect(channelHomesAct, SIGNAL(triggered()), this, SLOT(openChannelHomes()));
    QObject::connect(channelFavouritesAct, SIGNAL(triggered()), this, SLOT(openChannelFavourites()));
    QObject::connect(friendsAct, SIGNAL(triggered()), this, SLOT(openFriends()));
    QObject::connect(ignoreAct, SIGNAL(triggered()), this, SLOT(openIgnore()));
    QObject::connect(Core::instance()->busyAct, SIGNAL(triggered()), this, SLOT(buttonSetBusy()));
    QObject::connect(Core::instance()->awayAct, SIGNAL(triggered()), this, SLOT(buttonSetAway()));
    QObject::connect(myStatsAct, SIGNAL(triggered()), this, SLOT(openMyStats()));
    QObject::connect(myProfileAct, SIGNAL(triggered()), this, SLOT(openMyProfile()));
    QObject::connect(myAvatarAct, SIGNAL(triggered()), this, SLOT(openMyAvatar()));

    // offlinemsg
    QObject::connect(Core::instance()->offlineMsgAct, SIGNAL(triggered()), this, SLOT(openOfflinemsg()));
    // awaylog
    QObject::connect(awaylogAct, SIGNAL(triggered()), this, SLOT(openAwaylog()));
    // onet cams
    QObject::connect(camsAct, SIGNAL(triggered()), this, SLOT(openCams()));
    // notes
    QObject::connect(notesAct, SIGNAL(triggered()), this, SLOT(openNotes()));
    // tray connect
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconPressed(QSystemTrayIcon::ActivationReason)));

    // signals from tabc
    QObject::connect(pTabC, SIGNAL(updateAwaylogStatus()), this, SLOT(updateAwaylogStatus()));
    QObject::connect(pTabC, SIGNAL(setModeration(bool)), pToolWidget, SLOT(setModeration(bool)));

    // signals tab
    QObject::connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    QObject::connect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    QObject::connect(pTabM, SIGNAL(tabMoved(int,int)), this, SLOT(tabMoved(int,int)));

    // signals pToolWidget
    QObject::connect(pToolWidget, SIGNAL(showMessage(QString&,QString&,MessageCategory)), pTabC, SLOT(showMessage(QString&,QString&,MessageCategory)));
    QObject::connect(pToolWidget, SIGNAL(showMessage(QString&,QString&,MessageCategory,QString,QString)), pTabC, SLOT(showMessage(QString&,QString&,MessageCategory,QString,QString)));
    QObject::connect(pToolWidget, SIGNAL(ctrlTabPressed()), this, SLOT(ctrlTabPressed()));
    QObject::connect(pToolWidget, SIGNAL(ctrlShiftTabPressed()), this, SLOT(ctrlShiftTabPressed()));

    // signals onet kernel
    QObject::connect(pOnetKernel, SIGNAL(addUser(QString,QString,QString,bool)), pTabC, SLOT(addUser(QString,QString,QString,bool)));
    QObject::connect(pOnetKernel, SIGNAL(delUser(QString,QString)), pTabC, SLOT(delUser(QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(quitUser(QString,QString)), pTabC, SLOT(quitUser(QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(changeFlag(QString,QString,QString)), pTabC, SLOT(changeFlag(QString,QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(changeFlag(QString,QString)), pTabC, SLOT(changeFlag(QString,QString)));
    QObject::connect(pOnetKernel, SIGNAL(nicklistRefresh(QString)), pTabC, SLOT(nicklistRefresh(QString)));

    // signals from network
    QObject::connect(Core::instance()->pNetwork, SIGNAL(setConnected()), this, SLOT(setConnected()));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(setDisconnected()), this, SLOT(setDisconnected()));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(setConnectEnabled(bool)), this, SLOT(setConnectEnabled(bool)));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(kernel(QString)), pOnetKernel, SLOT(kernel(QString)));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(authorize(QString,QString,QString)), pOnetAuth, SLOT(authorize(QString,QString,QString)));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(showMessageActive(QString&,MessageCategory)), pTabC, SLOT(showMessageActive(QString&,MessageCategory)));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(showMessageAll(QString&,MessageCategory)), pTabC, SLOT(showMessageAll(QString&,MessageCategory)));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(updateNick(QString)), pToolWidget, SLOT(updateNick(QString)));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(clearAllNicklist()), pTabC, SLOT(clearAllNicklist()));
    QObject::connect(Core::instance()->pNetwork, SIGNAL(updateActions()), this, SLOT(updateActions()));

    // auto-away
    QObject::connect(Core::instance()->autoAwayTimer, SIGNAL(timeout()), this, SLOT(timeoutAutoaway()));
}

void MainWindow::showWelcome()
{
    QString strStatus = "Status";
    QString strWelcome = "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%";
    QString strWebsite = "%Fb:courier%%C008100%"+tr("Official website")+" SCC%C3030ce%: http://simplechatclien.sf.net/ %Izaskoczony%";
    pTabC->addTab(strStatus);
    pTabC->showMessage(strStatus, strWelcome, DefaultMessage);
    pTabC->showMessage(strStatus, strWebsite, DefaultMessage);
}

void MainWindow::showOptions()
{
    QString strFirstRun = Core::instance()->settings.value("first_run");

    if (strFirstRun == "true")
    {
        Config *pConfig = new Config(false);
        pConfig->setValue("first_run", "false");
        Core::instance()->settings["first_run"] = "false";
        delete pConfig;

        QTimer::singleShot(1000*1, this, SLOT(openOptions())); // 1 sec
    }
}

// refresh colors
void MainWindow::refreshColors()
{
    QString strTabBackgroundColor;
    QString strBackgroundColor;

    if (Core::instance()->settings.value("background_color").toLower() != "#ffffff")
    {
#ifdef Q_WS_WIN
        QString strMidColor = this->palette().midlight().color().name();
        strTabBackgroundColor = QString("QTabBar::tab { padding: 4px; margin-right: 3px; border: 1px solid %1; border-top-left-radius: 6px; border-top-right-radius: 6px; min-width: 8ex; background-color: %2;}").arg(strMidColor, Core::instance()->settings.value("background_color"));
#endif
        strBackgroundColor = QString("background-color:%1;").arg(Core::instance()->settings.value("background_color"));
    }

    QString strDefaultFontColor;
    if (Core::instance()->settings.value("default_font_color").toLower() != "#000000")
        strDefaultFontColor = QString("color:%1;").arg(Core::instance()->settings.value("default_font_color"));

    this->setStyleSheet(QString("*{%1}%2").arg(strDefaultFontColor+strBackgroundColor, strTabBackgroundColor));

    pTabC->refreshColors();
}

// refresh CSS
void MainWindow::refreshCSS()
{
    pTabC->refreshCSS();
}

// refresh inputline color
void MainWindow::refreshToolWidgetValues()
{
    pToolWidget->setDefaultValues();
}

// buttons
void MainWindow::buttonConnect()
{
    if (!Core::instance()->pNetwork->isConnected())
    {
        connectAct->setText(tr("&Disconnect"));
        connectAct->setIconText(tr("&Disconnect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
        Core::instance()->settings["reconnect"] = "true";
        Core::instance()->pNetwork->connect();
    }
    else
    {
        Core::instance()->settings["reconnect"] = "false";
        Core::instance()->settings["logged"] = "off";
        connectAct->setText(tr("&Connect"));
        connectAct->setIconText(tr("&Connect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
        Core::instance()->pNetwork->disconnect();
#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on")
            qDebug() << "Set timerReconnect: stop";
#endif
        Core::instance()->pNetwork->timerReconnect->stop();
    }
}

void MainWindow::setConnected()
{
    connectAct->setText(tr("&Disconnect"));
    connectAct->setIconText(tr("&Disconnect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
}

void MainWindow::setDisconnected()
{
    connectAct->setText(tr("&Connect"));
    connectAct->setIconText(tr("&Connect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
}

void MainWindow::setConnectEnabled(bool bSet)
{
    connectAct->setEnabled(bSet);
}

void MainWindow::updateActions()
{
    QString strNick = Core::instance()->settings.value("nick");

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

void MainWindow::updateAwaylogStatus()
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

void MainWindow::openOptions()
{
    DlgOptions(this).exec();
}

// onet dialogs
void MainWindow::openChannelList()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgChannelList(this).exec();
}

void MainWindow::openChannelHomes()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgChannelHomes(this).exec();
}

void MainWindow::openChannelFavourites()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgChannelFavourites(this).exec();
}

void MainWindow::openFriends()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgFriends(this).exec();
}

void MainWindow::openIgnore()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgIgnore(this).exec();
}

void MainWindow::buttonSetBusy()
{
    // do not change status
    if (Core::instance()->busyAct->isChecked())
        Core::instance()->busyAct->setChecked(false);
    else
        Core::instance()->busyAct->setChecked(true);

    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
    {
        bool bBusy = Core::instance()->settings.value("busy") == "on" ? true : false;

        if (bBusy)
            Core::instance()->pNetwork->send("BUSY 0");
        else
            Core::instance()->pNetwork->send("BUSY 1");
    }
}

void MainWindow::buttonSetAway()
{
    // do not change status
    if (Core::instance()->awayAct->isChecked())
        Core::instance()->awayAct->setChecked(false);
    else
        Core::instance()->awayAct->setChecked(true);

    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
    {
        bool bAway = Core::instance()->settings.value("away") == "on" ? true : false;

        QString strReason;
        if (bAway)
            strReason = "";
        else
            strReason = tr("Not here right now");

        Core::instance()->pNetwork->send(QString("AWAY :%1").arg(strReason));
    }
}

void MainWindow::openOfflinemsg()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgOfflineMsg(this).exec();
}

void MainWindow::openAwaylog()
{
    DlgAwaylog(this, awaylogAct).exec();
}

void MainWindow::openCams()
{
#ifdef Q_WS_WIN
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
    {
        QString strMe = Core::instance()->settings.value("nick");
        (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strMe);
    }
#else
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        new DlgWebcam();
#endif
}

void MainWindow::openMyStats()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgMyStats(this).exec();
}

void MainWindow::openMyProfile()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgMyProfile(this).exec();
}

void MainWindow::openMyAvatar()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
        DlgMyAvatar(this).exec();
}

void MainWindow::openNotes()
{
    DlgNotes(this).exec();
}

void MainWindow::openAbout()
{
    DlgAbout(this).exec();
}

void MainWindow::buttonClose()
{
    this->close();
}

// tray button
void MainWindow::buttonRestoreMinimalize()
{
    if (this->isVisible())
        this->hide();
    else
        this->showNormal();
}

// tray
void MainWindow::trayIconPressed(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::Trigger)
    {
        if (this->isVisible())
            this->hide();
        else
            this->showNormal();
    }
    else if (activationReason == QSystemTrayIcon::Context)
        createTrayMenu();
}

// ctrl+tab pressed in inputline
void MainWindow::ctrlTabPressed()
{
    int index = pTabM->currentIndex();
    pTabM->setCurrentIndex(index+1);
}

// ctrl+shift+tab pressed in inputline
void MainWindow::ctrlShiftTabPressed()
{
    int index = pTabM->currentIndex();
    pTabM->setCurrentIndex(index-1);
}

int MainWindow::getCurrentTabIndex()
{
    if (pTabM->count() != 0)
        return pTabM->currentIndex();
    else
        return -1;
}

int MainWindow::getUserCount(QString strChannel)
{
    return pTabC->getUserCount(strChannel);
}

QString MainWindow::getUserModes(QString strNick, QString strChannel)
{
    return pTabC->getUserModes(strNick, strChannel);
}

QList<QString> MainWindow::getUserList(QString strChannel)
{
    return pTabC->getUserList(strChannel);
}

QString MainWindow::getUserAvatarPath(QString strNick)
{
    return pTabC->getUserAvatarPath(strNick);
}

void MainWindow::timeoutAutoaway()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "on"))
    {
        QDateTime cdt = QDateTime::currentDateTime();
        int t = (int)cdt.toTime_t(); // seconds that have passed since 1970

        int iLastActive = Core::instance()->settings.value("last_active").toInt();

        bool bAway = Core::instance()->settings.value("away") == "on" ? true : false;

        if ((!bAway) && (iLastActive != 0) && (t-iLastActive > 300))
            Core::instance()->pNetwork->send(QString("AWAY :%1").arg(tr("Not here right now")));
    }
}

// part tab
void MainWindow::tabCloseRequested(int index)
{
    pTabC->partTab(index);
}

// tab changed
void MainWindow::currentTabChanged(int index)
{
    QString strChannel = Core::instance()->getChannelNameFromIndex(index);

    // change tab color
    pTabM->setColor(index, QColor(Core::instance()->settings.value("default_font_color")));

    // hide/show settings on non channel
    if (strChannel[0] != '#')
        pToolWidget->setChannelSettings(false);
    else
        pToolWidget->setChannelSettings(true);

    // clear input line
    pToolWidget->clearInputLine();

    // moderation
    QString strMe = Core::instance()->settings.value("nick");
    QString strModes = Core::instance()->getUserModes(strMe, strChannel);
    if (strModes.contains("!")) pToolWidget->setModeration(true);
    else pToolWidget->setModeration(false);
}

// tab moved
void MainWindow::tabMoved(int from, int to)
{
    Core::instance()->lOpenChannels.move(from, to);
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    pTabC->resizeMainWindow(e->size());
}

void MainWindow::showMessage(QString &strChannel, QString &strData, MessageCategory eMessageCategory, QString strTime, QString strNick)
{
    pTabC->showMessage(strChannel, strData, eMessageCategory, strTime, strNick);
}
