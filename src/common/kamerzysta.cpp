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

#include <QDir>
#include <QFile>
#include <QHostAddress>
#include <QIcon>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QTcpSocket>
#include <QTimer>
#include <QTextStream>
#include "config.h"
#include "log.h"
#include "kamerzysta.h"

Kamerzysta::Kamerzysta(QTcpSocket *param1)
{
    log("Like constructor");

    socket = param1;

    timerGetPort = new QTimer();
    timerGetPort->setInterval(500);

    QObject::connect(socket, SIGNAL(connected()), this, SLOT(network_connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(network_disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(network_read()));
    QObject::connect(timerGetPort, SIGNAL(timeout()), this, SLOT(get_port()));
}

void Kamerzysta::close()
{
    log("Like destructor");

    QObject::disconnect(timerGetPort, SIGNAL(timeout()), this, SLOT(get_port()));
    QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(network_read()));
    QObject::disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::disconnect(socket, SIGNAL(disconnected()), this, SLOT(network_disconnected()));
    QObject::disconnect(socket, SIGNAL(connected()), this, SLOT(network_connected()));

    delete timerGetPort;
}

void Kamerzysta::show(QString n)
{
    strNick = n;

    QSettings settings;
    QString strUOKey = settings.value("uokey").toString();

    log("Nick:"+strNick);
    log("UO:"+strUOKey);

    get_path();
}

void Kamerzysta::log(QString strData)
{
    QSettings settings;
    if (settings.value("debug").toString() == "on")
    {
        Log *l = new Log();
        l->save("kamerzysta", strData);
        delete l;
    }
}

void Kamerzysta::get_path()
{
    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
    settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    strAppPath = settings.value("AppData").toString();

    QDir dir;
    if (dir.exists(strAppPath+"/Kamerzysta") == true)
    {
        strAppPath = strAppPath+"/Kamerzysta";
        strAppPath.replace("/", "\\");
    }
    else
    {
        QMessageBox::critical(0, tr("Error"), tr("Kamerzysta not found!"));
        close();
        return;
    }

    log("App path:"+strAppPath);

    if (QFile::exists(strAppPath+"\\port") == true)
    {
        log("Port:exist");
        kamerzysta_running();
    }
    else
    {
        log("Port:not exist");
        kamerzysta_not_running();
    }
}

void Kamerzysta::kamerzysta_not_running()
{
    log("Kamerzysta not running");

    if (QFile::exists(strAppPath+"\\installPath") == false)
    {
        QMessageBox::critical(0, tr("Error"), tr("Kamerzysta is wrong installed!"));
        close();
        return;
    }
    else
    {
        QFile file(strAppPath+"\\installPath");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        {
            QMessageBox::critical(0, tr("Error"), tr("Kamerzysta is wrong installed!"));
            close();
            return;
        }

        QTextStream in(&file);
        strKamerzystaFile = in.readLine();
        file.close();
    }

    log("Install path:"+strKamerzystaFile);

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

        // get port
        timerGetPort->start();
    }
    else
    {
        QMessageBox::critical(0, tr("Error"), tr("Kamerzysta is wrong installed!"));
        close();
        return;
    }
}

void Kamerzysta::kamerzysta_running()
{
    log("Kamerzysta running");

    // send
    network_send(QString("e%1").arg(strNick));

    // close
    close();
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

    log("Port:"+strPort);

    if (strPort.isEmpty() == false)
    {
        timerGetPort->stop();
        network_connect();
    }
    else
    {
        iTryGetPort++;
    }
}

void Kamerzysta::authorize()
{
    QSettings settings;
    QString strMe = settings.value("nick").toString();
    QString strUOKey = settings.value("uokey").toString();

    network_send(QString("d%1|%2").arg(strMe).arg(strUOKey));

    if (strNick.isEmpty())
        log("Nick empty!");

    if ((strNick != strMe) && (strNick.isEmpty() == false))
        network_send(QString("e%1").arg(strNick).arg(strUOKey));
}

void Kamerzysta::network_connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        log("Connecting...");
        socket->connectToHost(QHostAddress::LocalHost, iPort);
    }
}

void Kamerzysta::network_connected()
{
    log("Connected");
}

void Kamerzysta::network_disconnect()
{
    log("Disconnecting...");

    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void Kamerzysta::network_disconnected()
{
    log("Disconnected");

    // clear nick
    strNick = "";

    // stop timer
    if (timerGetPort->isActive() == true)
        timerGetPort->stop();

    // like destructor
    close();
}

void Kamerzysta::network_send(QString strData)
{
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i).toAscii());

        log("Send:"+QString(qbaData));

        socket->write(qbaData);
    }
}

void Kamerzysta::network_read()
{
    log("Ready read");

    while(socket->canReadLine())
    {
        // read line
        QByteArray data = socket->readLine().trimmed();
        QString strDataRecv = QString(data);

        // log
        log("Recv:"+strDataRecv);

        // data
        if (strDataRecv == "d")
            authorize();
    }
}

void Kamerzysta::error(QAbstractSocket::SocketError err)
{
    Q_UNUSED (err);

    log("Error:"+socket->errorString());

    if (socket->state() == QAbstractSocket::ConnectedState)
        network_disconnect();
    else
    {
        // clear nick
        strNick = "";

        // stop timer
        if (timerGetPort->isActive() == true)
            timerGetPort->stop();

        // like destructor
        close();
    }
}
