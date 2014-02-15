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

#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QPushButton>
#include <QUrl>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebPage>
#include "settings.h"
#include "update_gui.h"

#define DOWNLOAD_LINK "http://simplechatclien.sourceforge.net/download/"
#define DOWNLOAD_SITE_LINK "http://sourceforge.net/projects/simplechatclien/files/scc-%1.exe/download"
#define DOWNLOAD_DIRECT_LINK "http://%1.dl.sourceforge.net/project/simplechatclien/scc-%2.exe"

UpdateGui::UpdateGui(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Update"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    setDefaultValues();
    createSignals();

    accessManager = new QNetworkAccessManager;
    cookieJar = new QNetworkCookieJar();
    accessManager->setCookieJar(cookieJar);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkFinished(QNetworkReply*)));
}

UpdateGui::~UpdateGui()
{
    accessManager->deleteLater();
    cookieJar->deleteLater();
}

void UpdateGui::createGui()
{
#ifndef Q_WS_WIN
    ui.progressBar->hide();
#endif
    ui.pushButton_download->setIcon(QIcon(":/images/oxygen/16x16/download.png"));
    ui.pushButton_download->setText(tr("Download"));
}

void UpdateGui::setDefaultValues()
{
    strVersion = Settings::instance()->get("available_version");

    QStringList lAvailableVersion = strVersion.split(".");
    QString strCurrentMajor = lAvailableVersion.at(0);
    QString strCurrentMinor = lAvailableVersion.at(1);
    QString strCurrentPatch = lAvailableVersion.at(2);

    QString strShortVersion = QString("%1.%2.%3").arg(strCurrentMajor).arg(strCurrentMinor).arg(strCurrentPatch);

    ui.label_title->setText(QString("Simple Chat Client %1").arg(strShortVersion));
    ui.label_content->setText(Settings::instance()->get("whats_new"));
}

void UpdateGui::createSignals()
{
    connect(ui.pushButton_download, SIGNAL(clicked()), this, SLOT(buttonDownload()));
}

void UpdateGui::buttonDownload()
{
    ui.pushButton_download->setEnabled(false);

#ifdef Q_WS_WIN
    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(QString(DOWNLOAD_SITE_LINK).arg(strVersion))));
    pReply->setProperty("category", "sfsite");
#else
    QDesktopServices::openUrl(QUrl(DOWNLOAD_LINK));
#endif
}

// <meta http-equiv="refresh" content="5; url=https://downloads.sourceforge.net/project/simplechatclien/scc-1.6.2.0.exe?r=&amp;ts=1354737755&amp;use_mirror=switch">
// http://leaseweb.dl.sourceforge.net/project/simplechatclien/scc-1.0.13.917.exe
void UpdateGui::gotSFSite(QString site)
{
    QWebPage page;
    page.mainFrame()->setHtml(site);

    QWebElement document = page.mainFrame()->documentElement();
    QWebElement head = document.findFirst("head");
    QWebElement noscript = head.findFirst("noscript");

    QString strNoScript = noscript.toPlainText();
    QString strMirror = strNoScript.replace(QRegExp(".*use_mirror=(\\w+).*"), "\\1");

    if (strMirror.size() < 25)
    {
        QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(QString(DOWNLOAD_DIRECT_LINK).arg(strMirror, strVersion))));
        pReply->setProperty("category", "file");
        connect(pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    }
    else
        showError(tr("Cannot download file"));
}

void UpdateGui::gotFile(const QByteArray &bData)
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

void UpdateGui::networkFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
    {
        showError(tr("Cannot download file"));
        return;
    }

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

void UpdateGui::downloadProgress(qint64 bytesReceived,qint64 bytesTotal)
{
    ui.progressBar->setValue(bytesReceived);
    ui.progressBar->setMaximum(bytesTotal);
}

void UpdateGui::downloadError(QNetworkReply::NetworkError)
{
    showError(tr("Cannot download file"));
}

void UpdateGui::showError(const QString &strError)
{
    ui.label_title->setText(tr("Error"));
    ui.label_content->setText(QString("<center><b>%1</b></center>").arg(strError));
}
