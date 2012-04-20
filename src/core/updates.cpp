/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
#include <QNetworkReply>
#include <QStringList>
#include "core.h"
#include "update.h"
#include "updates.h"

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
    QString strSendVersion = Core::instance()->settings.value("version");
    QUrl url = QUrl(QString("http://simplechatclien.sourceforge.net/update.php?v=%1").arg(strSendVersion));

    accessManager->get(QNetworkRequest(url));
}

void Updates::compareVersion()
{
    QString strCurrentVersion = Core::instance()->settings.value("version");
    QStringList lCurrentVersion = strCurrentVersion.split(".");
    int iCurrentMajor = lCurrentVersion[0].toInt();
    int iCurrentMinor = lCurrentVersion[1].toInt();
    int iCurrentPatch = lCurrentVersion[2].toInt();
    int iCurrentVersion = (QString("%1%2%3").arg(iCurrentMajor).arg(iCurrentMinor).arg(iCurrentPatch)).toInt();

    QString strAvailableVersion = Core::instance()->settings.value("available_version");
    QStringList lAvailableVersion = strAvailableVersion.split(".");
    int iAvailableMajor = lAvailableVersion[0].toInt();
    int iAvailableMinor = lAvailableVersion[1].toInt();
    int iAvailablePatch = lAvailableVersion[2].toInt();
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
    Core::instance()->settings["version_status"] = strVersionStatus;

    if (Core::instance()->settings.value("debug") == "true")
        qDebug() << "Current version: " << strCurrentVersion << " Available version: " << strAvailableVersion << " Status: " << strVersionStatus;
}

void Updates::updateFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    // if errors
    if (reply->error())
        return;

    QString strSite = reply->readAll();

    if (!strSite.isEmpty())
    {
        QDomDocument doc;
        doc.setContent(strSite);

        QString strAvailableVersion = doc.elementsByTagName("version").item(0).toElement().text();
        QString strWhatsNew = doc.elementsByTagName("whats_new").item(0).toElement().text();
        QString strMOTD = doc.elementsByTagName("motd").item(0).toElement().text();

        if (!strWhatsNew.isEmpty())
            Core::instance()->settings["whats_new"] = strWhatsNew;

        if (!strMOTD.isEmpty())
        {
            Core::instance()->settings["motd"] = strMOTD;
            QString strMessageOfTheDay = QString("%Fb%%1 %2").arg(tr("Message Of The Day:"), strMOTD);
            Core::instance()->showMessage(STATUS, strMessageOfTheDay, DefaultMessage);
        }

        if (!strAvailableVersion.isEmpty())
        {
            Core::instance()->settings["available_version"] = strAvailableVersion;

            compareVersion();

            if (Core::instance()->settings["version_status"] == "outofdate")
                DlgUpdate(Core::instance()->mainWindow()).exec();
        }
    }
}
