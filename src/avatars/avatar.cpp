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

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include "avatar.h"
#include "tab_container.h"

Avatar::Avatar(TabContainer *param1, QMap <QString, QByteArray> *param2, QMap <QString, QByteArray> *param3)
{
    tabc = param1;
    mNickAvatar = param2;
    mChannelAvatar = param3;

    QObject::connect(this, SIGNAL(set_nick_avatar(QString)), tabc, SLOT(slot_update_nick_avatar(QString)));
    QObject::connect(this, SIGNAL(set_channel_avatar(QString)), tabc, SLOT(slot_update_channel_avatar(QString)));

    accessManager = new QNetworkAccessManager;
    QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(avatar_finished(QNetworkReply*)));
}

Avatar::~Avatar()
{
    accessManager->deleteLater();
}

void Avatar::get_avatar(QString strNickOrChannel, QString strCategory, QString strUrl)
{
    QNetworkReply *reply = accessManager->get(QNetworkRequest(strUrl));
    reply->setProperty("nickorchannel", strNickOrChannel);
    reply->setProperty("category", strCategory);
}

void Avatar::avatar_finished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QString strNickOrChannel = reply->property("nickorchannel").toString();
    QString strCategory = reply->property("category").toString();
    QByteArray bData = reply->readAll();

    if (bData.isEmpty() == false)
        set_avatar(strNickOrChannel, strCategory, bData);
}

void Avatar::set_avatar(QString strNickOrChannel, QString strCategory, QByteArray bAvatar)
{
    if (strCategory == "nick")
    {
        // insert
        if (mNickAvatar->contains(strNickOrChannel) == false)
            mNickAvatar->insert(strNickOrChannel, bAvatar);
        else
        {
            // update
            if (mNickAvatar->value(strNickOrChannel) != bAvatar)
            {
                mNickAvatar->remove(strNickOrChannel);
                mNickAvatar->insert(strNickOrChannel, bAvatar);
            }
        }

        emit set_nick_avatar(strNickOrChannel);
    }
    else if (strCategory == "channel")
    {
        // insert
        if (mChannelAvatar->contains(strNickOrChannel) == false)
            mChannelAvatar->insert(strNickOrChannel, bAvatar);
        else
        {
            // update
            if (mChannelAvatar->value(strNickOrChannel) != bAvatar)
            {
                mChannelAvatar->remove(strNickOrChannel);
                mChannelAvatar->insert(strNickOrChannel, bAvatar);
            }
        }

        emit set_channel_avatar(strNickOrChannel);
    }
}
