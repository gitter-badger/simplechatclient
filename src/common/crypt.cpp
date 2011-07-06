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

#include <QtCrypto>
// in OpenSuSE 11 try this:
//#include <QtCrypto/QtCrypto>
#include "config.h"
#include "crypt.h"

#ifdef Q_WS_X11
#include <QDebug>
#endif

Crypt::Crypt()
{
    Config *pConfig = new Config();
    strIv = pConfig->getValue("iv");
    delete pConfig;

    if (strIv.isEmpty())
        genIv();
}

QString Crypt::encrypt(QString strKey, QString strData)
{
    if ((strKey.isEmpty()) || (strData.isEmpty()))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: crypt: Cannot encrypt - empty argument");
#endif
        return QString::null;
    }

    QCA::Initializer init;
    QCA::SecureArray arg = QByteArray(strData.toAscii());

    if(!QCA::isSupported("aes256-cbc-pkcs7"))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: AES256-CBC not supported!");
#endif
        return QString::null;
    }
    else
    {
        if (strKey.length() < 16) { while(strKey.length() < 16) { strKey += " "; } }
        QCA::SymmetricKey key = QByteArray(strKey.toAscii());

        QCA::InitializationVector iv = QByteArray(strIv.toAscii());
        QCA::Cipher cipher(QString("aes256"),QCA::Cipher::CBC,
                           QCA::Cipher::DefaultPadding,
                           QCA::Encode,
                           key, iv);

        return QCA::arrayToHex(QCA::SecureArray(cipher.process(arg)).toByteArray());
    }

    return QString::null;
}

QString Crypt::decrypt(QString strKey, QString strData)
{
    if ((strKey.isEmpty()) || (strData.isEmpty()))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: crypt: Cannot decrypt - empty argument");
#endif
        return QString::null;
    }

    QCA::Initializer init;
    QCA::SecureArray arg = QCA::hexToArray(strData);

    if(!QCA::isSupported("aes256-cbc-pkcs7"))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: AES256-CBC not supported!");
#endif
        return QString::null;
    }
    else
    {
        if (strKey.length() < 16) { while(strKey.length() < 16) { strKey += " "; } }
        QCA::SymmetricKey key = QByteArray(strKey.toAscii());

        QCA::InitializationVector iv = QByteArray(strIv.toAscii());
        QCA::Cipher cipher(QString("aes256"),QCA::Cipher::CBC,
                           QCA::Cipher::DefaultPadding,
                           QCA::Decode,
                           key, iv);

        return QCA::SecureArray(cipher.process(arg)).data();
    }

    return QString::null;
}

void Crypt::genIv()
{
    strIv.clear();
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    while(strIv.length() < 32)
    {
        int cat = qrand() % 3;

        // 0-9
        if (cat == 0)
        {
            int i = qrand() % 9;
            QChar c = i+=48;
            strIv += c.toAscii();
        }
        // A-Z
        else if (cat == 1)
        {
            int i = qrand() % 25;
            QChar c = i+=65;
            strIv += c.toAscii();
        }
        // a-z
        else if (cat == 2)
        {
            int i = qrand() % 25;
            QChar c = i+=97;
            strIv += c.toAscii();
        }
    }

    Config *pConfig = new Config();
    pConfig->setValue("iv", strIv);
    delete pConfig;
}
