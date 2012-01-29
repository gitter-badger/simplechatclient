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

#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include "avatar.h"
#include "core.h"
#include "tab_container.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

Avatar::Avatar(TabContainer *_pTabC) : pTabC(_pTabC)
{
    connect(this, SIGNAL(setChannelAvatar(QString)), pTabC, SLOT(setChannelAvatar(QString)));

    accessManager = new QNetworkAccessManager;
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(avatarFinished(QNetworkReply*)));
}

Avatar::~Avatar()
{
    accessManager->deleteLater();
}

void Avatar::getAvatar(QString strNickOrChannel, QString strCategory, QString strUrl)
{
    QNetworkReply *reply = accessManager->get(QNetworkRequest(strUrl));
    reply->setProperty("nickorchannel", strNickOrChannel);
    reply->setProperty("category", strCategory);
}

void Avatar::avatarFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QString strNickOrChannel = reply->property("nickorchannel").toString();
    QString strCategory = reply->property("category").toString();
    QFileInfo fi(reply->url().toString());
    QString strAvatarPath = fi.fileName();
    QByteArray bData = reply->readAll();

    if (!bData.isEmpty())
        setAvatar(strNickOrChannel, strCategory, strAvatarPath, bData);
}

void Avatar::setAvatar(QString &strNickOrChannel, QString &strCategory, QString &strAvatarPath, QByteArray &bAvatar)
{
    if (strCategory == "nick")
    {
        saveAvatar(strAvatarPath, bAvatar);
        pTabC->setUserAvatarPath(strNickOrChannel, strAvatarPath);
    }
    else if (strCategory == "channel")
    {
        // return if channel not exist any more
        if (!Core::instance()->lOpenChannels.contains(strNickOrChannel)) return;

        Core::instance()->mChannelAvatar[strNickOrChannel] = bAvatar;

        emit setChannelAvatar(strNickOrChannel);
    }
}

void Avatar::saveAvatar(QString &strAvatarPath, QByteArray bAvatar)
{
    QString path;

#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    QString strCurrentProfile = Core::instance()->settings.value("current_profile");
    path += "/profiles/"+strCurrentProfile+"/avatars";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    strAvatarPath = path+"/"+strAvatarPath;

    QFile f(strAvatarPath);
    if (f.open(QIODevice::WriteOnly))
    {
        f.write(bAvatar);
        f.close();
    }
}
