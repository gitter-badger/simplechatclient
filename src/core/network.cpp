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

Network::Network(QAction *param1, QAction *param2, QString param3, int param4)
{
    connectAct = param1;
    lagAct = param2;
    strServer = param3;
    iPort = param4;

    networkThr = new NetworkThread(connectAct, lagAct, strServer, iPort);
    networkThr->start(QThread::InheritPriority);

    QObject::connect(networkThr, SIGNAL(send_to_kernel(QString)), this, SLOT(slot_kernel(QString)));
    QObject::connect(networkThr, SIGNAL(request_uo(QString, QString, QString)), this, SLOT(slot_request_uo(QString, QString, QString)));
    QObject::connect(networkThr, SIGNAL(show_msg_active(QString, int)), this, SLOT(slot_show_msg_active(QString, int)));
    QObject::connect(networkThr, SIGNAL(show_msg_all(QString, int)), this, SLOT(slot_show_msg_all(QString, int)));
    QObject::connect(networkThr, SIGNAL(update_nick(QString)), this, SLOT(slot_update_nick(QString)));
    QObject::connect(networkThr, SIGNAL(clear_nicklist(QString)), this, SLOT(slot_clear_nicklist(QString)));
    QObject::connect(networkThr, SIGNAL(clear_all_nicklist()), this, SLOT(slot_clear_all_nicklist()));

    QObject::connect(this, SIGNAL(sconnect()), networkThr, SLOT(connect()));
    QObject::connect(this, SIGNAL(sclose()), networkThr, SLOT(close()));
    QObject::connect(this, SIGNAL(ssend(QString)), networkThr, SLOT(send(QString)));
    QObject::connect(this, SIGNAL(sclear_queue()), networkThr, SLOT(clear_queue()));
}

Network::~Network()
{
    networkThr->quit();
    networkThr->wait();
    networkThr->deleteLater();
    networkThr->QObject::disconnect();
    delete networkThr;
}

// from network
void Network::slot_kernel(QString p1) { emit kernel(p1); }
void Network::slot_request_uo(QString p1, QString p2, QString p3) { emit request_uo(p1, p2, p3); }
void Network::slot_show_msg_active(QString p1, int p2) { emit show_msg_active(p1, p2); }
void Network::slot_show_msg_all(QString p1, int p2) { emit show_msg_all(p1, p2); }
void Network::slot_update_nick(QString p1) { emit update_nick(p1); }
void Network::slot_clear_nicklist(QString p1) { emit clear_nicklist(p1); }
void Network::slot_clear_all_nicklist() { emit clear_all_nicklist(); }

// to network

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
    emit sconnect();
}

void Network::close()
{
    emit sclose();
}

void Network::send(QString strData)
{
    emit ssend(strData);
}

void Network::clear_queue()
{
    emit sclear_queue();
}

void Network::slot_send(QString strData)
{
    emit ssend(strData);
}
