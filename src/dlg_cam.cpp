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

#include "dlg_cam.h"

dlg_cam::dlg_cam(QSettings *param1, QTcpSocket *param2)
{
    ui.setupUi(this);

    settings = param1;
    irc_socket = param2;

    socket = new QTcpSocket(this);
    timer = new QTimer(this);
    timer->setInterval(15*1000);

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(network_keepalive()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(network_connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(network_disconnected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(network_read()));
}

dlg_cam::~dlg_cam()
{
    delete socket;
}

void dlg_cam::set_nick(QString param1)
{
    strNick = param1;
    ui.label->setText("<p style=\"font-weight:bold;\">"+strNick+"</p>");
}

void dlg_cam::show_img(QByteArray data)
{
    QString path = QCoreApplication::applicationDirPath();

    QDir d(path);
    if (d.exists(path+"/tmp") == false)
        d.mkdir(path+"/tmp");

    QFile f(path+"/tmp/"+strNick+".cam");
    if (!f.open(QIODevice::Append))
        return;

    QTextStream out(&f);
    out << data;

    f.close();

    if (f.exists() == true)
    {
        QString img = path+"/tmp/"+strNick+".cam";
        ui.label_img->setPixmap(QPixmap(img));
    }
}

void dlg_cam::network_connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->connectToHost("czat-s.onet.pl", 5008);
        if (socket->waitForConnected())
            ui.label_img->setText("Po³±czono z serwerem");
        else
            ui.label_img->setText("Nie mo¿na po³±czyæ z serwerem kamerek");
    }
}

void dlg_cam::network_send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
        if (settings->value("debug").toString() == "on")
            qDebug() << "-> " << strData;

        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        socket->write(qbaData);
        if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->waitForBytesWritten() == false))
            ui.label_img->setText(QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()));
        else if (socket->state() == QAbstractSocket::UnconnectedState)
            ui.label_img->setText("Error: Nie uda³o siê wys³aæ danych! [Not connected]");
    }
    else
        ui.label_img->setText("Error: Nie uda³o siê wys³aæ danych! [Not connected]");
}

void dlg_cam::network_disconnect()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        dlg_cam::network_send(QString("UNSUBSCRIBE_BIG * %1").arg(strNick));
        socket->disconnectFromHost();
    }
}

void dlg_cam::network_read()
{
    strDataRecv.append(socket->readAll());
    if (strDataRecv.isEmpty()) return;

    if (strDataRecv[strDataRecv.length()-1] != '\n')
        return;

    QStringList strDataLine = strDataRecv.split("\r\n");
    strDataRecv.clear();

    qDebug() << "recv:" << strDataLine;
}

void dlg_cam::network_connected()
{
    QString strUOKey = settings->value("uokey").toString();
    ui.label_img->setText("Po³±czono z serwerem kamerek");
    dlg_cam::network_send("CAUTH 1234567890123456 3.1(applet)");
    dlg_cam::network_send(QString("AUTH %1 3.1(applet)").arg(strUOKey));
    dlg_cam::network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
    timer->start();
}

void dlg_cam::network_keepalive()
{
    if ((irc_socket->state() == QAbstractSocket::ConnectedState) && (irc_socket->isWritable() == true))
        dlg_cam::network_send(QString("KEEPALIVE_BIG %1").arg(strNick));
    else
        timer->stop();
}

void dlg_cam::network_disconnected()
{
    timer->stop();
    ui.label_img->setText("Roz³±czono z serwerem kamerek");
}

// copy of network::send
void dlg_cam::send(QString strData)
{
    if ((irc_socket->state() == QAbstractSocket::ConnectedState) && (irc_socket->isWritable() == true))
    {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "-> " << strData;
#endif
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        irc_socket->write(qbaData);
        if ((irc_socket->state() == QAbstractSocket::ConnectedState) && (irc_socket->waitForBytesWritten() == false))
        {
            int nop;
            nop = 1;
            //tabc->show_msg("Status", QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
        }
    }
    //else
        //tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

void dlg_cam::button_ok()
{
    this->hide();
}

void dlg_cam::showEvent(QShowEvent *event)
{
    event->accept();

    dlg_cam::network_connect();
}

void dlg_cam::hideEvent(QHideEvent *event)
{
    event->accept();

    dlg_cam::network_disconnect();
}
