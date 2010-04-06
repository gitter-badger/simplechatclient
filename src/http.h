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

#ifndef HTTP_H
#define HTTP_H

#include <QHttp>
#include <QObject>
#include <QStringList>
#include <QtNetwork>

class http : public QObject
{
    Q_OBJECT
public:
    http();
    ~http();

    void request(QString, QString);
    void request_clear();
    QHttp *get_http() { return pHttp; }
    QString read_http() { return pHttp->readAll(); }

private:
    QHttp *pHttp;
    QString strCookies;

private slots:
    void read_response_header(const QHttpResponseHeader &);

};

#endif // HTTP_H
