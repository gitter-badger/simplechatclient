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

#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include "core.h"
#include "dlg_captcha.h"
#include "tab_container.h"
#include "onet_auth.h"

OnetAuth::OnetAuth(TabContainer *param1)
{
    pTabC = param1;
    bAuthorizing = false;

    accessManager = new QNetworkAccessManager;
    cookieJar = new QNetworkCookieJar();
    accessManager->setCookieJar(cookieJar);
    QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkFinished(QNetworkReply*)));
}

OnetAuth::~OnetAuth()
{
    delete cookieJar;
    accessManager->deleteLater();
}

void OnetAuth::authorize(QString p1, QString p2, QString p3)
{
    strNick = p1;
    strNickAuth = p2;
    strPass = p3;
    strNickLen = QString("%1").arg(strNick.length());
    bRegisteredNick = strPass.isEmpty() ? false : true;
    bOverride = Core::instance()->settings.value("override") == "on" ? true : false;

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Override: " << bOverride; }
#endif

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on")
    {
        qDebug() << "Logged: " << Core::instance()->settings.value("logged");
        qDebug() << "Authorizing: " << bAuthorizing;
    }
#endif

    if (Core::instance()->settings.value("logged") == "on") return; // already logged

    if (bAuthorizing) return; // already authorizing
    bAuthorizing = true;

    getChat();
}

void OnetAuth::getChat()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: chat"; }
#endif
    // chat
    QString strUrl = "http://czat.onet.pl/chat.html";
    QString strContent = "ch=&n=&p=&category=0";

    QNetworkReply *pReply = accessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "get_chat");
    pReply->ignoreSslErrors();
}

void OnetAuth::getDeploy()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: deploy"; }
#endif
    // deploy
    QString strUrl = "http://czat.onet.pl/_s/deployOnetCzat.js";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", "get_deploy");
    pReply->ignoreSslErrors();
}

void OnetAuth::gotDeploy(QString strData)
{
    strVersion = this->getVersion(strData);
    strVersion = QString("1.1(%1 - R)").arg(strVersion);
    strVersionLen = QString("%1").arg(strVersion.length());
}

void OnetAuth::getKropka()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: kropka"; }
#endif
    // kropka
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=czat%2Fchat&SC=1&IP=&DG=id%3Dno-gemius&RI=&C1=&CL=std161&CS=1280x800x24&CW=1280x243&DU=http://czat.onet.pl/chat.html&DR=http://czat.onet.pl/";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", "get_kropka");
    pReply->ignoreSslErrors();
}

void OnetAuth::getKropkaFull()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: kropka full"; }
#endif
    // full
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=czat/applet/FULL";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", "get_kropka_full");
    pReply->ignoreSslErrors();
}

void OnetAuth::getSk()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: sk"; }
#endif
    // sk
    QString strUrl = "http://czat.onet.pl/sk.gif";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", "get_sk");
    pReply->ignoreSslErrors();
}

void OnetAuth::getSecureKropka()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: secure kropka"; }
#endif
    // secure kropka
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=secure&SC=1&CL=std161&CS=1280x800x24&CW=1280x243&DU=http://secure.onet.pl/&DR=";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", "get_secure_kropka");
    pReply->ignoreSslErrors();
}

void OnetAuth::getSecureMlogin()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: secure login"; }
#endif
    // secure login
    QString strContent = QString("r=&url=&login=%1&haslo=%2&app_id=20&ssl=1&ok=1").arg(strNick).arg(strPass);
    QString strUrl = "https://secure.onet.pl/mlogin.html";

    QNetworkReply *pReply = accessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "get_secure_mlogin");
    pReply->ignoreSslErrors();
}

void OnetAuth::getOverride()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: override"; }
#endif
    // override
    QString strContent = QString("api_function=userOverride&params=a:1:{s:4:\"nick\";s:%1:\"%2\";}").arg(strNickLen).arg(strNick);
    QString strUrl = "http://czat.onet.pl/include/ajaxapi.xml.php3";

    QNetworkReply *pReply = accessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "get_override");
    pReply->ignoreSslErrors();
}

void OnetAuth::getUo()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: getuo"; }
#endif
    // getuo
    QString strRegistered = bRegisteredNick == true ? "0" : "1";
    QString strContent = QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:%3;s:7:\"version\";s:%4:\"%5\";}").arg(strNickLen).arg(strNick).arg(strRegistered).arg(strVersionLen).arg(strVersion);
    QString strUrl = "http://czat.onet.pl/include/ajaxapi.xml.php3";

    QNetworkReply *pReply = accessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "get_uo");
    pReply->ignoreSslErrors();
}

void OnetAuth::showCaptchaDialog()
{
    // captcha dialog
    DlgCaptcha(cookieJar, &strCaptcha).exec();
}

void OnetAuth::getCheckCode()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: check code"; }
#endif
    // check code
    QString strContent = QString("api_function=checkCode&params=a:1:{s:4:\"code\";s:%1:\"%2\";}").arg(strCaptcha.size()).arg(strCaptcha);
    QString strUrl = "http://czat.onet.pl/include/ajaxapi.xml.php3";

    QNetworkReply *pReply = accessManager->post(QNetworkRequest(QUrl(strUrl)), strContent.toAscii());
    pReply->setProperty("category", "get_check_code");
    pReply->ignoreSslErrors();
}

void OnetAuth::networkFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QString strCategory = reply->property("category").toString();
    QByteArray bData = reply->readAll();

    if (bData.isEmpty())
        return;

    if (strCategory == "get_chat")
        getDeploy();
    else if (strCategory == "get_deploy")
    {
        gotDeploy(QString(bData));
        getKropka();
    }
    else if (strCategory == "get_kropka")
        getKropkaFull();
    else if (strCategory == "get_kropka_full")
        getSk();
    else if (strCategory == "get_sk")
    {
        if (bRegisteredNick)
            getSecureKropka();
        else
        {
            showCaptchaDialog();
            getCheckCode();
        }
    }
    else if (strCategory == "get_secure_kropka")
        getSecureMlogin();
    else if (strCategory == "get_secure_mlogin")
    {
        if (bOverride)
            getOverride();
        else
            getUo();
    }
    else if (strCategory == "get_override")
        getUo();
    else if (strCategory == "get_check_code")
        getUo();
    else if (strCategory == "get_uo")
    {
        // not empty key; not logged
        QString strData(bData);
        if ((!strData.isEmpty()) && (Core::instance()->settings.value("logged") == "off"))
        {
            requestFinished(strData);
            bAuthorizing = false;

            saveCookies();
        }
    }
}

void OnetAuth::saveCookies()
{
    // save cookies
    QList<QNetworkCookie> cookies = accessManager->cookieJar()->cookiesForUrl(QUrl("http://czat.onet.pl"));
    for (QList<QNetworkCookie>::iterator i = cookies.begin(); i != cookies.end(); ++i)
    {
        QString strKey = i->name();
        QString strValue = i->value();

        if (strKey == "onet_ubi")
            Core::instance()->settings["onet_ubi"] = strValue;
        else if (strKey == "onet_cid")
            Core::instance()->settings["onet_cid"] = strValue;
        else if (strKey == "onet_sid")
            Core::instance()->settings["onet_sid"] = strValue;
        else if (strKey == "onet_uid")
            Core::instance()->settings["onet_uid"] = strValue;
        else if (strKey == "onetzuo_ticket")
            Core::instance()->settings["onetzuo_ticket"] = strValue;
    }
}

QString OnetAuth::getVersion(QString strData)
{
    if (!strData.isEmpty())
    {
        if (strData.contains("OnetCzatLoader"))
        {
            QString strFind1 = "signed-OnetCzatLoader-";
            QString strFind2 = ".jar";
            int iPos1 = strData.indexOf(strFind1)+strFind1.length();
            int iPos2 = strData.indexOf(strFind2, iPos1);
            QString strVersion = strData.mid(iPos1, iPos2-iPos1);

            if ((!strVersion.isEmpty()) && (strVersion.length() > 0) && (strVersion.length() < 20))
                return strVersion;
        }
    }

    return "20110809-1252";
}

void OnetAuth::requestFinished(QString strData)
{
    if (!strData.isEmpty())
    {
        QDomDocument doc;
        doc.setContent(strData);

        // <?xml version="1.0" encoding="ISO-8859-2"?><root><uoKey>LY9j2sXwio0G_yo3PdpukDL8iZJGHXKs</uoKey><zuoUsername>~Succubi_test</zuoUsername><error err_code="TRUE"  err_text="wartość prawdziwa" ></error></root>
        // <?xml version="1.0" encoding="ISO-8859-2"?><root><error err_code="-2"  err_text="U.ytkownik nie zalogowany" ></error></root>

        QDomNode dError = doc.elementsByTagName("error").item(0);
        QString strErrorCode = dError.attributes().namedItem("err_code").nodeValue();
        QString strErrorText = dError.attributes().namedItem("err_text").nodeValue();

        if (strErrorCode == "TRUE")
        {
            QString strUOKey = doc.elementsByTagName("uoKey").item(0).toElement().text();
            QString strNick = doc.elementsByTagName("zuoUsername").item(0).toElement().text();

            Core::instance()->settings["uokey"] = strUOKey;
            Core::instance()->settings["uo_nick"] = strNick;

            // send auth
            if (Core::instance()->pNetwork->isConnected())
            {
                Core::instance()->pNetwork->send(QString("NICK %1").arg(strNickAuth));
                Core::instance()->pNetwork->send("AUTHKEY");
            }
        }
        else
        {
            QString strError = QString(tr("Error: Authentication error [%1]")).arg(strErrorText);
            pTabC->showMsgActive(strError, ErrorMessage);
        }
    }
    else
    {
        QString strError = tr("Error: Authorization Failed.");
        pTabC->showMsgActive(strError, ErrorMessage);
    }
}
