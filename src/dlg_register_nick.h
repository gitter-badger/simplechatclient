/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
#include <QDomDocument>
#include <QHideEvent>
#include <QHostInfo>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QSettings>
#include <QShowEvent>
#include <QUrl>
#include "config.h"
#include "crypt.h"
#include "ui_register_nick.h"

class DlgRegisterNick : public QDialog
{
    Q_OBJECT
public:
    DlgRegisterNick(QWidget *, QWidget *, QSettings *);

private:
    Ui::uiRegisterNick ui;
    QWidget *options;
    QSettings *settings;
    QNetworkReply *pReply;
    QNetworkAccessManager accessManager;
    QNetworkCookieJar *cookieJar;
    QMap <QString, QString> mCookies;

    void get_cookies();
    void get_img();
    void register_nick();
    void parse_result(QString);

private slots:
    void button_refresh();
    void button_ok();
    void button_cancel();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

};

#endif // DLG_REGISTER_NICK_H
