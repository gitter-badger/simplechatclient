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

#ifndef KAMERZYSTA_H
#define KAMERZYSTA_H

#include <QDir>
#include <QFile>
#include <QHostAddress>
#include <QIcon>
#include <QMessageBox>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QTcpSocket>
#include <QTimer>
#include <QTextStream>
#include "config.h"
#include "log.h"

class Kamerzysta : public QObject
{
    Q_OBJECT
public:
    Kamerzysta(QTcpSocket *);
    ~Kamerzysta();
    void show(QString, QString);

private:
    QString strNick;
    QString strUOKey;
    QString strAppPath;
    QString strKamerzystaFile;
    int iPort;
    QTcpSocket *socket;
    int iTryGetPort;

    void get_path();
    void kamerzysta_not_running();
    void kamerzysta_running();
    void network_connect();

private slots:
    void network_connected();
    void network_disconnected();
    void get_port();
    void network_send(QString);
    void network_disconnect();
    void network_read();
    void error(QAbstractSocket::SocketError);

};

#endif // KAMERZYSTA_H
