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
#include <QObject>
#include <QTimer>
#include <QUrl>

#include "captcha.h"
#include "core.h"
#include "onet_auth.h"
#include "tab_container.h"

#define AJAX_API "http://czat.onet.pl/include/ajaxapi.xml.php3"

OnetAuth::OnetAuth(TabContainer *_pTabC) : pTabC(_pTabC), bAuthorizing(false)
{
    accessManager = new QNetworkAccessManager;
    cookieJar = new QNetworkCookieJar();
    accessManager->setCookieJar(cookieJar);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkFinished(QNetworkReply*)));

    timerSk = new QTimer();
    timerSk->setInterval(1000*60*9); // 9 min
    connect(timerSk, SIGNAL(timeout()), this, SLOT(refreshSk()));
    timerSk->start();
}

OnetAuth::~OnetAuth()
{
    timerSk->stop();
    delete timerSk;
    accessManager->deleteLater();
    cookieJar->deleteLater();
}

void OnetAuth::authorize(QString p1, QString p2, QString p3)
{
    // fix too long registered nick
    if ((p1.size() > 32))
        p1 = p1.left(32);
    // fix too long unregistered nick
    if ((p3.isEmpty()) && (p1.size() > 31))
        p1 = p1.left(31);
    // fix too long auth nick
    if ((p2.size() > 32))
        p2 = p2.left(32);

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

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *pReply = accessManager->post(request, strContent.toAscii());
    pReply->setProperty("category", AT_chat);
}

void OnetAuth::getDeploy()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: deploy"; }
#endif
    // deploy
    QString strUrl = "http://czat.onet.pl/_s/deployOnetCzat.js";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_deploy);
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
    pReply->setProperty("category", AT_kropka);
}

void OnetAuth::getKropkaFull()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: kropka full"; }
#endif
    // full
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=czat/applet/FULL";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_kropkaFull);
}

void OnetAuth::getSk()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: sk"; }
#endif
    // sk
    QString strUrl = "http://czat.onet.pl/sk.gif";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_sk);
}

void OnetAuth::getSecureKropka()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: secure kropka"; }
#endif
    // secure kropka
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=secure&SC=1&CL=std161&CS=1280x800x24&CW=1280x243&DU=http://secure.onet.pl/&DR=";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_secureKropka);
}

void OnetAuth::getSecureLogin()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: secure login"; }
#endif
    // secure login
    QString strContent = QString("r=&url=&login=%1&haslo=%2&app_id=20&ssl=1&ok=1").arg(strNick, strPass);
    QString strUrl = "https://secure.onet.pl/mlogin.html";

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *pReply = accessManager->post(request, strContent.toAscii());
    pReply->setProperty("category", AT_secureLogin);
    pReply->ignoreSslErrors();
}

void OnetAuth::getOverride()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: override"; }
#endif
    // override
    QString strContent = QString("api_function=userOverride&params=a:1:{s:4:\"nick\";s:%1:\"%2\";}").arg(strNickLen, strNick);
    QString strUrl = AJAX_API;

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *pReply = accessManager->post(request, strContent.toAscii());
    pReply->setProperty("category", AT_override);
}

void OnetAuth::getUo()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: getuo"; }
#endif
    // getuo
    QString strRegistered = bRegisteredNick == true ? "0" : "1";
    QString strContent = QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:%3;s:7:\"version\";s:%4:\"%5\";}").arg(strNickLen, strNick, strRegistered, strVersionLen, strVersion);
    QString strUrl = AJAX_API;

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *pReply = accessManager->post(request, strContent.toAscii());
    pReply->setProperty("category", AT_uo);
}

void OnetAuth::showCaptchaDialog()
{
    // captcha dialog
    DlgCaptcha(Core::instance()->sccWindow(), cookieJar, &strCaptcha).exec();
}

void OnetAuth::getCheckCode()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: check code"; }
#endif
    // check code
    QString strCaptchaSize = QString::number(strCaptcha.size());
    QString strContent = QString("api_function=checkCode&params=a:1:{s:4:\"code\";s:%1:\"%2\";}").arg(strCaptchaSize, strCaptcha);
    QString strUrl = AJAX_API;

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *pReply = accessManager->post(request, strContent.toAscii());
    pReply->setProperty("category", AT_checkCode);
}

void OnetAuth::networkFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    int category = reply->property("category").toInt();
    QByteArray bData = reply->readAll();

    if (bData.isEmpty())
        return;

    switch (category)
    {
        case AT_chat:
            getDeploy();
            break;
        case AT_deploy:
            gotDeploy(QString(bData));
            getKropka();
            break;
        case AT_kropka:
            getKropkaFull();
            break;
        case AT_kropkaFull:
            getSk();
            break;
        case AT_sk:
            if (bRegisteredNick)
                getSecureKropka();
            else
            {
                showCaptchaDialog();
                getCheckCode();
            }
            break;
        case AT_secureKropka:
            getSecureLogin();
            break;
        case AT_secureLogin:
            if (bOverride)
                getOverride();
            else
                getUo();
            break;
        case AT_override:
            getUo();
            break;
        case AT_checkCode:
            getUo();
            break;
        case AT_uo:
            {
                QString strData(bData);
                if ((!strData.isEmpty()) && (Core::instance()->settings.value("logged") == "off"))
                {
                    requestFinished(strData);
                    bAuthorizing = false;

                    saveCookies();
                }
                break;
            }
        case AT_refreshSk:
            saveCookies();
            break;
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

    return "20111013-1222";
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
            pTabC->showMessageActive(strError, ErrorMessage);
        }
    }
    else
    {
        QString strError = tr("Error: Authorization Failed.");
        pTabC->showMessageActive(strError, ErrorMessage);
    }
}

void OnetAuth::refreshSk()
{
    if (Core::instance()->pNetwork->isConnected() && bRegisteredNick)
    {
#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on") { qDebug() << "Request: refresh_sk"; }
#endif
        // sk
        QString strUrl = "http://czat.onet.pl/sk.gif";

        QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
        pReply->setProperty("category", AT_refreshSk);
    }
}
