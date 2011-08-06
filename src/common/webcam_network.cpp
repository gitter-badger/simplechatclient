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

#include <QDateTime>
#include <QTcpSocket>
#include <QTimer>
#include "core.h"
#include "webcam_network.h"

WebcamNetwork::WebcamNetwork()
{
    iLastKeepAlive = 0;
    iLastActive = 0;
    bReconnecting = true;

    timerPingPong = new QTimer();
    timerPingPong->setInterval(1000*60*1); // 1 min

    socket = new QTcpSocket(this);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    QObject::connect(socket, SIGNAL(connected()), this, SLOT(networkConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(networkDisconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(networkError(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(networkRead()));
    QObject::connect(timerPingPong, SIGNAL(timeout()), this, SLOT(timeoutPingpong()));
}

bool WebcamNetwork::isConnected()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        return true;
    else
        return false;
}

void WebcamNetwork::clearAll()
{
    bText = true;
    iBytes_need = 0;
    iBytes_recv = 0;

    // timer
    if (timerPingPong->isActive())
        timerPingPong->stop();
}

void WebcamNetwork::networkConnect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        // clear all
        clearAll();

        // reconnect
        bReconnecting = true;

        // set last active
        QDateTime dt = QDateTime::currentDateTime();
        iLastActive = (int)dt.toTime_t();

        // connect
        socket->connectToHost("czat-s.onet.pl", 5008);

        // timer
        timerPingPong->start();
    }
}

void WebcamNetwork::networkDisconnect()
{
    bReconnecting = false;
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void WebcamNetwork::networkSend(QString strData)
{
    strData += "\n";
    QByteArray qbaData = strData.toAscii();

    networkSendb(qbaData);
}

void WebcamNetwork::networkSendb(QByteArray qbaData)
{
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable()))
    {
        int ret = socket->write(qbaData);
        if (ret == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
                emit error(QString(tr("Error: Failed to send data! [%1]")).arg(socket->errorString()));
            else if (socket->state() == QAbstractSocket::UnconnectedState)
                emit error(tr("Error: Failed to send data! [Not connected]"));
        }
        else
        {
#ifdef Q_WS_X11
            if (Core::instance()->settings.value("debug") == "on")
                qDebug() << "CAM -> " << QString(qbaData);
#endif
        }
    }
    else
        emit error(tr("Error: Failed to send data! [Not connected]"));
}

void WebcamNetwork::networkRead()
{
    // set last active
    QDateTime dt = QDateTime::currentDateTime();
    iLastActive = (int)dt.toTime_t();

    // read text
    if (bText)
    {
        // read line
        QByteArray data = socket->readLine().trimmed();
        QString strData = QString(data);
        emit textKernel(strData);
    }
    // read data (image, description)
    else
    {
        for (int i = 0; i < socket->bytesAvailable(); i++)
        {
            if (iBytes_recv < iBytes_need)
            {
                bData += socket->read(1);
                iBytes_recv++;
            }
        }

        if (iBytes_recv == iBytes_need)
        {
            emit dataKernel(bData);

            // clear bdata
            bData.clear();
            bText = true;
            iBytes_recv = 0;
            iBytes_need = 0;
        }
    }

    // again
    if (socket->bytesAvailable() != 0)
        networkRead();
}

void WebcamNetwork::networkConnected()
{
    emit connected();
}

void WebcamNetwork::networkDisconnected()
{
    emit disconnected();

    // clear all
    clearAll();

    // reconnect
    if (bReconnecting)
        QTimer::singleShot(1000*10, this, SLOT(slotNetworkConnect())); // 10 sec
}

void WebcamNetwork::networkError(QAbstractSocket::SocketError err)
{
    Q_UNUSED (err);

    emit error(socket->errorString());

    if (socket->state() == QAbstractSocket::ConnectedState)
        networkDisconnect();
    else if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        // clear all
        clearAll();

        // reconnect
        if (bReconnecting)
            QTimer::singleShot(1000*10, this, SLOT(slotNetworkConnect())); // 10 sec
    }
}

void WebcamNetwork::slotNetworkConnect()
{
    networkConnect();
}

void WebcamNetwork::timeoutPingpong()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    if (iLastActive+301 < iCurrent)
    {
        networkDisconnect();
        iLastActive = iCurrent;
    }
}
