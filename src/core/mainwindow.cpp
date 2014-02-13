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

#include <QDesktopWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QMovie>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>

#include "about_gui.h"
#include "autoaway.h"
#include "away.h"
#include "awaylog.h"
#include "awaylog_gui.h"
#include "busy.h"
#include "channel.h"
#include "channel_favourites_gui.h"
#include "channel_homes_gui.h"
#include "channel_list_gui.h"
#include "config.h"
#include "core.h"
#include "find_nick_gui.h"
#include "friends_gui.h"
#include "ignore_gui.h"
#include "invite_list_gui.h"
#include "invite.h"
#include "lag.h"
#include "message.h"
#include "my_avatar_gui.h"
#include "my_profile_gui.h"
#include "my_stats_gui.h"
#include "nick.h"
#include "notes_gui.h"
#include "notification.h"
#include "offline.h"
#include "offline_list_gui.h"
#include "options_gui.h"
#include "onet_auth.h"
#include "onet_kernel.h"
#include "settings.h"
#include "tab_container.h"
#include "tab_manager.h"
#include "tool_widget.h"

#if WITH_KAMERZYSTA
    #include "kamerzysta.h"
#else
    #include "webcam_gui.h"
#endif

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // init
    setWindowTitle("Simple Chat Client");
    setWindowIcon(QIcon(":/images/logo32x32.png"));

    setWindowGeometry();
    createActions();
    createMenus();

    // classes
    pTabM = new TabManager(this);
    pTabC = new TabContainer(pTabM);

    pOnetKernel = new OnetKernel(pTabC);
    pOnetAuth = new OnetAuth();

    // gui
    createGui();

    // refresh colors
    refreshColors();

    // create signals
    createSignals();
}

MainWindow::~MainWindow()
{
    delete pToolWidget;

    Autoaway::instance()->stop();

    delete pOnetAuth;
    delete pOnetKernel;

    QObject::disconnect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    delete pTabC;
    delete pTabM;

    // stop tray
    stopAnimatedTrayIcon();
    delete movieTrayIcon;

    // hide tray
    trayIcon->hide();

    // delete teray
    delete trayIcon;
    delete trayIconMenu;
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
    sccAction = new QAction(QIcon(":/images/logo16x16.png"), "Simple Chat Client", this);
    sccAction->setFont(QFont(this->font().family(), -1, 75, false));
    sccAction->setEnabled(false);

    connectAction = new QAction(QIcon(":/images/oxygen/16x16/network-connect.png"), tr("&Connect"), this);
    optionsAction = new QAction(QIcon(":/images/oxygen/16x16/preferences-system.png"), tr("Options"), this);

    // onet action
    channelListAction = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"), tr("Channel list"), this);
    channelHomesAction = new QAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("My channels"), this);
    channelFavouritesAction = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"), tr("Favorite channels"), this);
    friendsAction = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"), tr("Friends"), this);
    ignoreAction = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"), tr("Ignored"), this);
    findNickAction = new QAction(QIcon(":/images/oxygen/16x16/edit-find-user.png"), tr("Find nick"), this);

    myStatsAction = new QAction(QIcon(":/images/oxygen/16x16/office-chart-bar.png"),tr("My statistics"), this);
    myProfileAction = new QAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"),tr("My profile"), this);
    myAvatarAction = new QAction(QIcon(":/images/oxygen/16x16/edit-image-face-show.png"),tr("My avatar"), this);

    camsAction = new QAction(QIcon(":/images/oxygen/16x16/camera-web.png"),tr("Webcams"), this);
    notesAction = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Notes"), this);
    aboutAction = new QAction(QIcon(":/images/logo16x16.png"), tr("About SCC ..."), this);

    minimizeAction = new QAction(QIcon(":/images/oxygen/16x16/view-close.png"), tr("Mi&nimize"), this);
    maximizeAction = new QAction(QIcon(":/images/oxygen/16x16/view-fullscreen.png"), tr("Ma&ximize"), this);
    restoreAction = new QAction(QIcon(":/images/oxygen/16x16/view-restore.png"), tr("&Restore"), this);
    quitAction = new QAction(QIcon(":/images/oxygen/16x16/application-exit.png"), tr("&Quit"), this);

    // shortcut
    connectAction->setShortcuts(QKeySequence::New);
    quitAction->setShortcut(tr("Ctrl+Q"));
    optionsAction->setShortcut(tr("Ctrl+O"));

    // onet shortcut
    channelListAction->setShortcut(tr("Ctrl+L"));
    channelHomesAction->setShortcut(tr("Ctrl+M"));
    channelFavouritesAction->setShortcut(tr("Ctrl+U"));
    friendsAction->setShortcut(tr("Ctrl+P"));
    ignoreAction->setShortcut(tr("Ctrl+I"));
    findNickAction->setShortcut(tr("Ctrl+G"));
    camsAction->setShortcut(tr("Ctrl+K"));
}

void MainWindow::createMenus()
{
    // main menu
    fileMenu = menuBar()->addMenu(tr("SCC"));
    fileMenu->addAction(connectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(aboutAction);
    fileMenu->addAction(quitAction);

    // onet menu
    chatMenu = menuBar()->addMenu(tr("&Chat"));
    chatMenu->addAction(channelListAction);
    chatMenu->addAction(findNickAction);
    chatMenu->addAction(camsAction);
    chatMenu->addSeparator();
    chatMenu->addAction(Lag::instance()->lagAction);

    // onet my menu
    myMenu = menuBar()->addMenu(tr("&My"));
    myMenu->addAction(myStatsAction);
    myMenu->addAction(myProfileAction);
    myMenu->addAction(myAvatarAction);
    myMenu->addAction(channelHomesAction);
    myMenu->addAction(channelFavouritesAction);
    myMenu->addAction(friendsAction);
    myMenu->addAction(ignoreAction);
    myMenu->addAction(notesAction);
    myMenu->addSeparator();
    myMenu->addAction(Busy::instance()->busyAction);
    myMenu->addAction(Away::instance()->awayAction);

    // options menu
    optionsMenu = menuBar()->addMenu(tr("&Settings"));
    optionsMenu->addAction(optionsAction);

    // notification
    notificationToolButton = new QToolButton(this);
    notificationToolButton->setIcon(QIcon(":/images/oxygen/16x16/emblem-important.png"));
    notificationToolButton->setText(tr("N&otification"));
    notificationToolButton->setMenu(Notification::instance()->getNotificationMenu());
    notificationToolButton->setPopupMode(QToolButton::InstantPopup);
    notificationToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    notificationToolButton->setVisible(false);

    // toolbar
    toolBar = addToolBar(tr("Navigation bar"));
    toolBar->setIconSize(QSize(16,16));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(connectAction);
    toolBar->addAction(channelListAction);
    toolBar->addAction(channelHomesAction);
    toolBar->addAction(friendsAction);
    toolBar->addAction(ignoreAction);
    toolBar->addAction(camsAction);
    toolBar->addAction(notesAction);
    notificationAction = toolBar->addWidget(notificationToolButton);

    // tray menu
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(sccAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    // tray
    trayIcon = new QSystemTrayIcon(QIcon(":/images/logo16x16.png"), this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("Simple Chat Client");
    trayIcon->setVisible(true);

    // movie tray icon
    movieTrayIcon = new QMovie(":/images/logo16x16.gif");
}

void MainWindow::createSignals()
{
    // signals buttons
    connect(connectAction, SIGNAL(triggered()), this, SLOT(buttonConnect()));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(openOptions()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(openAbout()));

    // signals onet dialogs
    connect(channelListAction, SIGNAL(triggered()), this, SLOT(openChannelList()));
    connect(channelHomesAction, SIGNAL(triggered()), this, SLOT(openChannelHomes()));
    connect(channelFavouritesAction, SIGNAL(triggered()), this, SLOT(openChannelFavourites()));
    connect(friendsAction, SIGNAL(triggered()), this, SLOT(openFriends()));
    connect(ignoreAction, SIGNAL(triggered()), this, SLOT(openIgnore()));
    connect(myStatsAction, SIGNAL(triggered()), this, SLOT(openMyStats()));
    connect(myProfileAction, SIGNAL(triggered()), this, SLOT(openMyProfile()));
    connect(myAvatarAction, SIGNAL(triggered()), this, SLOT(openMyAvatar()));
    connect(findNickAction, SIGNAL(triggered()), this, SLOT(openFindNick()));
    connect(camsAction, SIGNAL(triggered()), this, SLOT(openCams()));
    connect(notesAction, SIGNAL(triggered()), this, SLOT(openNotes()));

    connect(notificationToolButton->menu(), SIGNAL(aboutToShow()), this, SLOT(stopAnimatedTrayIcon()));
    connect(Awaylog::instance()->awaylogAction, SIGNAL(triggered()), this, SLOT(openAwaylog()));
    connect(Invite::instance()->inviteAction, SIGNAL(triggered()), this, SLOT(openInviteList()));
    connect(Offline::instance()->offlineMessagesAction, SIGNAL(triggered()), this, SLOT(openOfflineMessages()));

    connect(movieTrayIcon, SIGNAL(frameChanged(int)), this, SLOT(updateTrayIcon()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // signals tab
    connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    connect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

    // signals from network
    connect(Core::instance()->network, SIGNAL(socketStateChanged()), this, SLOT(updateButtons()));
    connect(Core::instance()->network, SIGNAL(kernel(const QString&)), pOnetKernel, SLOT(kernel(const QString&)));
    connect(Core::instance()->network, SIGNAL(authorize(QString,QString)), pOnetAuth, SLOT(authorize(QString,QString)));
    connect(Core::instance()->network, SIGNAL(updateNick(const QString&)), this, SLOT(updateNick(const QString&)));

    // signals from auth
    connect(pOnetAuth, SIGNAL(updateNick(const QString&)), this, SLOT(updateNick(const QString&)));
    connect(pOnetAuth, SIGNAL(authStateChanged()), this, SLOT(updateButtons()));
}

void MainWindow::init()
{
    // focus
    setTabOrder(pTabM, pToolWidget);
    pToolWidget->setFocus();

    // add default tabs
    addDefaultTabs();

    // show welcome
    showWelcome();

    // first run - show options if config not exist
    if (Settings::instance()->get("first_run") == "true")
        firstRun();

    // auto connect
    if (Settings::instance()->get("auto_connect") == "true")
        buttonConnect();
}

void MainWindow::addDefaultTabs()
{
    // debug
    if (Settings::instance()->get("debug") == "true")
        pTabC->addTab(DEBUG_WINDOW);

    // status
    pTabC->addTab(STATUS_WINDOW);
}

void MainWindow::showWelcome()
{
    QString strWelcome = "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%";
    Message::instance()->showMessage(STATUS_WINDOW, strWelcome, MessageDefault);
}

void MainWindow::firstRun()
{
    Settings::instance()->set("first_run", "false");

    Config *pConfig = new Config(SettingsConfig);
    pConfig->set("first_run", "false");
    delete pConfig;

    //QTimer::singleShot(1000*1, this, SLOT(openOptions())); // 1 sec
}

void MainWindow::setTrayMenuVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    setVisible(visible);
}

// refresh colors
void MainWindow::refreshColors()
{
    QString strTabBackgroundColor;
    QString strBackgroundColor;

    if (Settings::instance()->get("background_color").toLower() != "#ffffff")
    {
#ifdef Q_WS_WIN
        QString strMidColor = this->palette().midlight().color().name();
        strTabBackgroundColor = QString("QTabBar::tab { padding: 4px; margin-right: 3px; border: 1px solid %1; border-top-left-radius: 6px; border-top-right-radius: 6px; min-width: 8ex; background-color: %2;}").arg(strMidColor, Settings::instance()->get("background_color"));
#endif
        strBackgroundColor = QString("background-color:%1;").arg(Settings::instance()->get("background_color"));
    }

    QString strDefaultColor;
    if (Settings::instance()->get("default_color").toLower() != "#000000")
        strDefaultColor = QString("color:%1;").arg(Settings::instance()->get("default_color"));

    this->setStyleSheet(QString("*{%1}%2").arg(strDefaultColor+strBackgroundColor, strTabBackgroundColor));

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
    if (!Core::instance()->network->isConnected())
    {
        Settings::instance()->set("reconnect", "true");
        Core::instance()->network->connect();
    }
    else
    {
        Settings::instance()->set("reconnect", "false");
        Settings::instance()->set("logged", "false");
        Core::instance()->network->disconnect();

        if (Settings::instance()->get("debug") == "true")
            qDebug() << "Set timerReconnect: stop";
        Core::instance()->network->timerReconnect->stop();
    }
}

void MainWindow::updateButtons()
{
    bool bUpdateMenu = false;

    if ((Settings::instance()->get("socket_state") == "unknown") || (Settings::instance()->get("authorizing") == "true"))
    {
        bUpdateMenu = false;
        connectAction->setEnabled(false);
    }
    else
    {
        if (Settings::instance()->get("socket_state") == "disconnected")
        {
            bUpdateMenu = true;
            connectAction->setEnabled(true);
            connectAction->setText(tr("&Connect"));
            connectAction->setIconText(tr("&Connect"));
            connectAction->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
        }
        else if (Settings::instance()->get("socket_state") == "connected")
        {
            bUpdateMenu = true;
            connectAction->setEnabled(true);
            connectAction->setText(tr("&Disconnect"));
            connectAction->setIconText(tr("&Disconnect"));
            connectAction->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
        }
    }

    if (bUpdateMenu)
    {
        QString strNick = Settings::instance()->get("nick");

        bool bRegistered = (strNick.at(0) == '~' ? false : true);

        if (bRegistered)
        {
            channelHomesAction->setEnabled(true);
            channelFavouritesAction->setEnabled(true);
            friendsAction->setEnabled(true);
            ignoreAction->setEnabled(true);
            myStatsAction->setEnabled(true);
            myProfileAction->setEnabled(true);
            myAvatarAction->setEnabled(true);
        }
        else
        {
            channelHomesAction->setEnabled(false);
            channelFavouritesAction->setEnabled(false);
            friendsAction->setEnabled(false);
            ignoreAction->setEnabled(false);
            myStatsAction->setEnabled(false);
            myProfileAction->setEnabled(false);
            myAvatarAction->setEnabled(false);
        }
    }
}

void MainWindow::openOptions()
{
    OptionsGui(this).exec();
}

void MainWindow::openChannelList()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        ChannelListGui(this).exec();
}

void MainWindow::openChannelHomes()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        ChannelHomesGui(this).exec();
}

void MainWindow::openChannelFavourites()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        ChannelFavouritesGui(this).exec();
}

void MainWindow::openFriends()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        FriendsGui(this).exec();
}

void MainWindow::openIgnore()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        IgnoreGui(this).exec();
}

void MainWindow::openFindNick()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        FindNickGui(this).exec();
}

void MainWindow::openCams()
{
#if WITH_KAMERZYSTA
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
    {
        QString strMe = Settings::instance()->get("nick");
        (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strMe);
    }
#else
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        new WebcamGui();
#endif
}

void MainWindow::openMyStats()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        MyStatsGui(this).exec();
}

void MainWindow::openMyProfile()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        MyProfileGui(this).exec();
}

void MainWindow::openMyAvatar()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        MyAvatarGui(this).exec();
}

void MainWindow::openNotes()
{
    (new NotesGui(this))->show();
}

void MainWindow::openAbout()
{
    AboutGui(this).exec();
}

void MainWindow::openAwaylog()
{
    AwaylogGui(this).exec();
}

void MainWindow::openInviteList()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        InviteListGui(this).exec();
}

void MainWindow::openOfflineMessages()
{
    if ((Core::instance()->network->isConnected()) && (Settings::instance()->get("logged") == "true"))
        OfflineListGui(this).exec();
}

// animated tray icon
void MainWindow::startAnimatedTrayIcon()
{
    movieTrayIcon->start();
}

void MainWindow::stopAnimatedTrayIcon()
{
    movieTrayIcon->stop();
    trayIcon->setIcon(QIcon(":/images/logo16x16.png"));
}

void MainWindow::updateTrayIcon()
{
    QIcon icon(movieTrayIcon->currentPixmap());
    trayIcon->setIcon(icon);
}

// tray
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            if (this->isVisible())
                this->hide();
            else
                this->show();
            break;
        case QSystemTrayIcon::Context:
            setTrayMenuVisible(this->isVisible());
            break;
        default:
            break;
    }
}

bool MainWindow::inputLineKeyEvent(QKeyEvent *k)
{
    // ctrl+tab pressed in inputline
    // alt+right pressed in inputline
    if (((k->key() == Qt::Key_Tab) && (k->modifiers() == Qt::ControlModifier)) ||
        ((k->key() == Qt::Key_Right) && (k->modifiers() == Qt::AltModifier)))
    {
        int index = pTabM->currentIndex();
        if (pTabM->count()-1 != index)
            pTabM->setCurrentIndex(index+1);
        else
            pTabM->setCurrentIndex(0);
        return true;
    }
    // ctrl+shift+tab pressed in inputline
    // alt+left pressed in inputline
    else if (((k->key() == Qt::Key_Backtab) && (k->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier))) ||
            ((k->key() == Qt::Key_Left) && (k->modifiers() == Qt::AltModifier)))
    {
        int index = pTabM->currentIndex();
        if (index != 0)
            pTabM->setCurrentIndex(index-1);
        else
            pTabM->setCurrentIndex(pTabM->count()-1);
        return true;
    }
    // ctrl+F
    else if ((k->key() == Qt::Key_F) && (k->modifiers() == Qt::ControlModifier))
    {
        QString strChannel = Channel::instance()->getCurrentName();
        Channel::instance()->getChatView(strChannel)->search();
        return true;
    }
    // alt+1
    else if ((k->key() == Qt::Key_1) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 1)
            pTabM->setCurrentIndex(0);
        return true;
    }
    // alt+2
    else if ((k->key() == Qt::Key_2) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 2)
            pTabM->setCurrentIndex(1);
        return true;
    }
    // alt+3
    else if ((k->key() == Qt::Key_3) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 3)
            pTabM->setCurrentIndex(2);
        return true;
    }
    // alt+4
    else if ((k->key() == Qt::Key_4) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 4)
            pTabM->setCurrentIndex(3);
        return true;
    }
    // alt+5
    else if ((k->key() == Qt::Key_5) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 5)
            pTabM->setCurrentIndex(4);
        return true;
    }
    // alt+6
    else if ((k->key() == Qt::Key_6) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 6)
            pTabM->setCurrentIndex(5);
        return true;
    }
    // alt+7
    else if ((k->key() == Qt::Key_7) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 7)
            pTabM->setCurrentIndex(6);
        return true;
    }
    // alt+8
    else if ((k->key() == Qt::Key_8) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 8)
            pTabM->setCurrentIndex(7);
        return true;
    }
    // alt+9
    else if ((k->key() == Qt::Key_9) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 9)
            pTabM->setCurrentIndex(8);
        return true;
    }
    // alt+0
    else if ((k->key() == Qt::Key_0) && (k->modifiers() == Qt::AltModifier))
    {
        if (pTabM->count() >= 10)
            pTabM->setCurrentIndex(9);
        return true;
    }

    return false;
}

int MainWindow::getCurrentTabIndex()
{
    return pTabM->currentIndex();
}

QSystemTrayIcon *MainWindow::getTrayIcon()
{
    return trayIcon;
}

QToolButton *MainWindow::getNotificationToolButton()
{
    return notificationToolButton;
}

QAction *MainWindow::getNotificationAction()
{
    return notificationAction;
}

void MainWindow::refreshToolButtons(const QString &strChannel)
{
    if (strChannel.isEmpty()) return;

    QString strMe = Settings::instance()->get("nick");

    // hide/show settings on non channel
    if (strChannel.at(0) != '#')
        pToolWidget->showChannelSettings(false);
    else
        pToolWidget->showChannelSettings(true);

    // moderation
    QString strModes = Nick::instance()->getModes(strMe, strChannel);
    if (strModes.contains(FLAG_MOD))
        pToolWidget->showModeration(true);
    else
        pToolWidget->showModeration(false);
}

void MainWindow::updateNick(const QString &strNick)
{
    setWindowTitle(QString("Simple Chat Client - %1").arg(strNick));
}

void MainWindow::updateChannelIcon(int index, const QString &avatar)
{
    pTabM->updateIcon(index, avatar);
}

void MainWindow::insertTextToInputLine(const QString &strText)
{
    pToolWidget->insertTextToInputLine(strText);
}

void MainWindow::toolButtonFix(QWidget *widget)
{
#ifdef Q_WS_WIN
    // workaround for QTBUG-21808
    Q_ASSERT(widget);
    widget->setStyleSheet("QTabBar QToolButton {"
                          "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f2f2f2, stop:0.5 #ebebeb, stop:0.51 #dddddd, stop:1 #cfcfcf);"
                          "margin: 1px;"
                          "border: 1px solid #707070;"
                          "border-radius: 2px;"
                          "}"
                          "QTabBar QToolButton:pressed {"
                          "margin: 2px 0 0 2px;"
                          "border: 1px solid #000000;"
                          "}");
#else
    Q_UNUSED(widget);
#endif
}

// part tab
void MainWindow::tabCloseRequested(int index)
{
    pTabC->partTab(index);
}

void MainWindow::setTabColor(const QString &strChannel, MessageCategory eMessageCategory)
{
    if (eMessageCategory == MessageHighlight)
        pTabM->setAlert(strChannel, ChannelHighlight);
    else if (eMessageCategory != MessageDefault)
        pTabM->setAlert(strChannel, ChannelGreen);
    else
        pTabM->setAlert(strChannel, ChannelRed);
}

// change tab
void MainWindow::changeCurrentTab(int index)
{
    pTabM->setCurrentIndex(index);
}

// tab changed
void MainWindow::currentTabChanged(int index)
{
    // change tab color
    pTabM->setColor(index, QColor(Settings::instance()->get("default_color")));

    // clear input line
    pToolWidget->clearInputLine();

    // refresh tool buttons
    QString strChannel = Channel::instance()->getNameFromIndex(index);
    refreshToolButtons(strChannel);
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    pTabC->resizeMainWindow(e->size());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isVisible() && Settings::instance()->get("minimize_to_tray") == "true")
    {
        hide();
        event->ignore();
    }
    else
    {
        if (Settings::instance()->get("always_quit") == "true")
        {
            qApp->quit();
            return;
        }

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/images/logo16x16.png"));
        msgBox.setWindowTitle(tr("Quit program"));
        msgBox.setText(tr("Are you sure you want to quit?"));

        QCheckBox dontPrompt(tr("Do not prompt again"), &msgBox);
        msgBox.addButton(&dontPrompt, QMessageBox::ActionRole);

        QPushButton *quitButton = msgBox.addButton(tr("Quit"), QMessageBox::ActionRole);
        QPushButton *cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::ActionRole);

        quitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
        cancelButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

        dontPrompt.blockSignals(true); // hack: blocking singals so QMessageBox won't close
        msgBox.exec();

        if (msgBox.clickedButton() == quitButton)
        {
            if (dontPrompt.checkState() == Qt::Checked)
            {
                Config *pConfig = new Config();
                pConfig->set("always_quit", "true");
                Settings::instance()->set("always_quit", "true");
                delete pConfig;
            }

            qApp->quit();
        }
        else
            event->ignore();
    }
}
