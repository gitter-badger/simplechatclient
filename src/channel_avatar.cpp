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

channel_avatar_thread::channel_avatar_thread(QString param1, QString param2)
{
    strUrl = param1;
    strChannel = param2;
}

void channel_avatar_thread::run()
{
    QTimer::singleShot(0, this, SLOT(threadWork()));

    exec();
}

void channel_avatar_thread::threadWork()
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
}

channel_avatar::channel_avatar(tab_container *param1, QString param2, QString param3)
{
    tabc = param1;
    strUrl = param2;
    strChannel = param3;
}

void channel_avatar::start_thread()
{
    channelAvatarThr = new channel_avatar_thread(strUrl, strChannel);
    QObject::connect(channelAvatarThr, SIGNAL(set_avatar(QString, QByteArray)), this, SLOT(setAvatar(QString, QByteArray)));
    channelAvatarThr->start(QThread::LowPriority);
}

void channel_avatar::setAvatar(QString strChannel, QByteArray bData)
{
    tabc->set_logo(strChannel, bData);

    channelAvatarThr->quit();
    channelAvatarThr->wait();
    channelAvatarThr->QObject::disconnect();
    delete channelAvatarThr;
}
