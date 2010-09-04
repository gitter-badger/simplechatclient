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

    if (QFile::exists(strAppPath+"\\port") == true)
        kamerzysta_running();
    else
        kamerzysta_not_running();
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
        QTimer::singleShot(1000*10, this, SLOT(network_create()));
    }
}

void Kamerzysta::kamerzysta_running()
{
    network_send(QString("e%1").arg(strNick));
}

void Kamerzysta::network_create()
{
    get_port();
    network_connect();
}

void Kamerzysta::get_port()
{
    QFile file(strAppPath+"\\port");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString strPort = in.readLine();
    iPort = strPort.toInt();
    file.close();
}

void Kamerzysta::network_connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
        socket->connectToHost("localhost", iPort);
}

void Kamerzysta::network_send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i).toAscii());

        socket->write(qbaData);
    }
}

void Kamerzysta::network_disconnect()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void Kamerzysta::network_read()
{
    if (socket->bytesAvailable() <= 0) return;

    for (int i = 0; i < socket->bytesAvailable(); i++)
    {
        QString b = socket->read(1);
        strDataRecv += b;
    }

    strDataRecv.replace(QRegExp("(\r|\n)"), "");

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

    strDataRecv.clear();
    network_read();
}

void Kamerzysta::error(QAbstractSocket::SocketError err)
{
    Q_UNUSED (err);

    if (socket->state() == QAbstractSocket::ConnectedState)
        network_disconnect();
}