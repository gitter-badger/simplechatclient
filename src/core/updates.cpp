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
    QUrl url = QUrl(QString("http://simplechatclien.sourceforge.net/update.php?version=%1").arg(strSendVersion));

    accessManager->get(QNetworkRequest(url));
}

void Updates::version(QString strAvailableVersion)
{
    Core::instance()->settings["available_version"] = strAvailableVersion;

    QString strCurrentVersion = Core::instance()->settings.value("version");

    QStringList lCurrentVersion = strCurrentVersion.split(".");
    QString strCurrentRev = lCurrentVersion.last();
    int iCurrentRev = strCurrentRev.toInt();

    QStringList lAvailableVersion = strAvailableVersion.split(".");
    QString strAvailableRev = lAvailableVersion.last();
    int iAvailableRev = strAvailableRev.toInt();

#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "on")
            qDebug() << "Current rev: " << strCurrentRev << " Available rev: " << strAvailableRev;
#endif

    if (iCurrentRev < iAvailableRev)
        DlgUpdate(Core::instance()->sccWindow(), strAvailableVersion).exec();
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

        QString strVersion = doc.elementsByTagName("currentVersion").item(0).toElement().text();

        if (!strVersion.isEmpty())
            version(strVersion);
    }
}
