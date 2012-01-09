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

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QObject>
#include <QUrl>
#include <QUuid>

#include "avatar_client.h"
#include "core.h"

#define AVATAR_API "http://czat.onet.pl/_x/ludzie/avatars/api.php3"

AvatarClient::AvatarClient() : QNetworkAccessManager(), basicRequest(QUrl(AVATAR_API))
{
    // Creation of the cookies manager
    pCookies = new QNetworkCookieJar(this);

    this->setCookieJar(pCookies);

    // Connection to the normal replyFinished function
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    getCookies();
}

AvatarClient::~AvatarClient()
{
    pCookies->deleteLater();
}

void AvatarClient::getCookies()
{
    /* TODO: replace with central cookies storage */

    QList<QNetworkCookie> cookieList;
    QNetworkCookie cookie;

    QList<QString> lNames;
    lNames << "onet_ubi" << "onet_cid" << "onet_sid" << "onet_uid" << "onetzuo_ticket";
    foreach (QString strName, lNames)
    {
        cookie.setName(strName.toAscii());
        cookie.setValue(Core::instance()->settings.value(strName).toAscii());
        cookieList.append(cookie);
    }

    this->cookieJar()->setCookiesFromUrl(cookieList, QUrl("http://czat.onet.pl"));
}

QString AvatarClient::createRid()
{
    QString strUuid = QUuid::createUuid().toString();
    strUuid.remove("{"); strUuid.remove("}");
    return strUuid;
}

void AvatarClient::requestGetCollections()
{
    QString postData = QString("fnc=getCollections&rdr=xml&rid=%1").arg(createRid());

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_getCollections);

    //qDebug() << request.url().toString() << postData.toAscii();

    QNetworkReply *pReply = this->post(request, postData.toAscii());
}

void AvatarClient::requestGetCollectionAvatars(int iId)
{
    QString postData = QString("fnc=getAvatarsFromCollect&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:1:{s:10:\"collectIds\";i:%1;}").arg(iId);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_getAvatarsFromCollect);

    //qDebug() << request.url().toString() << postData.toAscii();

    QNetworkReply *pReply = this->post(request, postData.toAscii());
}

void AvatarClient::requestGetMyAvatars()
{
    QString postData = QString("fnc=loadFAvatars&rdr=xml&rid=%1").arg(createRid());

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_loadFAvatars);

    //qDebug() << request.url().toString() << postData.toAscii();

    QNetworkReply *pReply = this->post(request, postData.toAscii());
}

void AvatarClient::requestSetAvatar(int iImgId, int iAlbumId)
{
    QString postData = QString("fnc=setAvatar&rdr=xml&rid=%1").arg(createRid());
    if (iAlbumId == 0)
        postData += QString("&envelope=a:2:{s:7:\"avImgId\";i:%1;s:9:\"avAlbumId\";N;}")
            .arg(iImgId);
    else
        postData += QString("&envelope=a:2:{s:7:\"avImgId\";i:%1;s:9:\"avAlbumId\";i;%2}")
            .arg(iImgId, iAlbumId);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_setAvatar);

    //qDebug() << request.url().toString() << postData.toAscii();

    QNetworkReply *pReply = this->post(request, postData.toAscii());
}

void AvatarClient::requestUploadImage(QString strFileName, QByteArray bData)
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
    QString strEnvelope = QString("a:1:{s:14:\"imageInputName\";s:%1:\"%2\";}").arg(QString::number(strFileFormName.size()), strFileFormName);

    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "fnc", "uploadImage");
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "rdr", "xml");
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "rid", createRid());
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary, "envelope", strEnvelope);
    bBody += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"; filename=\"%3\"\r\n").arg(strBoundary, strFileFormName, strFileName);
    bBody += QString("Content-Type: image/jpeg\r\n\r\n");
    bBody += bData;
    bBody += QString("\r\n");
    bBody += QString("--%1--\r\n").arg(strBoundary);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("multipart/form-data; boundary=%1").arg(strBoundary));
    request.setHeader(QNetworkRequest::ContentLengthHeader, bBody.size());
    request.setAttribute(QNetworkRequest::User, RT_uploadImage);

    //qDebug() << request.url().toString() << strFileName << strBoundary << bBody.size();

    QNetworkReply *pReply = this->post(request, bBody);
    QFileInfo info = QFile(strFileName);
    pReply->setProperty("fileName", info.fileName());
}

void AvatarClient::requestUpdatePhoto(MyAvatarMeta avatar)
{
    QString postData = QString("fnc=updatePhoto&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:7:{s:5:\"imgId\";i:%1;s:5:\"mHash\";s:%2:\"%3\";s:5:\"angle\";i:%4;s:4:\"crop\";s:%5:\"%6\";s:4:\"desc\";s:%7:\"%8\";s:5:\"width\";i:%9;s:6:\"height\";i:%10;}")
        .arg(avatar.imgId)
        .arg(avatar.img.size())
        .arg(avatar.img)
        .arg(avatar.angle)
        .arg(avatar.crop.size())
        .arg(avatar.crop)
        .arg(avatar.desc.size())
        .arg(QString(avatar.desc))
        .arg(avatar.width)
        .arg(avatar.height);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_updatePhoto);

    //qDebug() << request.url().toString() << postData.toAscii();

    QNetworkReply *pReply = this->post(request, postData.toAscii());
}

void AvatarClient::requestAddPhoto(MyAvatarMeta avatar)
{
    QString strImgId = createRid();
    QString postData = QString("fnc=addPhoto&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:7:{s:5:\"imgId\";s:%1:\"%2\";s:5:\"mHash\";s:%3:\"%4\";s:5:\"angle\";i:%5;s:4:\"crop\";s:%6:\"%7\";s:4:\"desc\";s:%8:\"%9\";s:5:\"width\";i:%10;s:6:\"height\";i:%11;}")
        .arg(strImgId.size())
        .arg(strImgId)
        .arg(avatar.img.size())
        .arg(avatar.img)
        .arg(avatar.angle)
        .arg(avatar.crop.size())
        .arg(avatar.crop)
        .arg(avatar.desc.size())
        .arg(QString(avatar.desc))
        .arg(avatar.width)
        .arg(avatar.height);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_addPhoto);

    //qDebug() << request.url().toString() << postData.toAscii();

    QNetworkReply *pReply = this->post(request, postData.toAscii());
}

void AvatarClient::requestDeletePhoto(QString strImgId)
{
    QString postData = QString("fnc=deletePhoto&rdr=xml&rid=%1").arg(createRid());
    postData += QString("&envelope=a:1:{s:5:\"imgId\";i:%1;}").arg(strImgId);

    QNetworkRequest request(basicRequest);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, RT_deletePhoto);

    //qDebug() << request.url().toString() << postData.toAscii();

    QNetworkReply *pReply = this->post(request, postData.toAscii());
}

void AvatarClient::requestGetAvatar(QString strUrl, AvatarType type)
{
    QNetworkRequest request(basicRequest);
    request.setUrl(QUrl(strUrl));
    switch (type)
    {
        case AT_my:
            request.setAttribute(QNetworkRequest::User,RT_getMyAvatar);
            break;
        case AT_collection:
            request.setAttribute(QNetworkRequest::User,RT_getCollectionAvatar);
            break;
        case AT_other:
            request.setAttribute(QNetworkRequest::User,RT_getAvatar);
            break;
    }

    //qDebug() << request.url().toString();

    QNetworkReply *pReply = this->get(request);
}

void AvatarClient::replyFinished(QNetworkReply *pReply)
{
    QNetworkRequest request = pReply->request();
    RequestType requestType = (RequestType) request.attribute(QNetworkRequest::User,RT_undefined).toInt();

    if (pReply->error() != 0)
    {
        qDebug() << "An error occured during the Avatar request (" << pReply->error() << " :: Request: " << requestType << ")" ;
        emit errorOccured(pReply->error());
    }
    else
    {
        QByteArray bData = pReply->readAll();
        QString strUrl;
        QString strFileName;

        switch (requestType)
        {
        case RT_getCollections:
            emit getCollectionsReady(QString(bData));
            break;
        case RT_getAvatarsFromCollect:
            emit getCollectionAvatarsReady(QString(bData));
            break;
        case RT_loadFAvatars:
            emit getMyAvatarsReady(QString(bData));
            break;
        case RT_setAvatar:
            emit setAvatarReady(QString(bData));
            break;
        case RT_uploadImage:
            strFileName = pReply->property("fileName").toString();
            emit uploadImageReady(QString(bData), strFileName);
            break;
        case RT_updatePhoto:
            emit updatePhotoReady(QString(bData));
            break;
        case RT_addPhoto:
            emit addPhotoReady(QString(bData));
            break;
        case RT_deletePhoto:
            emit deletePhotoReady(QString(bData));
            break;
        case RT_getMyAvatar:
            strUrl = pReply->url().toString();
            emit getAvatarReady(strUrl, bData, AT_my);
            break;
        case RT_getCollectionAvatar:
            strUrl = pReply->url().toString();
            emit getAvatarReady(strUrl, bData, AT_collection);
            break;
        case RT_getAvatar:
            strUrl = pReply->url().toString();
            emit getAvatarReady(strUrl, bData, AT_other);
            break;
        case RT_undefined:
            //qDebug() << "RequestType: RT_undefined";
            break;
        }
    }

    pReply->deleteLater();
}

