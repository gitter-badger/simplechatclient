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

#include "defines.h"
class QTimer;
class QTcpSocket;
#include <QAbstractSocket>
#include <QThread>

/**
 * Network thread class
 */
class Network : public QThread
{
    Q_OBJECT
public:
    Network(QString, int);
    virtual ~Network();
    void run();
    void setReconnect(bool);
    bool isConnected();
    bool isWritable();
    QTimer *timerReconnect;

public slots:
    void connect();
    void disconnect();
    void send(QString);
    void sendQueue(QString);

private:
    QString strServer;
    int iPort;
    QTcpSocket *socket;
    QTimer *timerPong;
    QTimer *timerPing;
    QTimer *timerLag;
    QTimer *timerQueue;
    int iActive;
    QList<QString> msgSendQueue;
    QList<QString> msgSendQueueNS;
    bool bAuthorized;

    void authorize();
    void clearAll();
    void write(QString);

private slots:
    void connected();
    void disconnected();
    void reconnect();
    void recv();
    void error(QAbstractSocket::SocketError);
    void stateChanged(QAbstractSocket::SocketState);
    void timeoutPong();
    void timeoutPing();
    void timeoutLag();
    void timeoutQueue();

signals:
    void setConnected();
    void setDisconnected();
    void setConnectEnabled(bool);
    void kernel(QString);
    void authorize(QString, QString, QString);
    void showMessageAll(QString&, MessageCategory);
    void showMessageActive(QString&, MessageCategory);
    void updateNick(QString);
    void updateActions();
    void clearAllNicklist();
};

#endif // NETWORK_H
