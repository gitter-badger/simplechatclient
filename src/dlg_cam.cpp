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

dlg_cam::dlg_cam(QSettings *param1, QTcpSocket *param2, QString param3)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    settings = param1;
    irc_socket = param2;
    strNick = param3;
    ui.label->setText("<p style=\"font-weight:bold;\">"+strNick+"</p>");
}

void dlg_cam::show_img(QByteArray bData)
{
    QPixmap pixmap;
    pixmap.loadFromData(bData);
    ui.label_img->setPixmap(pixmap);
}

void dlg_cam::network_connect()
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

void dlg_cam::network_send(QString strData)
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
                ui.label_img->setText(QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()));
            else if (socket->state() == QAbstractSocket::UnconnectedState)
                ui.label_img->setText("Error: Nie uda³o siê wys³aæ danych! [Not connected]");
        }
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

    if ((bText == false) && (iBytes_recv < iBytes_need)) dlg_cam::network_read();
    if ((bText == false) && (iBytes_recv == iBytes_need))
    {
        if (iCam_cmd == 202)
            dlg_cam::show_img(bData);
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

        dlg_cam::network_read();
    }

    if ((bText == true) && (bAuthorized == false))
        strDataRecv += "\r\n";

    if ((bText == true) && (strDataRecv.length() > 2) && (strDataRecv.at(strDataRecv.length()-1) != '\n')) dlg_cam::network_read();
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
                ui.label_img->setText("Podany u¿ytkownik nie wysy³a obrazu");
                dlg_cam::network_disconnect();
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
            ui.label_img->setText("Ustawianie tylko ogl±dania");
            dlg_cam::network_send("SENDMODE 0");
            ui.label_img->setText("Pobieranie obrazu");
            dlg_cam::network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
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
            dlg_cam::network_send(QString("AUTH %1 3.00.159").arg(strUOKey));
            bAuthorized = true;
        }
        // 403 11 ACCESS_DENIED
        // Invalid key
        else if (strDataList[0] == "403")
        {
            ui.label_img->setText("B³±d autoryzacji");
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 403;
        }
        // 405 0 USER_GONE Restonka
        else if (strDataList[0] == "405")
        {
            ui.label_img->setText("Podany u¿ytkownik opu¶ci³ czat");
            dlg_cam::network_disconnect();
        }
        // 408 0 NO_SUCH_USER_SUBSCRIBE LOLexx
        else if (strDataList[0] == "408")
        {
            ui.label_img->setText("Podany u¿ytkownik nie ma w³±czonej kamerki");
            dlg_cam::network_disconnect();
        }
        // 412 0 SUBSCRIBE_FAILED olgusia32
        else if (strDataList[0] == "412")
        {
            ui.label_img->setText("Nie uda³o siê pobraæ obrazu z kamerki");
            dlg_cam::network_disconnect();
        }
        // 413 0 SUBSCRIBE_DENIED aliina
        else if (strDataList[0] == "413")
        {
            ui.label_img->setText("Nie uda³o siê pobraæ obrazu z kamerki");
            dlg_cam::network_disconnect();
        }
        // 508 0 SESSION_OVERRIDEN
        else if (strDataList[0] == "508")
        {
            // nothing
        }

        dlg_cam::network_read();
    }
}

void dlg_cam::network_connected()
{
    ui.label_img->setText("Po³±czono z serwerem kamerek<br>Trwa autoryzacja...");
    dlg_cam::network_send(QString("CAUTH %1 3.00.159").arg("1234567890123456"));
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
    QString strText = ui.label_img->text();
    strText += "<br>Roz³±czono z serwerem kamerek";
    ui.label_img->setText(strText);
}

// copy of network::send
void dlg_cam::send(QString strData)
{
    if ((irc_socket->state() == QAbstractSocket::ConnectedState) && (irc_socket->isWritable() == true))
    {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "CAM -> " << strData;
#endif
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        irc_socket->write(qbaData);
    }
    //else
        //tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

void dlg_cam::button_ok()
{
    this->close();
}

void dlg_cam::showEvent(QShowEvent *event)
{
    event->accept();

    ui.label_img->setText("Uruchamianie obs³ugi kamerek");
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
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(network_read()));

    dlg_cam::network_connect();
}

void dlg_cam::hideEvent(QHideEvent *event)
{
    event->accept();

    timer->stop();
    dlg_cam::network_disconnect();
    delete socket;

    ui.buttonBox->QObject::disconnect();
    timer->QObject::disconnect();
}
