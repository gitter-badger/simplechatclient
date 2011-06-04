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

#ifndef WEBCAM_NETWORK_H
#define WEBCAM_NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class WebcamNetwork : public QObject
{
    Q_OBJECT
public:
    WebcamNetwork();
    inline void set_bytes_need(int i) { iBytes_need = i; }
    inline void set_btext(bool b) { bText = b; }
    inline void set_last_keep_alive(int i) { iLastKeepAlive = i; }
    inline int get_last_keep_alive() { return iLastKeepAlive; }
    inline void set_reconnect(bool b) { bReconnecting = b; }

    void clear_all();
    bool is_connected();
    void network_connect();
    void network_send(QString);
    void network_sendb(QByteArray);
    void network_disconnect();

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
    void network_read();
    void network_connected();
    void network_disconnected();
    void network_error(QAbstractSocket::SocketError);
    void slot_network_connect();
    void timeout_pingpong();

signals:
    void data_kernel(QByteArray);
    void text_kernel(QString);
    void error(QString);
    void connected();
    void disconnected();
};

#endif // WEBCAM_NETWORK_H
