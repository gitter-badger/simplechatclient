/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QUrl>
#include "core.h"
#include "mainwindow.h"
#include "update.h"

#define UPDATE_LINK_1 "http://simplechatclien.sourceforge.net/download/"
#define UPDATE_LINK_2 "https://github.com/simplechatclient/simplechatclient/downloads"

#define UPDATE_DOWNLOAD_LINK_1 "http://sourceforge.net/projects/simplechatclien/files/"
#define UPDATE_DOWNLOAD_LINK_2 "http://cloud.github.com/downloads/simplechatclient/simplechatclient/"

DlgUpdate::DlgUpdate(MainWindow *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Update"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    setDefaultValues();
    createSignals();

    accessManager = new QNetworkAccessManager;
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkFinished(QNetworkReply*)));
}

DlgUpdate::~DlgUpdate()
{
    accessManager->deleteLater();
}

void DlgUpdate::createGui()
{
#ifndef Q_WS_WIN
    ui.progressBar->hide();
#endif
    ui.pushButton_download->setIcon(QIcon(":/images/oxygen/16x16/download.png"));
    ui.pushButton_download->setText(tr("Download"));
}

void DlgUpdate::setDefaultValues()
{
    strVersion = Settings::instance()->get("available_version");

    QStringList lAvailableVersion = strVersion.split(".");
    QString strCurrentMajor = lAvailableVersion[0];
    QString strCurrentMinor = lAvailableVersion[1];
    QString strCurrentPatch = lAvailableVersion[2];

    QString strShortVersion = QString("%1.%2.%3").arg(strCurrentMajor).arg(strCurrentMinor).arg(strCurrentPatch);

    ui.label_title->setText(QString("Simple Chat Client %1").arg(strShortVersion));
    ui.label_content->setText(Settings::instance()->get("whats_new"));
}

void DlgUpdate::createSignals()
{
    connect(ui.pushButton_download, SIGNAL(clicked()), this, SLOT(buttonDownload()));
}

void DlgUpdate::buttonDownload()
{
    ui.pushButton_download->setEnabled(false);

    int update_url = Core::instance()->settings["update_url"].toInt();

#ifdef Q_WS_WIN
    if (update_url == 1)
    {
        QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(QString("%1scc-%2.exe/download").arg(UPDATE_DOWNLOAD_LINK_1, strVersion))));
        pReply->setProperty("category", "sfsite");
    }
    else if (update_url == 2)
    {
        QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(QString("%1scc-%2.exe").arg(UPDATE_DOWNLOAD_LINK_2, strVersion))));
        pReply->setProperty("category", "file");
        connect(pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    }
#else
    QString strUrl;
    if (update_url == 1) strUrl = UPDATE_LINK_1;
    else if (update_url == 2) strUrl = UPDATE_LINK_2;

    QDesktopServices::openUrl(QUrl(strUrl));
#endif
}

// <a href="http://downloads.sourceforge.net..." class="direct-download">
// http://leaseweb.dl.sourceforge.net/project/simplechatclien/scc-1.0.13.917.exe
void DlgUpdate::gotSFSite(QString site)
{
    site.replace(QRegExp(".*<a href=\"http://downloads.sourceforge.net.*use_mirror=(.*)\" class=\"direct-download\">.*"), "http://\\1.dl.sourceforge.net/project/simplechatclien/scc-"+strVersion+".exe");

    if (QUrl(site).isValid())
    {
        QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(site)));
        pReply->setProperty("category", "file");
        connect(pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    }
    else
        showError(tr("Cannot download file"));
}

void DlgUpdate::gotFile(const QByteArray &bData)
{
    QString path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).absoluteFilePath();

    // save
    QString fileName = path+"/scc-"+strVersion+".exe";

    QFile file(fileName);
    if (file.exists()) file.remove();
    if (!file.open(QIODevice::WriteOnly))
    {
        showError(tr("Cannot save file"));
        return;
    }
    file.write(bData);
    file.close();

    // execute
    QString strExecute = QDir::toNativeSeparators(fileName);
    bool bExecute = QDesktopServices::openUrl(QUrl::fromLocalFile(strExecute));

    if (!bExecute)
    {
        showError(tr("Cannot open saved file"));
        return;
    }

    exit(0);
}

void DlgUpdate::networkFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QString strCategory = reply->property("category").toString();
    QByteArray bData = reply->readAll();

    if (bData.isEmpty())
    {
        showError(tr("Cannot download file"));
        return;
    }

    if (strCategory == "sfsite")
        gotSFSite(QString(bData));
    else if (strCategory == "file")
        gotFile(bData);
}

void DlgUpdate::downloadProgress(qint64 bytesReceived,qint64 bytesTotal)
{
    ui.progressBar->setValue(bytesReceived);
    ui.progressBar->setMaximum(bytesTotal);
}

void DlgUpdate::downloadError(QNetworkReply::NetworkError)
{
    showError(tr("Cannot download file"));
}

void DlgUpdate::showError(const QString &strError)
{
    ui.label_title->setText(tr("Error"));
    ui.label_content->setText(QString("<center><b>%1</b></center>").arg(strError));
}
