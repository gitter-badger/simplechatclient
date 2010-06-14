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

#include "update.h"

updater::updater(QSettings *param1, tab_container *param2)
{
    settings = param1;
    tabc = param2;
}

void updater::check_for_updates(QString param1)
{
    strCurrentVersion = settings->value("version").toString();
    QStringList lCurrentVersion = strCurrentVersion.split(".");
    QString strCurrentRev = lCurrentVersion.last();
    int iCurrentRev = strCurrentRev.toInt();

    strAvailableVersion = param1;
    if (strAvailableVersion == QString::null)
    {
        tabc->show_msg("Status", "Nie mo¿na pobraæ dostêpnej wersji programu.", 0);
        return;
    }
    QStringList lAvailableVersion = strAvailableVersion.split(".");
    QString strAvailableRev = lAvailableVersion.last();
    int iAvailableRev = strAvailableRev.toInt();

#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "Current rev: " << strCurrentRev << " Available rev: " << strAvailableRev;
#endif

    if (iCurrentRev == iAvailableRev)
        tabc->show_msg("Status", "U¿ywasz aktualnej wersji programu.", 0);
    else if (iCurrentRev > iAvailableRev)
        tabc->show_msg("Status", "U¿ywasz testowej wersji programu.", 0);
    else if (iCurrentRev < iAvailableRev)
    {
        tabc->show_msg("Status", "Wymagana aktualizacja do wersji: "+strAvailableVersion, 0);
        (new dlg_update(strAvailableVersion))->show();
    }
}

QString updater::get_available_version()
{
    QString strVersion;
    strVersion = QString::null;

    QEventLoop eventLoop;
    QNetworkReply *pReply;
    QNetworkAccessManager accessManager;
    QNetworkCookieJar *cookieJar = new QNetworkCookieJar();
    accessManager.setCookieJar(cookieJar);

    bool bHost = true;

    QHostInfo test_host = QHostInfo::fromName("simplechatclien.sourceforge.net");
    if (test_host.error() != QHostInfo::NoError)
         bHost = false;

    if (bHost == true)
    {
        QString strSendVersion = settings->value("version").toString();
        pReply = accessManager.get(QNetworkRequest(QUrl(QString("http://simplechatclien.sourceforge.net/update.php?version=%1").arg(strSendVersion))));
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        QString strSite = pReply->readAll();
        delete pReply;

        QDomDocument doc;
        doc.setContent(strSite);

        strVersion = doc.elementsByTagName("currentVersion").item(0).toElement().text();
    }
    else
        tabc->show_msg("Status", "Nie mo¿na po³±czyæ siê z serwerem aktualizacji.", 0);

    return strVersion;
}
