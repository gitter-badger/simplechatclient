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

#ifndef ONET_AUTH_H
#define ONET_AUTH_H

#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include "tab_container.h"

class OnetAuth : public QObject
{
    Q_OBJECT
public:
    OnetAuth(QSettings *, TabContainer *);
    QString transform_key(QString);

public slots:
    void request_uo(QString, QString, QString);

private:
    QSettings *settings;
    TabContainer *tabc;

    QString get_version(QString);
    void request_finished(QString, QString);

signals:
    void send(QString);

};

#endif // ONET_AUTH_H
