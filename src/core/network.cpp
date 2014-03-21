/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDateTime>
#include <QTcpSocket>
#include <QTimer>
#include "autoaway.h"
#include "away.h"
#include "busy.h"
#include "config.h"
#include "core.h"
#include "simple_crypt.h"
#include "lag.h"
#include "nick.h"
#include "message.h"
#include "settings.h"
#include "network.h"

Network::Network(const QString &_strServer, int _iPort) : strServer(_strServer), iPort(_iPort), iActive(0), bAuthorized(false)
{
    Settings::instance()->set("reconnect", "true");

    timerReconnect = new QTimer();
    timerReconnect->setInterval(1000*60*2); // 2 min
    timerPong = new QTimer();
    timerPong->setInterval(1000*60*1); // 1 min
    timerPing = new QTimer();
    timerPing->setInterval(1000*30); // 30 sec

    socket = new QTcpSocket(this);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));

    QObject::connect(timerPong, SIGNAL(timeout()), this, SLOT(timeoutPong()));
    QObject::connect(timerPing, SIGNAL(timeout()), this, SLOT(timeoutPing()));

    QObject::connect(timerReconnect, SIGNAL(timeout()), this, SLOT(reconnect()));
}

Network::~Network()
{
    socket->deleteLater();
    timerPing->stop();
    timerPong->stop();
    timerReconnect->stop();
    Autoaway::instance()->stop();
}

void Network::run()
{
    exec();
}

bool Network::isConnected()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        return true;
    else
        return false;
}

void Network::clearAll()
{
    // close cam socket
    if (Core::instance()->kamerzystaSocket->state() == QAbstractSocket::ConnectedState)
        Core::instance()->kamerzystaSocket->disconnectFromHost();

    // set lag
    Lag::instance()->reset();

    // update busy button
    Busy::instance()->stop();

    // update away button
    Away::instance()->stop();

    // update nick
    emit updateNick(tr("(Unregistered)"));

    // clear nick
    Nick::instance()->clear();

    // state
    Settings::instance()->set("logged", "false");

    // timer
    timerPong->stop();
    timerPing->stop();

    // reconnect
    timerReconnect->stop();

    // auto-away
    Autoaway::instance()->stop();

    // last active
    Settings::instance()->set("last_active", "0");

    // clear queue
    msgSendQueue.clear();

    // authorized
    bAuthorized = false;
}

void Network::authorize()
{
    // authorized
    bAuthorized = true;

    // get nick
    QString strNick = Settings::instance()->get("nick");
    QString strPass = Settings::instance()->get("pass");

    // nick & pass is null
    if ((strNick.isEmpty()) && (strPass.isEmpty()))
        strNick = "~test";

    // decrypt pass
    if (!strPass.isEmpty())
    {
        SimpleCrypt *pSimpleCrypt = new SimpleCrypt();
        strPass = pSimpleCrypt->decrypt(strNick, strPass);
        delete pSimpleCrypt;
    }

    // request uo key
    emit authorize(strNick, strPass);
}

void Network::connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        // clear all
        clearAll();

        // connect
        socket->connectToHost(strServer, iPort);
    }
    else
    {
        QString strError = tr("Error: Could not connect to the server - connection already exists!");
        Message::instance()->showMessageAll(strError, MessageError);
    }
}

void Network::connected()
{
    Lag::instance()->reset();

    // set active
    iActive = QDateTime::currentMSecsSinceEpoch();

    // start timers
    timerPong->start();
    timerPing->start();

    // display
    QString strDisplay = tr("Connected to server");
    Message::instance()->showMessageAll(strDisplay, MessageError);

    // authorize
    if (!bAuthorized)
        authorize();
}

void Network::disconnect()
{
    // clear queue
    msgSendQueue.clear();

    // send quit
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState))
    {
        socket->write("QUIT\r\n");
        socket->waitForBytesWritten();
    }

    // clear all
    clearAll();

    // close
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void Network::disconnected()
{
    if (socket->error() != QAbstractSocket::UnknownSocketError)
    {
        QString strError = QString(tr("Disconnected from server [%1]")).arg(socket->errorString());
        Message::instance()->showMessageAll(strError, MessageError);
    }
    else
    {
        QString strError = tr("Disconnected from server");
        Message::instance()->showMessageAll(strError, MessageError);
    }

    // clear all
    clearAll();

    // reconnect
    if ((!timerReconnect->isActive()) && (!bAuthorized))
        timerReconnect->start();
}

void Network::reconnect()
{
    timerReconnect->stop();

    if (Settings::instance()->get("reconnect") == "true")
    {
        if ((!this->isConnected()) && (Settings::instance()->get("logged") == "false"))
        {
            QString strDisplay = tr("Reconnecting...");
            Message::instance()->showMessageAll(strDisplay, MessageInfo);
            connect();
        }
    }
}

void Network::write(const QString &strData)
{
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState))
    {
        if (Settings::instance()->get("debug") == "true")
            Message::instance()->showMessage(DEBUG_WINDOW, "-> "+strData, MessageDefault);

        if (socket->write((strData+"\r\n").toAscii()) == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
            {
                QString strError = QString(tr("Error: Could not send data! [%1]")).arg(socket->errorString());
                Message::instance()->showMessageActive(strError, MessageError);
            }
            else if (socket->state() == QAbstractSocket::UnconnectedState)
            {
                QString strError = tr("Error: Could not send data! [Not connected]");
                Message::instance()->showMessageActive(strError, MessageError);
            }
        }
    }
    else
    {
        QString strError = tr("Error: Could not send data! [Not connected]");
        Message::instance()->showMessageActive(strError, MessageError);
    }
}

void Network::send(const QString &strData)
{
    msgSendQueue.append(strData);

    sendQueue();
}

void Network::recv()
{
    while (socket->canReadLine())
    {
        // read line
        QByteArray data = socket->readLine().trimmed();

        // set active
        iActive = QDateTime::currentMSecsSinceEpoch();

        // process to kernel
        emit kernel(QString(data));
    }
}

void Network::error(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::RemoteHostClosedError) return; // supported by disconnected
qDebug() << "Disconnected from server :" << socket->errorString();

    QString strError = QString(tr("Disconnected from server [%1]")).arg(socket->errorString());
    Message::instance()->showMessageAll(strError, MessageError);

    // clear all
    clearAll();

    // reconnect
    if ((!timerReconnect->isActive()) && (!bAuthorized))
        timerReconnect->start();
}

/**
 * Disable connect button if state not connected and not unconnected
 */
void Network::stateChanged(QAbstractSocket::SocketState socketState)
{
    if (Settings::instance()->get("debug") == "true")
        qDebug() << "Network socket state changed to: " << socketState;

    if (socketState == QAbstractSocket::UnconnectedState)
        Settings::instance()->set("socket_state", "disconnected");
    else if (socketState == QAbstractSocket::ConnectedState)
        Settings::instance()->set("socket_state", "connected");
    else
        Settings::instance()->set("socket_state", "unknown");

    emit socketStateChanged();
}

void Network::timeoutPong()
{
    qint64 iCurrentTime = QDateTime::currentMSecsSinceEpoch();

    // update lag
    if (iActive+60000 < iCurrentTime)
        Lag::instance()->update(iCurrentTime-iActive);

    // check pong timeout
    if (iActive+301000 < iCurrentTime) // 301 sec
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            QString strDisplay = tr("No PONG reply from server in 301 seconds. Disconnecting...");
            Message::instance()->showMessageAll(strDisplay, MessageError);

            // disconnect
            disconnect();
        }
        iActive = iCurrentTime;
    }
}

void Network::timeoutPing()
{
    QString strMSecs = QString::number(QDateTime::currentMSecsSinceEpoch());

    if ((isConnected()) && (Settings::instance()->get("logged") == "true"))
        send(QString("PING :%1").arg(strMSecs));
}
#include <QDebug>

void Network::sendQueue()
{
    // empty queue
    if (msgSendQueue.isEmpty())
        return;

    bool locked = mutex.tryLock();
    if (!locked) return;

    // check disconnected
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        msgSendQueue.clear();
        msgSendHistory.clear();
        mutex.unlock();
        return;
    }

    const int flood = 41;
    const int threshold = 3;
    const int recvq = 8192;
    const int sendq = 262144;

    int current_flood = 0;
    int current_recvq = 0;
    int current_sendq = 0;
    int current_time = QDateTime::currentMSecsSinceEpoch() / 1000;
qDebug() << "current_time: " << current_time;
    if (!msgSendHistory.isEmpty())
    {
        for (int i = current_time - threshold +1; i <= current_time; ++i)
        {
qDebug() << "i: " << i;
            if (msgSendHistory.contains(i))
            {
                NetworkStats statistic = msgSendHistory.value(i);
                current_recvq += statistic.recvq;
                current_flood += statistic.flood;
                current_sendq += statistic.sendq;
qDebug() << "i: " << i << " message: recvq " << statistic.recvq << " flood: " << statistic.flood;
            }
        }
    }
qDebug() << "history count: " << msgSendHistory.size();
qDebug() << "current_recv: " << current_recvq;
qDebug() << "current_flood: " << current_flood;

    QString message = msgSendQueue.first();
qDebug() << "message: " << message;

    int add_flood = 1;
    int add_recvq = message.size()+3;

qDebug() << "new recv: " << current_recvq+add_recvq;
qDebug() << "new flood: " << current_flood+add_flood;

    if (((current_flood < flood) && (current_recvq < recvq) && (current_sendq < sendq)) &&
        ((current_flood+add_flood < flood) && (current_recvq+add_recvq < recvq)))
    {
        // add
        if (msgSendHistory.contains(current_time))
        {
            msgSendHistory[current_time].flood += add_flood;
            msgSendHistory[current_time].recvq += add_recvq;
        }
        else
        {
            NetworkStats new_stats;
            new_stats.flood = add_flood;
            new_stats.recvq = add_recvq;
            new_stats.sendq = 0;

            msgSendHistory.insert(current_time, new_stats);
        }

        // send
        write(message);

        // take
        msgSendQueue.removeFirst();

        // clear
        int max_history = current_time - (flood*2);

        QMutableHashIterator<int, NetworkStats> old_history(msgSendHistory);
        while (old_history.hasNext())
        {
            old_history.next();

            if (old_history.key() < max_history)
                old_history.remove();
        }
    }
qDebug() << "-------------------------";

    if (!msgSendQueue.isEmpty())
    {
        QTimer::singleShot(100, this, SLOT(sendQueue()));
    }

    mutex.unlock();
}
