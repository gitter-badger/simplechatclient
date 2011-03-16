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

#include <QDateTime>
#include <QDesktopWidget>
#include <QDomDocument>
#include <QFileDialog>
#include <QNetworkReply>
#include <QPushButton>
#include <QSettings>
#include <QUuid>
#include "network.h"
#include "dlg_my_avatar.h"

DlgMyAvatar::DlgMyAvatar(QWidget *parent, Network *param1, QMap <QString, QByteArray> *param2) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("My avatar"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    myparent = parent;
    pNetwork = param1;
    mNickAvatar = param2;
    bReadedCollectionNames = false;

    ui.pushButton_add_avatar->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_remove_avatar->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_set_empty_avatar1->setIcon(QIcon(":/images/oxygen/16x16/image-missing.png"));
    ui.pushButton_apply_avatar->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_empty_avatar2->setIcon(QIcon(":/images/oxygen/16x16/image-missing.png"));
    ui.pushButton_apply_collection_avatar->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.tabWidget->setTabText(0, tr("My avatars"));
    ui.tabWidget->setTabText(1, tr("Collection of avatars"));
    ui.pushButton_add_avatar->setText(tr("Add"));
    ui.pushButton_remove_avatar->setText(tr("Remove"));
    ui.pushButton_set_empty_avatar1->setText(tr("Apply empty"));
    ui.pushButton_apply_avatar->setText(tr("Apply avatar"));
    ui.pushButton_set_empty_avatar2->setText(tr("Apply empty"));
    ui.pushButton_apply_collection_avatar->setText(tr("Apply avatar"));

    // nick
    QSettings settings;
    QString strMe = settings.value("nick").toString();
    ui.label_my_nick->setText(strMe);

    // avatar
    refresh_avatar();

    // temporarily disabled
    ui.pushButton_add_avatar->setEnabled(false);

    QObject::connect(ui.pushButton_add_avatar, SIGNAL(clicked()), this, SLOT(button_add_avatar()));
    QObject::connect(ui.pushButton_apply_collection_avatar, SIGNAL(clicked()), this, SLOT(button_apply_collection_avatar()));
    QObject::connect(ui.pushButton_apply_avatar, SIGNAL(clicked()), this, SLOT(button_apply_avatar()));
    QObject::connect(ui.pushButton_remove_avatar, SIGNAL(clicked()), this, SLOT(button_remove_avatar()));
    QObject::connect(ui.pushButton_set_empty_avatar1, SIGNAL(clicked()), this, SLOT(button_set_empty_avatar()));
    QObject::connect(ui.pushButton_set_empty_avatar2, SIGNAL(clicked()), this, SLOT(button_set_empty_avatar()));
    QObject::connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tab_changed(int)));
    QObject::connect(ui.listWidget_list_collections, SIGNAL(currentTextChanged(QString)), this, SLOT(collection_changed(QString)));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    networkAccessManager = new QNetworkAccessManager;
    cookieJar = new QNetworkCookieJar();
    networkAccessManager->setCookieJar(cookieJar);
    QObject::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(network_finished(QNetworkReply*)));

    get_cookies();

    // load my avatars
    load_my_avatars();
}

DlgMyAvatar::~DlgMyAvatar()
{
    delete cookieJar;
    networkAccessManager->deleteLater();
}

void DlgMyAvatar::get_cookies()
{
    QList <QNetworkCookie> cookieList;
    QNetworkCookie cookie;
    QSettings settings;

    cookie.setName("onet_ubi");
    cookie.setValue(settings.value("onet_ubi").toByteArray());
    cookieList.append(cookie);

    cookie.setName("onet_cid");
    cookie.setValue(settings.value("onet_cid").toByteArray());
    cookieList.append(cookie);

    cookie.setName("onet_sid");
    cookie.setValue(settings.value("onet_sid").toByteArray());
    cookieList.append(cookie);

    cookie.setName("onet_uid");
    cookie.setValue(settings.value("onet_uid").toByteArray());
    cookieList.append(cookie);

    networkAccessManager->cookieJar()->setCookiesFromUrl(cookieList, QUrl("http://czat.onet.pl"));
}

void DlgMyAvatar::get_avatar(QString strUrl)
{
    QNetworkReply *pReply = networkAccessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", "get_avatar");
}

void DlgMyAvatar::got_avatar(QString strUrl, QByteArray bData)
{
    if (lMyAvatars.contains(strUrl))
        draw_my_avatar(strUrl, bData);
    else if (lCollections.contains(strUrl))
        draw_avatar_from_collect(strUrl, bData);
}

void DlgMyAvatar::refresh_avatar()
{
    QSettings settings;
    QString strMe = settings.value("nick").toString();

    if (mNickAvatar->contains(strMe) == true)
    {
        QByteArray bAvatar = mNickAvatar->value(strMe);
        QPixmap pixmap;
        pixmap.loadFromData(bAvatar);
        ui.label_my_avatar->setPixmap(pixmap);
    }
    else
    {
        ui.label_my_avatar->setText(tr("No photo available"));
    }
}

void DlgMyAvatar::load_my_avatars()
{
    // load avatars
    QString strUuid = QUuid::createUuid().toString();
    QString strContent = QString("fnc=loadFAvatars&rdr=xml&rid=%1").arg(strUuid);
    QString strUrl = "http://czat.onet.pl/_x/ludzie/avatars/api.php3";

    QNetworkReply *pReply = networkAccessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "load_my_avatars");
}

// <root><error><code>0</code><text><![CDATA[OK]]></text></error><data><count>1</count><images><angle>0</angle><crop><![CDATA[0-0-675-675]]></crop><height>677</height><img><![CDATA[9250d9265e492780cc1bb46e955ed21d]]></img><imgId>44231885</imgId><width>1016</width><desc><![CDATA[100_3893.jpg]]></desc><mApp>19</mApp><mSrv><![CDATA[http://foto1.m.onet.pl/_m/]]></mSrv></images></data><reqId><![CDATA[3722766d-737e-41e8-9c26-5fff7b0e5e3b]]></reqId></root>
void DlgMyAvatar::got_my_avatars(QString strResult)
{
    // clear
    lMyAvatars.clear();
    mMyAvatarsID.clear();
    ui.listWidget_my_avatars->clear();

    // set avatars
    QDomDocument doc;
    doc.setContent(strResult);
    QDomElement docElem = doc.documentElement();

    // error
    QDomNodeList error = docElem.elementsByTagName("error");
    QString code = error.at(0).firstChildElement("code").text();
    //QString text = error.at(0).firstChildElement("text").text();

    if (code != "0")
        return;

    // data
    QDomNodeList avatarsNodeList = docElem.elementsByTagName("data");

    for (int i = 0; i < avatarsNodeList.size(); i++)
    {
        QDomNodeList nodeList = avatarsNodeList.at(i).childNodes();

        for (int i = 0; i < nodeList.size(); i++)
        {
            QDomElement el = nodeList.at(i).toElement();
            QDomNode pEntries = el.firstChild();

            QString mSrv;
            QString img;
            QString zoom = "1";
            QString mApp;
            QString crop;
            QString angle;

            QString imgId;
            while (!pEntries.isNull())
            {
                QString tagName = pEntries.toElement().tagName();

                if (tagName == "mSrv")
                    mSrv = pEntries.toElement().text();
                else if (tagName == "img")
                    img = pEntries.toElement().text();
                else if (tagName == "mApp")
                    mApp = pEntries.toElement().text();
                else if (tagName == "crop")
                    crop = pEntries.toElement().text();
                else if (tagName == "angle")
                    angle = pEntries.toElement().text();
                else if (tagName == "imgId")
                    imgId = pEntries.toElement().text();

                pEntries = pEntries.nextSibling();
            }

            if ((mSrv.isEmpty() == false) && (img.isEmpty() == false) && (mApp.isEmpty() == false) && (crop.isEmpty() == false) && (angle.isEmpty() == false) && (imgId.isEmpty() == false))
            {
                QString strUrl = QString("%1%2,%3,%4,%5-%6.jpg").arg(mSrv).arg(img).arg(zoom).arg(mApp).arg(crop).arg(angle);
                lMyAvatars.append(strUrl);
                mMyAvatarsID.insert(strUrl, imgId);

                get_avatar(strUrl);
            }
        }
    }
}

void DlgMyAvatar::draw_my_avatar(QString strUrl, QByteArray bData)
{
    QString strID = mMyAvatarsID[strUrl];

    QPixmap pixmap;
    pixmap.loadFromData(bData);

    QListWidgetItem *item = new QListWidgetItem();
    item->setIcon(QIcon(pixmap));
    item->setData(Qt::UserRole, strUrl);
    item->setData(Qt::UserRole+1, strID);

    ui.listWidget_my_avatars->insertItem(ui.listWidget_my_avatars->count(), item);
}

void DlgMyAvatar::get_collections()
{
    // get collections
    QString strUuid = QUuid::createUuid().toString();
    QString strContent = QString("fnc=getCollections&rdr=xml&rid=%1").arg(strUuid);
    QString strUrl = "http://czat.onet.pl/_x/ludzie/avatars/api.php3";

    QNetworkReply *pReply = networkAccessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "get_collections");
}

// <root><error><code>1</code><text><![CDATA[Not logged]]></text></error><reqId><![CDATA[{d1f76dc3-d939-4389-aa5c-4bb428a62363}]]></reqId></root>
// <root><error><code>0</code><text><![CDATA[OK]]></text></error><data><id>5</id><name><![CDATA[Zestaw 1]]></name></data><data><id>6</id><name><![CDATA[Zestaw 2]]></name></data><count>2</count><reqId><![CDATA[{5ed3b996-6faa-4bcb-903d-d4a394235795}]]></reqId></root>
void DlgMyAvatar::got_collections(QString strResult)
{
    // clear
    lNameCollections.clear();
    mCollectionId.clear();
    ui.listWidget_list_collections->clear();

    // set collections
    QDomDocument doc;
    doc.setContent(strResult);
    QDomElement docElem = doc.documentElement();

    // error
    QDomNodeList error = docElem.elementsByTagName("error");
    QString code = error.at(0).firstChildElement("code").text();
    //QString text = error.at(0).firstChildElement("text").text();

    if (code != "0")
        return;

    // data
    QDomNodeList nodeList = docElem.elementsByTagName("data");

    for (int i = 0; i < nodeList.size(); i++)
    {
        QDomElement el = nodeList.at(i).toElement();
        QDomNode pEntries = el.firstChild();

        QString name;
        int id;
        while (!pEntries.isNull())
        {
            QDomElement eData = pEntries.toElement();
            QString tagName = eData.tagName();

            if (tagName == "name")
                name = eData.text();
            else if (tagName == "id")
                id = eData.text().toInt();

            pEntries = pEntries.nextSibling();
        }
        lNameCollections.append(name);
        mCollectionId.insert(name, id);
    }

    // draw
    draw_collections();
}

void DlgMyAvatar::draw_collections()
{
    for (int i = 0; i < lNameCollections.size(); i++)
        ui.listWidget_list_collections->insertItem(i, lNameCollections.at(i));
}

void DlgMyAvatar::get_avatars_from_collect(int index)
{
    // get avatars from collect
    QString strUuid = QUuid::createUuid().toString();
    QString strContent = QString("fnc=getAvatarsFromCollect&rdr=xml&rid=%1").arg(strUuid);
    strContent += QString("&envelope=a:1:{s:10:\"collectIds\";i:%1;}").arg(index);
    QString strUrl = "http://czat.onet.pl/_x/ludzie/avatars/api.php3";

    QNetworkReply *pReply = networkAccessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "get_avatars_from_collect");
}

// <root><error><code>-1</code><text><![CDATA[getAvatarsFromCollect: -2|Nie istnieje kolekcja o Id: 1]]></text></error><data><![CDATA[]]></data><reqId><![CDATA[{52bb1aab-c1b0-4e5a-9b06-e46285374251}]]></reqId></root>
// <root><error><code>0</code><text><![CDATA[OK]]></text></error><data><avatars><avatarId>110</avatarId><collectId>5</collectId><fileName><![CDATA[9f78f599c34c97336ae3a501fe29395e]]></fileName><avType>4</avType><server><![CDATA[http://foto2.m.onet.pl/_m/]]></server><img><![CDATA[9f78f599c34c97336ae3a501fe29395e]]></img><crop><![CDATA[]]></crop><angle>0</angle><bbox>0</bbox><mApp>27</mApp><ext><![CDATA[jpg]]></ext></avatars><avatarsCnt>27</avatarsCnt><collectId>5</collectId></data><reqId><![CDATA[eae58d2a-f090-4439-9f97-a109a5e2e6f4]]></reqId></root>
void DlgMyAvatar::got_avatars_from_collect(QString strResult)
{
    // clear
    lCollections.clear();
    ui.listWidget_collections->clear();

    // set avatars
    QDomDocument doc;
    doc.setContent(strResult);
    QDomElement docElem = doc.documentElement();

    // error
    QDomNodeList error = docElem.elementsByTagName("error");
    QString code = error.at(0).firstChildElement("code").text();
    //QString text = error.at(0).firstChildElement("text").text();

    if (code != "0")
        return;

    // data
    QDomNodeList avatarsNodeList = docElem.elementsByTagName("data");

    for (int i = 0; i < avatarsNodeList.size(); i++)
    {
        QDomNodeList nodeList = avatarsNodeList.at(i).childNodes();

        for (int i = 0; i < nodeList.size(); i++)
        {
            QDomElement el = nodeList.at(i).toElement();
            QDomNode pEntries = el.firstChild();

            QString server;
            QString img;
            QString ext;
            QString zoom = "1";
            QString mApp;
            QString angle;
            while (!pEntries.isNull())
            {
                QString tagName = pEntries.toElement().tagName();

                if (tagName == "server")
                    server = pEntries.toElement().text();
                else if (tagName == "img")
                    img = pEntries.toElement().text();
                else if (tagName == "ext")
                    ext = pEntries.toElement().text();
                else if (tagName == "mApp")
                    mApp = pEntries.toElement().text();
                else if (tagName == "angle")
                    angle = pEntries.toElement().text();

                pEntries = pEntries.nextSibling();
            }

            if ((server.isEmpty() == false) && (img.isEmpty() == false) && (ext.isEmpty() == false))
            {
                QString strUrl = QString("%1%2,%3,%4,%5.%6").arg(server).arg(img).arg(zoom).arg(mApp).arg(angle).arg(ext);
                lCollections.append(strUrl);

                get_avatar(strUrl);
            }
        }
    }
}

void DlgMyAvatar::draw_avatar_from_collect(QString strUrl, QByteArray bData)
{
    QPixmap pixmap;
    pixmap.loadFromData(bData);

    QListWidgetItem *item = new QListWidgetItem();
    item->setIcon(QIcon(pixmap));
    item->setData(Qt::UserRole, strUrl);

    ui.listWidget_collections->insertItem(ui.listWidget_collections->count(), item);
}

void DlgMyAvatar::tab_changed(int index)
{
    if (index == 1)
    {
        if (bReadedCollectionNames == false)
        {
            // get
            get_collections();

            bReadedCollectionNames = true;
        }
    }
}

void DlgMyAvatar::collection_changed(QString strName)
{
    if (mCollectionId.contains(strName) == true)
    {
        int index = mCollectionId[strName];

        // get collect
        get_avatars_from_collect(index);
    }
}

void DlgMyAvatar::button_add_avatar()
{
    QString strSelectedFilter;
    QString strFileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     "",
                                     tr("JPG Files (*.jpg)"),
                                     &strSelectedFilter,
                                     0);
    if (strFileName.isEmpty() == false)
    {
        QByteArray bFileContent;

        // read file
        QFile f(strFileName);
        if (!f.open(QIODevice::ReadOnly))
            return;
        bFileContent = f.readAll();
        f.close();

        // header
        qsrand(QDateTime::currentDateTime().toTime_t());
        QString strRand = QVariant(qrand()).toString()+QVariant(qrand()).toString()+QVariant(qrand()).toString();
        QString strBoundary = QString("---------------------------%1").arg(strRand);
        QString strEndBoundary= QString("\r\n--%1--\r\n").arg(strBoundary);
        QString strContentType = QString("multipart/form-data; boundary=%1").arg(strBoundary);

        // send
        QByteArray send;
        QString strRid = QUuid::createUuid().toString();
        strRid.remove("{"); strRid.remove("}");
        QString strUuid = QUuid::createUuid().toString();
        strUuid.remove("{"); strUuid.remove("}");
        QString strEnvelope = QString("a:1:{s:14:\"imageInputName\";s:%1:\"%2\";}").arg(strUuid.size()).arg(strUuid);

        send += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary).arg("fnc").arg("uploadImage");
        send += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary).arg("rdr").arg("xml");
        send += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary).arg("rid").arg(strRid);
        send += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"\r\n\r\n%3\r\n").arg(strBoundary).arg("envelope").arg(strEnvelope);
        send += QString("--%1\r\nContent-Disposition: form-data; name=\"%2\"; filename=\"%3\"\r\n").arg(strBoundary).arg(strUuid).arg(strFileName);
        send += QString("Content-Type: image/jpeg\r\n\r\n");
        send += bFileContent;
        send += strEndBoundary;

        // request
        QNetworkRequest request;
        QString strUrl = "http://czat.onet.pl/_x/ludzie/avatars/api.php3";
        request.setUrl(QUrl(strUrl));
        request.setHeader(QNetworkRequest::ContentTypeHeader, strContentType.toAscii());
        request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(send.size()).toString());

        // reply
        QNetworkReply *pReply = networkAccessManager->post(request, send);
        pReply->setProperty("category", "add_avatar");
    }
}

// a:2:{s:5:"error";a:2:{s:4:"code";i:-500;s:4:"text";s:10:"Bad params";}s:5:"reqId";i:0;}
// <root><data><mHash><![CDATA[7bec84066455a12e87e7bea1decc58a1]]></mHash><width>200</width><height>200</height><exif><![CDATA[]]></exif><fotoSrv><![CDATA[http://foto1.m.onet.pl/_m/]]></fotoSrv><mApp>19</mApp></data><reqId><![CDATA[37bebcec-da81-4a17-85aa-8b1205e4b0d9]]></reqId></root>
void DlgMyAvatar::got_add_avatar(QString strResult)
{
}

void DlgMyAvatar::button_apply_avatar()
{
    if (ui.listWidget_my_avatars->selectedItems().isEmpty() == false)
    {
        QString strLink = ui.listWidget_my_avatars->selectedItems().at(0)->data(Qt::UserRole).toString();

        if (strLink.isEmpty() == false)
        {
            pNetwork->send(QString("NS SET avatar %1").arg(strLink));
            QTimer::singleShot(1000*5, this, SLOT(refresh_avatar())); // 5 sec
        }
    }
}

void DlgMyAvatar::button_remove_avatar()
{
    if (ui.listWidget_my_avatars->selectedItems().isEmpty() == false)
    {
        QString strImgId = ui.listWidget_my_avatars->selectedItems().at(0)->data(Qt::UserRole+1).toString();

        if (strImgId.isEmpty() == false)
        {
            // delete avatar
            QString strUuid = QUuid::createUuid().toString();
            QString strContent = QString("fnc=deletePhoto&rdr=xml&rid=%1").arg(strUuid);
            strContent += QString("&envelope=a:1:{s:5:\"imgId\";i:%1;}").arg(strImgId);
            QString strUrl = "http://czat.onet.pl/_x/ludzie/avatars/api.php3";

            QNetworkReply *pReply = networkAccessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
            pReply->setProperty("category", "remove_avatar");
        }
    }
}

// <root><error><code>0</code><text><![CDATA[OK]]></text></error><data><![CDATA[]]></data><reqId><![CDATA[3dba2705-f507-43d7-ae75-a677457f027f]]></reqId></root>
void DlgMyAvatar::got_remove_avatar(QString strResult)
{
    // set result
    QDomDocument doc;
    doc.setContent(strResult);
    QDomElement docElem = doc.documentElement();

    // error
    QDomNodeList error = docElem.elementsByTagName("error");
    QString code = error.at(0).firstChildElement("code").text();
    //QString text = error.at(0).firstChildElement("text").text();

    if (code != "0")
        return;

    QTimer::singleShot(1000*5, this, SLOT(refresh_avatar())); // 5 sec
}

void DlgMyAvatar::button_set_empty_avatar()
{
    pNetwork->send(QString("NS SET avatar"));
}

void DlgMyAvatar::button_apply_collection_avatar()
{
    if (ui.listWidget_collections->selectedItems().isEmpty() == false)
    {
        QString strLink = ui.listWidget_collections->selectedItems().at(0)->data(Qt::UserRole).toString();

        if (strLink.isEmpty() == false)
        {
            pNetwork->send(QString("NS SET avatar %1").arg(strLink));
            QTimer::singleShot(1000*5, this, SLOT(refresh_avatar())); // 5 sec
        }
    }
}


void DlgMyAvatar::network_finished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QString strUrl = reply->url().toString();
    QString strCategory = reply->property("category").toString();
    QByteArray bData = reply->readAll();

    if (bData.isEmpty() == true)
        return;

    if (strCategory == "get_avatar")
        got_avatar(strUrl, bData);
    else if (strCategory == "load_my_avatars")
        got_my_avatars(QString(bData));
    else if (strCategory == "get_collections")
        got_collections(QString(bData));
    else if (strCategory == "get_avatars_from_collect")
        got_avatars_from_collect(QString(bData));
    else if (strCategory == "add_avatar")
        got_add_avatar(QString(bData));
    else if (strCategory == "remove_avatar")
        got_remove_avatar(QString(bData));
}

void DlgMyAvatar::button_close()
{
    this->close();
}
