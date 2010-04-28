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

channel_avatar_thread::channel_avatar_thread(int param1, QString param2, QString param3)
{
    i = param1;
    strUrl = param2;
    strChannel = param3;
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

    emit set_avatar(i, strChannel, bData);
}

channel_avatar::channel_avatar(tab_container *param1)
{
    tabc = param1;
    i = 0;
}

void channel_avatar::start_thread(QString param1, QString param2)
{
    strUrl = param1;
    strChannel = param2;

    channelAvatarThr.insert(i, new channel_avatar_thread(i, strUrl, strChannel));
    QObject::connect(channelAvatarThr.at(i), SIGNAL(set_avatar(int, QString, QByteArray)), this, SLOT(setAvatar(int, QString, QByteArray)));
    channelAvatarThr.at(i)->start(QThread::LowPriority);
    i++;
}

void channel_avatar::setAvatar(int iNumber, QString strChannel, QByteArray bData)
{
    tabc->set_logo(strChannel, bData);

    channelAvatarThr.at(iNumber)->quit();
    channelAvatarThr.at(iNumber)->wait();
    channelAvatarThr.at(iNumber)->QObject::disconnect();
    //delete (channelAvatarThr[iNumber]);
    //channelAvatarThr.removeAt(iNumber);
    //qDebug() << "i: " << iNumber << " count:" << channelAvatarThr.size();
}
