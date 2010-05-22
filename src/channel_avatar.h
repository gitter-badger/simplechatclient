/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#ifndef CHANNEL_AVATAR_H
#define CHANNEL_AVATAR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QThread>
#include "tab_container.h"

class channel_avatar_thread : public QThread
{
    Q_OBJECT
public:
    channel_avatar_thread(QString, QString);
    void run();

private:
    QString strChannel;
    QString strUrl;

private slots:
    void threadWork();

signals:
    void set_avatar(QString, QByteArray);
    void stop_thread();

};

class channel_avatar : public QObject
{
    Q_OBJECT
public:
    channel_avatar(tab_container *, QString, QString);
    void start_thread();

private:
    tab_container *tabc;
    QString strChannel;
    QString strUrl;
    channel_avatar_thread *channelAvatarThr;

public slots:
    void stop_thread();

};

#endif // CHANNEL_AVATAR_H
