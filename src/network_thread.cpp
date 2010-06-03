/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

networkThread::networkThread(QAction *param1, QSettings *param2)
{
    connectAct = param1;
    settings = param2;

    strServer = "czat-app.onet.pl";
    iPort = 5015;

    iActive = 0;
    settings->setValue("reconnect", "true");
    timer = new QTimer();
    timer->setInterval(1*60*1000); // 1 min

    socket = new QTcpSocket(this);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

networkThread::~networkThread()
{
    if (timer->isActive() == true)
        timer->stop();
    socket->close();
    delete socket;
}

void networkThread::run()
{
    exec();
}

bool networkThread::is_connected()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        return true;
    else
        return false;
}

bool networkThread::is_writable()
{
    return socket->isWritable();
}

void networkThread::connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        QDateTime dt = QDateTime::currentDateTime();
        iActive = (int)dt.toTime_t();

        socket->connectToHost(strServer, iPort);
        timer->start();
    }
    else
        emit show_msg_all("Error: Nie mo¿na siê po³±czyæ z serwerem - po³±czenie ju¿ istnieje!", 9);
}

void networkThread::reconnect()
{
    if (settings->value("reconnect").toString() == "true")
    {
        if ((this->is_connected() == false) && (settings->value("logged").toString() == "off"))
        {
            emit show_msg_all("Ponowne ³±czenie z serwerem...", 7);
            emit connect();
        }
    }
}

void networkThread::close()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();

    if (timer->isActive() == true)
        timer->stop();
}

void networkThread::send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "-> " << strData;
#endif
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        if (socket->write(qbaData) == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
                emit show_msg_active(QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
            else if (socket->state() == QAbstractSocket::UnconnectedState)
                emit show_msg_active("Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
        }
    }
    else
        emit show_msg_active("Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

void networkThread::recv()
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
        QTimer::singleShot(3*1000, this, SLOT(recv()));
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
        QTimer::singleShot(3*1000, this, SLOT(recv()));
}

void networkThread::connected()
{
    emit show_msg_all("Po³±czono z serwerem", 9);

    config *pConfig = new config();

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
        qcrypt *pCrypt = new qcrypt();
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

    // send auth
    emit send(QString("NICK %1").arg(strNick));
    emit send("AUTHKEY");

    // request uo key
    emit request_uo(strCurrentNick, strPass);
}

void networkThread::disconnected()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        connectAct->setText("&Po³±cz");
        connectAct->setIconText("&Po³±cz");

        if (socket->error() != QAbstractSocket::UnknownSocketError)
            emit show_msg_all(QString("Roz³±czono z serwerem [%1]").arg(socket->errorString()), 9);
        else
            emit show_msg_all("Roz³±czono z serwerem", 9);

        // update nick
        emit update_nick("(niezalogowany)");

        // clear nicklist
        emit clear_all_nicklist();

        // state
        settings->setValue("logged", "off");

        // timer
        timer->stop();

        // reconnect
        QTimer::singleShot(30*1000, this, SLOT(reconnect()));
    }
}

void networkThread::error(QAbstractSocket::SocketError err)
{
    connectAct->setText("&Po³±cz");
    connectAct->setIconText("&Po³±cz");

    emit show_msg_all(QString("Roz³±czono z serwerem [%1]").arg(socket->errorString()), 9);

    if (socket->state() == QAbstractSocket::ConnectedState)
        emit close();
}

void networkThread::timeout()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    if (iActive+301 < iCurrent)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
            emit show_msg_all("Serwer nieaktywny od 301 sekund. Roz³±czanie...", 9);
        emit close();
        iActive = iCurrent;
    }
}
