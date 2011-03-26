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

#ifndef DLG_EMAIL_H
#define DLG_EMAIL_H

class MainWindow;
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include "ui_email.h"

class DlgEmail : public QDialog
{
    Q_OBJECT
public:
    DlgEmail(MainWindow *, QString, QString);
    virtual ~DlgEmail();

private:
    Ui::uiEmail ui;
    QString strChannel;
    QString strEmail;
    QNetworkAccessManager *accessManager;
    QNetworkCookieJar *cookieJar;

    void get_cookies();
    void get_img();
    void got_img(QByteArray);
    void set_email();
    void parse_result(QString);

private slots:
    void network_finished(QNetworkReply*);
    void button_ok();
    void button_refresh();
    void button_cancel();
};

#endif // DLG_EMAIL_H
