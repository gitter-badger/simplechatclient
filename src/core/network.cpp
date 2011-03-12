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
#include <QHostInfo>
#include <QSettings>
#include "config.h"
#include "crypt.h"
#include "network.h"

Network::Network(QString param1, int param2)
{
    strServer = param1;
    iPort = param2;

    iActive = 0;
    bDefaultEnabledQueue = true;
    bAuthorized = false;
    timerReconnect = new QTimer();
    timerReconnect->setInterval(1000*30); // 30 sec
    QSettings settings;
    settings.setValue("reconnect", "true");
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
    QObject::connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(state_changed(QAbstractSocket::SocketState)));

    QObject::connect(timerPong, SIGNAL(timeout()), this, SLOT(timeout_pong()));
    QObject::connect(timerPing, SIGNAL(timeout()), this, SLOT(timeout_ping()));
    QObject::connect(timerLag, SIGNAL(timeout()), this, SLOT(timeout_lag()));
    QObject::connect(timerQueue, SIGNAL(timeout()), this, SLOT(timeout_queue()));

    QObject::connect(timerReconnect, SIGNAL(timeout()), this, SLOT(reconnect()));
}

Network::~Network()
{
    socket->deleteLater();
}

void Network::run()
{
    exec();
}

bool Network::is_connected()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        return true;
    else
        return false;
}

bool Network::is_writable()
{
    return socket->isWritable();
}

void Network::clear_all()
{
    // close cam socket
    emit close_cam_socket();

    // set button
    emit set_disconnected();

    // set lag
    emit set_lag("Lag: ?");

    // update nick
    emit update_nick(tr("(Unregistered)"));

    // clear nicklist
    emit clear_all_nicklist();

    // state
    QSettings settings;
    settings.setValue("logged", "off");

    // timer
    if (timerPong->isActive() == true)
        timerPong->stop();
    if (timerPing->isActive() == true)
        timerPing->stop();
    if (timerLag->isActive() == true)
        timerLag->stop();
    if (timerQueue->isActive() == true)
        timerQueue->stop();

    // reconnect
    if (timerReconnect->isActive() == true)
        timerReconnect->stop();

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
    QSettings settings;
    QString strNick = settings.value("nick").toString();
    QString strPass = settings.value("pass").toString();

    // nick & pass is null
    if ((strNick.isEmpty() == true) && (strPass.isEmpty() == true))
        strNick = "~test";

    // decrypt pass
    if (strPass.isEmpty() == false)
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->decrypt(strNick, strPass);
        delete pCrypt;
    }

    // correct nick
    if ((strPass.isEmpty() == true) && (strNick[0] != '~'))
        strNick = "~"+strNick;
    if ((strPass.isEmpty() == false) && (strNick[0] == '~'))
        strNick = strNick.right(strNick.length()-1);

    Config *pConfig = new Config();
    settings.setValue("nick", strNick);
    pConfig->set_value("nick", strNick);
    delete pConfig;

    // update nick
    emit update_nick(strNick);
    // update actions
    emit update_actions();

    // set current nick
    QString strCurrentNick = strNick;
        if (strCurrentNick[0] == '~')
    strCurrentNick = strNick.right(strNick.length()-1);

    // request uo key
    emit authorize(strCurrentNick, strNick, strPass);
}

void Network::connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        // host
        QHostInfo hInfo = QHostInfo::fromName(strServer);

        if (hInfo.error() != QHostInfo::NoError)
        {
            emit show_msg_all(QString(tr("Error: Could not connect to the server [%1]")).arg(hInfo.errorString()), 9);

            // clear all
            clear_all();

            // reconnect
            if ((timerReconnect->isActive() == false) && (bAuthorized == false))
                timerReconnect->start();

            return;
        }

        // clear all
        clear_all();

        // random
        int iRandom = qrand() % hInfo.addresses().size();

        // set active
        QDateTime dt = QDateTime::currentDateTime();
        iActive = (int)dt.toTime_t();

        // connect
        socket->connectToHost(hInfo.addresses().at(iRandom).toString(), iPort);

        // start timers
        timerPong->start();
        timerPing->start();
        timerLag->start();
        timerQueue->start();
    }
    else
        emit show_msg_all(tr("Error: Could not connect to the server - connection already exists!"), 9);
}

void Network::connected()
{
    emit set_connected();
    emit set_lag("Lag: ?");

    emit show_msg_all(tr("Connected to server"), 9);

    // authorize
    if (bAuthorized == false)
        authorize();
}

void Network::disconnect()
{
    // clear queue
    msgSendQueue.clear();

    // send quit
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
        socket->write("QUIT\r\n");
        socket->waitForBytesWritten();
    }

    // close
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();

    // clear all
    clear_all();
}

void Network::disconnected()
{
    if (socket->error() != QAbstractSocket::UnknownSocketError)
        emit show_msg_all(QString(tr("Disconnected from server [%1]")).arg(socket->errorString()), 9);
    else
        emit show_msg_all(tr("Disconnected from server"), 9);

    // clear all
    clear_all();

    // reconnect
    if ((timerReconnect->isActive() == false) && (bAuthorized == false))
        timerReconnect->start();
}

void Network::reconnect()
{
    if (timerReconnect->isActive() == true)
        timerReconnect->stop();

    QSettings settings;
    if (settings.value("reconnect").toString() == "true")
    {
        if ((this->is_connected() == false) && (settings.value("logged").toString() == "off"))
        {
            emit show_msg_all(tr("Reconnecting..."), 7);
            emit connect();
        }
    }
}

void Network::write(QString strData)
{
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
#ifdef Q_WS_X11
        QSettings settings;
        if (settings.value("debug").toString() == "on")
            qDebug() << "-> " << strData;
#endif
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i).toAscii());

        if (socket->write(qbaData) == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
                emit show_msg_active(QString(tr("Error: Could not send data! [%1]")).arg(socket->errorString()), 9);
            else if (socket->state() == QAbstractSocket::UnconnectedState)
                emit show_msg_active(tr("Error: Could not send data! [Not connected]"), 9);
        }
    }
    else
        emit show_msg_active(tr("Error: Could not send data! [Not Connected]"), 9);
}

void Network::send(QString strData)
{
    // default enabled queue
    if (bDefaultEnabledQueue == true)
    {
        msgSendQueue.append(strData);
    }
    else
    {
        QSettings settings;
        if (settings.value("disable_avatars").toString() == "on") // without avatars
            write(strData);
        else // with avatars
            msgSendQueue.append(strData);
    }
}

void Network::recv()
{
    while(socket->canReadLine())
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
    if (error == QAbstractSocket::RemoteHostClosedError) return; // supported by disconnected

    emit show_msg_all(QString(tr("Disconnected from server [%1]")).arg(socket->errorString()), 9);

    // clear all
    clear_all();

    // reconnect
    if ((timerReconnect->isActive() == false) && (bAuthorized == false))
        timerReconnect->start();
}

/**
 * Disable connect button if state not connected and not unconnected
 */
void Network::state_changed(QAbstractSocket::SocketState socketState)
{
    if ((socketState != QAbstractSocket::UnconnectedState) && (socketState != QAbstractSocket::ConnectedState))
        emit set_connect_enabled(false);
    else
        emit set_connect_enabled(true);
}

void Network::timeout_lag()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    // update lag
    if (iCurrent-iActive > 30+10)
        emit set_lag(QString("Lag: %1s").arg(iCurrent-iActive));
}

void Network::timeout_pong()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    // check timeout
    if (iActive+301 < iCurrent)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            emit show_msg_all(tr("No PONG reply from server in 301 seconds. Disconnecting..."), 9);

            // disconnect
            disconnect();
        }
        iActive = iCurrent;
    }
}

void Network::timeout_ping()
{
    QDateTime dta = QDateTime::currentDateTime();
    int i1 = (int)dta.toTime_t(); // seconds that have passed since 1970
    QString t2 = dta.toString("zzz"); // miliseconds

    QSettings settings;
    if ((is_connected() == true) && (is_writable() == true) && (settings.value("logged").toString() == "on"))
        emit send(QString("PING :%1.%2").arg(i1).arg(t2));
}

void Network::timeout_queue()
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        msgSendQueue.clear();
        return;
    }

    if (msgSendQueue.size() > 0)
        write(msgSendQueue.takeFirst());
}
