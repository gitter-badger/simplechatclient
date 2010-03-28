/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#include "http.h"

http::http(QHttp *param1)
{
    pHttp = param1;
    QObject::connect(pHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(read_response_header(const QHttpResponseHeader &)));
}

http::~http()
{
    pHttp->close();
}

void http::request_new()
{
    strCookies.clear();
    pHttp->abort();
}

void http::request(QString strUrl, QString strContent)
{
    if (strUrl.isEmpty() == true)
        return;

    int iHttpId;
    QString strMethod;
    QUrl url = strUrl;
    QByteArray qContent;

    if (url.userName().isEmpty() == false)
        pHttp->setUser(url.userName(), url.password());

    if (strContent.isEmpty() == false)
    {
        strMethod = "POST";
        qContent = strContent.toAscii();
    }
    else
        strMethod = "GET";

    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    pHttp->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

    QUrl::FormattingOption options = QUrl::RemoveAuthority;
    QString strPath = url.toString(options);
    if (strPath.indexOf("https:") != -1) strPath = strPath.replace("https:", QString::null);
    if (strPath.indexOf("http:") != -1) strPath = strPath.replace("http:", QString::null);
    if (strPath.isEmpty() == true) strPath = "/";

    QHttpRequestHeader header(strMethod, strPath, 1, 1);
    header.setValue("Host", url.host());
    header.setValue("User-Agent", "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.0; pl; rv:1.9.1.4; Trident/4.0;Gecko/20091028 Java/1.6.0_15");
    header.setValue("Content-Language", "pl");
    header.setValue("Content-Type", "application/x-www-form-urlencoded");
    header.setValue("Cache-Control", "no-cache");
    header.setValue("Connection", "close"); // keep-alive
    //header.setValue("Keep-Alive", "300");
    header.setValue("Accept", "*/*");

    if (strCookies.isEmpty() == false)
        header.setValue("Cookie", strCookies);

    if (strMethod == "POST")
        iHttpId = pHttp->request(header,qContent,0);
    else
        iHttpId = pHttp->request(header,0,0);
}

void http::read_response_header(const QHttpResponseHeader &responseHeader)
{
    switch (responseHeader.statusCode())
    {
        case 200: // Ok
        case 301: // Moved Permanently
        case 303: // See Other
        case 307: // Temporary Redirect
                {
                    QString strResponse = responseHeader.toString();
                    QStringList strResponseList = strResponse.split("\r\n");
                    for (int i = 0; i < strResponseList.size(); i++)
                    {
                        if (strResponseList[i].indexOf("Set-Cookie") != -1)
                        {
                            QString strCookie = strResponseList[i];
                            strCookie = strCookie.right(strCookie.length() - strCookie.indexOf("Set-Cookie: ") -12);
                            QString strCookie_key = strCookie.mid(0, strCookie.indexOf("="));
                            QString strCookie_value = strCookie.mid(strCookie.indexOf("=")+1,strCookie.indexOf(";")-strCookie.indexOf("=")-1);
                            strCookies += strCookie_key +"="+ strCookie_value+"; ";
                        }
                    }
                    break;
                }

        case 302: // Found
                {
                    if (responseHeader.value("Location").isEmpty() == false)
                        http::request(responseHeader.value("Location"), QString::null);
                    break;
                }
        default:
                {
                    pHttp->abort();
                    break;
                }
    }
}
