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
#include <QNetworkReply>
#include <QStringList>
#include "core.h"
#include "update.h"
#include "message.h"
#include "settings.h"
#include "updates.h"

#define UPDATE_URL_1 "http://simplechatclien.sourceforge.net/update.xml"
#define UPDATE_URL_2 "http://simplechatclient.github.com/update.xml"

Updates * Updates::Instance = 0;

Updates * Updates::instance()
{
    if (!Instance)
    {
        Instance = new Updates();
    }

    return Instance;
}

Updates::Updates()
{
    accessManager = new QNetworkAccessManager;
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateFinished(QNetworkReply*)));
}

Updates::~Updates()
{
    accessManager->deleteLater();
}

void Updates::checkUpdate()
{
    QString strPlatform = "unknown";
#ifdef Q_WS_WIN
    strPlatform = "windows";
#else
    strPlatform = "linux";
#endif

    QString strContent = QString("params={\"id\":\"%1\",\"version\":\"%2\",\"platform\":\"%3\"}").arg(Settings::instance()->get("unique_id"), Settings::instance()->get("version"), strPlatform);
    QString strUrl = UPDATE_URL_1;

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *pReply = accessManager->post(request, strContent.toAscii());
    pReply->setProperty("update_url", "1");
}

void Updates::compareVersion()
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
        strVersionStatus = "uptodate";
    else if (iAvailableVersion < iCurrentVersion)
        strVersionStatus = "beta";
    else if (iAvailableVersion > iCurrentVersion)
        strVersionStatus = "outofdate";
    else
        strVersionStatus = "unknown";

    // save status
    Settings::instance()->set("version_status", strVersionStatus);

    if (Settings::instance()->get("debug") == "true")
        qDebug() << "Current version: " << strCurrentVersion << " Available version: " << strAvailableVersion << " Status: " << strVersionStatus;
}

void Updates::saveSettings(QString strUpdateXml)
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

void Updates::readSettings()
{
    QString strMOTD = Settings::instance()->get("motd");
    QString strAvailableVersion = Settings::instance()->get("available_version");

    if (!strMOTD.isEmpty())
    {
        QString strMessageOfTheDay = QString("%Fb%%1 %2").arg(tr("Message Of The Day:"), strMOTD);
        Message::instance()->showMessage(STATUS_WINDOW, strMessageOfTheDay, MessageDefault);
    }

    if (!strAvailableVersion.isEmpty())
    {
        compareVersion();

        if (Settings::instance()->get("version_status") == "outofdate")
            DlgUpdate().exec();
    }
}

void Updates::updateFinished(QNetworkReply *reply)
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
        readSettings();
    }
}
