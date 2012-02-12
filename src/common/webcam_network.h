/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef WEBCAM_NETWORK_H
#define WEBCAM_NETWORK_H

class QTcpSocket;
class QTimer;
#include <QAbstractSocket>
#include <QObject>

class WebcamNetwork : public QObject
{
    Q_OBJECT
public:
    WebcamNetwork();
    virtual ~WebcamNetwork();
    inline void setBytesNeed(int i) { iBytes_need = i; }
    inline void setBText(bool b) { bText = b; }
    inline void setLastKeepAlive(int i) { iLastKeepAlive = i; }
    inline int getLastKeepAlive() { return iLastKeepAlive; }
    inline void setReconnect(bool b) { bReconnecting = b; }

    void clearAll();
    bool isConnected();
    void networkConnect();
    void networkSend(QString);
    void networkSendb(QByteArray);
    void networkDisconnect();

private:
    QTcpSocket *socket;
    QString strData;
    QByteArray bData;
    int iBytes_need;
    int iBytes_recv;
    QTimer *timerPingPong;
    int iLastActive;
    int iLastKeepAlive;
    bool bText;
    bool bReconnecting;

private slots:
    void networkRead();
    void networkConnected();
    void networkDisconnected();
    void networkError(QAbstractSocket::SocketError);
    void slotNetworkConnect();
    void timeoutPingpong();

signals:
    void dataKernel(QByteArray);
    void textKernel(QString);
    void error(QString);
    void connected();
    void disconnected();
};

#endif // WEBCAM_NETWORK_H
