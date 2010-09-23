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

#include "channel_avatar.h"

ChannelAvatarThread::ChannelAvatarThread(QString param1, QString param2, QMap <QString, QByteArray> *param3)
{
    strChannel = param1;
    strUrl = param2;
    mChannelAvatar = param3;
}

void ChannelAvatarThread::run()
{
    QTimer::singleShot(0, this, SLOT(thread_work()));

    exec();
}

void ChannelAvatarThread::thread_work()
{
    if (mChannelAvatar->contains(strChannel) == false)
    {
        QNetworkAccessManager accessManager;
        QNetworkReply *pReply;
        QEventLoop eventLoop;
        pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        QByteArray bData = pReply->readAll();

        delete pReply;

        emit set_channel_avatar(strChannel, bData);
        emit set_avatar(strChannel);
    }
    else
        emit set_avatar(strChannel);

    emit stop_thread();
}

ChannelAvatar::ChannelAvatar(TabContainer *param1, QString param2, QString param3, QMap <QString, QByteArray> *param4)
{
    tabc = param1;
    strChannel = param2;
    strUrl = param3;
    mChannelAvatar = param4;

    channelAvatarThr = new ChannelAvatarThread(strChannel, strUrl, mChannelAvatar);
    QObject::connect(channelAvatarThr, SIGNAL(set_avatar(QString)), tabc, SLOT(update_channel_avatar(QString)));
    QObject::connect(channelAvatarThr, SIGNAL(set_channel_avatar(QString, QByteArray)), this, SLOT(set_channel_avatar(QString, QByteArray)));
    QObject::connect(channelAvatarThr, SIGNAL(stop_thread()), this, SLOT(stop_thread()));
    channelAvatarThr->start(QThread::InheritPriority);
}

void ChannelAvatar::kill_thread()
{
    channelAvatarThr->quit();
    channelAvatarThr->wait();
    channelAvatarThr->QObject::disconnect();
    channelAvatarThr->deleteLater();
    delete channelAvatarThr;
}

void ChannelAvatar::set_channel_avatar(QString strChannel, QByteArray bAvatar)
{
    if (mChannelAvatar->contains(strChannel) == false)
        mChannelAvatar->insert(strChannel, bAvatar);
}

void ChannelAvatar::stop_thread()
{
    kill_thread();
    emit do_remove_cathread(this);
}
