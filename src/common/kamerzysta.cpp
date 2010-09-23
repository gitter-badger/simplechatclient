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

#include "kamerzysta.h"

Kamerzysta::Kamerzysta(QTcpSocket *param1)
{
    socket = param1;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    QObject::connect(socket, SIGNAL(connected()), this, SLOT(network_connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(network_disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(network_read()));
}

Kamerzysta::~Kamerzysta()
{
    network_disconnect();
}

void Kamerzysta::show(QString param1, QString param2)
{
    strNick = param1;
    strUOKey = param2;

    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Nick:"+strNick);
        l->save("kamerzysta", "UO:"+strUOKey);
        delete l;
    }

    get_path();
}

void Kamerzysta::get_path()
{
    strAppPath = QDir::toNativeSeparators(QDir::homePath());
    QDir dir;

    if (dir.exists(strAppPath+"\\Application Data\\Kamerzysta") == true)
        strAppPath = strAppPath+"\\Application Data\\Kamerzysta";
    else if (dir.exists(strAppPath+"\\AppData\\Kamerzysta") == true)
        strAppPath = strAppPath+"\\AppData\\Kamerzysta";
    else if (dir.exists(strAppPath+"\\AppData\\Local\\Kamerzysta") == true)
        strAppPath = strAppPath+"\\AppData\\Local\\Kamerzysta";
    else if (dir.exists(strAppPath+"\\AppData\\LocalLow\\Kamerzysta") == true)
        strAppPath = strAppPath+"\\AppData\\LocalLow\\Kamerzysta";
    else if (dir.exists(strAppPath+"\\AppData\\Roaming\\Kamerzysta") == true)
        strAppPath = strAppPath+"\\AppData\\Roaming\\Kamerzysta";
    else if (dir.exists(strAppPath+"\\Dane aplikacji\\Kamerzysta") == true)
        strAppPath = strAppPath+"\\Dane aplikacji\\Kamerzysta";
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText(QString(tr("Kamerzysta not found!")));
        msgBox.exec();
        return;
    }

    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "App path:"+strAppPath);
        delete l;
    }

    if (QFile::exists(strAppPath+"\\port") == true)
    {
        if (settings.value("debug").toString() == "on")
        {
            Log *l = new Log();
            l->save("kamerzysta", "Port:exist");
            delete l;
        }

        kamerzysta_running();
    }
    else
    {
        if (settings.value("debug").toString() == "on")
        {
            Log *l = new Log();
            l->save("kamerzysta", "Port:not exist");
            delete l;
        }

        kamerzysta_not_running();
    }
}

void Kamerzysta::kamerzysta_not_running()
{
    if (QFile::exists(strAppPath+"\\installPath") == false)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText(QString(tr("Kamerzysta is wrong installed!")));
        msgBox.exec();
        return;
    }
    else
    {
        QFile file(strAppPath+"\\installPath");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
            return;

        QTextStream in(&file);
        strKamerzystaFile = in.readLine();
        file.close();
    }

    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Install path:"+strKamerzystaFile);
        delete l;
    }

    if ((strKamerzystaFile.isEmpty() == false) && (QFile::exists(strKamerzystaFile) == true))
    {
        int iPos = strKamerzystaFile.lastIndexOf("\\");
        QString strKamerzystaPath = strKamerzystaFile.left(iPos);
        strKamerzystaPath.replace("\\", "\\\\");
        strKamerzystaFile = strKamerzystaFile.right(strKamerzystaFile.length() - iPos -1);

        // change current path
        QString path = QDir::currentPath();
        QDir::setCurrent(strKamerzystaPath);

        // start kamerzysta.exe
        QProcess::startDetached(strKamerzystaFile);

        // change current path
        QDir::setCurrent(path);

        // network
        iTryGetPort = 0;
        QTimer::singleShot(1, this, SLOT(get_port()));
    }
}

void Kamerzysta::kamerzysta_running()
{
    network_send(QString("e%1").arg(strNick));
}

void Kamerzysta::network_connected()
{
    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Connected");
        delete l;
    }
}

void Kamerzysta::network_disconnected()
{
    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Disconnected");
        delete l;
    }
}

void Kamerzysta::get_port()
{
    QString strPort;

    QFile file(strAppPath+"\\port");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text) == true)
    {
        QTextStream in(&file);
        strPort = in.readLine();
        iPort = strPort.toInt();
        file.close();
    }

    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Port:"+strPort);
        delete l;
    }

    iTryGetPort++;
    if (strPort.isEmpty() == false)
        network_connect();
    else
    {
        if (iTryGetPort < 120)
            QTimer::singleShot(1000*1, this, SLOT(get_port()));
    }
}

void Kamerzysta::network_connect()
{
    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Connecting...");
        delete l;
    }

    if (socket->state() == QAbstractSocket::UnconnectedState)
        socket->connectToHost(QHostAddress::LocalHost, iPort);
}

void Kamerzysta::network_send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i).toAscii());

        QSettings settings;
        if (settings.value("debug").toString() == "on")
        {
            Log *l = new Log();
            l->save("kamerzysta", "Send:"+QString(qbaData));
            delete l;
        }

        socket->write(qbaData);
    }
}

void Kamerzysta::network_disconnect()
{
    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Disconnecting...");
        delete l;
    }

    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void Kamerzysta::network_read()
{
    while(socket->canReadLine())
    {
        // read line
        QByteArray data = socket->readLine().trimmed();
        QString strDataRecv = QString(data);

        // data
        if (strDataRecv == "d")
        {
            Config *pConfig = new Config();
            QString strMe = pConfig->get_value("login-nick");
            delete pConfig;

            network_send(QString("d%1|%2").arg(strMe).arg(strUOKey));
            if (strNick != strMe)
                network_send(QString("e%1").arg(strNick).arg(strUOKey));
        }

        QSettings settings;
        if (settings.value("debug").toString() == "on")
        {
            Log *l = new Log();
            l->save("kamerzysta", "Recv:"+strDataRecv);
            delete l;
        }
    }
}

void Kamerzysta::error(QAbstractSocket::SocketError err)
{
    Q_UNUSED (err);

    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", "Error:"+socket->errorString());
        delete l;
    }

    if (socket->state() == QAbstractSocket::ConnectedState)
        network_disconnect();
}
