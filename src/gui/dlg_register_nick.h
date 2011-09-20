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

class QNetworkAccessManager;
class QNetworkCookieJar;
class QNetworkReply;
class DlgProfileAdd;
class MainWindow;
#include <QDialog>
#include "ui_register_nick.h"

class DlgRegisterNick : public QDialog
{
    Q_OBJECT
public:
    DlgRegisterNick(MainWindow *, DlgProfileAdd *);
    virtual ~DlgRegisterNick();

private:
    Ui::uiRegisterNick ui;
    DlgProfileAdd *pDlgProfileAdd;
    QNetworkAccessManager *accessManager;
    QNetworkCookieJar *cookieJar;
    QMap<QString,QString> mCookies;

    void createGui();
    void createSignals();

    void getCookies();
    void gotCookies();
    void getImg();
    void gotImg(QByteArray);
    void registerNick();
    void parseResult(QString);

private slots:
    void networkFinished(QNetworkReply*);
    void buttonRefresh();
    void buttonOk();
};

#endif // DLG_REGISTER_NICK_H
