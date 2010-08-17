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

#include "dlg_register_nick.h"

DlgRegisterNick::DlgRegisterNick(QWidget *parent, QWidget *param1, QSettings *param2) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Register nick"));

    options = param1;
    settings = param2;

    // close options
    options->close();

    ui.label_nick->setText(tr("Nick:"));
    ui.label_password->setText(tr("Password:"));
    ui.label_confirm_password->setText(tr("Confirm password:"));
    ui.label_code->setText(tr("Code:"));
    ui.pushButton_refresh->setText(tr("Refresh"));

    cookieJar = new QNetworkCookieJar();
    accessManager.setCookieJar(cookieJar);
}

void DlgRegisterNick::get_cookies()
{
    QEventLoop eventLoop;

    bool bHost = true;

    QHostInfo hKropkaOnetPl = QHostInfo::fromName("kropka.onet.pl");
    if (hKropkaOnetPl.error() != QHostInfo::NoError)
         bHost = false;

    if (bHost == true)
    {
        pReply = accessManager.get(QNetworkRequest(QUrl("http://kropka.onet.pl/_s/kropka/1?DV=czat%2Findex")));
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        // save cookies
        QList <QNetworkCookie> cookies = accessManager.cookieJar()->cookiesForUrl(QUrl("http://czat.onet.pl"));
        for (QList <QNetworkCookie>::iterator i = cookies.begin(); i != cookies.end(); ++i)
        {
            QString strKey = i->name();
            QString strValue = i->value();

            mCookies.insert(strKey, strValue);
        }

        delete pReply;
    }
}

void DlgRegisterNick::get_img()
{
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

        // show img
        QByteArray bData = pReply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bData);
        ui.label_img->setPixmap(pixmap);

        delete pReply;
    }
    else
        ui.label_img->setText(tr("Unable to download image"));
}

void DlgRegisterNick::register_nick()
{
    QEventLoop eventLoop;

    bool bHost = true;

    QHostInfo hCzatOnetPl = QHostInfo::fromName("czat.onet.pl");
    if (hCzatOnetPl.error() != QHostInfo::NoError)
         bHost = false;

    QString strData;

    if (bHost == true)
    {
        QString strNick = ui.lineEdit_nick->text();
        QString strNickLength = QString::number(strNick.length());
        QString strPassword = ui.lineEdit_password->text();
        QString strPasswordLength = QString::number(strPassword.length());
        QString strCode = ui.lineEdit_code->text();
        QString strCodeLength = QString::number(strCode.length());

        // set cookies
        QList <QNetworkCookie> cookieList;
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

        accessManager.cookieJar()->setCookiesFromUrl(cookieList, QUrl("http://czat.onet.pl"));

        // request
        strData = QString("api_function=registerNick&params=a:3:{s:4:\"nick\";s:%1:\"%2\";s:4:\"pass\";s:%3:\"%4\";s:4:\"code\";s:%5:\"%6\";}").arg(strNickLength).arg(strNick).arg(strPasswordLength).arg(strPassword).arg(strCodeLength).arg(strCode);
        pReply = accessManager.post(QNetworkRequest(QUrl("http://czat.onet.pl/include/ajaxapi.xml.php3")), strData.toAscii());
        QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        QString strResult = pReply->readAll();

        delete pReply;

        parse_result(strResult);
    }
    else
    {
        ui.lineEdit_code->clear();
        get_img();
    }
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

        if (strPassword.isEmpty() == false)
        {
            Crypt *pCrypt = new Crypt();
            strPassword = pCrypt->encrypt(strNick, strPassword);
            delete pCrypt;
        }

        // save values
        Config *pConfig = new Config();
        pConfig->set_value("login-nick", strNick);
        pConfig->set_value("login-pass", strPassword);
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
            strErrText = QString(tr("Unknown error: %1").arg(strErrCode));

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(strErrText);
        msgBox.exec();

        get_img();
    }
}

void DlgRegisterNick::button_ok()
{
    bool bPasswords = false;

    if (ui.lineEdit_password->text() == ui.lineEdit_confirm_password->text())
        bPasswords = true;

    if (bPasswords == true)
        register_nick();
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setText(tr("Given passwords are not identical."));
        msgBox.exec();

        ui.lineEdit_code->clear();
        get_img();
    }
}

void DlgRegisterNick::button_refresh()
{
    ui.lineEdit_code->clear();
    get_img();
}

void DlgRegisterNick::button_cancel()
{
    this->close();
}

void DlgRegisterNick::showEvent(QShowEvent *event)
{
    event->accept();

    QObject::connect(ui.pushButton_refresh, SIGNAL(clicked()), this, SLOT(button_refresh()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));

    get_cookies();
    get_img();
}

void DlgRegisterNick::hideEvent(QHideEvent *event)
{
    event->accept();

    delete cookieJar;
    ui.pushButton_refresh->QObject::disconnect();
    ui.buttonBox->QObject::disconnect();
}