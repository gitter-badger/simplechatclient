/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
#include "core.h"
#include "log.h"
#include "kamerzysta.h"

Kamerzysta::Kamerzysta(QTcpSocket *_socket) : socket(_socket)
{
    log("Like constructor");

    timerGetPort = new QTimer();
    timerGetPort->setInterval(500);

    connect(socket, SIGNAL(connected()), this, SLOT(networkConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(networkDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(networkRead()));
    connect(timerGetPort, SIGNAL(timeout()), this, SLOT(getPort()));
}

void Kamerzysta::close()
{
    log("Like destructor");

    QObject::disconnect(timerGetPort, SIGNAL(timeout()), this, SLOT(getPort()));
    QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(networkRead()));
    QObject::disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::disconnect(socket, SIGNAL(disconnected()), this, SLOT(networkDisconnected()));
    QObject::disconnect(socket, SIGNAL(connected()), this, SLOT(networkConnected()));

    delete timerGetPort;
}

void Kamerzysta::show(const QString &n)
{
    strNick = n;

    QString strUOKey = Core::instance()->settings.value("uokey");

    log("Nick:"+strNick);
    log("UO:"+strUOKey);

    getPath();
}

void Kamerzysta::log(const QString &strData)
{
    if (Core::instance()->settings.value("debug") == "true")
        Log::save("kamerzysta", strData);
}

void Kamerzysta::getPath()
{
    QSettings winSettings(QSettings::UserScope, "Onet.pl", "InstalledApps");
    winSettings.beginGroup("Kamerzysta");
    strAppPath = winSettings.value("DataPath").toString();

    QDir dir;
    if (!dir.exists(strAppPath))
    {
        QMessageBox::critical(0, tr("Error"), tr("Kamerzysta not found!"));
        close();
        return;
    }

    log("App path:"+strAppPath);

    if (QFile::exists(strAppPath+"\\port"))
    {
        log("Port:exist");
        kamerzystaRunning();
    }
    else
    {
        log("Port:not exist");
        kamerzystaNotRunning();
    }
}

void Kamerzysta::kamerzystaNotRunning()
{
    log("Kamerzysta not running");

    if (!QFile::exists(strAppPath+"\\installPath"))
    {
        QMessageBox::critical(0, tr("Error"), tr("Kamerzysta is wrong installed!"));
        close();
        return;
    }
    else
    {
        QFile file(strAppPath+"\\installPath");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(0, tr("Error"), tr("Kamerzysta is wrong installed!"));
            close();
            return;
        }

        QTextStream ts(&file);
        strKamerzystaFile = ts.readLine();
        file.close();
    }

    log("Install path:"+strKamerzystaFile);

    if ((!strKamerzystaFile.isEmpty()) && (QFile::exists(strKamerzystaFile)))
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

void Kamerzysta::kamerzystaRunning()
{
    log("Kamerzysta running");

    // send
    networkSend(QString("e%1").arg(strNick));

    // close
    close();
}

void Kamerzysta::getPort()
{
    QString strPort;

    QFile file(strAppPath+"\\port");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream ts(&file);
        strPort = ts.readLine();
        iPort = strPort.toInt();
        file.close();
    }

    log("Port:"+strPort);

    if (!strPort.isEmpty())
    {
        timerGetPort->stop();
        networkConnect();
    }
    else
    {
        iTryGetPort++;
    }
}

void Kamerzysta::authorize()
{
    QString strMe = Core::instance()->settings.value("nick");
    QString strUOKey = Core::instance()->settings.value("uokey");

    networkSend(QString("d%1|%2").arg(strMe, strUOKey));

    if (strNick.isEmpty())
        log("Nick empty!");

    if ((strNick != strMe) && (!strNick.isEmpty()))
        networkSend(QString("e%1").arg(strNick, strUOKey));
}

void Kamerzysta::invite(QString strInviteNick)
{
    strInviteNick.remove(0,1);
    log(QString("Invite:%1").arg(strInviteNick));

    Core::instance()->pNetwork->send(QString("PRIV %1").arg(strInviteNick));
}

void Kamerzysta::networkConnect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        log("Connecting...");
        socket->connectToHost(QHostAddress::LocalHost, iPort);
    }
}

void Kamerzysta::networkConnected()
{
    log("Connected");
}

void Kamerzysta::networkDisconnect()
{
    log("Disconnecting...");

    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void Kamerzysta::networkDisconnected()
{
    log("Disconnected");

    // clear nick
    strNick = "";

    // stop timer
    if (timerGetPort->isActive())
        timerGetPort->stop();

    // like destructor
    close();
}

void Kamerzysta::networkSend(const QString &strData)
{
    if ((socket->isValid()) && (socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable()))
    {
        log("Send:"+strData);

        socket->write((strData+"\r\n").toAscii());
    }
}

void Kamerzysta::networkRead()
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
        if (!strDataRecv.isEmpty())
        {
            if (strDataRecv == "d")
                authorize();
            else if (strDataRecv[0] == 'e')
                invite(strDataRecv);
        }
    }
}

void Kamerzysta::error(QAbstractSocket::SocketError)
{
    log("Error:"+socket->errorString());

    if (socket->state() == QAbstractSocket::ConnectedState)
        networkDisconnect();
    else
    {
        // clear nick
        strNick = "";

        // stop timer
        if (timerGetPort->isActive())
            timerGetPort->stop();

        // like destructor
        close();
    }
}
