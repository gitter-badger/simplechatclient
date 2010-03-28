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

#ifndef IRC_AUTH_H
#define IRC_AUTH_H

#include <QHttp>
#include <QObject>
#include <QTimer>
#include "http.h"

class irc_auth : public QObject
{
    Q_OBJECT
public:
    irc_auth(QHttp *, QSettings *);
    ~irc_auth();

    void request_uo_start(QString, QString);
    void request_uo_stop();
    void request_uo();
    QString transform_key(QString);

private:
    http *pHttp;
    QTimer *timer;
    int iTimer;
    QString strNick;
    QString strPass;
    QSettings *settings;

private slots:
    void timer_timeout();

};

#endif // IRC_AUTH_H
