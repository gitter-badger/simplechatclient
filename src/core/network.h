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

#ifndef NETWORK_H
#define NETWORK_H

#include <QAction>
#include <QTcpSocket>
#include <QThread>

/**
 * Network thread class
 */
class Network : public QThread
{
    Q_OBJECT
public:
    Network(QAction *, QAction *, QString, int);
    ~Network();
    void run();
    void set_reconnect(bool);
    bool is_connected();
    bool is_writable();

public slots:
    void connect();
    void close();
    void send(QString);
    void clear_queue();

private:
    QAction *connectAct;
    QAction *lagAct;
    QString strServer;
    int iPort;
    QTcpSocket *socket;
    QTimer *timerPong;
    QTimer *timerPing;
    QTimer *timerLag;
    QTimer *timerQueue;
    int iActive;
    QList <QString> msgSendQueue;
    bool bReconnecting;
    bool bDefaultEnabledQueue;

    void write(QString);

private slots:
    void reconnect();
    void recv();
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void state_changed(QAbstractSocket::SocketState);
    void timeout_pong();
    void timeout_ping();
    void timeout_lag();
    void timeout_queue();

signals:
    void kernel(QString);
    void request_uo(QString, QString, QString);
    void show_msg_all(QString, int);
    void show_msg_active(QString, int);
    void update_nick(QString);
    void clear_nicklist(QString);
    void clear_all_nicklist();
};

#endif // NETWORK_H
