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

#ifndef DLG_USER_PROFILE_H
#define DLG_USER_PROFILE_H

class Network;
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ui_user_profile.h"

class DlgUserProfile : public QDialog
{
    Q_OBJECT
public:
    DlgUserProfile(QWidget *, Network *);
    void set_nick(QString);

public slots:
    void set_user_info(QString, QString, QString);

private:
    Ui::uiUserProfile ui;
    QWidget *myparent;
    Network *pNetwork;
    int iWidth; // widget width
    QPixmap avatar;
    QNetworkAccessManager accessManager;
    QNetworkReply *pReply;
    QString strNick;

    void clear_info();
    void show_info();
    QString convert_desc(QString);
    QString convert_sex(QString);
    QString convert_age(QString);
    QString convert_country(QString);
    QString convert_type(QString);
    void show_avatar(QString);

private slots:
    void avatar_finished();
    void button_zoom();
    void button_more();
    void button_close();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);
};

#endif // DLG_USER_PROFILE_H
