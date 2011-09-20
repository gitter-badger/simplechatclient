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

#include <QDesktopWidget>
#include <QDir>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QPushButton>
#include <QUrl>
#include "core.h"
#include "mainwindow.h"
#include "dlg_update.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

DlgUpdate::DlgUpdate(MainWindow *parent, QString _strVersion) : QDialog(parent), strVersion(_strVersion)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Update"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    setDefaultValues();
    createSignals();

    accessManager = new QNetworkAccessManager;
    QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkFinished(QNetworkReply*)));
}

DlgUpdate::~DlgUpdate()
{
    accessManager->deleteLater();
}

void DlgUpdate::createGui()
{
#ifndef Q_WS_WIN
    ui.progressBar->hide();
    ui.pushButton_download->hide();
#endif
    ui.pushButton_download->setIcon(QIcon(":/images/oxygen/16x16/download.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_download->setText(tr("Download"));
}

void DlgUpdate::setDefaultValues()
{
    QString strLink;
#ifdef Q_WS_WIN
    strLink = tr("download");
#endif
#ifdef Q_WS_X11
    strLink += QString("<a href=\"http://simplechatclien.sourceforge.net/download/\">%1</a>").arg(tr("download"));
#endif

    QString strDisplay = QString(tr("A new version %1 is available. Please %2 an installer and upgrade.")).arg(strVersion).arg(strLink);
    ui.label_msg->setText(strDisplay);
}

void DlgUpdate::createSignals()
{
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    QObject::connect(ui.pushButton_download, SIGNAL(clicked()), this, SLOT(buttonDownload()));
}

void DlgUpdate::buttonDownload()
{
    ui.pushButton_download->setEnabled(false);

    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl("http://sourceforge.net/projects/simplechatclien/files/scc-"+strVersion+".exe/download")));
    pReply->setProperty("category", "site");
}

// <a href="http://downloads.sourceforge.net..." class="direct-download">
// http://leaseweb.dl.sourceforge.net/project/simplechatclien/scc-1.0.13.917.exe
void DlgUpdate::gotSite(QString site)
{
    site.replace(QRegExp(".*<a href=\"http://downloads.sourceforge.net.*use_mirror=(.*)\" class=\"direct-download\">.*"), "http://\\1.dl.sourceforge.net/project/simplechatclien/scc-"+strVersion+".exe");

    if (QUrl(site).isValid())
    {
        QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl(site)));
        pReply->setProperty("category", "file");
        QObject::connect(pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        QObject::connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    }
    else
        QMessageBox::critical(0, "", tr("Cannot download file"));
}

void DlgUpdate::gotFile(QByteArray bData)
{
    QString path;

#ifdef Q_WS_WIN
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    // create scc dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    // create update dir if not exist
    path += "/update";
    if (!QDir().exists(path))
        QDir().mkdir(path);

    // save
    QString fileName = "/scc-"+strVersion+".exe";

    QFile file(path+fileName);
    if (file.exists()) file.remove();
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0, "", tr("Cannot save file"));
        return;
    }
    file.write(bData);
    file.close();

    // execute
    QProcess pProcess;
    pProcess.setWorkingDirectory(path);
    ui.label_msg->setText(pProcess.workingDirectory());
    pProcess.startDetached("\""+path+fileName+"\"");

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
        QMessageBox::critical(0, "", tr("Cannot download file"));
        return;
    }

    if (strCategory == "site")
        gotSite(QString(bData));
    else if (strCategory == "file")
        gotFile(bData);
}

void DlgUpdate::downloadProgress(qint64 bytesReceived,qint64 bytesTotal)
{
    ui.progressBar->setValue(bytesReceived);
    ui.progressBar->setMaximum(bytesTotal);
}

void DlgUpdate::downloadError(QNetworkReply::NetworkError e)
{
    Q_UNUSED (e);
    QMessageBox::critical(0, "", tr("Cannot download file"));
}
