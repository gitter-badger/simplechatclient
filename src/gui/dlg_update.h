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

#ifndef DLG_UPDATE_H
#define DLG_UPDATE_H

class MainWindow;
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ui_update.h"

class DlgUpdate : public QDialog
{
    Q_OBJECT
public:
    DlgUpdate(MainWindow *, QString);
    virtual ~DlgUpdate();

private:
    Ui::uiUpdate ui;
    QString strVersion;
    QNetworkAccessManager *accessManager;

    void create_gui();
    void set_default_values();
    void create_signals();
    void got_site(QString);
    void got_file(QByteArray);

private slots:
    void button_download();
    void network_finished(QNetworkReply*);
    void downloadProgress(qint64,qint64);
    void downloadError(QNetworkReply::NetworkError);
};

#endif // DLG_UPDATE_H
