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

#include "dlg_user_profile.h"

DlgUserProfile::DlgUserProfile(QWidget *parent, Network *param1, QSettings *param2, sNickInfo param3) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Profile"));

    pNetwork = param1;
    settings = param2;
    sCurrentNickInfo = param3;

    ui.label_sex->setText(tr("Sex:"));
    ui.label_birthdate->setText(tr("Birthdate:"));
    ui.label_city->setText(tr("City:"));
    ui.label_country->setText(tr("Country:"));
    ui.label_shortDesc->setText(tr("ShortDesc:"));
    ui.label_type->setText(tr("Type:"));
    ui.label_email->setText(tr("Email:"));
    ui.label_www->setText(tr("Website:"));
}

void DlgUserProfile::show_info()
{
    ui.label_avatar->setText("");
    ui.label_nick->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(sCurrentNickInfo.nick));
    ui.plainTextEdit_longDesc->setPlainText(sCurrentNickInfo.longDesc);
    ui.lineEdit_sex->setText(convert_sex(sCurrentNickInfo.sex));
    ui.lineEdit_birthdate->setText(sCurrentNickInfo.birthdate);
    ui.lineEdit_city->setText(sCurrentNickInfo.city);
    ui.lineEdit_country->setText(sCurrentNickInfo.country);
    ui.plainTextEdit_shortDesc->setPlainText(sCurrentNickInfo.shortDesc);
    ui.lineEdit_type->setText(convert_type(sCurrentNickInfo.type));
    ui.lineEdit_email->setText(sCurrentNickInfo.email);
    ui.lineEdit_www->setText(sCurrentNickInfo.www);

    if (sCurrentNickInfo.avatar.isEmpty() == false)
        show_avatar(sCurrentNickInfo.avatar);
}

QString DlgUserProfile::convert_sex(QString strSex)
{
    if (strSex == "F")
        strSex = tr("Female");
    else if (strSex == "M")
        strSex = tr("Male");

    return strSex;
}

QString DlgUserProfile::convert_type(QString strType)
{
    if (strType == "0")
        strType = tr("Novice");
    else if (strType == "1")
        strType = tr("Beginner");
    else if (strType == "2")
        strType = tr("Master");
    else if (strType == "3")
        strType = tr("Guru");

    return strType;
}

void DlgUserProfile::show_avatar(QString strUrl)
{
    if (strUrl.indexOf(",") == -1) return; // wrong url

    // change url
    QStringList lUrl = strUrl.split(",");
    lUrl[1] = "3";
    strUrl = lUrl.join(",");

    // get url
    QNetworkAccessManager accessManager;
    QNetworkReply *pReply;
    QEventLoop eventLoop;
    pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
    QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QByteArray bData = pReply->readAll();

    // show avatar
    QPixmap pixmap;
    pixmap.loadFromData(bData);
    ui.label_avatar->setPixmap(pixmap);
}

void DlgUserProfile::button_close()
{
    this->close();
}

void DlgUserProfile::showEvent(QShowEvent *event)
{
    event->accept();

    show_info();

    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
}
