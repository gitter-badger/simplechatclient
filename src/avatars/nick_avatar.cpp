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

#include "nick_avatar.h"
NickAvatarThread::NickAvatarThread(QString param1, QString param2, QMap <QString, QByteArray> *param3)
{
    strNick = param1;
    strUrl = param2;
    mNickAvatar = param3;
}

void NickAvatarThread::run()
{
    QTimer::singleShot(0, this, SLOT(thread_work()));

    exec();
}

void NickAvatarThread::thread_work()
{
    if (mNickAvatar->contains(strNick) == false)
    {
        QNetworkAccessManager accessManager;
        QNetworkReply *pReply;
        QEventLoop eventLoop;
        pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        QByteArray bData = pReply->readAll();

        pReply->deleteLater();

        emit set_nick_avatar(strNick, bData);
        emit set_avatar(strNick);
    }

    emit stop_thread();
}

NickAvatar::NickAvatar(TabContainer *param1, QString param2, QString param3, QMap <QString, QByteArray> *param4)
{
    tabc = param1;
    strNick = param2;
    strUrl = param3;
    mNickAvatar = param4;

    nickAvatarThr = new NickAvatarThread(strNick, strUrl, mNickAvatar);
    QObject::connect(nickAvatarThr, SIGNAL(set_avatar(QString)), tabc, SLOT(slot_update_nick_avatar(QString)));
    QObject::connect(nickAvatarThr, SIGNAL(set_nick_avatar(QString, QByteArray)), this, SLOT(set_nick_avatar(QString, QByteArray)));
    QObject::connect(nickAvatarThr, SIGNAL(stop_thread()), this, SLOT(stop_thread()));
    nickAvatarThr->start(QThread::InheritPriority);
}

void NickAvatar::kill_thread()
{
    nickAvatarThr->quit();
    nickAvatarThr->wait();
    nickAvatarThr->QObject::disconnect();
    nickAvatarThr->deleteLater();
    delete nickAvatarThr;
}

void NickAvatar::set_nick_avatar(QString strNick, QByteArray bAvatar)
{
    if (mNickAvatar->contains(strNick) == false)
        mNickAvatar->insert(strNick, bAvatar);
}

void NickAvatar::stop_thread()
{
    kill_thread();
    emit sremove_nathread(this);
}
