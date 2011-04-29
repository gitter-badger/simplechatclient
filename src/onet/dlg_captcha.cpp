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
#include <QNetworkReply>
#include <QPushButton>
#include <QUrl>
#include "dlg_captcha.h"

DlgCaptcha::DlgCaptcha(QNetworkAccessManager *param1, QString *param2)
{
    ui.setupUi(this);
    setWindowTitle(tr("Captcha"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    accessManager = param1;
    strCaptcha = param2;

    create_gui();
    create_signals();

    QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(network_finished(QNetworkReply*)));

    get_img();
}

void DlgCaptcha::create_gui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));

    ui.label_text->setText(tr("Enter the characters you see:"));
}

void DlgCaptcha::create_signals()
{
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
}

void DlgCaptcha::get_img()
{
    // clear
    ui.label_img->setText(tr("Loading..."));

    // request
    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl("http://czat.onet.pl/myimg.gif")));
    pReply->setProperty("category", "get_captcha");
}

void DlgCaptcha::got_img(QByteArray bData)
{
    // show img
    QPixmap pixmap;
    pixmap.loadFromData(bData);
    ui.label_img->setPixmap(pixmap);
}

void DlgCaptcha::network_finished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QString strCategory = reply->property("category").toString();
    QByteArray bData = reply->readAll();

    if (bData.isEmpty() == true)
        return;

    if (strCategory == "get_captcha")
        got_img(bData);
}

void DlgCaptcha::button_ok()
{
    (*strCaptcha) = ui.lineEdit_code->text();
    this->close();
}
