/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QAction>
#include <QDateTime>
#include <QTcpSocket>
#include <QTimer>
#include "autoaway.h"
#include "config.h"
#include "core.h"
#include "crypt.h"
#include "nicklist.h"
#include "message.h"
#include "network.h"

Network::Network(const QString &_strServer, int _iPort) : strServer(_strServer), iPort(_iPort), iActive(0), bAuthorized(false)
{
    timerReconnect = new QTimer();
    timerReconnect->setInterval(1000*30); // 30 sec
    Core::instance()->settings["reconnect"] = "true";
    timerPong = new QTimer();
    timerPong->setInterval(1000*60*1); // 1 min
    timerPing = new QTimer();
    timerPing->setInterval(1000*30); // 30 sec
    timerLag = new QTimer();
    timerLag->setInterval(1000*10); // 10 sec
    timerQueue = new QTimer();
    timerQueue->setInterval(300); // 0.3 sec

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
    QObject::connect(timerLag, SIGNAL(timeout()), this, SLOT(timeoutLag()));
    QObject::connect(timerQueue, SIGNAL(timeout()), this, SLOT(timeoutQueue()));

    QObject::connect(timerReconnect, SIGNAL(timeout()), this, SLOT(reconnect()));
}

Network::~Network()
{
    socket->deleteLater();
    timerQueue->stop();
    timerLag->stop();
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

bool Network::isWritable()
{
    return socket->isWritable();
}

void Network::clearAll()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "true")
        DEBUG_BLOCK
#endif

    // close cam socket
    if (Core::instance()->kamerzystaSocket->state() == QAbstractSocket::ConnectedState)
        Core::instance()->kamerzystaSocket->disconnectFromHost();

    // set button
    emit setDisconnected();

    // set lag
    Core::instance()->lagAction->setText("Lag: ?");

    // update busy button
    Core::instance()->busyAction->setChecked(false);

    // update away button
    Core::instance()->awayAction->setChecked(false);

    // update nick
    emit updateNick(tr("(Unregistered)"));

    // clear nicklist
    Nicklist::instance()->clearAllNicklist();

    // state
    Core::instance()->settings["logged"] = "false";

    // timer
    timerPong->stop();
    timerPing->stop();
    timerLag->stop();
    timerQueue->stop();

    // reconnect
    timerReconnect->stop();

    // auto-away
    Autoaway::instance()->stop();

    // last active
    Core::instance()->settings["last_active"] = "0";

    // clear queue
    msgSendQueue.clear();
    msgSendQueueNS.clear();

    // authorized
    bAuthorized = false;
}

void Network::authorize()
{
    // authorized
    bAuthorized = true;

    // get nick
    QString strNick = Core::instance()->settings.value("nick");
    QString strPass = Core::instance()->settings.value("pass");

    // nick & pass is null
    if ((strNick.isEmpty()) && (strPass.isEmpty()))
        strNick = "~test";

    // decrypt pass
    if (!strPass.isEmpty())
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->decrypt(strNick, strPass);
        delete pCrypt;
    }

    // update nick
    emit updateNick(strNick);
    // update actions
    emit updateActions();

    // set current nick
    QString strCurrentNick = strNick;
    if (strCurrentNick[0] == '~')
        strCurrentNick = strNick.right(strNick.length()-1);

    // request uo key
    emit authorize(strCurrentNick, strNick, strPass);
}

void Network::connect()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "true")
        DEBUG_BLOCK
#endif

    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        // clear all
        clearAll();

        // set active
        QDateTime dt = QDateTime::currentDateTime();
        iActive = (int)dt.toTime_t();

        // connect
        socket->connectToHost(strServer, iPort);

        // start timers
        timerPong->start();
        timerPing->start();
        timerLag->start();
        timerQueue->start();
    }
    else
    {
        QString strError = tr("Error: Could not connect to the server - connection already exists!");
        Message::instance()->showMessageAll(strError, ErrorMessage);
    }
}

void Network::connected()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "true")
        DEBUG_BLOCK
#endif

    emit setConnected();
    Core::instance()->lagAction->setText("Lag: ?");

    QString strDisplay = tr("Connected to server");
    Message::instance()->showMessageAll(strDisplay, ErrorMessage);

    // authorize
    if (!bAuthorized)
        authorize();
}

void Network::disconnect()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "true")
        DEBUG_BLOCK
#endif

    // clear queue
    msgSendQueue.clear();
    msgSendQueueNS.clear();

    // send quit
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable()))
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
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "true")
        DEBUG_BLOCK
#endif

    if (socket->error() != QAbstractSocket::UnknownSocketError)
    {
        QString strError = QString(tr("Disconnected from server [%1]")).arg(socket->errorString());
        Message::instance()->showMessageAll(strError, ErrorMessage);
    }
    else
    {
        QString strError = tr("Disconnected from server");
        Message::instance()->showMessageAll(strError, ErrorMessage);
    }

    // clear all
    clearAll();

    // reconnect
    if ((!timerReconnect->isActive()) && (!bAuthorized))
        timerReconnect->start();
}

void Network::reconnect()
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "true")
        DEBUG_BLOCK
#endif

    timerReconnect->stop();

    if (Core::instance()->settings.value("reconnect") == "true")
    {
        if ((!this->isConnected()) && (Core::instance()->settings.value("logged") == "false"))
        {
            QString strDisplay = tr("Reconnecting...");
            Message::instance()->showMessageAll(strDisplay, InfoMessage);
            connect();
        }
    }
}

void Network::write(const QString &strData)
{
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable()))
    {
        if (Core::instance()->settings.value("debug") == "true")
            qDebug() << "-> " << strData;

        if (socket->write((strData+"\r\n").toAscii()) == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
            {
                QString strError = QString(tr("Error: Could not send data! [%1]")).arg(socket->errorString());
                Message::instance()->showMessageActive(strError, ErrorMessage);
            }
            else if (socket->state() == QAbstractSocket::UnconnectedState)
            {
                QString strError = tr("Error: Could not send data! [Not connected]");
                Message::instance()->showMessageActive(strError, ErrorMessage);
            }
        }
    }
    else
    {
        QString strError = tr("Error: Could not send data! [Not Connected]");
        Message::instance()->showMessageActive(strError, ErrorMessage);
    }
}

void Network::send(const QString &strData)
{
    if (strData.startsWith("NS"))
        msgSendQueueNS.append(strData);
    else
        write(strData);
}

void Network::sendQueue(const QString &strData)
{
    if (strData.startsWith("NS"))
        msgSendQueueNS.append(strData);
    else
        msgSendQueue.append(strData);
}

void Network::recv()
{
    while (socket->canReadLine())
    {
        // read line
        QByteArray data = socket->readLine().trimmed();

        // set active
        QDateTime dt = QDateTime::currentDateTime();
        iActive = (int)dt.toTime_t();

        // process to kernel
        emit kernel(QString(data));
    }
}

void Network::error(QAbstractSocket::SocketError error)
{
#ifdef Q_WS_X11
    if (Core::instance()->settings.value("debug") == "true")
        DEBUG_BLOCK
#endif

    if (error == QAbstractSocket::RemoteHostClosedError) return; // supported by disconnected

    QString strError = QString(tr("Disconnected from server [%1]")).arg(socket->errorString());
    Message::instance()->showMessageAll(strError, ErrorMessage);

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
    if (Core::instance()->settings.value("debug") == "true")
    {
#ifdef Q_WS_X11
        DEBUG_BLOCK
#endif
        qDebug() << "Network socket state changed to: " << socketState;
    }

    if ((socketState != QAbstractSocket::UnconnectedState) && (socketState != QAbstractSocket::ConnectedState))
        emit setConnectEnabled(false);
    else
        emit setConnectEnabled(true);
}

void Network::timeoutLag()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    // update lag
    if (iCurrent-iActive > 30+10)
        Core::instance()->lagAction->setText(QString("Lag: %1s").arg(iCurrent-iActive));
}

void Network::timeoutPong()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    // check timeout
    if (iActive+301 < iCurrent)
    {
#ifdef Q_WS_X11
        if (Core::instance()->settings.value("debug") == "true")
            DEBUG_BLOCK
#endif

        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            QString strDisplay = tr("No PONG reply from server in 301 seconds. Disconnecting...");
            Message::instance()->showMessageAll(strDisplay, ErrorMessage);

            // disconnect
            disconnect();
        }
        iActive = iCurrent;
    }
}

void Network::timeoutPing()
{
    QDateTime dta = QDateTime::currentDateTime();
    int i1 = (int)dta.toTime_t(); // seconds that have passed since 1970
    QString t1 = QString::number(i1);
    QString t2 = dta.toString("zzz"); // miliseconds

    if ((isConnected()) && (isWritable()) && (Core::instance()->settings.value("logged") == "true"))
        emit send(QString("PING :%1.%2").arg(t1, t2));
}

void Network::timeoutQueue()
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        msgSendQueue.clear();
        msgSendQueueNS.clear();
        return;
    }

    if (msgSendQueue.size() > 0)
        write(msgSendQueue.takeFirst());
    else
    {
        if (msgSendQueueNS.size() > 0)
            write(msgSendQueueNS.takeFirst());
    }
}
