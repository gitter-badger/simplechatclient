/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
 *   Copyright (C) 2012 Dariusz Markowicz <dmarkowicz@alari.pl>             *
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

#ifndef AVATAR_CLIENT_H
#define AVATAR_CLIENT_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class QNetworkCookieJar;
class MyAvatarModel;

class AvatarClient : public QNetworkAccessManager
{
    Q_OBJECT
public:
    enum AvatarType
    {
        AT_my,
        AT_myRaw,
        AT_collection,
        AT_other
    };

    AvatarClient();
    virtual ~AvatarClient();

    void requestGetCollections();
    void requestGetCollectionAvatars(int);
    void requestGetMyAvatars();
    void requestSetAvatar(int, int);
    void requestUploadImage(const QString &, const QByteArray &);
    void requestUpdatePhoto(const MyAvatarModel &);
    void requestAddPhoto(const MyAvatarModel &);
    void requestDeletePhoto(const QString &);
    void requestGetAvatar(const QString &, AvatarType);

signals:
    void getCollectionsReady(const QString &);
    void getCollectionAvatarsReady(const QString &);
    void getMyAvatarsReady(const QString &);
    void setAvatarReady(const QString &);
    void uploadImageReady(const QString &, const QString &);
    void updatePhotoReady(const QString &);
    void addPhotoReady(const QString &);
    void deletePhotoReady(const QString &);
    void getAvatarReady(const QString &, const QByteArray &, AvatarClient::AvatarType);
    void errorOccured(QNetworkReply::NetworkError);

protected slots:
    void replyFinished(QNetworkReply *);

private:
    // Names here intentionally similar to Onet Avatar API
    enum RequestType
    {
        RT_getCollections,
        RT_getAvatarsFromCollect,
        RT_loadFAvatars,
        RT_setAvatar,
        RT_uploadImage,
        RT_updatePhoto,
        RT_addPhoto,
        RT_deletePhoto,

        RT_getMyAvatar,
        RT_getMyRawAvatar,
        RT_getCollectionAvatar,
        RT_getAvatar,

        RT_undefined
    };

    void getCookies();
    QString createRid();

    QNetworkRequest basicRequest;
    QNetworkCookieJar *pCookies;

};

#endif /* AVATAR_CLIENT_H */
