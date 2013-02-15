/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "avatar.h"
#include "channel.h"
#include "channel_favourites_model.h"
#include "channel_homes_model.h"
#include "nick.h"
#include "settings.h"

#ifdef Q_WS_WIN
    #include <QCoreApplication>
    #include <QDesktopServices>
#else
    #include "scc-config.h"
#endif

Avatar * Avatar::Instance = 0;

Avatar * Avatar::instance()
{
    if (!Instance)
    {
        Instance = new Avatar();
    }

    return Instance;
}

Avatar::Avatar()
{
    accessManager = new QNetworkAccessManager;
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpFinished(QNetworkReply*)));
}

Avatar::~Avatar()
{
    accessManager->deleteLater();
}

void Avatar::get(const QString &strNickOrChannel, const QString &strCategory, const QString &strUrl)
{
    QFileInfo fi(strUrl);
    QString strAvatarFile = fi.fileName();
    QString strAvatarPath = getAvatarPath(strAvatarFile);

    if (!QFile::exists(strAvatarPath))
    {
        QNetworkReply *reply = accessManager->get(QNetworkRequest(strUrl));
        reply->setProperty("nickorchannel", strNickOrChannel);
        reply->setProperty("category", strCategory);
        reply->setProperty("file", strAvatarFile);
        reply->setProperty("path", strAvatarPath);
    }
    else
    {
        updateAvatar(strCategory, strNickOrChannel, strAvatarFile);
    }
}

void Avatar::httpFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QByteArray bAvatar = reply->readAll();
    if (!bAvatar.isEmpty())
    {
        QString strNickOrChannel = reply->property("nickorchannel").toString();
        QString strCategory = reply->property("category").toString();
        QString strAvatarFile = reply->property("file").toString();
        QString strAvatarPath = reply->property("path").toString();

        saveAvatar(strAvatarPath, bAvatar);
        updateAvatar(strCategory, strNickOrChannel, strAvatarFile);
    }
}

void Avatar::updateAvatar(const QString &strCategory, const QString &strNickOrChannel, const QString &strAvatarFile)
{
    if (strCategory == "nick")
    {
        Nick::instance()->setAvatar(strNickOrChannel, strAvatarFile);
    }
    else if (strCategory == "channel")
    {
        Channel::instance()->setAvatar(strNickOrChannel, strAvatarFile);
        ChannelHomes::instance()->setAvatar(strNickOrChannel, strAvatarFile);
        ChannelFavouritesModel::instance()->setAvatar(strNickOrChannel, strAvatarFile);
    }
}

void Avatar::saveAvatar(const QString &strAvatarPath, const QByteArray &bAvatar)
{
    QFile f(strAvatarPath);
    if (f.open(QIODevice::WriteOnly))
    {
        f.write(bAvatar);
        f.close();
    }
}

QString Avatar::getAvatarPath(const QString &strAvatar)
{
    QString strCurrentProfile = Settings::instance()->get("current_profile");
    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc/";
#else
    path = QDir::homePath()+"/.scc/";
#endif

    path += "profiles/"+strCurrentProfile+"/avatars/";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    return path+strAvatar;
}

QString Avatar::getEmptyUserAvatar()
{
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = SCC_DATA_DIR;
#endif

    return path+"/images/user_avatar.png";
}
