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

network::network(QHttp *param1, QAction *param2, QSettings *param3)
{
    http = param1;
    connectAct = param2;
    settings = param3;

    iActive = 0;
    settings->setValue("reconnect", "true");
    timer = new QTimer();
    timer->setInterval(1000*60*1); // 1 min
    timer->start();
    socket = new QTcpSocket(this);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

network::~network()
{
    timer->stop();
    socket->close();
    delete socket;
}

void network::set_hostport(QString s, int p)
{
    strServer = s;
    iPort = p;
}

void network::set_dlg(tab_container *param1, irc_auth *param2, dlg_channel_settings *param3, dlg_channel_homes *param4, dlg_channel_list *param5, dlg_channel_favourites *param6, dlg_friends *param7, dlg_ignore *param8, dlg_moderation *param9)
{
    tabc = param1;
    pIrc_auth = param2;
    dlgchannel_settings = param3;
    dlgchannel_homes = param4;
    dlgchannel_list = param5;
    dlgchannel_favourites = param6;
    dlgfriends = param7;
    dlgignore = param8;
    dlgmoderation = param9;
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
            network::reconnect();
        }
    }
    else
        tabc->show_msg_all("Error: Nie mo¿na siê po³±czyæ z serwerem - po³±czenie ju¿ istnieje!", 9);
}

void network::reconnect()
{
    if (settings->value("reconnect").toString() == "true")
    {
        tabc->show_msg_all("Ponowne ³±czenie z serwerem...", 7);
        connectAct->setText("&Roz³±cz");
        connectAct->setIconText("&Roz³±cz");
        network::close();
        network::connect();
        if (network::is_connected() == true)
        {
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
    }
}

void network::close()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->disconnectFromHost();
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
    strDataRecv.append(socket->readAll());
    if (strDataRecv.isEmpty()) return;

    if (strDataRecv[strDataRecv.length()-1] != '\n')
    {
        if (socket->bytesAvailable() != 0)
            network::recv();
        else
            return;
    }

    QStringList strDataLine = strDataRecv.split("\r\n");
    strDataRecv.clear();

    QDateTime dt = QDateTime::currentDateTime();
    iActive = (int)dt.toTime_t();

    for (int i = 0; i < strDataLine.size(); i++)
    {
        QString strLine = strDataLine[i];
        if (strLine.isEmpty() == false)
        {
            mutex.lock();
            irc_kernel *pIrc_kernel = new irc_kernel(socket, http, tabc, strLine, settings, dlgchannel_settings, dlgchannel_homes, dlgchannel_list, dlgchannel_favourites, dlgfriends, dlgignore, dlgmoderation);
            pIrc_kernel->kernel();
            delete pIrc_kernel;
            mutex.unlock();
        }
    }
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

        // reconnect
        network::reconnect();
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
