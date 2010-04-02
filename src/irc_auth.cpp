/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr �uczko <piotr.luczko@gmail.com>               *
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

#include "irc_auth.h"

irc_auth::irc_auth(QHttp *param1, QSettings *param2)
{
    qHttp = param1;
    settings = param2;
}

void irc_auth::request_uo(QString param1, QString param2)
{
    mutex.lock();
    QString strNick = param1;
    QString strPass = param2;
    QString strVersion;
    bool bOverride;
    bool bSsl = true;
    QEventLoop loop;

    QString strOverride = settings->value("override").toString();

    if (strOverride == "on")
        bOverride = true;
    else
        bOverride = false;

    http *pHttp = new http(qHttp);
    pHttp->request_clear();

    pHttp->request("http://czat.onet.pl/_s/deployOnetCzat.js", QString::null);
    QObject::connect(qHttp, SIGNAL(done(bool)), &loop, SLOT(quit()));
    loop.exec();

    if (strVersion.isEmpty() == true)
        strVersion = "20090619-1228_2";
    strVersion = QString("1.1(%1 - R)").arg(strVersion);

    QString strNickLen = QString("%1").arg(strNick.length());
    QString strVersionLen = QString("%1").arg(strVersion.length());

    pHttp->request("http://czat.onet.pl/chat.html", "ch=&n=&p=&category=0");
    QObject::connect(qHttp, SIGNAL(done(bool)), &loop, SLOT(quit()));
    loop.exec();

    pHttp->request("http://kropka.onet.pl/_s/kropka/1?DV=czat/applet/FULL", QString::null);
    QObject::connect(qHttp, SIGNAL(done(bool)), &loop, SLOT(quit()));
    loop.exec();

    pHttp->request("http://czat.onet.pl/sk.gif", QString::null);
    QObject::connect(qHttp, SIGNAL(done(bool)), &loop, SLOT(quit()));
    loop.exec();

    // nicki stale
    if (strPass.isEmpty() == false)
    {
        if (bSsl == true) pHttp->request("https://secure.onet.pl/_s/kropka/1?DV=secure", QString::null);
        else pHttp->request("http://secure.onet.pl/_s/kropka/1?DV=secure", QString::null);
        QObject::connect(qHttp, SIGNAL(done(bool)), &loop, SLOT(quit()));
        loop.exec();

        if (bSsl == true) pHttp->request("https://secure.onet.pl/index.html", QString("r=secure.onet.pl&url=&login=%1&haslo=%2&ssl=on&ok=Ok").arg(strNick).arg(strPass));
        else pHttp->request("http://secure.onet.pl/index.html", QString("r=secure.onet.pl&url=&login=%1&haslo=%2&ok=Ok").arg(strNick).arg(strPass));
        QObject::connect(qHttp, SIGNAL(done(bool)), &loop, SLOT(quit()));
        loop.exec();

        if (bOverride == true)
        {
            pHttp->request("http://czat.onet.pl/include/ajaxapi.xml.php3", QString("api_function=userOverride&params=a:1:{s:4:\"nick\";s:%1:\"%2\";}").arg(strNickLen).arg(strNick));
            QObject::connect(qHttp, SIGNAL(requestFinished(int,bool)), &loop, SLOT(quit()));
            loop.exec();

            settings->setValue("override", "off");
        }

        pHttp->request("http://czat.onet.pl/include/ajaxapi.xml.php3", QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:0;s:7:\"version\";s:%3:\"%4\";}").arg(strNickLen).arg(strNick).arg(strVersionLen).arg(strVersion));
        QObject::connect(qHttp, SIGNAL(requestFinished(int,bool)), &loop, SLOT(quit()));
        loop.exec();
    }
    // nicki tyldowe
    else
    {
        pHttp->request("http://czat.onet.pl/include/ajaxapi.xml.php3", QString("api_function=getUoKey&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:8:\"tempNick\";i:1;s:7:\"version\";s:%3:\"%4\";}").arg(strNickLen).arg(strNick).arg(strVersionLen).arg(strVersion));
        QObject::connect(qHttp, SIGNAL(requestFinished(int,bool)), &loop, SLOT(quit()));
        loop.exec();
    }

    delete pHttp;
    mutex.unlock();
}

QString irc_auth::transform_key(QString s)
{
    static int f1[78] = {
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
