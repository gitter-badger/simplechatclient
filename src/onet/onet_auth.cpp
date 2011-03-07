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
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>
#include "tab_container.h"
#include "onet_auth.h"

OnetAuth::OnetAuth(TabContainer *param1)
{
    tabc = param1;
}

QString OnetAuth::network_request(QNetworkAccessManager *accessManager, QString strLink, QString strContent)
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

    QString strData = pReply->readAll();
    QString strRedir = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

    pReply->deleteLater();

    if (strRedir.isEmpty() == false)
        network_request(accessManager, strRedir, QString::null);

    return strData;
}

void OnetAuth::authorize(QString param1, QString param2, QString param3)
{
    QSettings settings;
    if (settings.value("logged") == "on") return; // already logged

    QString strNick = param1;
    QString strNickAuth = param2;
    QString strPass = param3;
    QString strVersion;
    bool bOverride;
    QString strData;
    QString strGetUo;

    QNetworkAccessManager accessManager;
    QNetworkCookieJar *cookieJar = new QNetworkCookieJar();
    accessManager.setCookieJar(cookieJar);

    QString strOverride = settings.value("override").toString();

    if (strOverride == "on")
        bOverride = true;
    else
        bOverride = false;

    QString strRandom = "";
    while(strRandom.length() < 11)
    {
        int i = qrand() % 9;
        QChar c = i+=48;
        strRandom += c.toAscii();
    }

    bool bHost1 = true;
    bool bHost2 = true;
    bool bHost3 = true;

    QHostInfo test_host1 = QHostInfo::fromName("czat.onet.pl");
    if (test_host1.error() != QHostInfo::NoError)
         bHost1 = false;

    QHostInfo test_host2 = QHostInfo::fromName("secure.onet.pl");
    if (test_host2.error() != QHostInfo::NoError)
         bHost2 = false;

    QHostInfo test_host3 = QHostInfo::fromName("kropka.onet.pl");
    if (test_host3.error() != QHostInfo::NoError)
         bHost3 = false;

    if ((bHost1 == true) && (bHost2 == true) && (bHost3 == true))
    {
        QString strKropka;

        // chat
        strKropka = network_request(&accessManager, "http://czat.onet.pl/chat.html", "ch=&n=&p=&category=0");
        strKropka = get_kropka(strKropka);

        // deploy
        strVersion = network_request(&accessManager, "http://czat.onet.pl/_s/deployOnetCzat.js", QString::null);
        strVersion = this->get_version(strVersion);
        strVersion = QString("1.1(%1 - R)").arg(strVersion);

        QString strNickLen = QString("%1").arg(strNick.length());
        QString strVersionLen = QString("%1").arg(strVersion.length());

        // kropka
        strKropka += "&RI=&C1=&CL=std153&CS=1024x768&CW=1024x768&DU=http://czat.onet.pl/chat.html&DR=http://czat.onet.pl";
        network_request(&accessManager, strKropka, QString::null);

        // targetowanie behawioralne - rc
        network_request(&accessManager, QString("http://rc.onetwl.pl/Get/onet/JS/GetRcmd.js?ord=%1").arg(strRandom), QString::null);

        // targetowanie behawioralne - tr
        QString strContentGroup = "";
        QString strParams = "&CustomerId=onet&WebsiteId=czat.onet.pl&AC=on";
        network_request(&accessManager, QString("http://tr.onetwl.pl/Cnt/onet/CP/%1?ord=%2%3").arg(strContentGroup).arg(strRandom).arg(strParams), QString::null);

        // full
        network_request(&accessManager, "http://kropka.onet.pl/_s/kropka/1?DV=czat/applet/FULL", QString::null);

        // sk
        network_request(&accessManager, "http://czat.onet.pl/sk.gif", QString::null);

        // registered nick
        if (strPass.isEmpty() == false)
        {
            QString strSecureKropka;

            // secure
            strSecureKropka = network_request(&accessManager, "http://secure.onet.pl/", QString::null);
            strSecureKropka = get_kropka(strSecureKropka);

            // secure kropka
            strSecureKropka += "&RI=&C1=&CL=std153&SX=secure.onet.pl&CS=&CW=&DU=http://secure.onet.pl/&DR=";
            network_request(&accessManager, strSecureKropka, QString::null);

            // secure login
            strData = QString("r=&url=&login=%1&haslo=%2&app_id=20&ssl=1&ok=1").arg(strNick).arg(strPass);
            network_request(&accessManager, "https://secure.onet.pl/mlogin.html", strData);

            if (bOverride == true)
            {
                // override
                strData = QString("api_function=userOverride&params=a:1:{s:4:\"nick\";s:%1:\"%2\";}").arg(strNickLen).arg(strNick);
                network_request(&accessManager, "http://czat.onet.pl/include/ajaxapi.xml.php3", strData);
            }

            // getuo
            strData = QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:0;s:7:\"version\";s:%3:\"%4\";}").arg(strNickLen).arg(strNick).arg(strVersionLen).arg(strVersion);
            strGetUo = network_request(&accessManager, "http://czat.onet.pl/include/ajaxapi.xml.php3", strData);
        }
        // unregistered nick
        else
        {
            strData = QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:1;s:7:\"version\";s:%3:\"%4\";}").arg(strNickLen).arg(strNick).arg(strVersionLen).arg(strVersion);
            strGetUo = network_request(&accessManager, "http://czat.onet.pl/include/ajaxapi.xml.php3", strData);
        }
    }
    else
    {
        tabc->show_msg("Status", tr("Error: Authentication error [onet.pl servers are not available]"), 9);
    }

    // not empty key; not logged
    if ((strGetUo.isEmpty() == false) && (settings.value("logged") == "off"))
    {
        request_finished(strNickAuth, strGetUo);

        // save cookies
        QList <QNetworkCookie> cookies = accessManager.cookieJar()->cookiesForUrl(QUrl("http://czat.onet.pl"));
        for (QList <QNetworkCookie>::iterator i = cookies.begin(); i != cookies.end(); ++i)
        {
            QString strKey = i->name();
            QString strValue = i->value();

            if (strKey == "onet_ubi")
                settings.setValue("onet_ubi", strValue);
            else if (strKey == "onet_cid")
                settings.setValue("onet_cid", strValue);
            else if (strKey == "onet_sid")
                settings.setValue("onet_sid", strValue);
            else if (strKey == "onet_uid")
                settings.setValue("onet_uid", strValue);
        }
    }

    delete cookieJar;
}

QString OnetAuth::transform_key(QString s)
{
    static int f1[] = {
        29, 43, 7, 5, 52, 58, 30, 59, 26, 35,
        35, 49, 45, 4, 22, 4, 0, 7, 4, 30,
        51, 39, 16, 6, 32, 13, 40, 44, 14, 58,
        27, 41, 52, 33, 9, 30, 30, 52, 16, 45,
        43, 18, 27, 52, 40, 52, 10, 8, 10, 14,
        10, 38, 27, 54, 48, 58, 17, 34, 6, 29,
        53, 39, 31, 35, 60, 44, 26, 34, 33, 31,
        10, 36, 51, 44, 39, 53, 5, 56
    };

    static int f2[] = {
        7, 32, 25, 39, 22, 26, 32, 27, 17, 50,
        22, 19, 36, 22, 40, 11, 41, 10, 10, 2,
        10, 8, 44, 40, 51, 7, 8, 39, 34, 52,
        52, 4, 56, 61, 59, 26, 22, 15, 17, 9,
        47, 38, 45, 10, 0, 12, 9, 20, 51, 59,
        32, 58, 19, 28, 11, 40, 8, 28, 6, 0,
        13, 47, 34, 60, 4, 56, 21, 60, 59, 16,
        38, 52, 61, 44, 8, 35, 4, 11
    };

    static int f3[] = {
        60, 30, 12, 34, 33, 7, 15, 29, 16, 20,
        46, 25, 8, 31, 4, 48, 6, 44, 57, 16,
        12, 58, 48, 59, 21, 32, 2, 18, 51, 8,
        50, 29, 58, 6, 24, 34, 11, 23, 57, 43,
        59, 50, 10, 56, 27, 32, 12, 59, 16, 4,
        40, 39, 26, 10, 49, 56, 51, 60, 21, 37,
        12, 56, 39, 15, 53, 11, 33, 43, 52, 37,
        30, 25, 19, 55, 7, 34, 48, 36
    };

    static int p1[] = {
        11, 9, 12, 0, 1, 4, 10, 13, 3, 6,
        7, 8, 15, 5, 2, 14
    };

    static int p2[] = {
        1, 13, 5, 8, 7, 10, 0, 15, 12, 3,
        14, 11, 2, 9, 6, 4
    };

    int ai[16];
    int ai1[16];

    if (s.length() < 16)
        return QString::null;

    for (int i = 0; i < 16; i++)
    {
        int c = s[i].toAscii();
        ai[i] = (c > 57 ? c > 90 ? (c - 97) + 36 : (c - 65) + 10 : c - 48);
    }

    for (int i = 0; i < 16; i++)
        ai[i] = f1[ai[i] + i];

    //  arraycopy
    for (int i = 0; i < 16; i++) ai1[i] = ai[i];

    for (int i = 0; i < 16; i++)
        ai[i] = (ai[i] + ai1[p1[i]]) % 62;

    for (int i = 0; i < 16; i++)
        ai[i] = f2[ai[i] + i];

    //  arraycopy
    for (int i = 0; i < 16; i++) ai1[i] = ai[i];

    for (int i = 0; i < 16; i++)
        ai[i] = (ai[i] + ai1[p2[i]]) % 62;

    for (int i = 0; i < 16; i++)
        ai[i] = f3[ai[i] + i];

    for (int i = 0; i < 16; i++)
    {
        int j = ai[i];
        ai[i] = j >= 10 ? j >= 36 ? (97 + j) - 36 : (65 + j) - 10 : 48 + j;
    }

    //  result
    QString strResult;
    for (int i = 0; i < 16; i++)
        strResult += ai[i];

    return strResult;
}

QString OnetAuth::get_version(QString strData)
{
    if (strData.isEmpty() == false)
    {
        if (strData.indexOf("OnetCzatLoader") != -1)
        {
            QString strFind1 = "signed-OnetCzatLoader-";
            QString strFind2 = ".jar";
            int iPos1 = strData.indexOf(strFind1)+strFind1.length();
            int iPos2 = strData.indexOf(strFind2, iPos1);
            QString strVersion = strData.mid(iPos1, iPos2-iPos1);

            if ((strVersion.isEmpty() == false) && (strVersion.length() > 0) && (strVersion.length() < 20))
                return strVersion;
        }
    }

    return "20101008-1609";
}

QString OnetAuth::get_kropka(QString strData)
{
    QString strResult;
    if (strData.isEmpty() == false)
    {
        QString strFind1 = "onetKropka.src = '";
        QString strFind2 = "'";
        int iPos1 = strData.indexOf(strFind1)+strFind1.length();
        int iPos2 = strData.indexOf(strFind2, iPos1);
        strResult = strData.mid(iPos1, iPos2-iPos1);
    }
    return strResult;
}

void OnetAuth::request_finished(QString strNickAuth, QString strData)
{
    if (strData.isEmpty() == false)
    {
        QDomDocument doc;
        doc.setContent(strData);

        // <?xml version="1.0" encoding="ISO-8859-2"?><root><uoKey>LY9j2sXwio0G_yo3PdpukDL8iZJGHXKs</uoKey><zuoUsername>~Succubi_test</zuoUsername><error err_code="TRUE"  err_text="wartość prawdziwa" ></error></root>
        if (strData.indexOf("uoKey") != -1)
        {
            if (strData.indexOf("err_code=\"TRUE\"") != -1)
            {
                QString strUOKey = doc.elementsByTagName("uoKey").item(0).toElement().text();
                QString strNick = doc.elementsByTagName("zuoUsername").item(0).toElement().text();
                QSettings settings;
                settings.setValue("uokey", strUOKey);
                settings.setValue("uo_nick", strNick);

                // send auth
                emit send(QString("NICK %1").arg(strNickAuth));
                emit send("AUTHKEY");
            }
            else
            {
                tabc->show_msg("Status", tr("Error: Authorization Failed."), 9);
            }
        }
        // <?xml version="1.0" encoding="ISO-8859-2"?><root><error err_code="-2"  err_text="U.ytkownik nie zalogowany" ></error></root>
        else if (strData.indexOf("error err_code=") != -1)
        {
            if (strData.indexOf("err_code=\"TRUE\"") != -1)
            {
                tabc->show_msg("Status", tr("Error: Authentication error [Nick is already logged into the chat]"), 9);
            }
            else
            {
                QDomNode dError = doc.elementsByTagName("error").item(0);
                QString strErrorText = dError.attributes().namedItem("err_text").nodeValue();

                tabc->show_msg("Status", QString(tr("Error: Authentication error [%1]")).arg(strErrorText), 9);
            }
        }
    }
    else
    {
        tabc->show_msg("Status", tr("Error: Authorization Failed."), 9);
    }
}
