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

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QObject>
#include <QStringList>
#include <QUrl>
#include <QUuid>

#include "core.h"
#include "my_avatar_model.h"

#include "avatar_client.h"

#define AVATAR_API "http://czat.onet.pl/_x/ludzie/avatars/api.php3"

AvatarClient::AvatarClient() : QNetworkAccessManager(), basicRequest(QUrl(AVATAR_API))
{
    // Connection to the normal replyFinished function
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    getCookies();
}

AvatarClient::~AvatarClient()
{
}

void AvatarClient::getCookies()
{
    /* TODO: replace with central cookies storage */

    QList<QNetworkCookie> cookies;
    QNetworkCookie cookie;

    QStringList names;
    names << "onet_ubi" << "onet_cid" << "onet_sid" << "onet_uid" << "onetzuo_ticket";
    foreach (QString name, names)
    {
        cookie.setName(name.toAscii());
        cookie.setValue(Core::instance()->settings.value(name).toAscii());
        cookies.append(cookie);
    }

    this->cookieJar()->setCookiesFromUrl(cookies, QUrl("http://czat.onet.pl"));
}

QString AvatarClient::createRid()
{
    QString uuidStr = QUuid::createUuid().toString();
    uuidStr.remove("{"); uuidStr.remove("}");
    return uuidStr;
}

void AvatarClient::requestGetCollections()
{
    QString postData = QString("fnc=getCollections&rdr=xml&rid=%1").arg(createRid());

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_getCollections);

    //qDebug() << request.url().toString() << postData.toAscii();

    this->post(request, postData.toAscii());
}

void AvatarClient::requestGetCollectionAvatars(int id)
{
    QString postData = QString("fnc=getAvatarsFromCollect&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:1:{s:10:\"collectIds\";i:%1;}").arg(id);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_getAvatarsFromCollect);

    //qDebug() << request.url().toString() << postData.toAscii();

    this->post(request, postData.toAscii());
}

void AvatarClient::requestGetMyAvatars()
{
    QString postData = QString("fnc=loadFAvatars&rdr=xml&rid=%1").arg(createRid());

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_loadFAvatars);

    //qDebug() << request.url().toString() << postData.toAscii();

    this->post(request, postData.toAscii());
}

void AvatarClient::requestSetAvatar(int imgId, int albumId)
{
    QString postData = QString("fnc=setAvatar&rdr=xml&rid=%1").arg(createRid());
    if (albumId == 0)
        postData += QString("&envelope=a:2:{s:7:\"avImgId\";i:%1;s:9:\"avAlbumId\";N;}")
            .arg(imgId);
    else
        postData += QString("&envelope=a:2:{s:7:\"avImgId\";i:%1;s:9:\"avAlbumId\";i;%2}")
            .arg(imgId, albumId);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_setAvatar);

    //qDebug() << request.url().toString() << postData.toAscii();

    this->post(request, postData.toAscii());
}

void AvatarClient::requestUploadImage(const QString &fileName, const QByteArray &data)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    QByteArray bBoundary = QByteArray("boundary_.oOo._")
               + QByteArray::number(qrand()).toBase64()
               + QByteArray::number(qrand()).toBase64()
               + QByteArray::number(qrand()).toBase64();

    // boundary must not be longer than 70 characters, see RFC 2046, section 5.1.1
    if (bBoundary.count() > 70)
        bBoundary = bBoundary.left(70);

    QString strBoundary = QString(bBoundary);

    QByteArray bBody;
    QString strFileFormName = createRid();
    QString strEnvelope = QString("a:1:{s:14:\"imageInputName\";s:%1:\"%2\";}").arg(strFileFormName.size()).arg(strFileFormName);

    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "fnc", "uploadImage");
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "rdr", "xml");
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "rid", createRid());
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "envelope", strEnvelope);
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"; filename=\"%3\"\r\n").arg(strBoundary, strFileFormName, fileName);
    bBody += QString("Content-Type: image/jpeg\r\n\r\n");
    bBody += data;
    bBody += QString("\r\n");
    bBody += QString("--%1--\r\n").arg(strBoundary);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("multipart/form-data; boundary=%1").arg(strBoundary));
    request.setHeader(QNetworkRequest::ContentLengthHeader, bBody.size());
    request.setAttribute(QNetworkRequest::User, RT_uploadImage);

    qDebug() << request.url().toString() << fileName << strBoundary << bBody.size();

    QNetworkReply *pReply = this->post(request, bBody);
    QFileInfo info = QFile(fileName);
    pReply->setProperty("fileName", info.fileName());
}

void AvatarClient::requestUpdatePhoto(const MyAvatarModel &avatar)
{
    QString postData = QString("fnc=updatePhoto&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:7:{s:5:\"imgId\";i:%1;s:5:\"mHash\";s:%2:\"%3\";s:5:\"angle\";i:%4;s:4:\"crop\";s:%5:\"%6\";s:4:\"desc\";s:%7:\"%8\";s:5:\"width\";i:%9;s:6:\"height\";i:%10;}")
        .arg(avatar.imgId())
        .arg(avatar.img().size())
        .arg(avatar.img())
        .arg(avatar.angle())
        .arg(avatar.crop().size())
        .arg(avatar.crop())
        .arg(avatar.desc().size())
        .arg(QString(avatar.desc()))
        .arg(avatar.width())
        .arg(avatar.height());

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_updatePhoto);

    //qDebug() << request.url().toString() << postData.toAscii();

    this->post(request, postData.toAscii());
}

void AvatarClient::requestAddPhoto(const MyAvatarModel &avatar)
{
    QString strImgId = createRid();
    QString postData = QString("fnc=addPhoto&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:7:{s:5:\"imgId\";s:%1:\"%2\";s:5:\"mHash\";s:%3:\"%4\";s:5:\"angle\";i:%5;s:4:\"crop\";s:%6:\"%7\";s:4:\"desc\";s:%8:\"%9\";s:5:\"width\";i:%10;s:6:\"height\";i:%11;}")
        .arg(strImgId.size())
        .arg(strImgId)
        .arg(avatar.img().size())
        .arg(avatar.img())
        .arg(avatar.angle())
        .arg(avatar.crop().size())
        .arg(avatar.crop())
        .arg(avatar.desc().size())
        .arg(QString(avatar.desc()))
        .arg(avatar.width())
        .arg(avatar.height());

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_addPhoto);

    //qDebug() << request.url().toString() << postData.toAscii();

    this->post(request, postData.toAscii());
}

void AvatarClient::requestDeletePhoto(int imgId)
{
    QString postData = QString("fnc=deletePhoto&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:1:{s:5:\"imgId\";i:%1;}").arg(imgId);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_deletePhoto);

    //qDebug() << request.url().toString() << postData.toAscii();

    this->post(request, postData.toAscii());
}

void AvatarClient::requestGetAvatar(const QString &avatarUrl, AvatarType type)
{
    QNetworkRequest request(basicRequest);
    request.setUrl(QUrl(avatarUrl));
    switch (type)
    {
        case AT_my:
            request.setAttribute(QNetworkRequest::User,RT_getMyAvatar);
            break;
        case AT_myRaw:
            request.setAttribute(QNetworkRequest::User,RT_getMyRawAvatar);
            break;
        case AT_collection:
            request.setAttribute(QNetworkRequest::User,RT_getCollectionAvatar);
            break;
        case AT_other:
            request.setAttribute(QNetworkRequest::User,RT_getAvatar);
            break;
    }

    //qDebug() << request.url().toString();

    this->get(request);
}

void AvatarClient::replyFinished(QNetworkReply *reply)
{
    QNetworkRequest request = reply->request();
    RequestType requestType = (RequestType) request.attribute(QNetworkRequest::User,RT_undefined).toInt();

    if (reply->error() != 0)
    {
        qDebug() << "An error occured during the Avatar request (" << reply->error() << " :: Request: " << requestType << ")" ;
        emit errorOccured(reply->error());
    }
    else
    {
        switch (requestType)
        {
        case RT_getCollections:
            emit getCollectionsReady(reply->readAll());
            break;
        case RT_getAvatarsFromCollect:
            emit getCollectionAvatarsReady(reply->readAll());
            break;
        case RT_loadFAvatars:
            emit getMyAvatarsReady(reply->readAll());
            break;
        case RT_setAvatar:
            emit setAvatarReady(reply->readAll());
            break;
        case RT_uploadImage:
            emit uploadImageReady(reply->readAll(), reply->property("fileName").toString());
            break;
        case RT_updatePhoto:
            emit updatePhotoReady(reply->readAll());
            break;
        case RT_addPhoto:
            emit addPhotoReady(reply->readAll());
            break;
        case RT_deletePhoto:
            emit deletePhotoReady(reply->readAll());
            break;
        case RT_getMyAvatar:
            emit getAvatarReady(reply->readAll(), reply->url().toString(), AT_my);
            break;
        case RT_getMyRawAvatar:
            emit getAvatarReady(reply->readAll(), reply->url().toString(), AT_myRaw);
            break;
        case RT_getCollectionAvatar:
            emit getAvatarReady(reply->readAll(), reply->url().toString(), AT_collection);
            break;
        case RT_getAvatar:
            emit getAvatarReady(reply->readAll(), reply->url().toString(), AT_other);
            break;
        case RT_undefined:
            //qDebug() << "RequestType: RT_undefined";
            break;
        }
    }

    reply->deleteLater();
}

