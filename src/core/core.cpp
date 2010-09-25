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

Core::Core(QMainWindow *parent, QString param1, int param2, Notify *param3, QAction *param4, QToolBar *param5)
{
    // params
    myparent = parent;
    strServer = param1;
    iPort = param2;
    pNotify = param3;
    connectAct = param4;
    toolBar = param5;

    // lag
    lagAct = new QAction("Lag: 0s", myparent);

    // init all
    camSocket = new QTcpSocket();

    pTabM = new TabManager(myparent);
    myparent->setCentralWidget(pTabM);

    pNetwork = new Network(connectAct, lagAct, strServer, iPort);
    pTabC = new TabContainer(myparent, pNetwork, pTabM, pNotify, &mNickAvatar, &mChannelAvatar, camSocket);

    pDlg_channel_settings = new DlgChannelSettings(myparent, pNetwork);
    pDlg_moderation = new DlgModeration(myparent);
    pDlg_channel_list = new DlgChannelList(myparent);
    pDlg_channel_homes = new DlgChannelHomes(myparent, pNetwork, &mChannelAvatar, pDlg_channel_settings);
    pDlg_channel_favourites = new DlgChannelFavourites(myparent, pNetwork, &mChannelAvatar);
    pDlg_friends = new DlgFriends(myparent, pNetwork, &mNickAvatar);
    pDlg_ignore = new DlgIgnore(myparent, pNetwork, &mNickAvatar);

    pOnet_kernel = new OnetKernel(myparent, pNetwork, pTabC, &mNickAvatar, &mChannelAvatar, pDlg_channel_settings, pDlg_channel_homes, pDlg_channel_list, pDlg_channel_favourites, pDlg_friends, pDlg_ignore, pDlg_moderation);
    pOnet_auth = new OnetAuth(pTabC);

    pTabC->set_dlg(pDlg_channel_settings, pDlg_moderation);

    // welcome
    pTabC->show_msg("Status", "%Fi:courier%"+tr("Welcome to the Simple Chat Client")+" %Ihehe%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%Cff6500%"+tr("Official website")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/ %Izaskoczony%", 0);
    pTabC->show_msg("Status", "%Fb:courier%%C008100%"+tr("Official Forum")+" SCC%C3030ce%: http://simplechatclien.sourceforge.net/forum/ %Ioczko%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%Cff0000%"+tr("Known bugs")+"%C3030ce%: http://sourceforge.net/apps/trac/simplechatclien/report/6 %Iskwaszony%", 0);
    pTabC->show_msg("Status", "%Fbi:courier%%C8800ab%"+tr("Bug reporting")+"%C3030ce%: http://sourceforge.net/apps/trac/simplechatclien/newticket %Ipanda%", 0);

    // toolbar
    toolBar->addSeparator();
    toolBar->addAction(lagAct);

    // signals tab
    QObject::connect(pTabM, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
    QObject::connect(pDlg_moderation, SIGNAL(display_msg(QString,QString,int)), pTabC, SLOT(slot_show_msg(QString,QString,int)));

    // signals lag
    QObject::connect(pOnet_kernel, SIGNAL(set_lag(QString)), this, SLOT(set_lag(QString)));

    // signals to network
    QObject::connect(pDlg_moderation, SIGNAL(send(QString)), pNetwork, SLOT(slot_send(QString)));
    QObject::connect(pDlg_channel_list, SIGNAL(send(QString)), pNetwork, SLOT(slot_send(QString)));
    QObject::connect(pOnet_auth, SIGNAL(send(QString)), pNetwork, SLOT(slot_send(QString)));

    // signals from network
    QObject::connect(pNetwork, SIGNAL(kernel(QString)), pOnet_kernel, SLOT(kernel(QString)));
    QObject::connect(pNetwork, SIGNAL(request_uo(QString, QString, QString)), pOnet_auth, SLOT(request_uo(QString,QString,QString)));
    QObject::connect(pNetwork, SIGNAL(show_msg_active(QString, int)), pTabC, SLOT(slot_show_msg_active(QString, int)));
    QObject::connect(pNetwork, SIGNAL(show_msg_all(QString, int)), pTabC, SLOT(slot_show_msg_all(QString, int)));
    QObject::connect(pNetwork, SIGNAL(update_nick(QString)), pTabC, SLOT(slot_update_nick(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_nicklist(QString)), pTabC, SLOT(slot_clear_nicklist(QString)));
    QObject::connect(pNetwork, SIGNAL(clear_all_nicklist()), pTabC, SLOT(slot_clear_all_nicklist()));
}

Core::~Core()
{
    // clear arrays
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
void Core::set_lag(QString strValue) { lagAct->setText(strValue); }

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
