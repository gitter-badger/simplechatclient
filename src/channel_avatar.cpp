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

#include "channel_avatar.h"

ChannelAvatarThread::ChannelAvatarThread(QString param1, QString param2)
{
    strChannel = param1;
    strUrl = param2;
}

void ChannelAvatarThread::run()
{
    QTimer::singleShot(0, this, SLOT(thread_work()));

    exec();
}

void ChannelAvatarThread::thread_work()
{
    QNetworkAccessManager accessManager;
    QNetworkReply *pReply;
    QEventLoop eventLoop;
    pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
    QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QByteArray bData = pReply->readAll();

    delete pReply;

    emit set_avatar(strChannel, bData);
    emit stop_thread();
}

ChannelAvatar::ChannelAvatar(TabContainer *param1, QString param2, QString param3)
{
    tabc = param1;
    strChannel = param2;
    strUrl = param3;

    channelAvatarThr = new ChannelAvatarThread(strChannel, strUrl);
    QObject::connect(channelAvatarThr, SIGNAL(set_avatar(QString, QByteArray)), tabc, SLOT(set_logo(QString, QByteArray)));
    QObject::connect(channelAvatarThr, SIGNAL(stop_thread()), this, SLOT(stop_thread()));
    channelAvatarThr->start(QThread::LowPriority);
}

void ChannelAvatar::stop_thread()
{
    channelAvatarThr->quit();
    channelAvatarThr->wait();
    channelAvatarThr->QObject::disconnect();
    channelAvatarThr->deleteLater();
    delete channelAvatarThr;

    emit do_remove_cathread(this);
}
