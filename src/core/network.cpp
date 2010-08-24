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

#include "network.h"

Network::Network(QWidget *parent, QAction *param1, QSettings *param2)
{
    connectAct = param1;
    settings = param2;

    networkThr = new NetworkThread(connectAct, settings);
    networkThr->start(QThread::InheritPriority);

    QObject::connect(networkThr, SIGNAL(send_to_kernel(QString)), parent, SLOT(kernel(QString)));
    QObject::connect(networkThr, SIGNAL(request_uo(QString, QString, QString)), parent, SLOT(request_uo(QString, QString, QString)));
    QObject::connect(networkThr, SIGNAL(show_msg_active(QString, int)), parent, SLOT(show_msg_active(QString, int)));
    QObject::connect(networkThr, SIGNAL(show_msg_all(QString, int)), parent, SLOT(show_msg_all(QString, int)));
    QObject::connect(networkThr, SIGNAL(update_nick(QString)), parent, SLOT(update_nick(QString)));
    QObject::connect(networkThr, SIGNAL(clear_nicklist(QString)), parent, SLOT(clear_nicklist(QString)));
    QObject::connect(networkThr, SIGNAL(clear_all_nicklist()), parent, SLOT(clear_all_nicklist()));

    QObject::connect(this, SIGNAL(do_connect()), networkThr, SLOT(connect()));
    QObject::connect(this, SIGNAL(do_close()), networkThr, SLOT(close()));
    QObject::connect(this, SIGNAL(do_send(QString)), networkThr, SLOT(send(QString)));
}

Network::~Network()
{
    networkThr->quit();
    networkThr->wait();
    networkThr->deleteLater();
    networkThr->QObject::disconnect();
    delete networkThr;
}

bool Network::is_connected()
{
    return networkThr->is_connected();
}

bool Network::is_writable()
{
    return networkThr->is_writable();
}

void Network::connect()
{
    emit do_connect();
}

void Network::close()
{
    emit do_close();
}

void Network::send(QString strData)
{
    emit do_send(strData);
}

void Network::send_slot(QString strData)
{
    emit do_send(strData);
}
