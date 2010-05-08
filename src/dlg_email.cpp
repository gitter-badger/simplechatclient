/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#include "dlg_email.h"

dlg_email::dlg_email(QString param1, QString param2)
{
    ui.setupUi(this);

    strChannel = param1;
    strEmail = param2;

    strChannel = strChannel.right(strChannel.length()-1);

    cookieJar = new QNetworkCookieJar();
    accessManager.setCookieJar(cookieJar);
}

void dlg_email::get_img()
{
    QEventLoop eventLoop;

    bool bHost = true;

    QHostInfo test_host = QHostInfo::fromName("czat.onet.pl");
    if (test_host.error() != QHostInfo::NoError)
         bHost = false;

    if (bHost == true)
    {
        pReply = accessManager.get(QNetworkRequest(QUrl("http://czat.onet.pl/myimg.gif")));
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        QByteArray bData = pReply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bData);
        ui.label_2->setPixmap(pixmap);

        delete pReply;
    }
    else
        ui.label_2->setText("Nie mo¿na pobraæ obrazka");
}

void dlg_email::set_email()
{
    QEventLoop eventLoop;

    bool bHost = true;

    QHostInfo test_host = QHostInfo::fromName("czat.onet.pl");
    if (test_host.error() != QHostInfo::NoError)
         bHost = false;

    QString strResult;

    if (bHost == true)
    {
        QString strData = QString("api_function=setChannelEmail&params=a:3:{s:4:\"name\";s:%1:\"%2\";s:5:\"email\";s:%3:\"%4\";s:4:\"code\";s:%5:\"%6\";}").arg(QString::number(strChannel.length())).arg(strChannel).arg(QString::number(strEmail.length())).arg(strEmail).arg(QString::number(ui.lineEdit->text().length())).arg(ui.lineEdit->text());
        pReply = accessManager.post(QNetworkRequest(QUrl("http://czat.onet.pl/include/ajaxapi.xml.php3")), strData.toAscii());
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        strResult = pReply->readAll();

        delete pReply;

        dlg_email::parse_result(strResult);
    }
    else
    {
        ui.lineEdit->clear();
        dlg_email::get_img();
    }

}

// <?xml version="1.0" encoding="ISO-8859-2"?><root><status>0</status><error err_code="0"  err_text="OK" ></error></root>
///<?xml version="1.0" encoding="ISO-8859-2"?><root><error err_code="-7"  err_text="Nieprawid.owy kod CAPTCHA" ></error></root>
void dlg_email::parse_result(QString strResult)
{
    QDomDocument doc;
    doc.setContent(strResult);

    QDomNode dError = doc.elementsByTagName("error").item(0);
    QString strErrCode = dError.attributes().namedItem("err_code").nodeValue();
    QString strErrText = dError.attributes().namedItem("err_text").nodeValue();

    if (strErrCode == "0")
    {
        this->close();
    }
    else
    {
        ui.lineEdit->clear();

        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(strErrText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();

        dlg_email::get_img();
    }
}

void dlg_email::button_ok()
{
    dlg_email::set_email();
}

void dlg_email::button_refresh()
{
    dlg_email::get_img();
}

void dlg_email::button_cancel()
{
    this->close();
}

void dlg_email::showEvent(QShowEvent *event)
{
    event->accept();

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(button_ok()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button_refresh()));
    QObject::connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(button_cancel()));
    dlg_email::get_img();
}

void dlg_email::hideEvent(QHideEvent *event)
{
    event->accept();

    ui.pushButton->QObject::disconnect();
    ui.pushButton_2->QObject::disconnect();
    ui.pushButton_3->QObject::disconnect();
}
