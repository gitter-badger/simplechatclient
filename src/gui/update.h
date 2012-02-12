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

#ifndef UPDATE_H
#define UPDATE_H

class QNetworkAccessManager;
class MainWindow;
#include <QDialog>
#include <QNetworkReply>
#include "ui_update.h"

class DlgUpdate : public QDialog
{
    Q_OBJECT
public:
    DlgUpdate(MainWindow *, const QString&);
    virtual ~DlgUpdate();

private:
    Ui::uiUpdate ui;
    QString strVersion;
    QNetworkAccessManager *accessManager;

    void createGui();
    void setDefaultValues();
    void createSignals();
    void gotSite(QString);
    void gotFile(const QByteArray&);

private slots:
    void buttonDownload();
    void networkFinished(QNetworkReply*);
    void downloadProgress(qint64,qint64);
    void downloadError(QNetworkReply::NetworkError);
};

#endif // UPDATE_H
