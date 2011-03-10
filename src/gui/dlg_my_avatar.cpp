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

#include <QDesktopWidget>
#include <QDomDocument>
#include <QEventLoop>
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
    bReadedCollections = false;

    ui.pushButton_apply_collection_avatar->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.tabWidget->setTabText(0, tr("My avatars"));
    ui.tabWidget->setTabText(1, tr("Collection of avatars"));
    ui.pushButton_apply_collection_avatar->setText(tr("Apply avatar"));

    // nick
    QSettings settings;
    QString strMe = settings.value("nick").toString();
    ui.label_my_nick->setText(strMe);

    // avatar
    refresh_avatar();

    QObject::connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tab_changed(int)));
    QObject::connect(ui.listWidget_list_collections, SIGNAL(currentTextChanged(QString)), this, SLOT(collection_changed(QString)));
    QObject::connect(ui.pushButton_apply_collection_avatar, SIGNAL(clicked()), this, SLOT(button_apply_collection_avatar()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    cookieJar = new QNetworkCookieJar();
    accessManager = new QNetworkAccessManager;
    accessManager->setCookieJar(cookieJar);

    get_cookies();
}

DlgMyAvatar::~DlgMyAvatar()
{
    delete cookieJar;
    accessManager->deleteLater();
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

    accessManager->cookieJar()->setCookiesFromUrl(cookieList, QUrl("http://czat.onet.pl"));
}

QString DlgMyAvatar::network_request(QString strLink, QString strContent)
{
    QEventLoop eventLoop;
    QNetworkReply *pReply;

    if (strContent.isEmpty())
        pReply = accessManager->get(QNetworkRequest(QUrl(strLink)));
    else
        pReply = accessManager->post(QNetworkRequest(QUrl(strLink)), strContent.toAscii());

    pReply->ignoreSslErrors();
    QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    pReply->deleteLater();

    if (pReply->error())
        return QString::null;

    QString strData = pReply->readAll();
    QString strRedir = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

    if (strRedir.isEmpty() == false)
        network_request(strRedir, QString::null);

    return strData;
}

QByteArray DlgMyAvatar::get_avatar(QString strUrl)
{
    QEventLoop eventLoop;
    QNetworkAccessManager *accessManager = new QNetworkAccessManager;
    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    accessManager->deleteLater();
    pReply->deleteLater();

    if (pReply->error())
        return QByteArray();

    QByteArray bData = pReply->readAll();

    if (bData.isEmpty() == false)
        return bData;

    return QByteArray();
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

// <root><error><code>1</code><text><![CDATA[Not logged]]></text></error><reqId><![CDATA[{d1f76dc3-d939-4389-aa5c-4bb428a62363}]]></reqId></root>
// <root><error><code>0</code><text><![CDATA[OK]]></text></error><data><id>5</id><name><![CDATA[Zestaw 1]]></name></data><data><id>6</id><name><![CDATA[Zestaw 2]]></name></data><count>2</count><reqId><![CDATA[{5ed3b996-6faa-4bcb-903d-d4a394235795}]]></reqId></root>
void DlgMyAvatar::get_collections()
{
    // get collections
    QString strUuid = QUuid::createUuid().toString();
    QString strContent = QString("fnc=getCollections&rdr=xml&rid=%1").arg(strUuid);
    QString strCollections = network_request("http://czat.onet.pl/_x/ludzie/avatars/api.php3", strContent);

    // clear
    mCollections.clear();
    ui.listWidget_list_collections->clear();

    // if empty
    if (strCollections.isEmpty() == true)
        return;

    // set collections
    QDomDocument doc;
    doc.setContent(strCollections);
    QDomElement docElem = doc.documentElement();

    // error
    QDomNodeList error = docElem.elementsByTagName("error");
    QString code = error.at(0).firstChildElement("code").text();
    //QString text = error.at(0).firstChildElement("text").text();

    if (code != "0")
        return;

    // data
    QDomNodeList nodeList = docElem.elementsByTagName("data");

    for (int i = 0; i < nodeList.count(); i++)
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
        mCollections.insert(name, id);
    }
}

void DlgMyAvatar::draw_collections()
{
    int i = 0;
    QMapIterator<QString, int> iter(mCollections);
    while (iter.hasNext())
    {
        iter.next();

        ui.listWidget_list_collections->insertItem(i, iter.key());

        i++;
    }
}

// "<root><error><code>-1</code><text><![CDATA[getAvatarsFromCollect: -2|Nie istnieje kolekcja o Id: 1]]></text></error><data><![CDATA[]]></data><reqId><![CDATA[{52bb1aab-c1b0-4e5a-9b06-e46285374251}]]></reqId></root>
// <root><error><code>0</code><text><![CDATA[OK]]></text></error><data><avatars><avatarId>110</avatarId><collectId>5</collectId><fileName><![CDATA[9f78f599c34c97336ae3a501fe29395e]]></fileName><avType>4</avType><server><![CDATA[http://foto2.m.onet.pl/_m/]]></server><img><![CDATA[9f78f599c34c97336ae3a501fe29395e]]></img><crop><![CDATA[]]></crop><angle>0</angle><bbox>0</bbox><mApp>27</mApp><ext><![CDATA[jpg]]></ext></avatars><avatarsCnt>27</avatarsCnt><collectId>5</collectId></data><reqId><![CDATA[eae58d2a-f090-4439-9f97-a109a5e2e6f4]]></reqId></root>
void DlgMyAvatar::get_avatars_from_collect(int index)
{
    // get avatars from collect
    QString strUuid = QUuid::createUuid().toString();
    QString strContent = QString("fnc=getAvatarsFromCollect&rdr=xml&rid=%1").arg(strUuid);
    strContent += QString("&envelope=a:1:{s:10:\"collectIds\";i:%1;}").arg(index);
    QString strAvatarsFromCollect = network_request("http://czat.onet.pl/_x/ludzie/avatars/api.php3", strContent);

    // if empty
    if (strAvatarsFromCollect.isEmpty() == true)
        return;

    // set avatars
    QDomDocument doc;
    doc.setContent(strAvatarsFromCollect);
    QDomElement docElem = doc.documentElement();

    // error
    QDomNodeList error = docElem.elementsByTagName("error");
    QString code = error.at(0).firstChildElement("code").text();
    //QString text = error.at(0).firstChildElement("text").text();

    if (code != "0")
        return;

    // avatar list
    QList<QString> lAvatars;

    // data
    QDomNodeList avatarsNodeList = docElem.elementsByTagName("data");

    for (int i = 0; i < avatarsNodeList.count(); i++)
    {
        QDomNodeList nodeList = avatarsNodeList.at(i).childNodes();

        for (int i = 0; i < nodeList.count(); i++)
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
                lAvatars.append(QString("%1%2,%3,%4,%5.%6").arg(server).arg(img).arg(zoom).arg(mApp).arg(angle).arg(ext));
        }
    }

    // set avatars
    QMap<QString, QByteArray> mbAvatars;
    for (int i = 0; i < lAvatars.count(); i++)
    {
        QByteArray bAvatar = get_avatar(lAvatars.at(i));

        if (bAvatar.isEmpty() == false)
            mbAvatars.insert(lAvatars.at(i), bAvatar);
    }

    // set collection
    mCollectionAvatars.insert(index, mbAvatars);
}

void DlgMyAvatar::draw_avatars_from_collect(int index)
{
    // refresh avatars
    QMap <QString, QByteArray> mbAvatars = mCollectionAvatars.value(index);

    int i = 0;
    QMapIterator<QString, QByteArray> iter(mbAvatars);
    while (iter.hasNext())
    {
        iter.next();

        QString strLink = iter.key();
        QByteArray bAvatar = iter.value();

        QPixmap pixmap;
        pixmap.loadFromData(bAvatar);

        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon(pixmap));
        item->setData(Qt::UserRole, strLink);

        ui.listWidget_collections->insertItem(i, item);

        i++;
    }
}

void DlgMyAvatar::tab_changed(int index)
{
    if (index == 1)
    {
        if (bReadedCollections == false)
        {
            // get
            get_collections();

            // draw
            draw_collections();

            bReadedCollections = true;
        }
    }
}

void DlgMyAvatar::collection_changed(QString strName)
{
    // clear
    ui.listWidget_collections->clear();

    int index = -1;

    // get index
    for (int i = 0; i < ui.listWidget_list_collections->count(); i++)
    {
        QString strItemName = ui.listWidget_list_collections->item(i)->text();

        if (strItemName == strName)
        {
            QList<int> strlValues = mCollections.values();
            int iItemID = strlValues.at(i);

            index = iItemID;
            break;
        }
    }

    // wrong id
    if (index == -1)
        return;

    // read collect
    if (mCollectionAvatars.contains(index) == false)
        get_avatars_from_collect(index);

    // draw
    if (mCollectionAvatars.contains(index) == true)
        draw_avatars_from_collect(index);
}

void DlgMyAvatar::button_apply_collection_avatar()
{
    if (ui.listWidget_collections->selectedItems().isEmpty() == false)
    {
        QString strLink = ui.listWidget_collections->selectedItems().at(0)->data(Qt::UserRole).toString();

        if (strLink.isEmpty() == false)
        {
            pNetwork->send(QString("NS SET avatar %1").arg(strLink));
            QTimer::singleShot(1000*2, this, SLOT(refresh_avatar())); // 2 sec
        }
    }
}

void DlgMyAvatar::button_close()
{
    this->close();
}
