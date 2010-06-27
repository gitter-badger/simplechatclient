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

#include "nick_avatar.h"
NickAvatarThread::NickAvatarThread(QString param1, QString param2)
{
    strNick = param1;
    strUrl = param2;
}

void NickAvatarThread::run()
{
    QTimer::singleShot(0, this, SLOT(thread_work()));

    exec();
}

void NickAvatarThread::thread_work()
{
    QNetworkAccessManager accessManager;
    QNetworkReply *pReply;
    QEventLoop eventLoop;
    pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
    QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QByteArray bData = pReply->readAll();

    delete pReply;

    emit set_avatar(strNick, bData);
    emit stop_thread();
}

NickAvatar::NickAvatar(TabContainer *param1, QString param2, QString param3)
{
    tabc = param1;
    strNick = param2;
    strUrl = param3;

    nickAvatarThr = new NickAvatarThread(strNick, strUrl);
    QObject::connect(nickAvatarThr, SIGNAL(set_avatar(QString, QByteArray)), tabc, SLOT(update_nick_avatar(QString, QByteArray)));
    QObject::connect(nickAvatarThr, SIGNAL(stop_thread()), this, SLOT(stop_thread()));
    nickAvatarThr->start(QThread::LowPriority);
}

void NickAvatar::stop_thread()
{
    nickAvatarThr->quit();
    nickAvatarThr->wait();
    nickAvatarThr->QObject::disconnect();
    nickAvatarThr->deleteLater();
    delete nickAvatarThr;

    emit do_remove_nathread(this);
}
