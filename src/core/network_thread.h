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

#ifndef NETWORK_THREAD_H
#define NETWORK_THREAD_H

#include <QAction>
#include <QObject>
#include <QQueue>
#include <QSettings>
#include <QTcpSocket>
#include <QTimer>
#include "crypt.h"

struct NickStatus
{
    QString channel;
    QString nick;
    QString prefix;
    QString suffix;
};
typedef QList<NickStatus> sChannelNickStatus;


class NetworkThread : public QThread
{
    Q_OBJECT
public:
    NetworkThread(QAction *, QAction *, QString, int);
    ~NetworkThread();
    void run();
    void set_reconnect(bool);
    bool is_connected();
    bool is_writable();

public slots:
    void connect();
    void close();
    void send(QString);

private:
    QAction *connectAct;
    QAction *lagAct;
    QString strServer;
    int iPort;
    QTcpSocket *socket;
    QTimer *timerPingPong;
    QTimer *timerLag;
    QTimer *timerQueue;
    int iActive;
    QList <QString> msgSendQueue;

    void write(QString);

private slots:
    void reconnect();
    void recv();
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void timeout_pingpong();
    void timeout_lag();
    void timeout_queue();

signals:
    void send_to_kernel(QString);
    void request_uo(QString, QString, QString);
    void show_msg_all(QString, int);
    void show_msg_active(QString, int);
    void update_nick(QString);
    void clear_nicklist(QString);
    void clear_all_nicklist();

};

#endif // NETWORK_THREAD_H
