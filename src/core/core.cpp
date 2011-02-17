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

#include <QSettings>
#include "dlg_awaylog.h"
#include "dlg_cam.h"
#include "dlg_channel_favourites.h"
#include "dlg_channel_homes.h"
#include "dlg_channel_list.h"
#include "dlg_channel_settings.h"
#include "dlg_friends.h"
#include "dlg_ignore.h"
#include "dlg_moderation.h"
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
#include "core.h"

#ifdef Q_WS_WIN
#include "kamerzysta.h"
#endif

Core::Core()
{
}

void Core::init(QMainWindow *parent, QString param1, int param2, Notify *param3, QAction *param4, QToolBar *param5, QMenu *param6)
{
    // params
    myparent = parent;
    strServer = param1;
    iPort = param2;
    pNotify = param3;
    connectAct = param4;
    toolBar = param5;
    viewMenu = param6;

    // lag
    lagAct = new QAction("Lag: ?", myparent);

    // init all
    camSocket = new QTcpSocket();

    pTabM = new TabManager(myparent);
    myparent->setCentralWidget(pTabM);

    pNetwork = new Network(strServer, iPort);
    pNetwork->start(QThread::InheritPriority);

    // classes
    pTabC = new TabContainer(myparent, pNetwork, pTabM, pNotify, &mChannelAvatar, camSocket, &stlChannelNickStatus, &lAwaylog);

    pDlg_channel_settings = new DlgChannelSettings(myparent, pNetwork);
    pDlg_moderation = new DlgModeration(myparent);
    pDlg_channel_homes = new DlgChannelHomes(myparent, pNetwork, &mChannelAvatar, pDlg_channel_settings);
    pDlg_user_profile = new DlgUserProfile(myparent, pNetwork);
    pDlg_cam = new DlgCam(myparent, pNetwork, camSocket);

    pOnet_kernel = new OnetKernel(myparent, pNetwork, pTabC, pNotify, &mNickAvatar, &mChannelAvatar, pDlg_channel_settings, pDlg_channel_homes, &stlChannelList, &lChannelFavourites, &mFriends, &lIgnore, pDlg_moderation);
    pOnet_auth = new OnetAuth(pTabC);

    pTabC->set_dlg(pDlg_user_profile, pDlg_cam);

    // inputlinewidget
    bottomDockWidget = new QDockWidget(tr("Typing messages"), myparent);
    bottomDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    bottomDockWidget->setFocus();
    bottomDockWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea); // top and bottom
    inputLineDockWidget = new InputLineDockWidget(bottomDockWidget, pNetwork, pDlg_channel_settings, pDlg_moderation);
    bottomDockWidget->setWidget(inputLineDockWidget);
    myparent->addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);
    viewMenu->addAction(bottomDockWidget->toggleViewAction());

    // nicklistwidget
    rightDockWidget = new QDockWidget(tr("Users"), myparent);
    bottomDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    rightDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea); // left and right
    nickListDockWidget = new NickListDockWidget(rightDockWidget);
    rightDockWidget->setWidget(nickListDockWidget);
    myparent->addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);
    viewMenu->addAction(rightDockWidget->toggleViewAction());

    // hide nicklist and toolwidget on status
    rightDockWidget->hide();

    // welcome
    pTabC->add_tab("Status");
    pTabC->show_msg("Status", "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%Cff6500%"+tr("Official website")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/ %Izaskoczony%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%C008100%"+tr("Official Forum")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/forum/ %Ioczko%", 0);

    // toolbar
    toolBar->addSeparator();
    toolBar->addAction(lagAct);

    // create signals
    create_signals();
}

Core::~Core()
{
    // clear arrays
    clear_all_nicklist();

    // close network
    QSettings settings;
    settings.setValue("reconnect", "false");
    pNetwork->clear_queue();
    pNetwork->send("QUIT");
    pNetwork->close();

    delete pOnet_auth;
    delete pOnet_kernel;
    delete pDlg_cam;
    delete pDlg_user_profile;
    delete pDlg_channel_homes;
    delete pDlg_moderation;
    delete pDlg_channel_settings;

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
}

void Core::create_signals()
{
    // signals from tabc
    QObject::connect(pTabC, SIGNAL(create_nicklist(QString)), this, SLOT(create_nicklist(QString)));
    QObject::connect(pTabC, SIGNAL(remove_nicklist(QString)), this, SLOT(remove_nicklist(QString)));
    QObject::connect(pTabC, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(pTabC, SIGNAL(update_nick_avatar(QString)), this, SLOT(update_nick_avatar(QString)));
    QObject::connect(pTabC, SIGNAL(set_open_channels()), this, SLOT(set_open_channels()));

    // signals tab
    QObject::connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
    QObject::connect(pTabM, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(pDlg_moderation, SIGNAL(display_msg(QString,QString,int)), pTabC, SLOT(slot_show_msg(QString,QString,int)));

    // signals inputLineWidget
    QObject::connect(inputLineDockWidget, SIGNAL(show_msg(QString,QString,int)), pTabC, SLOT(slot_show_msg(QString,QString,int)));
    QObject::connect(inputLineDockWidget, SIGNAL(display_message(QString,QString,int)), pTabC, SLOT(slot_display_message(QString,QString,int)));
    QObject::connect(inputLineDockWidget, SIGNAL(change_font_size(QString)), pTabC, SLOT(slot_change_font_size(QString)));
    QObject::connect(inputLineDockWidget, SIGNAL(clear_content(QString)), pTabC, SLOT(slot_clear_content(QString)));

    // signals lag
    QObject::connect(pOnet_kernel, SIGNAL(set_lag(QString)), this, SLOT(set_lag(QString)));
    QObject::connect(pOnet_kernel, SIGNAL(update_nick(QString)), inputLineDockWidget, SLOT(slot_update_nick(QString)));
    QObject::connect(pOnet_kernel, SIGNAL(clear_nicklist(QString)), this, SLOT(clear_nicklist(QString)));

    // signals from kernel to nicklist
    QObject::connect(pOnet_kernel, SIGNAL(add_user(QString,QString,QString,QString)), this, SLOT(add_user(QString,QString,QString,QString)));
    QObject::connect(pOnet_kernel, SIGNAL(del_user(QString,QString)), this, SLOT(del_user(QString,QString)));
    QObject::connect(pOnet_kernel, SIGNAL(nicklist_refresh(QString)), this, SLOT(nicklist_refresh(QString)));
    QObject::connect(pOnet_kernel, SIGNAL(quit_user(QString,QString)), this, SLOT(quit_user(QString,QString)));
    QObject::connect(pOnet_kernel, SIGNAL(change_flag(QString,QString,QString)), this, SLOT(change_flag(QString,QString,QString)));
    QObject::connect(pOnet_kernel, SIGNAL(change_flag(QString,QString)), this, SLOT(change_flag(QString,QString)));
    QObject::connect(pOnet_kernel, SIGNAL(set_user_info(QString,QString,QString)), pDlg_user_profile, SLOT(set_user_info(QString,QString,QString)));
    QObject::connect(pOnet_kernel, SIGNAL(clear_channel_all_nick_avatars(QString)), this, SLOT(clear_channel_all_nick_avatars(QString)));

    // signals to network
    QObject::connect(pDlg_moderation, SIGNAL(send(QString)), pNetwork, SLOT(send(QString)));
    QObject::connect(pOnet_auth, SIGNAL(send(QString)), pNetwork, SLOT(send(QString)));

    // signals from network
    QObject::connect(pNetwork, SIGNAL(set_connected()), this, SLOT(set_connected()));
    QObject::connect(pNetwork, SIGNAL(set_disconnected()), this, SLOT(set_disconnected()));
    QObject::connect(pNetwork, SIGNAL(set_lag(QString)), this, SLOT(set_lag(QString)));
    QObject::connect(pNetwork, SIGNAL(set_connect_enabled(bool)), this, SLOT(set_connect_enabled(bool)));
    QObject::connect(pNetwork, SIGNAL(kernel(QString)), pOnet_kernel, SLOT(kernel(QString)));
    QObject::connect(pNetwork, SIGNAL(request_uo(QString,QString,QString)), pOnet_auth, SLOT(request_uo(QString,QString,QString)));
    QObject::connect(pNetwork, SIGNAL(show_msg_active(QString,int)), pTabC, SLOT(slot_show_msg_active(QString,int)));
    QObject::connect(pNetwork, SIGNAL(show_msg_all(QString,int)), pTabC, SLOT(slot_show_msg_all(QString,int)));
    QObject::connect(pNetwork, SIGNAL(update_nick(QString)), inputLineDockWidget, SLOT(slot_update_nick(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_nicklist(QString)), this, SLOT(clear_nicklist(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_all_nicklist()), this, SLOT(clear_all_nicklist()));
}

void Core::network_connect()
{
    pNetwork->connect();
}

void Core::network_disconnect()
{
    pNetwork->close();
}

void Core::network_send(QString data)
{
    pNetwork->send(data);
}

bool Core::network_is_connected()
{
    return pNetwork->is_connected();
}

void Core::network_clear_queue()
{
    pNetwork->clear_queue();
}

// refresh colors
void Core::refresh_colors()
{
    pTabC->refresh_colors();
}

// refresh background image
void Core::refresh_background_image()
{
    pTabC->refresh_background_image();
}

void Core::set_connected()
{
    connectAct->setText(tr("&Disconnect"));
    connectAct->setIconText(tr("&Disconnect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
}

void Core::set_disconnected()
{
    connectAct->setText(tr("&Connect"));
    connectAct->setIconText(tr("&Connect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
}

void Core::set_connect_enabled(bool bSet)
{
    connectAct->setEnabled(bSet);
}

// set lag
void Core::set_lag(QString strValue)
{
    lagAct->setText(strValue);
}

// onet dialogs
void Core::open_channel_list()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgChannelList(myparent, pNetwork, &stlChannelList).exec();
}

void Core::open_channel_homes()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_homes->show();
}

void Core::open_channel_favourites()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgChannelFavourites(myparent, pNetwork, &mChannelAvatar, &lChannelFavourites).exec();
}

void Core::open_friends()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgFriends(myparent, pNetwork, &mNickAvatar, &mFriends).exec();
}

void Core::open_ignore()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgIgnore(myparent, pNetwork, &mNickAvatar, &lIgnore).exec();
}

void Core::open_awaylog()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        DlgAwaylog(myparent, &lAwaylog).exec();
}

void Core::open_cams()
{
#ifdef Q_WS_WIN
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
    {
        QSettings settings;
        QString strMe = settings.value("nick").toString();
        QString strUOKey = settings.value("uokey").toString();
        (new Kamerzysta(camSocket))->show(strMe, strUOKey);
    }
#else
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_cam->show();
#endif
}

// part tab
void Core::tab_close_requested(int index)
{
    if (index != 0)
        pTabC->part_tab(index);
}

// tab changed
void Core::current_tab_changed(int index)
{
    QString strChannel = pTabC->get_name(index);

    // change tab name
    QString strTabText = pTabM->tabText(index);
    myparent->setWindowTitle(QString("Simple Chat Client - [%1]").arg(strTabText));

    // change tab color
    QSettings settings;
    pTabM->set_color(index, QColor(settings.value("default_font_color").toString()));

    // set tab active
    inputLineDockWidget->set_active(strChannel);

    // hide/show Status nicklist
    if (index == 0)
        rightDockWidget->hide();
    else
        rightDockWidget->show();

    // hide/show settings on priv
    if (strChannel[0] == '^')
        inputLineDockWidget->hide_channel_settings();
    else
        inputLineDockWidget->show_channel_settings();

    // nicklist
    if (mChannelNickListWidget.contains(strChannel))
    {
        // switch
        NickListWidget *nlw = mChannelNickListWidget.value(strChannel);
        nickListDockWidget->setCurrentWidget(nlw);
    }
    else
    {
        // create new
        create_nicklist(strChannel);
    }

    // set inputline users
    inputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

    // update nick count
    rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
}

void Core::create_nicklist(QString strChannel)
{
    if (mChannelNickListWidget.contains(strChannel) == false)
    {
        NickListWidget *nicklist = new NickListWidget(myparent, pNetwork, strChannel, &mNickAvatar, camSocket, &stlChannelNickStatus, pDlg_user_profile, pDlg_cam);
        nicklist->setParent(nickListDockWidget);
        nicklist->setItemDelegate(new NickListDelegate(nicklist));
        nicklist->show();

        mChannelNickListWidget.insert(strChannel, nicklist);
        nickListDockWidget->addWidget(nicklist);
    }
}

void Core::remove_nicklist(QString strChannel)
{
    if (mChannelNickListWidget.contains(strChannel) == true)
    {
        nickListDockWidget->removeWidget(mChannelNickListWidget.value(strChannel));
        delete mChannelNickListWidget.value(strChannel);
        mChannelNickListWidget.remove(strChannel);
    }
}

// nick list

bool Core::nicklist_exist(QString strChannel, QString strNick)
{
    if (mChannelNickListWidget.contains(strChannel) == true)
        return mChannelNickListWidget.value(strChannel)->exist(strNick, &stlChannelNickStatus);
    else
        return false;
}

void Core::add_user(QString strChannel, QString strNick, QString strPrefix, QString strSuffix)
{
    if ((nicklist_exist(strChannel, strNick) == false) && (mChannelNickListWidget.contains(strChannel) == true))
    {
        mChannelNickListWidget.value(strChannel)->add(strNick, strPrefix, strSuffix, &stlChannelNickStatus);

        // set inputline users
        if (inputLineDockWidget->get_active() == strChannel)
            inputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

        // update nick count for option hide join/part when > 200
        pTabC->add_user(strChannel);

        // update nick count
        if (inputLineDockWidget->get_active() == strChannel)
            rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
    }
}

void Core::del_user(QString strChannel, QString strNick)
{
    if ((nicklist_exist(strChannel, strNick) == true) && (mChannelNickListWidget.contains(strChannel) == true))
    {
        mChannelNickListWidget.value(strChannel)->remove(strNick, &stlChannelNickStatus);

        // set inputline users
        if (inputLineDockWidget->get_active() == strChannel)
            inputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

        // update nick count for option hide join/part when > 200
        pTabC->del_user(strChannel);

        // update nick count
        if (inputLineDockWidget->get_active() == strChannel)
            rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
    }
}

void Core::nicklist_refresh(QString strChannel)
{
    //raw 366: End of /NAMES list.
    if (mChannelNickListWidget.contains(strChannel) == true)
    {
        mChannelNickListWidget.value(strChannel)->expandAll();
        inputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));
    }
}

void Core::quit_user(QString strNick, QString strDisplay)
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.count(); i++)
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
            if (inputLineDockWidget->get_active() == strChannel)
                rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
        }
    }
}

void Core::change_flag(QString strNick, QString strChannel, QString strNewFlag)
{
    if (nicklist_exist(strChannel, strNick) == false) return; // nick not exist

    QString strOldPrefix;
    QString strOldSuffix;

    for (int i = 0; i < stlChannelNickStatus.count(); i++)
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

    for (int i = 0; i < strPrefix1.count(); i++)
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

    for (int i = 0; i < strSuffix1.count(); i++)
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
        if (strNewFlag == "+X") inputLineDockWidget->enable_moderation();
        else if (strNewFlag == "-X") inputLineDockWidget->disable_moderation();
    }
}

void Core::change_flag(QString strNick, QString strFlag)
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.count(); i++)
    {
        QString strChannel = strlChannels.at(i);

        if (nicklist_exist(strChannel, strNick) == true)
            change_flag(strNick, strChannel, strFlag);
    }
}

void Core::clear_nicklist(QString strChannel)
{
    // clear
    for (int i = 0; i < stlChannelNickStatus.count(); i++)
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
    if (inputLineDockWidget->get_active() == strChannel)
        rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strChannel)));
}

void Core::clear_all_nicklist()
{
    stlChannelNickStatus.clear();
    mNickAvatar.clear();
    mChannelAvatar.clear();

    QStringList strlChannels = pTabC->get_open_channels();

    for (int i = 0; i < strlChannels.count(); i++)
    {
        mChannelNickListWidget.value(strlChannels.at(i))->clear();

        // clear nick count for option hide join/part when > 200
        pTabC->clear_users(strlChannels.at(i));

        // update nick count
        if (inputLineDockWidget->get_active() == strlChannels.at(i))
            rightDockWidget->setWindowTitle(QString(tr("Users (%1)")).arg(pTabC->get_users(strlChannels.at(i))));
    }
}

void Core::update_nick_avatar(QString strNick)
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.count(); i++)
    {
        QString strChannel = strlChannels.at(i);

        // nicklist
        if (mChannelNickListWidget.value(strChannel)->exist(strNick, &stlChannelNickStatus) == true)
            mChannelNickListWidget.value(strChannel)->refresh_avatars();
    }
}

// clear all channel avatars
void Core::clear_channel_all_nick_avatars(QString strChannel)
{
    QStringList strlNicks = mChannelNickListWidget.value(strChannel)->get(&stlChannelNickStatus);

    for (int i = 0; i < strlNicks.count(); i++)
    {
        QString strNick = strlNicks.at(i);

        // remove nick avatar if nick is only in current channel; must be 1 (current channel)
        if ((mNickAvatar.contains(strNick) == true) && (pTabC->get_nick_channels(strNick) == 1))
            mNickAvatar.remove(strNick);
    }
}

void Core::set_open_channels()
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.count(); i++)
    {
        QString strChannel = strlChannels.at(i);

        mChannelNickListWidget.value(strChannel)->set_open_channels(strlChannels);
    }
}
