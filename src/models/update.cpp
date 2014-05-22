/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2014 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QAction>
#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include "core.h"
#include "update_gui.h"
#include "message.h"
#include "notification.h"
#include "settings.h"
#include "update.h"

#ifdef Q_OS_WIN
    #include <QSysInfo>
#endif

#define UPDATE_URL_1 "http://simplechatclien.sourceforge.net/update.xml"
#define UPDATE_URL_2 "http://simplechatclient.github.io/update.xml"

Update * Update::Instance = 0;

Update * Update::instance()
{
    if (!Instance)
    {
        Instance = new Update();
    }

    return Instance;
}

Update::Update()
{
    updateAction = new QAction(QIcon(":/images/oxygen/16x16/download.png") , tr("Updates"), this);
    updateAction->setVisible(false);

    accessManager = new QNetworkAccessManager;
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateFinished(QNetworkReply*)));
}

Update::~Update()
{
    accessManager->deleteLater();
}

void Update::checkUpdate()
{
    QString strUrl = UPDATE_URL_1;

    QString strAgentPlatform = this->getPlatform();
    QString strAgentUrl = "http://simplechatclien.sourceforge.net";
    QString strAgentProgram = "SimpleChatClient";
    QString strAgentVersion = Settings::instance()->get("version");
    QString strUserAgent = QString("Mozilla/5.0 (%1; +%2) %3/%4").arg(strAgentPlatform, strAgentUrl, strAgentProgram, strAgentVersion);

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));
    request.setHeader(QNetworkRequest::UserAgentHeader, strUserAgent.toLatin1());

    QString strUUID = Settings::instance()->get("unique_id");
    QRegExp rUUID("^[a-f0-9]{8}-([a-f0-9]{4}-){3}[a-f0-9]{12}$");
    if ((!strUUID.isEmpty()) && (rUUID.exactMatch(strUUID)))
    {
        QString strContent = QString("{\"uuid\":\"%1\"}").arg(strUUID);

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *pReply = accessManager->post(request, strContent.toLatin1());
        pReply->setProperty("update_url", "1");
    }
    else
    {
        QNetworkReply *pReply = accessManager->get(request);
        pReply->setProperty("update_url", "1");
    }
}

void Update::compareVersion()
{
    QString strCurrentVersion = Settings::instance()->get("version");
    QStringList lCurrentVersion = strCurrentVersion.split(".");
    int iCurrentMajor = lCurrentVersion.at(0).toInt();
    int iCurrentMinor = lCurrentVersion.at(1).toInt();
    int iCurrentPatch = lCurrentVersion.at(2).toInt();
    int iCurrentVersion = (QString("%1%2%3").arg(iCurrentMajor).arg(iCurrentMinor).arg(iCurrentPatch)).toInt();

    QString strAvailableVersion = Settings::instance()->get("available_version");
    QStringList lAvailableVersion = strAvailableVersion.split(".");
    int iAvailableMajor = lAvailableVersion.at(0).toInt();
    int iAvailableMinor = lAvailableVersion.at(1).toInt();
    int iAvailablePatch = lAvailableVersion.at(2).toInt();
    int iAvailableVersion = (QString("%1%2%3").arg(iAvailableMajor).arg(iAvailableMinor).arg(iAvailablePatch)).toInt();

    QString strVersionStatus;
    if (iAvailableVersion == iCurrentVersion)
        strVersionStatus = UPDATE_STATUS_UPTODATE;
    else if (iAvailableVersion < iCurrentVersion)
        strVersionStatus = UPDATE_STATUS_BETA;
    else if (iAvailableVersion > iCurrentVersion)
        strVersionStatus = UPDATE_STATUS_OUTOFDATE;
    else
        strVersionStatus = UPDATE_STATUS_UNKNOWN;

    // save status
    Settings::instance()->set("version_status", strVersionStatus);

    if (Settings::instance()->get("debug") == "true")
        qDebug() << "Current version: " << strCurrentVersion << " Available version: " << strAvailableVersion << " Status: " << strVersionStatus;
}

void Update::saveSettings(QString strUpdateXml)
{
    QDomDocument doc;
    doc.setContent(strUpdateXml);

    QString strAvailableVersion = doc.elementsByTagName("version").item(0).toElement().text();
    QString strWhatsNew = doc.elementsByTagName("whats_new").item(0).toElement().text();
    QString strMOTD = doc.elementsByTagName("motd").item(0).toElement().text();

    if (!strWhatsNew.isEmpty())
        Settings::instance()->set("whats_new", strWhatsNew);

    if (!strMOTD.isEmpty())
        Settings::instance()->set("motd", strMOTD);

    if (!strAvailableVersion.isEmpty())
        Settings::instance()->set("available_version", strAvailableVersion);
}

void Update::updateFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    int update_url = reply->property("update_url").toInt();

    // if errors
    if (reply->error())
    {
        if (update_url == 1)
        {
            QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(UPDATE_URL_2)));
            pReply->setProperty("update_url", "2");
        }
        else
        {
            if (Settings::instance()->get("debug") == "true")
                qWarning() << "Error: Cannot get update xml";
        }

        return;
    }

    QString strUpdateXml = reply->readAll();

    if (!strUpdateXml.isEmpty())
    {
        saveSettings(strUpdateXml);

        if (!Settings::instance()->get("motd").isEmpty())
        {
            QString strMOTD = Settings::instance()->get("motd");
            QString strMessageOfTheDay = QString("%Fb%%1 %2").arg(tr("Message Of The Day:"), strMOTD);
            Message::instance()->showMessage(STATUS_WINDOW, strMessageOfTheDay, MessageDefault);
        }

        if (Settings::instance()->get("available_version") != "0.0.0.0")
        {
            compareVersion();
            Notification::instance()->refreshUpdate();
        }
    }
}

QString Update::getPlatform()
{
    QString strPlatform = "Unknown OS";
#ifdef Q_OS_WIN
    switch (QSysInfo::windowsVersion())
    {
        case 0x0001: strPlatform = "Windows 3.1"; break;
        case 0x0002: strPlatform = "Windows 95"; break;
        case 0x0003: strPlatform = "Windows 98"; break;
        case 0x0004: strPlatform = "Windows Me"; break;
        case 0x0010: strPlatform = "Windows NT 4.0"; break;
        case 0x0020: strPlatform = "Windows NT 5.0"; break;
        case 0x0030: strPlatform = "Windows NT 5.1"; break;
        case 0x0040: strPlatform = "Windows NT 5.2"; break;
        case 0x0080: strPlatform = "Windows NT 6.0"; break;
        case 0x0090: strPlatform = "Windows NT 6.1"; break;
        case 0x00a0: strPlatform = "Windows NT 6.2"; break;
        case 0x00b0: strPlatform = "Windows NT 6.3"; break;

        case 0x0100: strPlatform = "Windows CE"; break;
        case 0x0200: strPlatform = "Windows CE .NET"; break;
        case 0x0300: strPlatform = "Windows CE 5.x"; break;
        case 0x0400: strPlatform = "Windows CE 6.x"; break;

        default: strPlatform = "Windows"; break;
    }
#else
    strPlatform = "Linux";
#endif

    return strPlatform;
}
