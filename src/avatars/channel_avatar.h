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

#ifndef CHANNEL_AVATAR_H
#define CHANNEL_AVATAR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QThread>
#include "tab_container.h"

class ChannelAvatarThread : public QThread
{
    Q_OBJECT
public:
    ChannelAvatarThread(QString, QString, QMap <QString, QByteArray> *);
    void run();

private:
    QString strChannel;
    QString strUrl;
    QMap <QString, QByteArray> *mChannelAvatar;

private slots:
    void thread_work();

signals:
    void set_avatar(QString);
    void set_channel_avatar(QString, QByteArray);
    void stop_thread();

};

class ChannelAvatar : public QObject
{
    Q_OBJECT
public:
    ChannelAvatar(TabContainer *, QString, QString, QMap <QString, QByteArray> *);
    void kill_thread();

private:
    TabContainer *tabc;
    QString strChannel;
    QString strUrl;
    QMap <QString, QByteArray> *mChannelAvatar;
    ChannelAvatarThread *channelAvatarThr;

public slots:
    void set_channel_avatar(QString, QByteArray);
    void stop_thread();

signals:
    void do_remove_cathread(ChannelAvatar*);

};

#endif // CHANNEL_AVATAR_H
