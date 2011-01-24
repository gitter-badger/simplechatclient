/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include "avatar.h"
#include "tab_container.h"

AvatarThread::AvatarThread(QString param1, QString param2, QString param3)
{
    strNickChannel = param1;
    strUrl = param2;
    strCategory = param3;
}

void AvatarThread::run()
{
    QTimer::singleShot(0, this, SLOT(thread_work()));

    exec();
}

void AvatarThread::thread_work()
{
    QNetworkAccessManager accessManager;
    QNetworkReply *pReply;
    QEventLoop eventLoop;
    pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
    QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QByteArray bData = pReply->readAll();
    pReply->deleteLater();

    emit set_avatar(strNickChannel, strCategory, bData);
    emit stop_thread();
}

Avatar::Avatar(TabContainer *param1, QString param2, QString param3, QString param4, QMap <QString, QByteArray> *param5, QMap <QString, QByteArray> *param6)
{
    tabc = param1;
    strNickChannel = param2;
    strUrl = param3;
    strCategory = param4;
    mNickAvatar = param5;
    mChannelAvatar = param6;

    avatarThr = new AvatarThread(strNickChannel, strUrl, strCategory);
    QObject::connect(this, SIGNAL(set_nick_avatar(QString)), tabc, SLOT(slot_update_nick_avatar(QString)));
    QObject::connect(this, SIGNAL(set_channel_avatar(QString)), tabc, SLOT(slot_update_channel_avatar(QString)));
    QObject::connect(avatarThr, SIGNAL(set_avatar(QString, QString, QByteArray)), this, SLOT(set_avatar(QString, QString, QByteArray)));
    QObject::connect(avatarThr, SIGNAL(stop_thread()), this, SLOT(stop_thread()));
    avatarThr->start(QThread::InheritPriority);
}

void Avatar::kill_thread()
{
    avatarThr->quit();
    avatarThr->wait();
    avatarThr->QObject::disconnect();
    avatarThr->deleteLater();
    delete avatarThr;
}

void Avatar::set_avatar(QString strNickChannel, QString strCategory, QByteArray bAvatar)
{
    if (strCategory == "nick")
    {
        // insert
        if (mNickAvatar->contains(strNickChannel) == false)
            mNickAvatar->insert(strNickChannel, bAvatar);
        else
        {
            // update
            if (mNickAvatar->value(strNickChannel) != bAvatar)
            {
                mNickAvatar->remove(strNickChannel);
                mNickAvatar->insert(strNickChannel, bAvatar);
            }
        }

        emit set_nick_avatar(strNickChannel);
    }
    else if (strCategory == "channel")
    {
        // insert
        if (mChannelAvatar->contains(strNickChannel) == false)
            mChannelAvatar->insert(strNickChannel, bAvatar);
        else
        {
            // update
            if (mChannelAvatar->value(strNickChannel) != bAvatar)
            {
                mChannelAvatar->remove(strNickChannel);
                mChannelAvatar->insert(strNickChannel, bAvatar);
            }
        }

        emit set_channel_avatar(strNickChannel);
    }
}

void Avatar::stop_thread()
{
    kill_thread();
    emit sremove_athread(this);
}
