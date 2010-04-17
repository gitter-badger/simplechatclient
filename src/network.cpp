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

#include "network.h"

network::network(QAction *param1, QSettings *param2)
{
    connectAct = param1;
    settings = param2;

    iActive = 0;
    settings->setValue("reconnect", "true");
    timer = new QTimer();
    timer->setInterval(1*60*1000); // 1 min
    timer->start();
    socket = new QTcpSocket(this);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

network::~network()
{
    delete pIrc_auth;
    timer->stop();
    socket->close();
    delete socket;
}

void network::set_hostport(QString s, int p)
{
    strServer = s;
    iPort = p;
}

void network::set_dlg(tab_container *param1, dlg_channel_settings *param2, dlg_channel_homes *param3, dlg_channel_list *param4, dlg_channel_favourites *param5, dlg_friends *param6, dlg_ignore *param7, dlg_moderation *param8)
{
    tabc = param1;
    dlgchannel_settings = param2;
    dlgchannel_homes = param3;
    dlgchannel_list = param4;
    dlgchannel_favourites = param5;
    dlgfriends = param6;
    dlgignore = param7;
    dlgmoderation = param8;

    pIrc_auth = new irc_auth(settings, tabc, socket);
}

bool network::is_connected()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        return true;
    else
        return false;
}

bool network::is_writable()
{
    return socket->isWritable();
}

void network::connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->connectToHost(strServer, iPort);
        if (socket->waitForConnected())
            tabc->show_msg_all("Po³±czono z serwerem", 9);
        else
        {
            tabc->show_msg("Status", QString("Error: Nie mo¿na siê po³±czyæ z serwerem! [%1]").arg(socket->errorString()), 9);
            connectAct->setText("&Po³±cz");
            connectAct->setIconText("&Po³±cz");

            // reconnect
            QTimer::singleShot(30*1000, this, SLOT(reconnect()));
        }
    }
    else
        tabc->show_msg_all("Error: Nie mo¿na siê po³±czyæ z serwerem - po³±czenie ju¿ istnieje!", 9);
}

void network::reconnect()
{
    if (settings->value("reconnect").toString() == "true")
    {
        connectAct->setText("&Roz³±cz");
        connectAct->setIconText("&Roz³±cz");
        if ((network::is_connected() == true) && (settings->value("logged").toString() == "off"))
        {
            tabc->show_msg_all("Ponowne ³±czenie z serwerem...", 7);
            config *pConfig = new config();
            QString strNick = pConfig->get_value("login-nick");
            QString strPass = pConfig->get_value("login-pass");
            delete pConfig;

            // update nick
            tabc->update_nick(strNick);

            if (strPass.isEmpty() == false)
            {
                qcrypt *pCrypt = new qcrypt();
                strPass = pCrypt->decrypt(strNick, strPass);
                delete pCrypt;
            }

            network::send(QString("NICK %1").arg(strNick));
            network::send("AUTHKEY");
            QString strNickCurrent = strNick;
            if (strNickCurrent[0] == '~')
                strNickCurrent = strNick.right(strNick.length()-1);
            pIrc_auth->request_uo(strNickCurrent, strPass);
        }
        else
        {
            if (settings->value("logged").toString() == "off")
                network::connect();
            if ((network::is_connected() == true) && (settings->value("logged").toString() == "off"))
                network::reconnect();
        }
    }
}

void network::close()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->disconnectFromHost();
        if ((socket->state() == QAbstractSocket::UnconnectedState) || (socket->waitForDisconnected()))
        {
            int nop;
            nop = 1;
        }
    }
}

void network::send(QString strData)
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

        socket->write(qbaData);
        if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->waitForBytesWritten() == false))
            tabc->show_msg_active(QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
        else if (socket->state() == QAbstractSocket::UnconnectedState)
            tabc->show_msg_active("Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
    }
    else
        tabc->show_msg_active("Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

void network::recv()
{
    bool bCompleted = true;

    strDataRecv.append(socket->readAll());
    if (strDataRecv.isEmpty()) return;

    if (strDataRecv[strDataRecv.length()-1] != '\n')
    {
        if (socket->bytesAvailable() != 0)
            network::recv();
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
        {
            mutex.lock();
            irc_kernel *pIrc_kernel = new irc_kernel(socket, tabc, strLine, settings, dlgchannel_settings, dlgchannel_homes, dlgchannel_list, dlgchannel_favourites, dlgfriends, dlgignore, dlgmoderation);
            pIrc_kernel->kernel();
            delete pIrc_kernel;
            mutex.unlock();
        }
    }

    if (bCompleted == false)
        QTimer::singleShot(3*1000, this, SLOT(recv()));
}

void network::disconnected()
{
    if ((socket->state() == QAbstractSocket::UnconnectedState) || (socket->waitForDisconnected()))
    {
        connectAct->setText("&Po³±cz");
        connectAct->setIconText("&Po³±cz");
        if (socket->error() != QAbstractSocket::UnknownSocketError)
            tabc->show_msg_all(QString("Roz³±czono z serwerem [%1]").arg(socket->errorString()), 9);
        else
            tabc->show_msg_all("Roz³±czono z serwerem", 9);

        // update nick
        tabc->update_nick("(niezalogowany)");

        // clear nicklist
        QStringList strlOpenChannels = tabc->get_open_channels();
        for (int i = 0; i < strlOpenChannels.size(); i++)
            tabc->clear_nicklist(strlOpenChannels[i]);

        // state
        settings->setValue("logged", "off");

        // reconnect
        QTimer::singleShot(30*1000, this, SLOT(reconnect()));
    }
}

void network::timeout()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    if (iActive+301 < iCurrent)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
            tabc->show_msg_all("Serwer nieaktywny od 301 sekund. Roz³±czanie...", 9);
        network::close();
    }
}
