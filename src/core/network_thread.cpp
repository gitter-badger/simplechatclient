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

#include "network_thread.h"

NetworkThread::NetworkThread(QAction *param1, QAction *param2)
{
    connectAct = param1;
    lagAct = param2;

    strServer = "czat-app.onet.pl";
    iPort = 5015;

    iActive = 0;
    QSettings settings;
    settings.setValue("reconnect", "true");
    timer = new QTimer();
    timer->setInterval(1*60*1000); // 1 min
    timerLag = new QTimer();
    timerLag->setInterval(30*1000); // 30 sec

    socket = new QTcpSocket(this);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    QTimer::singleShot(100, this, SLOT(send_buffer()));

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    QObject::connect(timerLag, SIGNAL(timeout()), this, SLOT(timeout_lag()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

NetworkThread::~NetworkThread()
{
    emit close();
    delete socket;
}

void NetworkThread::run()
{
    exec();
}

bool NetworkThread::is_connected()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        return true;
    else
        return false;
}

bool NetworkThread::is_writable()
{
    return socket->isWritable();
}

void NetworkThread::connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        QDateTime dt = QDateTime::currentDateTime();
        iActive = (int)dt.toTime_t();

        socket->connectToHost(strServer, iPort);
        timer->start();
        timerLag->start();
    }
    else
        emit show_msg_all(tr("Error: Could not connect to the server - connection already exists!"), 9);
}

void NetworkThread::reconnect()
{
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

void NetworkThread::close()
{
    // if buffer is not empty - wait
    if (sendBuffer.isEmpty() == false)
    {
        QTimer::singleShot(500, this, SLOT(close()));
        return;
    }

    // close
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();

    // stop timers
    if (timer->isActive() == true)
        timer->stop();
    if (timerLag->isActive() == true)
        timerLag->stop();
}

void NetworkThread::send_buffer()
{
    if (socket->state() != QAbstractSocket::ConnectedState)
        sendBuffer.clear();

    QList <QString> sendBufferCopy;
    for (int i = 0; i < sendBuffer.size(); i++)
        sendBufferCopy.append(sendBuffer.at(i));

    int iCount = sendBufferCopy.size();

    int iLimit = 5;
    QSettings settings;
    if ((settings.value("style").toString() == "classic") || (settings.value("disable_avatars").toString() == "on"))
        iLimit = 4096;

    if (iCount < iLimit)
    {
        for (int i = 0; i < iCount; i++)
        {
            send_data(sendBufferCopy.at(0));
            sendBuffer.removeOne(sendBufferCopy.at(0));
            sendBufferCopy.removeOne(sendBufferCopy.at(0));
        }
    }
    else
    {
        for (int i = 0; i < iLimit; i++)
        {
            send_data(sendBufferCopy.at(0));
            sendBuffer.removeOne(sendBufferCopy.at(0));
            sendBufferCopy.removeOne(sendBufferCopy.at(0));
        }
        QTimer::singleShot(1000, this, SLOT(send_buffer()));
        return;
    }

    QTimer::singleShot(100, this, SLOT(send_buffer()));
}

void NetworkThread::send_data(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
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

void NetworkThread::send(QString strData)
{
    sendBuffer << strData;
}

void NetworkThread::recv()
{
    bool bCompleted = true;

    strDataRecv.append(socket->readAll());
    if (strDataRecv.isEmpty()) return;

    if (strDataRecv[strDataRecv.length()-1] != '\n')
    {
        if (socket->bytesAvailable() != 0)
            this->recv();
        else
            bCompleted = false;
    }

    QStringList strDataLine = strDataRecv.split("\r\n");
    if (strDataLine.size() < 2)
        QTimer::singleShot(100, this, SLOT(recv()));
    strDataRecv.clear();

    if (bCompleted == false)
    {
        strDataRecv = strDataLine.last();
        strDataLine.removeLast();
    }

    QDateTime dt = QDateTime::currentDateTime();
    iActive = (int)dt.toTime_t();

    for (int i = 0; i < strDataLine.size(); i++)
    {
        QString strLine = strDataLine[i];
        if (strLine.isEmpty() == false)
            emit send_to_kernel(strLine);
    }

    if (bCompleted == false)
        QTimer::singleShot(100, this, SLOT(recv()));
}

void NetworkThread::connected()
{
    connectAct->setText(tr("&Disconnect"));
    connectAct->setIconText(tr("&Disconnect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-connect.png"));
    lagAct->setText("Lag: 0s");

    emit show_msg_all(tr("Connected to server"), 9);

    Config *pConfig = new Config();

    QString strNick = pConfig->get_value("login-nick");
    QString strPass = pConfig->get_value("login-pass");

    // nick & pass is null
    if ((strNick.isEmpty() == true) && (strPass.isEmpty() == true))
    {
        pConfig->set_value("login-nick", "~test");
        strNick = "~test";
    }

    // decrypt pass
    if (strPass.isEmpty() == false)
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->decrypt(strNick, strPass);
        delete pCrypt;
    }

    // correct nick
    if ((strPass.isEmpty() == true) && (strNick[0] != '~'))
    {
        strNick = "~"+strNick;
        pConfig->set_value("login-nick", strNick);
    }
    if ((strPass.isEmpty() == false) && (strNick[0] == '~'))
    {
        strNick = strNick.right(strNick.length()-1);
        pConfig->set_value("login-nick", strNick);
    }

    delete pConfig;

    // update nick
    emit update_nick(strNick);

    // set current nick
    QString strCurrentNick = strNick;
        if (strCurrentNick[0] == '~')
    strCurrentNick = strNick.right(strNick.length()-1);

    // request uo key
    emit request_uo(strCurrentNick, strNick, strPass);
}

void NetworkThread::disconnected()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        connectAct->setText(tr("&Connect"));
        connectAct->setIconText(tr("&Connect"));
        connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
        lagAct->setText("Lag: 0s");

        if (socket->error() != QAbstractSocket::UnknownSocketError)
            emit show_msg_all(QString(tr("Disconnected from server [%1]")).arg(socket->errorString()), 9);
        else
            emit show_msg_all(tr("Disconnected from server"), 9);

        // update nick
        emit update_nick(tr("(Unregistered)"));

        // clear nicklist
        emit clear_all_nicklist();

        // state
        QSettings settings;
        settings.setValue("logged", "off");

        // timer
        timer->stop();
        timerLag->stop();

        // reconnect
        QTimer::singleShot(30*1000, this, SLOT(reconnect()));
    }
}

void NetworkThread::error(QAbstractSocket::SocketError err)
{
    Q_UNUSED (err);

    connectAct->setText(tr("&Connect"));
    connectAct->setIconText(tr("&Connect"));
    connectAct->setIcon(QIcon(":/images/oxygen/16x16/network-disconnect.png"));
    lagAct->setText("Lag: 0s");

    if (socket->state() == QAbstractSocket::ConnectedState)
        emit close();
    else
        emit show_msg_all(QString(tr("Disconnected from server [%1]")).arg(socket->errorString()), 9);
}

void NetworkThread::timeout()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    if (iActive+301 < iCurrent)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
            emit show_msg_all(tr("No PONG reply from server in 301 seconds. Disconnecting..."), 9);
        emit close();
        iActive = iCurrent;
    }
}

void NetworkThread::timeout_lag()
{
    QDateTime dta = QDateTime::currentDateTime();
    int i1 = (int)dta.toTime_t(); // seconds that have passed since 1970
    QString t2 = dta.toString("zzz"); // miliseconds
    emit send(QString("PING :%1.%2").arg(i1).arg(t2));
}
