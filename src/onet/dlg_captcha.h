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

#ifndef DLG_CAPTCHA_H
#define DLG_CAPTCHA_H

#include <QDialog>
#include <QNetworkAccessManager>
#include "ui_captcha.h"

class DlgCaptcha : public QDialog
{
    Q_OBJECT
public:
    DlgCaptcha(QNetworkAccessManager *, QString *);

private:
    Ui::uiCaptcha ui;
    QString *strCaptcha;
    QNetworkAccessManager *accessManager;

private:
    void create_gui();
    void create_signals();
    void get_img();
    void got_img(QByteArray);

private slots:
    void network_finished(QNetworkReply *);
    void button_ok();
};

#endif // DLG_CAPTCHA_H
