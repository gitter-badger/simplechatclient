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

#ifndef DLG_REGISTER_NICK_H
#define DLG_REGISTER_NICK_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include "ui_register_nick.h"

class DlgRegisterNick : public QDialog
{
    Q_OBJECT
public:
    DlgRegisterNick(QWidget *, QWidget *);
    ~DlgRegisterNick();

private:
    Ui::uiRegisterNick ui;
    QWidget *options;
    QNetworkAccessManager *accessManager;
    QNetworkCookieJar *cookieJar;
    QMap <QString, QString> mCookies;

    void get_cookies();
    void got_cookies();
    void get_img();
    void got_img(QByteArray);
    void register_nick();
    void parse_result(QString);

private slots:
    void network_finished(QNetworkReply*);
    void button_refresh();
    void button_ok();
    void button_cancel();
};

#endif // DLG_REGISTER_NICK_H
