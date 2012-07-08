/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include <QStringList>
#include "avatar.h"
#include "core.h"
#include "nicklist.h"
#include "tab_container.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

Avatar::Avatar(TabContainer *_pTabC) : pTabC(_pTabC)
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
    QNetworkReply *reply = accessManager->get(QNetworkRequest(strUrl));
    reply->setProperty("nickorchannel", strNickOrChannel);
    reply->setProperty("category", strCategory);
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
        QFileInfo fi(reply->url().toString());
        QString strAvatarPath = getAvatarPath(fi.fileName());

        saveAvatar(strAvatarPath, bAvatar);

        if (strCategory == "nick")
        {
            Nicklist::instance()->setUserAvatarPath(strNickOrChannel, strAvatarPath);
        }
        else if (strCategory == "channel")
        {
            Core::instance()->mChannelAvatar[strNickOrChannel] = strAvatarPath;

            pTabC->setChannelAvatar(strNickOrChannel);
        }
    }
}

QString Avatar::getAvatarPath(const QString &strAvatarPath)
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

    return path+"/"+strAvatarPath;
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
