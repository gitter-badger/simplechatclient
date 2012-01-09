/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
class QObject;

enum AvatarType
{
    AT_my,
    AT_collection,
    AT_other
};

struct MyAvatarMeta
{
    int angle;
    QString crop;
    int height;
    QString img;
    int imgId;
    int width;
    QString desc;
    int mApp;
    QString mSrv;
    //QString exif; // unused; returned after upload photo
    MyAvatarMeta() : angle(-1), crop(), height(-1), img(),
        imgId(-1), width(-1), desc(), mApp(-1), mSrv() {}
};

class AvatarClient : public QNetworkAccessManager
{
    Q_OBJECT

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
        RT_getCollectionAvatar,
        RT_getAvatar,

        RT_undefined,
    };

    void getCookies();
    QString createRid();

    QNetworkRequest basicRequest;
    QNetworkCookieJar *pCookies;

protected slots:
    void replyFinished(QNetworkReply *pReply);

signals:
    void getCollectionsReady(QString strResult);
    void getCollectionAvatarsReady(QString strResult);
    void getMyAvatarsReady(QString strResult);
    void setAvatarReady(QString strResult);
    void uploadImageReady(QString strResult, QString strFileName);
    void updatePhotoReady(QString strResult);
    void addPhotoReady(QString strResult);
    void deletePhotoReady(QString strResult);
    void getAvatarReady(QString strUrl, QByteArray bData, AvatarType type);
    void errorOccured(QNetworkReply::NetworkError error);

public:
    AvatarClient();
    virtual ~AvatarClient();

    void requestGetCollections();
    void requestGetCollectionAvatars(int iId);
    void requestGetMyAvatars();
    void requestSetAvatar(int iImgId, int iAlbumId);
    void requestUploadImage(QString strFileName, QByteArray bData);
    void requestUpdatePhoto(MyAvatarMeta avatar);
    void requestAddPhoto(MyAvatarMeta avatar);
    void requestDeletePhoto(QString strImgId);
    void requestGetAvatar(QString strUrl, AvatarType type);
};

#endif /* AVATAR_CLIENT_H */
