/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QDateTime>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QToolBar>

#include "config.h"
#include "core.h"
#include "about.h"
#include "awaylog_model.h"
#include "awaylog.h"
#include "channel_favourites.h"
#include "channel_homes.h"
#include "channel_list.h"
#include "friends.h"
#include "ignore.h"
#include "message.h"
#include "my_avatar.h"
#include "my_profile.h"
#include "my_stats.h"
#include "nicklist.h"
#include "notes.h"
#include "offline.h"
#include "offlinemsg.h"
#include "options.h"
#include "onet_auth.h"
#include "onet_kernel.h"
#include "tab_container.h"
#include "tab_manager.h"
#include "tool_widget.h"

#if WITH_KAMERZYSTA
    #include "kamerzysta.h"
#else
    #include "webcam.h"
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
    Offline::instance()->offlineMsgAction->setVisible(false);

    // hide awaylog
    Awaylog::instance()->awaylogAction->setVisible(false);

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

    QObject::disconnect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    delete pTabC;
    delete pTabM;

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
    aboutAction = new QAction(QIcon(":/images/logo16x16.png"), tr("About SCC ..."), this);
    Awaylog::instance()->awaylogAction = new QAction(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Awaylog"), this);
    notesAction = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Notes"), this);

    // onet action
    channelListAction = new QAction(QIcon(":/images/oxygen/16x16/documentation.png"), tr("Channel list"), this);
    channelHomesAction = new QAction(QIcon(":/images/oxygen/16x16/configure.png"), tr("My channels"), this);
    channelFavouritesAction = new QAction(QIcon(":/images/oxygen/16x16/emblem-favorite.png"), tr("Favorite channels"), this);
    friendsAction = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending.png"), tr("Friends"), this);
    ignoreAction = new QAction(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"), tr("Ignored"), this);
    Offline::instance()->offlineMsgAction = new QAction(QIcon(":/images/oxygen/16x16/mail-mark-unread.png") , tr("Offline messages"), this);
    camsAction = new QAction(QIcon(":/images/oxygen/16x16/camera-web.png"),tr("Cams"), this);
    myStatsAction = new QAction(QIcon(":/images/oxygen/16x16/office-chart-bar.png"),tr("My statistics"), this);
    myProfileAction = new QAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"),tr("My profile"), this);
    myAvatarAction = new QAction(QIcon(":/images/oxygen/16x16/preferences-desktop-user.png"),tr("My avatar"), this);

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
    Offline::instance()->offlineMsgAction->setShortcut(tr("Ctrl+M"));
    Awaylog::instance()->awaylogAction->setShortcut(tr("Ctrl+J"));
    camsAction->setShortcut(tr("Ctrl+K"));
}

void MainWindow::createMenus()
{
    // main menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(connectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    // options menu
    optionsMenu = menuBar()->addMenu(tr("&Settings"));
    optionsMenu->addAction(optionsAction);

    // onet menu
    chatMenu = menuBar()->addMenu(tr("&Chat"));
    chatMenu->addAction(channelListAction);
    chatMenu->addAction(channelHomesAction);
    chatMenu->addAction(channelFavouritesAction);
    chatMenu->addAction(friendsAction);
    chatMenu->addAction(ignoreAction);
    chatMenu->addAction(Offline::instance()->offlineMsgAction);
    chatMenu->addAction(Awaylog::instance()->awaylogAction);
    chatMenu->addAction(camsAction);
    chatMenu->addSeparator();
    chatMenu->addAction(Core::instance()->busyAction);
    chatMenu->addAction(Core::instance()->awayAction);

    // onet my menu
    myMenu = menuBar()->addMenu(tr("&My"));
    myMenu->addAction(myStatsAction);
    myMenu->addAction(myProfileAction);
    myMenu->addAction(myAvatarAction);
    myMenu->addAction(notesAction);

    // help menu
    helpMenu = menuBar()->addMenu(tr("He&lp"));
    helpMenu->addAction(aboutAction);

    // toolbar
    toolBar = new QToolBar();
    toolBar = addToolBar(tr("Navigation bar"));
    toolBar->setIconSize(QSize(16,16));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(connectAction);
    toolBar->addAction(optionsAction);

    // onet toolbar
    toolBar->addAction(channelListAction);
    toolBar->addAction(channelHomesAction);
    toolBar->addAction(friendsAction);

    // offline messages
    toolBar->addAction(Offline::instance()->offlineMsgAction);
    // awaylog
    toolBar->addAction(Awaylog::instance()->awaylogAction);
    // onet cams
    toolBar->addAction(camsAction);
    // notes
    toolBar->addAction(notesAction);
    // lag
    toolBar->addSeparator();
    toolBar->addAction(Core::instance()->lagAction);

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
}

void MainWindow::setTrayMenuVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    setVisible(visible);
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
    connect(Core::instance()->busyAction, SIGNAL(triggered()), this, SLOT(buttonSetBusy()));
    connect(Core::instance()->awayAction, SIGNAL(triggered()), this, SLOT(buttonSetAway()));
    connect(myStatsAction, SIGNAL(triggered()), this, SLOT(openMyStats()));
    connect(myProfileAction, SIGNAL(triggered()), this, SLOT(openMyProfile()));
    connect(myAvatarAction, SIGNAL(triggered()), this, SLOT(openMyAvatar()));

    connect(Offline::instance()->offlineMsgAction, SIGNAL(triggered()), this, SLOT(openOfflinemsg()));
    connect(Awaylog::instance()->awaylogAction, SIGNAL(triggered()), this, SLOT(openAwaylog()));
    connect(camsAction, SIGNAL(triggered()), this, SLOT(openCams()));
    connect(notesAction, SIGNAL(triggered()), this, SLOT(openNotes()));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // signals from tabc
//    connect(pTabC, SIGNAL(setModeration(bool)), pToolWidget, SLOT(setModeration(bool)));

    // signals tab
    connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    connect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    connect(pTabM, SIGNAL(tabMoved(int,int)), this, SLOT(tabMoved(int,int)));

    // signals pToolWidget
    connect(pToolWidget, SIGNAL(ctrlTabPressed()), this, SLOT(ctrlTabPressed()));
    connect(pToolWidget, SIGNAL(ctrlShiftTabPressed()), this, SLOT(ctrlShiftTabPressed()));

    // signals from network
    connect(Core::instance()->pNetwork, SIGNAL(setConnected()), this, SLOT(setConnected()));
    connect(Core::instance()->pNetwork, SIGNAL(setDisconnected()), this, SLOT(setDisconnected()));
    connect(Core::instance()->pNetwork, SIGNAL(setConnectEnabled(bool)), this, SLOT(setConnectEnabled(bool)));
    connect(Core::instance()->pNetwork, SIGNAL(kernel(const QString&)), pOnetKernel, SLOT(kernel(const QString&)));
    connect(Core::instance()->pNetwork, SIGNAL(authorize(QString,QString,QString)), pOnetAuth, SLOT(authorize(QString,QString,QString)));
    connect(Core::instance()->pNetwork, SIGNAL(updateNick(const QString&)), pToolWidget, SLOT(updateNick(const QString&)));
    connect(Core::instance()->pNetwork, SIGNAL(updateActions()), this, SLOT(updateActions()));

    // auto-away
    connect(Core::instance()->autoAwayTimer, SIGNAL(timeout()), this, SLOT(timeoutAutoaway()));
}

void MainWindow::showWelcome()
{
    pTabC->addTab(STATUS);
    QString strWelcome = "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%";
    Message::instance()->showMessage(STATUS, strWelcome, DefaultMessage);
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
        connectAction->setText(tr("&Disconnect"));
        connectAction->setIconText(tr("&Disconnect"));
        connectAction->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
        Core::instance()->settings["reconnect"] = "true";
        Core::instance()->pNetwork->connect();
    }
    else
    {
        Core::instance()->settings["reconnect"] = "false";
        Core::instance()->settings["logged"] = "false";
        connectAction->setText(tr("&Connect"));
        connectAction->setIconText(tr("&Connect"));
        connectAction->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
        Core::instance()->pNetwork->disconnect();
        if (Core::instance()->settings.value("debug") == "true")
            qDebug() << "Set timerReconnect: stop";
        Core::instance()->pNetwork->timerReconnect->stop();
    }
}

void MainWindow::setConnected()
{
    connectAction->setText(tr("&Disconnect"));
    connectAction->setIconText(tr("&Disconnect"));
    connectAction->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
}

void MainWindow::setDisconnected()
{
    connectAction->setText(tr("&Connect"));
    connectAction->setIconText(tr("&Connect"));
    connectAction->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
}

void MainWindow::setConnectEnabled(bool bSet)
{
    connectAction->setEnabled(bSet);
}

void MainWindow::updateActions()
{
    QString strNick = Core::instance()->settings.value("nick");

    bool bRegistered = false;
    if (strNick[0] != '~')
        bRegistered = true;

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

void MainWindow::openOptions()
{
    DlgOptions(this).exec();
}

// onet dialogs
void MainWindow::openChannelList()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgChannelList(this).exec();
}

void MainWindow::openChannelHomes()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgChannelHomes(this).exec();
}

void MainWindow::openChannelFavourites()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgChannelFavourites(this).exec();
}

void MainWindow::openFriends()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgFriends(this).exec();
}

void MainWindow::openIgnore()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgIgnore(this).exec();
}

void MainWindow::buttonSetBusy()
{
    // do not change status
    if (Core::instance()->busyAction->isChecked())
        Core::instance()->busyAction->setChecked(false);
    else
        Core::instance()->busyAction->setChecked(true);

    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
    {
        bool bBusy = Core::instance()->settings.value("busy") == "true" ? true : false;

        if (bBusy)
            Core::instance()->pNetwork->send("BUSY 0");
        else
            Core::instance()->pNetwork->send("BUSY 1");
    }
}

void MainWindow::buttonSetAway()
{
    // do not change status
    if (Core::instance()->awayAction->isChecked())
        Core::instance()->awayAction->setChecked(false);
    else
        Core::instance()->awayAction->setChecked(true);

    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
    {
        bool bAway = Core::instance()->settings.value("away") == "true" ? true : false;

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
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgOfflineMsg(this).exec();
}

void MainWindow::openAwaylog()
{
    DlgAwaylog(this).exec();
}

void MainWindow::openCams()
{
#if WITH_KAMERZYSTA
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
    {
        QString strMe = Core::instance()->settings.value("nick");
        (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strMe);
    }
#else
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        new DlgWebcam();
#endif
}

void MainWindow::openMyStats()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgMyStats(this).exec();
}

void MainWindow::openMyProfile()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        DlgMyProfile(this).exec();
}

void MainWindow::openMyAvatar()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
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

QSystemTrayIcon *MainWindow::getTrayIcon()
{
    return trayIcon;
}

void MainWindow::timeoutAutoaway()
{
    if ((Core::instance()->pNetwork->isConnected()) && (Core::instance()->pNetwork->isWritable()) && (Core::instance()->settings.value("logged") == "true"))
    {
        QDateTime cdt = QDateTime::currentDateTime();
        int t = (int)cdt.toTime_t(); // seconds that have passed since 1970

        int iLastActive = Core::instance()->settings.value("last_active").toInt();

        bool bAway = Core::instance()->settings.value("away") == "true" ? true : false;

        if ((!bAway) && (iLastActive != 0) && (t-iLastActive > 300))
            Core::instance()->pNetwork->send(QString("AWAY :%1").arg(tr("Not here right now")));
    }
}

// part tab
void MainWindow::tabCloseRequested(int index)
{
    pTabC->partTab(index);
}

void MainWindow::setTabColor(const QString &strChannel, MessageCategory eMessageCategory)
{
    int index = Core::instance()->getIndexFromChannelName(strChannel);

    if (eMessageCategory == HighlightMessage)
        pTabM->setAlert(index, ChannelHighlight);
    else if (eMessageCategory != DefaultMessage)
        pTabM->setAlert(index, ChannelGreen);
    else
        pTabM->setAlert(index, ChannelRed);
}

// change tab
void MainWindow::changeCurrentTab(const QString &strChannel)
{
    int index = Core::instance()->getIndexFromChannelName(strChannel);
    pTabM->setCurrentIndex(index);
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
    QString strModes = Nicklist::instance()->getUserModes(strMe, strChannel);
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isVisible() && Core::instance()->settings["minimize_to_tray"] == "true")
    {
        hide();
        event->ignore();
    }
    else
    {
        if (Core::instance()->settings["always_quit"] == "true")
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
                pConfig->setValue("always_quit", "true");
                Core::instance()->settings["always_quit"] = "true";
                delete pConfig;
            }

            qApp->quit();
        }
        else
            event->ignore();
    }
}
