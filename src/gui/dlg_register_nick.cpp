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
#include <QDomDocument>
#include <QMessageBox>
#include <QNetworkReply>
#include <QSettings>
#include <QUrl>
#include "config.h"
#include "crypt.h"
#include "mainwindow.h"
#include "dlg_register_nick.h"

DlgRegisterNick::DlgRegisterNick(MainWindow *parent, QWidget *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Register nick"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    options = param1;

    // close options
    options->close();

    create_gui();
    create_signals();

    accessManager = new QNetworkAccessManager;
    cookieJar = new QNetworkCookieJar();
    accessManager->setCookieJar(cookieJar);
    QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(network_finished(QNetworkReply*)));

    get_cookies();
    get_img();
}

DlgRegisterNick::~DlgRegisterNick()
{
    delete cookieJar;
    accessManager->deleteLater();
}

void DlgRegisterNick::create_gui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
    ui.pushButton_refresh->setIcon(QIcon(":/images/oxygen/16x16/view-refresh.png"));

    ui.label_nick->setText(tr("Nick:"));
    ui.label_password->setText(tr("Password:"));
    ui.label_confirm_password->setText(tr("Confirm password:"));
    ui.label_code->setText(tr("Code:"));
    ui.pushButton_refresh->setText(tr("Refresh"));
}

void DlgRegisterNick::create_signals()
{
    QObject::connect(ui.pushButton_refresh, SIGNAL(clicked()), this, SLOT(button_refresh()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgRegisterNick::get_cookies()
{
    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl("http://kropka.onet.pl/_s/kropka/1?DV=czat%2Findex")));
    pReply->setProperty("category", "get_cookies");
}

void DlgRegisterNick::got_cookies()
{
    // save cookies
    QList<QNetworkCookie> cookies = accessManager->cookieJar()->cookiesForUrl(QUrl("http://czat.onet.pl"));
    for (QList<QNetworkCookie>::iterator i = cookies.begin(); i != cookies.end(); ++i)
    {
        QString strKey = i->name();
        QString strValue = i->value();

        mCookies.insert(strKey, strValue);
    }
}

void DlgRegisterNick::get_img()
{
    // disable button
    ui.pushButton_refresh->setEnabled(false);

    // clear
    ui.label_img->setText(tr("Loading..."));

    // request
    QNetworkReply *pReply = accessManager->get(QNetworkRequest(QUrl("http://czat.onet.pl/myimg.gif")));
    pReply->setProperty("category", "get_img");
}

void DlgRegisterNick::got_img(QByteArray bData)
{
    // show img
    QPixmap pixmap;
    pixmap.loadFromData(bData);
    ui.label_img->setPixmap(pixmap);

    // enable button
    ui.pushButton_refresh->setEnabled(true);
}

void DlgRegisterNick::register_nick()
{
    QString strNick = ui.lineEdit_nick->text();
    QString strNickLength = QString::number(strNick.length());
    QString strPassword = ui.lineEdit_password->text();
    QString strPasswordLength = QString::number(strPassword.length());
    QString strCode = ui.lineEdit_code->text();
    QString strCodeLength = QString::number(strCode.length());

    if ((strNick.isEmpty()) || (strPassword.isEmpty()) || (strCode.isEmpty()))
        return;

    // set cookies
    QList<QNetworkCookie> cookieList;
    QNetworkCookie cookie;

    QMapIterator <QString, QString> i(mCookies);
    while (i.hasNext())
    {
         i.next();

         QString strKey = i.key();
         QString strValue = i.value();

         cookie.setName(strKey.toAscii());
         cookie.setValue(strValue.toAscii());
         cookieList.append(cookie);
     }

    accessManager->cookieJar()->setCookiesFromUrl(cookieList, QUrl("http://czat.onet.pl"));

    // request
    QString strData = QString("api_function=registerNick&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:4:\"pass\";s:%3:\"%4\";s:4:\"code\";s:%5:\"%6\";}").arg(strNickLength).arg(strNick).arg(strPasswordLength).arg(strPassword).arg(strCodeLength).arg(strCode);
    QNetworkReply *pReply = accessManager->post(QNetworkRequest(QUrl("http://czat.onet.pl/include/ajaxapi.xml.php3")), strData.toAscii());
    pReply->setProperty("category", "register_nick");
}

// <?xml version="1.0" encoding="ISO-8859-2"?><root><status>-104</status><error err_code="0"  err_text="OK" ></error></root>
// <?xml version="1.0" encoding="ISO-8859-2"?><root><status>-3</status><error err_code="0"  err_text="OK" ></error></root>
// <?xml version="1.0" encoding="ISO-8859-2"?><root><status>-2</status><error err_code="0"  err_text="OK" ></error></root>
// <?xml version="1.0" encoding="ISO-8859-2"?><root><status>-1</status><error err_code="0"  err_text="OK" ></error></root>
// <?xml version="1.0" encoding="ISO-8859-2"?><root><status>1</status><error err_code="0"  err_text="OK" ></error></root>
void DlgRegisterNick::parse_result(QString strResult)
{
    QDomDocument doc;
    doc.setContent(strResult);

    QString strErrCode = doc.elementsByTagName("status").item(0).toElement().text();

    // correct
    if (strErrCode == "1")
    {
        // encrypt pass
        QString strNick = ui.lineEdit_nick->text();
        QString strPassword = ui.lineEdit_password->text();

        if (!strPassword.isEmpty())
        {
            Crypt *pCrypt = new Crypt();
            strPassword = pCrypt->encrypt(strNick, strPassword);
            delete pCrypt;
        }

        // save values
        QSettings settings;
        settings.setValue("nick", strNick);
        settings.setValue("pass", strPassword);

        Config *pConfig = new Config();
        pConfig->setValue("nick", strNick);
        pConfig->setValue("pass", strPassword);
        delete pConfig;

        // close
        this->close();
    }
    else
    {
        ui.lineEdit_code->clear();

        QString strErrText;
        if (strErrCode == "-1")
            strErrText = tr("Invalid password. Password must be at least 6 characters including at least one number.");
        else if (strErrCode == "-2")
            strErrText = tr("Nick already exist");
        else if (strErrCode == "-3")
            strErrText = tr("Invalid nick");
        else if (strErrCode == "-104")
            strErrText = tr("Invalid code");
        else
            strErrText = QString(tr("Unknown error: %1")).arg(strErrCode);

        QMessageBox::critical(0, tr("Error"), strErrText);
        get_img();
    }
}

void DlgRegisterNick::network_finished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error())
        return;

    QString strCategory = reply->property("category").toString();
    QByteArray bData = reply->readAll();

    if (bData.isEmpty())
        return;

    if (strCategory == "get_img")
        got_img(bData);
    else if (strCategory == "get_cookies")
        got_cookies();
    else if (strCategory == "register_nick")
        parse_result(QString(bData));
}

void DlgRegisterNick::button_ok()
{
    bool identical = false;

    if (ui.lineEdit_password->text() == ui.lineEdit_confirm_password->text())
        identical = true;

    if (identical)
        register_nick();
    else
    {
        ui.lineEdit_code->clear();
        QMessageBox::critical(0, tr("Error"), tr("Given passwords are not identical."));
        get_img();
    }
}

void DlgRegisterNick::button_refresh()
{
    ui.lineEdit_code->clear();
    get_img();
}
