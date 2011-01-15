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

#include "dlg_email.h"

DlgEmail::DlgEmail(QWidget *parent, QString param1, QString param2) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Change email address"));

    strChannel = param1;
    strEmail = param2;

    ui.pushButton_ok->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.pushButton_refresh->setIcon(QIcon(":/images/oxygen/16x16/view-refresh.png"));
    ui.pushButton_cancel->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.label_msg->setText(tr("Enter the characters you see:"));
    ui.pushButton_ok->setText(tr("OK"));
    ui.pushButton_refresh->setText(tr("Refresh"));
    ui.pushButton_cancel->setText(tr("Cancel"));

    strChannel = strChannel.right(strChannel.length()-1);

    cookieJar = new QNetworkCookieJar();
    accessManager.setCookieJar(cookieJar);

    set_cookies();
}

void DlgEmail::set_cookies()
{
    QList <QNetworkCookie> cookieList;
    QNetworkCookie cookie;
    QSettings settings;

    cookie.setName("onet_ubi");
    cookie.setValue(settings.value("onet_ubi").toByteArray());
    cookieList.append(cookie);

    cookie.setName("onet_cid");
    cookie.setValue(settings.value("onet_cid").toByteArray());
    cookieList.append(cookie);

    cookie.setName("onet_sid");
    cookie.setValue(settings.value("onet_sid").toByteArray());
    cookieList.append(cookie);

    cookie.setName("onet_uid");
    cookie.setValue(settings.value("onet_uid").toByteArray());
    cookieList.append(cookie);

    accessManager.cookieJar()->setCookiesFromUrl(cookieList, QUrl("http://czat.onet.pl"));
}

void DlgEmail::get_img()
{
    // disable button
    ui.pushButton_refresh->setEnabled(false);

    QEventLoop eventLoop;

    bool bHost = true;

    QHostInfo hCzatOnetPl = QHostInfo::fromName("czat.onet.pl");
    if (hCzatOnetPl.error() != QHostInfo::NoError)
         bHost = false;

    if (bHost == true)
    {
        pReply = accessManager.get(QNetworkRequest(QUrl("http://czat.onet.pl/myimg.gif")));
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        QByteArray bData = pReply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bData);
        ui.label_img->setPixmap(pixmap);

        pReply->deleteLater();
    }
    else
        ui.label_img->setText(tr("Unable to download image"));

    // enable button
    ui.pushButton_refresh->setEnabled(true);
}

void DlgEmail::set_email()
{
    QEventLoop eventLoop;

    bool bHost = true;

    QHostInfo hCzatOnetPl = QHostInfo::fromName("czat.onet.pl");
    if (hCzatOnetPl.error() != QHostInfo::NoError)
         bHost = false;

    QString strData;

    if (bHost == true)
    {
        QString strChannelLength = QString::number(strChannel.length());
        QString strEmailLength = QString::number(strEmail.length());
        QString strCodeLength = QString::number(ui.lineEdit_code->text().length());
        QString strCode = ui.lineEdit_code->text();

        strData = QString("api_function=setChannelEmail&params=a:3:{s:4:\"name\";s:%1:\"%2\";s:5:\"email\";s:%3:\"%4\";s:4:\"code\";s:%5:\"%6\";}").arg(strChannelLength).arg(strChannel).arg(strEmailLength).arg(strEmail).arg(strCodeLength).arg(strCode);
        pReply = accessManager.post(QNetworkRequest(QUrl("http://czat.onet.pl/include/ajaxapi.xml.php3")), strData.toAscii());
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        QString strResult = pReply->readAll();

        pReply->deleteLater();

        parse_result(strResult);
    }
    else
    {
        ui.lineEdit_code->clear();
        get_img();
    }
}

// <?xml version="1.0" encoding="ISO-8859-2"?><root><status>0</status><error err_code="0"  err_text="OK" ></error></root>
///<?xml version="1.0" encoding="ISO-8859-2"?><root><error err_code="-7"  err_text="Nieprawid.owy kod CAPTCHA" ></error></root>
void DlgEmail::parse_result(QString strResult)
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
        ui.lineEdit_code->clear();

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setWindowTitle("Error");
        msgBox.setText(strErrText);
        msgBox.exec();

        get_img();
    }
}

void DlgEmail::button_ok()
{
    set_email();
}

void DlgEmail::button_refresh()
{
    ui.lineEdit_code->clear();
    get_img();
}

void DlgEmail::button_cancel()
{
    this->close();
}

void DlgEmail::showEvent(QShowEvent *event)
{
    event->accept();
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    QObject::connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(button_ok()));
    QObject::connect(ui.pushButton_refresh, SIGNAL(clicked()), this, SLOT(button_refresh()));
    QObject::connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(button_cancel()));
    get_img();
}

void DlgEmail::hideEvent(QHideEvent *event)
{
    event->accept();

    delete cookieJar;
    ui.pushButton_ok->QObject::disconnect();
    ui.pushButton_refresh->QObject::disconnect();
    ui.pushButton_cancel->QObject::disconnect();
}
