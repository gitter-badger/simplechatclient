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

#include "dlg_cam.h"

DlgCam::DlgCam(Network *param1, QSettings *param2, QString param3)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Webcam"));

    pNetwork = param1;
    settings = param2;
    strNick = param3;
    ui.label->setText("<p style=\"font-weight:bold;\">"+strNick+"</p>");
}

void DlgCam::show_img(QByteArray bData)
{
    QPixmap pixmap;
    pixmap.loadFromData(bData);
    ui.label_img->setPixmap(pixmap);
}

void DlgCam::network_connect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->connectToHost("czat-s.onet.pl", 5008);

        bText = true;
        bAuthorized = false;
        iBytes_need = 0;
        iBytes_recv = 0;
        iCam_cmd = 0;
    }
}

void DlgCam::network_send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "CAM -> " << strData;
#endif

        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        if (socket->write(qbaData) == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
                ui.label_img->setText(QString(tr("Error: Failed to send data! [%1]")).arg(socket->errorString()));
            else if (socket->state() == QAbstractSocket::UnconnectedState)
                ui.label_img->setText(tr("Error: Failed to send data! [Not connected]"));
        }
    }
    else
        ui.label_img->setText(tr("Error: Failed to send data! [Not connected]"));
}

void DlgCam::network_disconnect()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        network_send(QString("UNSUBSCRIBE_BIG * %1").arg(strNick));
        socket->disconnectFromHost();
    }

    if (timer->isActive() == true)
        timer->stop();
}

void DlgCam::network_read()
{
    if (bText == true)
    {
        if (socket->bytesAvailable() <= 0) return;

        for (int i = 0; i < socket->bytesAvailable(); i++)
        {
            QString b = socket->read(1);
            strDataRecv += b;
            if (b == "\n") break;
        }
    }
    else
    {
        if (socket->bytesAvailable() <= 0) return;

        for (int i = 0; i < socket->bytesAvailable(); i++)
        {
            if (iBytes_recv < iBytes_need)
            {
                bData += socket->read(1);
                iBytes_recv++;
            }
        }
    }

    if ((bText == false) && (iBytes_recv < iBytes_need)) network_read();
    if ((bText == false) && (iBytes_recv == iBytes_need))
    {
        if (iCam_cmd == 202)
            show_img(bData);
        else if (iCam_cmd == 252)
        {
            QString strDesc = bData;
            if (strDesc.left(9) == "SETSTATUS")
            {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "CAM <- " << strDesc;
#endif
                QString strStatus = strDesc.right(strDesc.length()-10);
                ui.textEdit->setText(strStatus);
            }
        }
        else if (iCam_cmd == 403)
        {
            QString strError = bData;
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "CAM <- " << strError;
#endif
            QString strImg = ui.label_img->text() +"<br>"+strError;
            ui.label_img->setText(strImg);
        }

        bData.clear();
        iBytes_need = 0;
        iBytes_recv = 0;
        iCam_cmd = 0;
        bText = true;

        network_read();
    }

    if ((bText == true) && (bAuthorized == false))
        strDataRecv += "\r\n";

    if ((bText == true) && (strDataRecv.length() > 2) && (strDataRecv.at(strDataRecv.length()-1) != '\n')) network_read();
    else if ((bText == true) && (strDataRecv.length() > 2) && (strDataRecv.at(strDataRecv.length()-1) == '\n'))
    {
        strDataRecv = strDataRecv.left(strDataRecv.length()-2);
        QStringList strDataList = strDataRecv.split(" ");

#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "CAM <- " << strDataRecv;
#endif

        strDataRecv.clear();

        // 202 17244 IMAGE_UPDATE_BIG Ekscentryk
        if (strDataList[0] == "202")
        {
            iBytes_need = strDataList[1].toInt();
            if (iBytes_need != 0)
            {
                bText = false;
                iCam_cmd = 202;
            }
            else
            {
                iBytes_need = 0;
                ui.label_img->setText(tr("This user does not send data"));
                network_disconnect();
            }
        }
        // 211 19995 Noemi_01@0
        // 211 13584 Noemi_01@1
        // 211 13584 Noemi_01@2
        // 211 13584 Noemi_01@3
        // 211 29 my_dwoje@4
        // 211 17 myszka29brunetka@5
        else if (strDataList[0] == "211")
        {
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 211;
        }
        // 231 0 OK scc_test
        else if (strDataList[0] == "231")
        {
            ui.label_img->setText(tr("Setting mode for viewing only"));
            network_send("SENDMODE 0");
            ui.label_img->setText(tr("Downloading image"));
            network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
            timer->start();
        }
        // 232 0 CMODE 0
        else if (strDataList[0] == "232")
        {
            // nothing
        }
        // 233 0 QUALITY_FACTOR 1
        else if (strDataList[0] == "233")
        {
            // nothing
        }
        // 250 12519 OK
        else if (strDataList[0] == "250")
        {
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 250;
        }
        // 251 52 UPDATE
        else if (strDataList[0] == "251")
        {
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 251;
        }
        // 252 41 USER_STATUS pati28ash
        else if (strDataList[0] == "252")
        {
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 252;
        }
        // 253 0 USER_VOTES Delikatna 38
        else if (strDataList[0] == "253")
        {
            // nothing
        }
        // 254 1489 USER_COUNT_UPDATE
        else if (strDataList[0] == "254")
        {
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 254;
        }
        // 264 0 CODE_ACCEPTED ffffffff 2147483647
        else if (strDataList[0] == "264")
        {
            // nothing
        }
        // 267 0 SENDMODE=0
        else if (strDataList[0] == "267")
        {
            // nothing
        }
        // 268 0
        else if (strDataList[0] == "268")
        {
            QString strUOKey = settings->value("uokey").toString();
            network_send(QString("AUTH %1 3.00.159").arg(strUOKey));
            bAuthorized = true;
        }
        // 403 11 ACCESS_DENIED
        // Invalid key
        else if (strDataList[0] == "403")
        {
            ui.label_img->setText(tr("Authorization failed"));
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 403;
        }
        // 405 0 USER_GONE Restonka
        else if (strDataList[0] == "405")
        {
            ui.label_img->setText(tr("The specified user has left the chat"));
            network_disconnect();
        }
        // 408 0 NO_SUCH_USER_SUBSCRIBE LOLexx
        else if (strDataList[0] == "408")
        {
            ui.label_img->setText(tr("The specified user does not have a webcam enabled"));
            network_disconnect();
        }
        // 412 0 SUBSCRIBE_FAILED olgusia32
        else if (strDataList[0] == "412")
        {
            ui.label_img->setText(tr("Failed to retrieve the image from the webcam"));
            network_disconnect();
        }
        // 413 0 SUBSCRIBE_DENIED aliina
        else if (strDataList[0] == "413")
        {
            ui.label_img->setText(tr("Failed to retrieve the image from the webcam"));
            network_disconnect();
        }
        // 508 0 SESSION_OVERRIDEN
        else if (strDataList[0] == "508")
        {
            // nothing
        }
        // 520 0 INVALID_UOKEY 1q3j0llVg40cu2784j9EVoz8sRdfNl3w
        else if (strDataList[0] == "520")
        {
            ui.label_img->setText(tr("Invalid authorization key"));
            network_disconnect();
        }

        network_read();
    }
}

void DlgCam::network_connected()
{
    ui.label_img->setText(tr("Connected to server webcam."));
    ui.label_img->setText(ui.label_img->text()+"<br>"+tr("Please wait ..."));
    network_send(QString("CAUTH %1 3.00.159").arg("1234567890123456"));
}

void DlgCam::network_keepalive()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        network_send(QString("KEEPALIVE_BIG %1").arg(strNick));
    else
        timer->stop();
}

void DlgCam::network_disconnected()
{
    timer->stop();

    QString strText = ui.label_img->text();
    strText += "<br>"+tr("Disconnected from server webcams");
    ui.label_img->setText(strText);
}

void DlgCam::error(QAbstractSocket::SocketError err)
{
    ui.label_img->setText(QString(tr("Disconnected from server [%1]")).arg(socket->errorString()));

    if (socket->state() == QAbstractSocket::ConnectedState)
        network_disconnect();
}

void DlgCam::button_ok()
{
    this->close();
}

void DlgCam::showEvent(QShowEvent *event)
{
    event->accept();

    ui.label_img->setText(tr("Starting the service webcams"));
    ui.textEdit->setText("");

    socket = new QTcpSocket();
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    timer = new QTimer();
    timer->setInterval(2*1000); // 2 sec

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(network_keepalive()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(network_connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(network_disconnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(network_read()));

    network_connect();
}

void DlgCam::hideEvent(QHideEvent *event)
{
    event->accept();

    network_disconnect();
    delete socket;

    ui.buttonBox->QObject::disconnect();
    timer->QObject::disconnect();
}
