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
#include <QEventLoop>
#include <QHostInfo>
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
}

QString OnetAuth::networkRequest(QNetworkAccessManager *accessManager, QString strLink, QString strContent)
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

    if (!strRedir.isEmpty())
    {
        if (!strRedir.contains("http"))
        {
            strLink.remove("http://");
            strLink = strLink.left(strLink.indexOf("/"));
            strRedir = "http://"+strLink+strRedir;
        }

        networkRequest(accessManager, strRedir, QString::null);
    }

    return strData;
}

void OnetAuth::authorize(QString strNick, QString strNickAuth, QString strPass)
{
    if ((QHostInfo::fromName("czat.onet.pl").error() != QHostInfo::NoError) || (QHostInfo::fromName("secure.onet.pl").error() != QHostInfo::NoError) || (QHostInfo::fromName("kropka.onet.pl").error() != QHostInfo::NoError))
    {
        QString strError = tr("Error: Authentication error [onet.pl servers are not available]");
        pTabC->showMsgActive(strError, ErrorMessage);
        return;
    }

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on")
    {
        qDebug() << "Logged: " << Core::instance()->settings.value("logged");
        qDebug() << "Authorizing: " << bAuthorizing;
    }
#endif

    if (Core::instance()->settings.value("logged") == "on") return; // already logged

    if (bAuthorizing) return; // already authorizing

    bool bOverride;
    QString strData;
    QString strGetUo;
    bAuthorizing = true;

    QNetworkAccessManager *accessManager = new QNetworkAccessManager;
    QNetworkCookieJar *cookieJar = new QNetworkCookieJar();
    accessManager->setCookieJar(cookieJar);

    QString strOverride = Core::instance()->settings.value("override");

    if (strOverride == "on")
        bOverride = true;
    else
        bOverride = false;

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Override: " << bOverride; }
#endif

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: chat"; }
#endif
    // chat
    networkRequest(accessManager, "http://czat.onet.pl/chat.html", "ch=&n=&p=&category=0");

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: deploy"; }
#endif
    // deploy
    QString strVersion = networkRequest(accessManager, "http://czat.onet.pl/_s/deployOnetCzat.js", QString::null);
    strVersion = this->getVersion(strVersion);
    strVersion = QString("1.1(%1 - R)").arg(strVersion);

    QString strNickLen = QString("%1").arg(strNick.length());
    QString strVersionLen = QString("%1").arg(strVersion.length());

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: kropka"; }
#endif
    // kropka
    QString strKropka = "http://kropka.onet.pl/_s/kropka/1?DV=czat%2Fchat&SC=1&IP=&DG=id%3Dno-gemius&RI=&C1=&CL=std161&CS=1280x800x24&CW=1280x243&DU=http://czat.onet.pl/chat.html&DR=http://czat.onet.pl/";
    networkRequest(accessManager, strKropka, QString::null);

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: kropka full"; }
#endif
    // full
    networkRequest(accessManager, "http://kropka.onet.pl/_s/kropka/1?DV=czat/applet/FULL", QString::null);

#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: sk"; }
#endif
    // sk
    networkRequest(accessManager, "http://czat.onet.pl/sk.gif", QString::null);

    // registered nick
    if (!strPass.isEmpty())
    {
#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: secure kropka"; }
#endif
        // secure kropka
        QString strSecureKropka = "http://kropka.onet.pl/_s/kropka/1?DV=secure&SC=1&CL=std161&CS=1280x800x24&CW=1280x243&DU=http://secure.onet.pl/&DR=";
        networkRequest(accessManager, strSecureKropka, QString::null);

#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: secure login"; }
#endif
        // secure login
        strData = QString("r=&url=&login=%1&haslo=%2&app_id=20&ssl=1&ok=1").arg(strNick).arg(strPass);
        networkRequest(accessManager, "https://secure.onet.pl/mlogin.html", strData);

        if (bOverride)
        {
#ifdef Q_WS_X11
            if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: override"; }
#endif
            // override
            strData = QString("api_function=userOverride&params=a:1:{s:4:\"nick\";s:%1:\"%2\";}").arg(strNickLen).arg(strNick);
            networkRequest(accessManager, "http://czat.onet.pl/include/ajaxapi.xml.php3", strData);
        }

#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: getuo"; }
#endif
        // getuo
        strData = QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:0;s:7:\"version\";s:%3:\"%4\";}").arg(strNickLen).arg(strNick).arg(strVersionLen).arg(strVersion);
        strGetUo = networkRequest(accessManager, "http://czat.onet.pl/include/ajaxapi.xml.php3", strData);
    }
    // unregistered nick
    else
    {
        // captcha dialog
        QString strCaptcha;
        DlgCaptcha(accessManager, &strCaptcha).exec();

#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: check code"; }
#endif
        // check code
        strData = QString("api_function=checkCode&params=a:1:{s:4:\"code\";s:%1:\"%2\";}").arg(strCaptcha.size()).arg(strCaptcha);
        strGetUo = networkRequest(accessManager, "http://czat.onet.pl/include/ajaxapi.xml.php3", strData);

#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: getuo"; }
#endif
        // getuo
        strData = QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:1;s:7:\"version\";s:%3:\"%4\";}").arg(strNickLen).arg(strNick).arg(strVersionLen).arg(strVersion);
        strGetUo = networkRequest(accessManager, "http://czat.onet.pl/include/ajaxapi.xml.php3", strData);
    }

    // not empty key; not logged
    if ((!strGetUo.isEmpty()) && (Core::instance()->settings.value("logged") == "off"))
    {
        requestFinished(strNickAuth, strGetUo);

        saveCookies(accessManager);
    }

    bAuthorizing = false;
    delete cookieJar;
    accessManager->deleteLater();
}

void OnetAuth::saveCookies(QNetworkAccessManager *accessManager)
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

    return "20110425-2020";
}

void OnetAuth::requestFinished(QString strNickAuth, QString strData)
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
            emit send(QString("NICK %1").arg(strNickAuth));
            emit send("AUTHKEY");
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
