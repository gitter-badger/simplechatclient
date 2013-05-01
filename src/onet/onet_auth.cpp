/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTimer>
#include <QUrl>

#include "captcha_gui.h"
#include "core.h"
#include "message.h"
#include "settings.h"
#include "onet_auth.h"

#define AJAX_API "http://czat.onet.pl/include/ajaxapi.xml.php3"

OnetAuth::OnetAuth()
{
    accessManager = new QNetworkAccessManager();
    cookieJar = new QNetworkCookieJar();
    accessManager->setCookieJar(cookieJar);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkFinished(QNetworkReply*)));

    Settings::instance()->set("authorizing", "false");

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

void OnetAuth::authorize(QString _strNick, QString _strPass)
{
    if (Settings::instance()->get("logged") == "true")
        return; // already logged

    if (Settings::instance()->get("authorizing") == "true")
        return; // already authorizing

    Settings::instance()->set("authorizing", "true");
    emit authStateChanged();

    strFullNick = _strNick.left(32);
    strNick = (_strNick.startsWith('~') ? _strNick.remove(0,1).left(31) : _strNick.left(32));
    strPass = _strPass;
    strNickLen = QString::number(strNick.length());
    bRegisteredNick = strPass.isEmpty() ? false : true;
    bOverride = Settings::instance()->get("override") == "true" ? true : false;

    if (Settings::instance()->get("debug") == "true")
    {
        qDebug() << "Override: " << bOverride;
        qDebug() << "Logged: " << Settings::instance()->get("logged");
        qDebug() << "Authorizing: " << Settings::instance()->get("authorizing");
    }

    // update nick
    emit updateNick(strFullNick);

    // remove cookies
    removeCookies();

    getChat();
}

void OnetAuth::getChat()
{
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: chat"; }

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
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: deploy"; }

    // deploy
    QString strUrl = "http://czat.onet.pl/_s/deployOnetCzat.js";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_deploy);
}

void OnetAuth::gotDeploy(const QString &strData)
{
    strVersion = this->getVersion(strData);
    strVersion = QString("1.1(%1 - R)").arg(strVersion);
    strVersionLen = QString::number(strVersion.length());
}

void OnetAuth::getKropka()
{
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: kropka"; }

    // kropka
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=czat%2Fchat&SC=1&IP=&DG=id%3Dno-gemius&RI=&C1=&CL=std161&CS=1280x800x24&CW=1280x243&DU=http://czat.onet.pl/chat.html&DR=http://czat.onet.pl/";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_kropka);
}

void OnetAuth::getKropkaFull()
{
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: kropka full"; }

    // full
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=czat/applet/FULL";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_kropkaFull);
}

void OnetAuth::getSk()
{
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: sk"; }

    // sk
    QString strUrl = "http://czat.onet.pl/sk.gif";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_sk);
}

void OnetAuth::getSecureKropka()
{
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: secure kropka"; }

    // secure kropka
    QString strUrl = "http://kropka.onet.pl/_s/kropka/1?DV=secure&SC=1&CL=std161&CS=1280x800x24&CW=1280x243&DU=http://secure.onet.pl/&DR=";

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
    pReply->setProperty("category", AT_secureKropka);
}

void OnetAuth::getSecureLogin()
{
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: secure login"; }

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
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: override"; }

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
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: getuo"; }

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
    CaptchaGui(cookieJar, &strCaptcha).exec();
}

void OnetAuth::getCheckCode()
{
    if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: check code"; }

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
    {
        if (Settings::instance()->get("debug") == "true")
            qWarning() << "Error OnetAuth network: " << reply->errorString();

        Settings::instance()->set("authorizing", "false");
        emit authStateChanged();
        return;
    }

    int category = reply->property("category").toInt();
    QByteArray bData = reply->readAll();

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
                if (Settings::instance()->get("logged") == "false")
                {
                    requestFinished(strData);

                    saveCookies();
                }

                Settings::instance()->set("authorizing", "false");
                emit authStateChanged();
                break;
            }
        case AT_refreshSk:
            saveCookies();
            break;
        default:
            if (Settings::instance()->get("debug") == "true")
                qWarning() << "Error OnetAuth undefined category";

            Settings::instance()->set("authorizing", "false");
            emit authStateChanged();
            break;
    }
}

void OnetAuth::saveCookies()
{
    QStringList constCookies;
    constCookies << "onet_ubi" << "onet_cid" << "onet_sid" << "onet_uid" << "onetzuo_ticket" << "onet_uoi" << "onet_sgn";

    // save cookies
    QList<QNetworkCookie> cookies = accessManager->cookieJar()->cookiesForUrl(QUrl("http://czat.onet.pl"));
    foreach (QNetworkCookie cookie, cookies)
    {
        QString strKey = cookie.name();
        QString strValue = cookie.value();

        if (constCookies.contains(strKey))
            Settings::instance()->set(strKey, strValue);
    }
}

void OnetAuth::removeCookies()
{
    QStringList constCookies;
    constCookies << "onet_ubi" << "onet_cid" << "onet_sid" << "onet_uid" << "onetzuo_ticket" << "onet_uoi" << "onet_sgn";

    foreach (QString constCookie, constCookies)
        Settings::instance()->set(constCookie, QString::null);

    // clear from cookie jar
#if (QT_VERSION >= 0x050000)
    QList<QNetworkCookie> cookies = accessManager->cookieJar()->cookiesForUrl(QUrl("http://czat.onet.pl"));
    foreach (QNetworkCookie cookie, cookies)
        accessManager->cookieJar()->deleteCookie(cookie);
#else
    /*
    https://github.com/simplechatclient/simplechatclient/issues/280

    cookieJar->deleteLater();
    cookieJar = new QNetworkCookieJar();
    accessManager->setCookieJar(cookieJar);
    */
#endif
}

QString OnetAuth::getVersion(const QString &strData)
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

    return "20120711-1544b";
}

void OnetAuth::requestFinished(const QString &strData)
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

        if (strErrorCode.toLower() == "true")
        {
            QString strUOKey = doc.elementsByTagName("uoKey").item(0).toElement().text();
            QString strNick = doc.elementsByTagName("zuoUsername").item(0).toElement().text();

            Settings::instance()->set("uokey", strUOKey);
            Settings::instance()->set("uo_nick", strNick);

            // send auth
            if (Core::instance()->network->isConnected())
            {
                Core::instance()->network->send(QString("NICK %1").arg(strFullNick));
                Core::instance()->network->send("AUTHKEY");
            }
        }
        else
        {
            QString strError = QString(tr("Error: Authentication error [%1]")).arg(strErrorText);
            Message::instance()->showMessageActive(strError, MessageError);

            Core::instance()->network->disconnect();
        }
    }
    else
    {
        QString strError = tr("Error: Authorization Failed.");
        Message::instance()->showMessageActive(strError, MessageError);

        Core::instance()->network->disconnect();
    }
}

void OnetAuth::refreshSk()
{
    if (Core::instance()->network->isConnected() && bRegisteredNick)
    {
        if (Settings::instance()->get("debug") == "true") { qDebug() << "Request: refresh_sk"; }

        // sk
        QString strUrl = "http://czat.onet.pl/sk.gif";

        QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(strUrl)));
        pReply->setProperty("category", AT_refreshSk);
    }
}
