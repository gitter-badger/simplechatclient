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

#include "core.h"

Core::Core(QMainWindow *parent, QString param1, int param2, Notify *param3, QAction *param4, QToolBar *param5, QMenu *param6)
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
    lagAct = new QAction("Lag: 0s", myparent);

    // init all
    camSocket = new QTcpSocket();

    pTabM = new TabManager(myparent);
    myparent->setCentralWidget(pTabM);

    pNetwork = new Network(connectAct, lagAct, strServer, iPort);

    // classes
    pTabC = new TabContainer(myparent, pNetwork, pTabM, pNotify, &mChannelAvatar, camSocket, &mChannelNickStatus);

    pDlg_channel_settings = new DlgChannelSettings(myparent, pNetwork);
    pDlg_moderation = new DlgModeration(myparent);
    pDlg_channel_list = new DlgChannelList(myparent);
    pDlg_channel_homes = new DlgChannelHomes(myparent, pNetwork, &mChannelAvatar, pDlg_channel_settings);
    pDlg_channel_favourites = new DlgChannelFavourites(myparent, pNetwork, &mChannelAvatar);
    pDlg_friends = new DlgFriends(myparent, pNetwork, &mNickAvatar);
    pDlg_ignore = new DlgIgnore(myparent, pNetwork, &mNickAvatar);

    pOnet_kernel = new OnetKernel(myparent, pNetwork, pTabC, pNotify, &mNickAvatar, &mChannelAvatar, pDlg_channel_settings, pDlg_channel_homes, pDlg_channel_list, pDlg_channel_favourites, pDlg_friends, pDlg_ignore, pDlg_moderation);
    pOnet_auth = new OnetAuth(pTabC);

    // inputlinewidget
    bottomDockWidget = new QDockWidget(tr("Typing messages"), myparent);
    bottomDockWidget->setFocus();
    bottomDockWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea); // top and bottom
    inputLineDockWidget = new InputLineDockWidget(bottomDockWidget, pNetwork, pDlg_channel_settings, pDlg_moderation);
    bottomDockWidget->setWidget(inputLineDockWidget);
    myparent->addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);
    viewMenu->addAction(bottomDockWidget->toggleViewAction());

    // nicklistwidget
    rightDockWidget = new QDockWidget(tr("Users"), myparent);
    rightDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea); // left and right
    nickListDockWidget = new NickListDockWidget(rightDockWidget);
    rightDockWidget->setWidget(nickListDockWidget);
    myparent->addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);
    viewMenu->addAction(rightDockWidget->toggleViewAction());

    // hide nicklist and toolwidget on status
    rightDockWidget->hide();

    // welcome
    pTabC->show_msg("Status", "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%Cff6500%"+tr("Official website")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/ %Izaskoczony%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%C008100%"+tr("Official Forum")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/forum/ %Ioczko%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%Cff0000%"+tr("Known bugs")+"%C3030ce%: http://sourceforge.net/apps/trac/simplechatclien/report/6 %Iskwaszony%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%C8800ab%"+tr("Bug reporting")+"%C3030ce%: http://sourceforge.net/apps/trac/simplechatclien/newticket %Ipanda%", 0);

    // toolbar
    toolBar->addSeparator();
    toolBar->addAction(lagAct);

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
    QObject::connect(inputLineDockWidget, SIGNAL(set_scroll(QString,bool)), pTabC, SLOT(slot_set_scroll(QString,bool)));

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
    QObject::connect(pOnet_kernel, SIGNAL(set_user_info(QString,QString,QString)), this, SLOT(set_user_info(QString,QString,QString)));

    // signals to network
    QObject::connect(pDlg_moderation, SIGNAL(send(QString)), pNetwork, SLOT(slot_send(QString)));
    QObject::connect(pDlg_channel_list, SIGNAL(send(QString)), pNetwork, SLOT(slot_send(QString)));
    QObject::connect(pOnet_auth, SIGNAL(send(QString)), pNetwork, SLOT(slot_send(QString)));

    // signals from network
    QObject::connect(pNetwork, SIGNAL(kernel(QString)), pOnet_kernel, SLOT(kernel(QString)));
    QObject::connect(pNetwork, SIGNAL(request_uo(QString,QString,QString)), pOnet_auth, SLOT(request_uo(QString,QString,QString)));
    QObject::connect(pNetwork, SIGNAL(show_msg_active(QString,int)), pTabC, SLOT(slot_show_msg_active(QString,int)));
    QObject::connect(pNetwork, SIGNAL(show_msg_all(QString,int)), pTabC, SLOT(slot_show_msg_all(QString,int)));
    QObject::connect(pNetwork, SIGNAL(update_nick(QString)), inputLineDockWidget, SLOT(slot_update_nick(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_nicklist(QString)), this, SLOT(clear_nicklist(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_all_nicklist()), this, SLOT(clear_all_nicklist()));
}

Core::~Core()
{
    // clear arrays
    mChannelNickStatus.clear();
    mNickAvatar.clear();
    mChannelAvatar.clear();

    // close network
    QSettings settings;
    settings.setValue("reconnect", "false");
    pNetwork->send("QUIT");
    pNetwork->close();

    delete pOnet_auth;
    delete pOnet_kernel;
    delete pDlg_ignore;
    delete pDlg_friends;
    delete pDlg_channel_favourites;
    delete pDlg_channel_homes;
    delete pDlg_channel_list;
    delete pDlg_moderation;
    delete pDlg_channel_settings;
    delete pNetwork;
    delete bottomDockWidget;
    delete rightDockWidget;
    delete pTabC;
    delete pTabM;
    delete camSocket;
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

// refresh colors
void Core::refresh_colors()
{
    pTabC->refresh_colors();
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
        pDlg_channel_list->show();
}

void Core::open_channel_homes()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_homes->show();
}

void Core::open_channel_favourites()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_channel_favourites->show();
}

void Core::open_friends()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_friends->show();
}

void Core::open_ignore()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        pDlg_ignore->show();
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
    pTabM->set_color(index, QColor(0,0,0));

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

    // set scroll
    bool bChangeScroll = pTabC->get_scroll(strChannel);
    inputLineDockWidget->change_scroll(bChangeScroll);
}

void Core::create_nicklist(QString strChannel)
{
    if (mChannelNickListWidget.contains(strChannel) == false)
    {
        NickListWidget *nicklist = new NickListWidget(myparent, pNetwork, strChannel, &mNickAvatar, camSocket, &mChannelNickStatus);
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
        return mChannelNickListWidget.value(strChannel)->exist(strNick, &mChannelNickStatus);
    else
        return false;
}

void Core::add_user(QString strChannel, QString strNick, QString strPrefix, QString strSuffix)
{
    if ((nicklist_exist(strChannel, strNick) == false) && (mChannelNickListWidget.contains(strChannel) == true))
    {
        mChannelNickListWidget.value(strChannel)->add(strNick, strPrefix, strSuffix, &mChannelNickStatus);

        // set inputline users
        if (inputLineDockWidget->get_active() == strChannel)
            inputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

        /// REGRESSION
        //iNickCount++;
        //nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));
    }
}

void Core::del_user(QString strChannel, QString strNick)
{
    if ((nicklist_exist(strChannel, strNick) == true) && (mChannelNickListWidget.contains(strChannel) == true))
    {
        mChannelNickListWidget.value(strChannel)->remove(strNick, &mChannelNickStatus);

        // set inputline users
        if (inputLineDockWidget->get_active() == strChannel)
            inputLineDockWidget->set_userslist(mChannelNickListWidget.value(strChannel));

        /// REGRESSION
        //iNickCount--;
        //nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));
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
        }
    }
}

void Core::change_flag(QString strNick, QString strChannel, QString strNewFlag)
{
    if (nicklist_exist(strChannel, strNick) == false) return; // nick not exist

    QString strOldPrefix;
    QString strOldSuffix;

    for (int i = 0; i < mChannelNickStatus.count(); i++)
    {
        if ((mChannelNickStatus.at(i).nick == strNick) && (mChannelNickStatus.at(i).channel == strChannel))
        {
            strOldPrefix = mChannelNickStatus.at(i).prefix;
            strOldSuffix = mChannelNickStatus.at(i).suffix;
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
    /// REGRESSION
    //iNickCount = 0;
    //nickCount->setText(QString(tr("%1 User(s)")).arg(iNickCount));

    for (int i = 0; i < mChannelNickStatus.count(); i++)
    {
        if (mChannelNickStatus.at(i).channel == strChannel)
        {
            mChannelNickStatus.removeAt(i);
            i--;
        }
    }
}

void Core::clear_all_nicklist()
{
    mChannelNickStatus.clear();
    mNickAvatar.clear();
    mChannelAvatar.clear();

    QStringList strlChannels = pTabC->get_open_channels();

    for (int i = 0; i < strlChannels.count(); i++)
        mChannelNickListWidget.value(strlChannels.at(i))->clear();
}

void Core::set_user_info(QString strNick, QString strKey, QString strValue)
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.count(); i++)
    {
        QString strChannel = strlChannels.at(i);

        // nicklist
        if (mChannelNickListWidget.value(strChannel)->exist(strNick, &mChannelNickStatus) == true)
            mChannelNickListWidget.value(strChannel)->set_user_info(strNick, strKey, strValue);

        // mainwebview
        MainWebView *mainWebView = pTabC->get_webview(strChannel);

        if (mainWebView->get_current_nick() == strNick)
            mainWebView->set_user_info(strNick, strKey, strValue);
    }
}

void Core::update_nick_avatar(QString strNick)
{
    QStringList strlChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlChannels.count(); i++)
    {
        QString strChannel = strlChannels.at(i);

        // nicklist
        if (mChannelNickListWidget.value(strChannel)->exist(strNick, &mChannelNickStatus) == true)
            mChannelNickListWidget.value(strChannel)->refresh_avatars();
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

/*

/// REGRESSION

void TabWidget::set_link(QString strUrl)
{
    webLink->setText(QString("<a href=\"%1\" style=\"color:#0000FF;text-decoration:none;\" >"+tr("Channel website")+"</a>").arg(strUrl));
    webLink->setToolTip(strUrl);
}

QStringList TabWidget::get_nicklist()
{
    return nicklist->get(&nickStatus);
}

void TabContainer::clear_channel_all_nick_avatars(QString strChannel)
{
    int i = get_index(strChannel);
    if (i != -1)
    {
        QStringList strlList = tw[i]->get_nicklist();

        for (int i = 0; i < strlList.count(); i++)
        {
            QString strNick = strlList.at(i);

            // remove nick from avatars if not exist on open channels; must be 1 (current channel)
            if ((mNickAvatar->contains(strNick) == true) && (get_nick_channels(strNick) == 1))
                mNickAvatar->remove(strNick);
        }
    }
}

// for avatars (if nick not in any channels->remove avatar)
int TabContainer::get_nick_channels(QString strNick)
{
    int iResult = 0;

    for (int i = 0; i < tw.count(); i++)
    {
        if (tw[i]->nicklist_exist(strNick) == true)
            iResult++;
    }

    return iResult;
}
*/
