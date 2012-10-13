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

#ifndef REGISTER_NICK_H
#define REGISTER_NICK_H

#include <QDialog>
#include "ui_register_nick.h"
class DlgProfileAdd;

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkCookieJar;
class QNetworkReply;
QT_END_NAMESPACE

class DlgRegisterNick : public QDialog
{
    Q_OBJECT
public:
    DlgRegisterNick(DlgProfileAdd *_pDlgProfileAdd, QWidget *parent = 0);
    virtual ~DlgRegisterNick();

private:
    enum RegisterType
    {
        RT_img,
        RT_cookies,
        RT_register
    };

    Ui::uiRegisterNick ui;
    DlgProfileAdd *pDlgProfileAdd;
    QNetworkAccessManager *accessManager;
    QNetworkCookieJar *cookieJar;
    QHash<QString,QString> mCookies;

    void createGui();
    void createSignals();

    void getCookies();
    void gotCookies();
    void getImg();
    void gotImg(const QByteArray &bData);
    void registerNick();
    void parseResult(const QString &strResult);

private slots:
    void networkFinished(QNetworkReply *reply);
    void buttonRefresh();
    void buttonOk();
};

#endif // REGISTER_NICK_H
