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
#include <QHostInfo>
#include <QSettings>
#include <QStringList>
#include "dlg_update.h"
#include "update.h"

Update::Update(QWidget *parent)
{
    myparent = parent;
}

void Update::check_update()
{
    QHostInfo test_host = QHostInfo::fromName("simplechatclien.sourceforge.net");
    if (test_host.error() == QHostInfo::NoError)
    {
        QSettings settings;
        QString strSendVersion = settings.value("version").toString();

        pReply = accessManager.get(QNetworkRequest(QUrl(QString("http://simplechatclien.sourceforge.net/update.php?version=%1").arg(strSendVersion))));
        QObject::connect(pReply, SIGNAL(finished()), this, SLOT(update_finished()));
    }
}

void Update::version(QString strAvailableVersion)
{
    QSettings settings;
    QString strCurrentVersion = settings.value("version").toString();
    QStringList lCurrentVersion = strCurrentVersion.split(".");
    QString strCurrentRev = lCurrentVersion.last();
    int iCurrentRev = strCurrentRev.toInt();

    if (strAvailableVersion == QString::null)
    {
        //tabc->show_msg("Status", tr("Can not download the available version."), 0);
        return;
    }
    QStringList lAvailableVersion = strAvailableVersion.split(".");
    QString strAvailableRev = lAvailableVersion.last();
    int iAvailableRev = strAvailableRev.toInt();

#ifdef Q_WS_X11
        if (settings.value("debug").toString() == "on")
            qDebug() << "Current rev: " << strCurrentRev << " Available rev: " << strAvailableRev;
#endif

    //if (iCurrentRev == iAvailableRev)
        //tabc->show_msg("Status", tr("You are using up-to-date version."), 0);
    //else if (iCurrentRev > iAvailableRev)
        //tabc->show_msg("Status", tr("You are using a test version."), 0);
    if (iCurrentRev < iAvailableRev)
    {
        //tabc->show_msg("Status", tr("A new version is available."), 0);
        (new DlgUpdate(myparent, strAvailableVersion))->show();
    }
}

void Update::update_finished()
{
    QString strSite = pReply->readAll();
    pReply->QObject::disconnect();
    pReply->deleteLater();

    if (strSite.isEmpty() == false)
    {
        QDomDocument doc;
        doc.setContent(strSite);

        QString strVersion = doc.elementsByTagName("currentVersion").item(0).toElement().text();
        version(strVersion);
    }
}
