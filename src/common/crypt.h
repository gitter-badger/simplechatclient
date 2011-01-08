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

#ifndef CRYPT_H
#define CRYPT_H

#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QObject>
#include <QtCrypto>
// in OpenSuSE 11 try this:
//#include <QtCrypto/QtCrypto>
#include "config.h"

/**
 * Crypt class
 */
class Crypt : public QObject
{
    Q_OBJECT
public:
    Crypt();
    /**
     * Encrypt
     * @param QString key
     * @param QString data
     * @return QString encrypted data
     */
    QString encrypt(QString, QString);
    /**
     * Decrypt
     * @param QString key
     * @param QString data
     * @return QString decrypted data
     */
    QString decrypt(QString, QString);

private:
    QString strIv;

    /**
     * Generate iv
     */
    void gen_iv();

};

#endif // CRYPT_H
