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

DlgCam::DlgCam(QWidget *parent, Network *param1, QTcpSocket *param2) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Webcams"));

    pNetwork = param1;
    camSocket = param2;

    ui.label_nick->setText("<p style=\"font-weight:bold;\">"+strNick+"</p>");
    ui.tabWidget->setTabText(0, tr("Viewing"));
    ui.tabWidget->setTabText(1, tr("Broadcasting"));
    ui.tabWidget->setTabText(2, tr("Settings"));
    ui.label_fun->setText(tr("Funs"));
    ui.label_broadcast_status->setText("<span style=\"color:#ff0000;font-weight:bold;\">"+tr("No bradcasting")+"</span>");
    ui.pushButton_broadcast->setText(tr("Start broadcast"));
    ui.radioButton_broadcast_public->setText(tr("Public"));
    ui.radioButton_broadcast_private->setText(tr("Private"));
    ui.groupBox_settings->setTitle(tr("Settings"));
    ui.label_device->setText(tr("Device:"));

    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.tableWidget_nick_rank_spectators->resizeColumnsToContents();

    // clear
    ui.label_img->setText(tr("Starting the service webcams"));
    ui.textEdit_desc->hide();
    ui.textEdit_desc->setText("");
    ui.tableWidget_nick_rank_spectators->clear();
    ui.listWidget_funs->clear();
    bBroadcasting = false;
    bBroadcasting_pubpriv = false;
    bFirstSendPUT = false;
    bReadySendPUT = true;
    iLastSendPUT = 0;
    iLastKeepAlive = 0;
#ifndef Q_WS_WIN
    bCreatedCaptureCv = false;
#endif

    // set labels
    QStringList strlLabels;
    strlLabels << tr("Nick") << tr("Rank") << tr("Spectators");
    ui.tableWidget_nick_rank_spectators->setHorizontalHeaderLabels(strlLabels);

    //camSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    //camSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    QObject::connect(ui.tableWidget_nick_rank_spectators, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(change_user(int,int)));
    QObject::connect(ui.pushButton_broadcast, SIGNAL(clicked()), this, SLOT(broadcast_start_stop()));
    QObject::connect(ui.radioButton_broadcast_public, SIGNAL(clicked()), this, SLOT(broadcast_public()));
    QObject::connect(ui.radioButton_broadcast_private, SIGNAL(clicked()), this, SLOT(broadcast_private()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
#ifndef Q_WS_WIN
    QObject::connect(camSocket, SIGNAL(connected()), this, SLOT(network_connected()));
    QObject::connect(camSocket, SIGNAL(disconnected()), this, SLOT(network_disconnected()));
    QObject::connect(camSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(network_error(QAbstractSocket::SocketError)));
    QObject::connect(camSocket, SIGNAL(readyRead()), this, SLOT(network_read()));
#endif
}

DlgCam::~DlgCam()
{
    // network disconnect
    network_disconnect();

#ifndef Q_WS_WIN
    // destroy capture
    if (ui.comboBox_device->isEnabled() == true)
    {
        if (bCreatedCaptureCv == true)
        {
            cvReleaseCapture(&captureCv);
            bCreatedCaptureCv = false;
        }
    }
#endif
}

void DlgCam::set_nick(QString strN)
{
    strNick = strN;
    ui.label_nick->setText("<p style=\"font-weight:bold;\">"+strNick+"</p>");

    // clear desc
    ui.textEdit_desc->setText("");
    ui.textEdit_desc->hide();
}

QString DlgCam::get_cauth()
{
    // aplet -> 1234567890123456
    // kamerzysta -> 96 chars (?)

    return "1234567890123456";
}

#ifndef Q_WS_WIN
bool DlgCam::exist_video_device()
{
    // search video device
    for (int i = 0; i < 99; i++)
    {
        if (QFile::exists("/dev/video"+QString::number(i)) == true)
            return true;
    }
    return false;
}

void DlgCam::detect_broadcasting()
{
    // disable broadcasting
    ui.tabWidget->setTabEnabled(1, false);
    ui.comboBox_device->setEnabled(false);

    // clear device combobox
    ui.comboBox_device->clear();

    // search video device
    for (int i = 0; i < 99; i++)
    {
        if (QFile::exists("/dev/video"+QString::number(i)) == true)
        {
            if (ui.comboBox_device->isEnabled() == false)
            {
                ui.comboBox_device->setEnabled(true);
                ui.tabWidget->setTabEnabled(1, true);
            }

            // add video
            ui.comboBox_device->addItem("/dev/video"+QString::number(i));
        }
    }
}

void DlgCam::set_broadcasting()
{
    if (ui.comboBox_device->isEnabled() == true)
    {
        if (bCreatedCaptureCv == false)
        {
            // create capture
            captureCv = cvCreateCameraCapture(CV_CAP_ANY);
            if (!captureCv)
                qWarning() << "Error: cannot create camera capture!";
            else
                bCreatedCaptureCv = true;

            // default selected video
            ui.comboBox_device->setCurrentIndex(0);

            // read self video
            QTimer::singleShot(1000*1, this, SLOT(read_video())); // 1sec
        }
    }
}
#endif

void DlgCam::show_img(QByteArray bData)
{
    QPixmap pixmap;
    pixmap.loadFromData(bData);
    ui.label_img->setPixmap(pixmap);
}

void DlgCam::network_connect()
{
    if (camSocket->state() == QAbstractSocket::UnconnectedState)
    {
        camSocket->connectToHost("czat-s.onet.pl", 5008);

        bText = true;
        iCam_cmd = 0;
        iBytes_need = 0;
        iBytes_recv = 0;
    }
}

void DlgCam::network_send(QString strData)
{
    strData += "\n";

    QByteArray qbaData;
    for (int i = 0; i < strData.size(); i++)
        qbaData.insert(i, strData.at(i).toAscii());

    network_sendb(qbaData);
}

void DlgCam::network_sendb(QByteArray qbaData)
{
    if ((camSocket->state() == QAbstractSocket::ConnectedState) && (camSocket->isWritable() == true))
    {
#ifdef Q_WS_X11
        QSettings settings;
        if (settings.value("debug").toString() == "on")
            qDebug() << "CAM -> " << QString(qbaData);
#endif

        if (camSocket->write(qbaData) == -1)
        {
            if (camSocket->state() == QAbstractSocket::ConnectedState)
                ui.label_img->setText(QString(tr("Error: Failed to send data! [%1]")).arg(camSocket->errorString()));
            else if (camSocket->state() == QAbstractSocket::UnconnectedState)
                ui.label_img->setText(tr("Error: Failed to send data! [Not connected]"));
        }
    }
    else
        ui.label_img->setText(tr("Error: Failed to send data! [Not connected]"));
}

void DlgCam::network_disconnect()
{
    if (camSocket->state() == QAbstractSocket::ConnectedState)
    {
        if (strNick.isEmpty() == false)
        {
            network_send(QString("UNSUBSCRIBE_BIG %1").arg(strNick));
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
        }
        camSocket->disconnectFromHost();
    }
}

void DlgCam::network_read()
{
    // read text
    if (bText == true)
    {
        if (camSocket->bytesAvailable() <= 0) return;

        // read line
        QByteArray data = camSocket->readLine().trimmed();
        QString strData = QString(data);
        text_kernel(strData);
    }
    // read data (image, description)
    else
    {
        if (camSocket->bytesAvailable() <= 0) return;

        for (int i = 0; i < camSocket->bytesAvailable(); i++)
        {
            if (iBytes_recv < iBytes_need)
            {
                bData += camSocket->read(1);
                iBytes_recv++;
            }
        }

        if (iBytes_recv == iBytes_need)
            data_kernel();
    }

    // again
    network_read();
}

void DlgCam::network_connected()
{
    ui.label_img->setText(tr("Connected to server webcam."));
    ui.label_img->setText(ui.label_img->text()+"<br>"+tr("Please wait ..."));
    QString strCAUTH = get_cauth();
    network_send(QString("CAUTH %1 3.00.159").arg(strCAUTH));
}

void DlgCam::network_disconnected()
{
    QString strText = ui.label_img->text();
    strText += "<br>"+tr("Disconnected from server webcams");
    ui.label_img->setText(strText);

    network_send("STOP");
    ui.label_broadcast_status->setText(tr("<span style=\"color:#ff0000;font-weight:bold;\">""No bradcasting")+"</span>");

    // reconnect
    //network_connect();
}

void DlgCam::network_error(QAbstractSocket::SocketError err)
{
    Q_UNUSED (err);

    ui.label_img->setText(QString(tr("Disconnected from server [%1]")).arg(camSocket->errorString()));

    if (camSocket->state() == QAbstractSocket::ConnectedState)
        network_disconnect();
}

void DlgCam::data_kernel()
{
    // 202 14283 IMAGE_UPDATE_BIG psotnica2603
    if (iCam_cmd == 202)
    {
        show_img(bData);
    }
    // scc_test:1:2/0/#Quiz/0,2/0/#Relax/0,2/0/#Scrabble/0,4/0/#scc/0:0::0
    else if (iCam_cmd == 250)
    {
        QString strData(bData);
        QStringList strDataList = strData.split("\n");

        // clear
        ui.tableWidget_nick_rank_spectators->clear();
        ui.tableWidget_nick_rank_spectators->setRowCount(0);
        ui.tableWidget_nick_rank_spectators->verticalHeader()->hide();

        // set labels
        QStringList strlLabels;
        strlLabels << tr("Nick") << tr("Rank") << tr("Spectators");
        ui.tableWidget_nick_rank_spectators->setHorizontalHeaderLabels(strlLabels);

        // set rows
        int rows = strDataList.count()-1;
        ui.tableWidget_nick_rank_spectators->setRowCount(rows);

        int row = 0;
        foreach (QString strLine, strDataList)
        {
            QStringList strLineList = strLine.split(":");
            if (strLineList.count() == 6)
            {
                QString strUser = strLineList[0];
                QString strUnknown1 = strLineList[1]; // always 1 (?)
                QString strChannelsParams = strLineList[2];
                QString strSpectators = strLineList[3];
                QString strUdget = strLineList[4]; // udget (012345)
                QString strRank = strLineList[5]; // -500 to 500

                if (strChannelsParams.isEmpty() == false)
                {
                    QStringList strlChannelsParams = strChannelsParams.split(",");
                    foreach (QString strChannelParams, strlChannelsParams)
                    {
                        QStringList strlChannelParams = strChannelParams.split("/");
                        if (strlChannelParams.count() == 4)
                        {
                            QString strChannelCategory = strlChannelParams[0];
                            QString strUnknown2 = strlChannelParams[1]; // always 0 (?)
                            QString strChannelName = strlChannelParams[2];
                            QString strUnknown3 = strlChannelParams[3]; // always 0 (?)
                        }
                    }
                }

                // add to table
                ui.tableWidget_nick_rank_spectators->setItem(row, 0, new QTableWidgetItem(strUser));
                ui.tableWidget_nick_rank_spectators->setItem(row, 1, new QTableWidgetItem(strRank));
                ui.tableWidget_nick_rank_spectators->setItem(row, 2, new QTableWidgetItem(strSpectators));
                row++;
            }
            else
            {
                // wrong (?)
            }
        }
        ui.tableWidget_nick_rank_spectators->resizeColumnsToContents();
    }
    // scc_test:1:2/0/#Quiz/0,2/0/#Relax/0,2/0/#Scrabble/0,4/0/#scc/0:0::0
    else if (iCam_cmd == 251)
    {
        QString strLine(bData);
        if (strLine.isEmpty() == false)
        {
            QStringList strLineList = strLine.split(":");
            if (strLineList.count() == 6)
            {
                QString strUser = strLineList[0];
                QString strUnknown1 = strLineList[1]; // always 1 (?)
                QString strChannelsParams = strLineList[2];
                QString strSpectators = strLineList[3];
                QString strUdget = strLineList[4]; // udget (012345)
                QString strRank = strLineList[5]; // -500 to 500
                if (strChannelsParams.isEmpty() == false)
                {
                    QStringList strlChannelsParams = strChannelsParams.split(",");
                    foreach (QString strChannelParams, strlChannelsParams)
                    {
                        QStringList strlChannelParams = strChannelParams.split("/");
                        if (strlChannelParams.count() == 4)
                        {
                            QString strChannelCategory = strlChannelParams[0];
                            QString strUnknown2 = strlChannelParams[1]; // always 0 (?)
                            QString strChannelName = strlChannelParams[2];
                            QString strUnknown3 = strlChannelParams[3]; // always 0 (?)
                        }
                    }
                }
            }
            else
            {
                // wrong or nothing changed
            }
        }
    }
    // SETSTATUS //panda
    else if (iCam_cmd == 252)
    {
        QString strDesc = bData;
        if (strDesc.left(9) == "SETSTATUS")
        {
#ifdef Q_WS_X11
            QSettings settings;
            if (settings.value("debug").toString() == "on")
                qDebug() << "CAM <- " << strDesc;
#endif
            QString strStatus = strDesc.right(strDesc.length()-10);
            ui.textEdit_desc->show();
            ui.textEdit_desc->setText(strStatus);
        }
    }
    // scc_test 2 0
    else if (iCam_cmd == 254)
    {
        QString strData(bData);
        QStringList strDataList = strData.split("\n");

        // clear
        ui.tableWidget_nick_rank_spectators->clear();
        ui.tableWidget_nick_rank_spectators->setRowCount(0);
        ui.tableWidget_nick_rank_spectators->verticalHeader()->hide();

        // set labels
        QStringList strlLabels;
        strlLabels << tr("Nick") << tr("Rank") << tr("Spectators");
        ui.tableWidget_nick_rank_spectators->setHorizontalHeaderLabels(strlLabels);

        // set rows
        int rows = strDataList.count()-1;
        ui.tableWidget_nick_rank_spectators->setRowCount(rows);

        int row = 0;
        foreach (QString strLine, strDataList)
        {
            if (strLine.isEmpty() == false)
            {
                QStringList strLineList = strLine.split(" ");
                if (strLineList.count() == 3) // correct ?
                {
                    QString strUser = strLineList[0];
                    QString strUsers = strLineList[1];
                    QString strRank = strLineList[2];

                    // add to table
                    ui.tableWidget_nick_rank_spectators->setItem(row, 0, new QTableWidgetItem(strUser));
                    ui.tableWidget_nick_rank_spectators->setItem(row, 1, new QTableWidgetItem(strRank));
                    ui.tableWidget_nick_rank_spectators->setItem(row, 2, new QTableWidgetItem(strUsers));
                }
                row++;
            }
        }
        ui.tableWidget_nick_rank_spectators->resizeColumnsToContents();
    }
    else if (iCam_cmd == 403)
    {
        QString strError = bData;
#ifdef Q_WS_X11
        QSettings settings;
        if (settings.value("debug").toString() == "on")
            qDebug() << "CAM <- " << strError;
#endif
        QString strImg = ui.label_img->text() +"<br>"+strError;
        ui.label_img->setText(strImg);
    }

    bData.clear();
    iCam_cmd = 0;
    bText = true;
    iBytes_need = 0;
    iBytes_recv = 0;
}

void DlgCam::text_kernel(QString strData)
{
    QStringList strDataList = strData.split(" ");

#ifdef Q_WS_X11
    QSettings settings;
    if (settings.value("debug").toString() == "on")
        qDebug() << "CAM <- " << strData;
#endif

    // check correct text
    if (strDataList.count() < 3)
        return;

    // 200 0 OK
    if (strDataList[0] == "200")
    {
        if (lLastCommand.count() == 0) return; // empty lLastCommand

        QString strLastCommand = lLastCommand.takeFirst();

        if (strLastCommand == "UNSUBSCRIBE_BIG")
        {
            network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
        }
        else if (strLastCommand == "PUT2")
        {
            QDateTime dt = QDateTime::currentDateTime();
            qint64 iCurrentTime = (qint64)dt.toTime_t(); // seconds that have passed since 1970

            bReadySendPUT = true;
            iLastSendPUT = iCurrentTime;
        }
    }
    // 202 17244 IMAGE_UPDATE_BIG Ekscentryk
    else if ((strDataList[0] == "202") && (strDataList.count() == 4))
    {
        iBytes_need = strDataList[1].toInt();
        if (iBytes_need != 0)
        {
            bText = false;

            QString strUser = strDataList[3];
            if (strUser == strNick)
                iCam_cmd = 202;
            else
                iCam_cmd = 0;

            // re-send
            if (strUser == strNick)
            {
                if ((strNick.isEmpty() == false) && (this->isHidden() == false))
                {
                    QDateTime dt = QDateTime::currentDateTime();
                    qint64 iCurrentTime = (qint64)dt.toTime_t(); // seconds that have passed since 1970
                    iLastKeepAlive = iCurrentTime;

                    network_send(QString("KEEPALIVE_BIG %1").arg(strNick));
                }
            }
        }
        else
        {
            ui.label_img->setText(tr("This user does not send data"));
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
    // 221 0 UDPUT_OK
    else if (strDataList[0] == "221")
    {
        // ignore
    }
    // 231 0 OK scc_test
    else if ((strDataList[0] == "231") && (strDataList.count() == 4))
    {
        ui.label_img->setText(tr("Setting mode for viewing only"));
        network_send("SENDMODE 0");
        ui.radioButton_broadcast_public->setChecked(true);
        bBroadcasting_pubpriv = false;

        if (strNick.isEmpty() == false)
        {
            ui.label_img->setText(tr("Downloading image"));
            network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
        }
        else
            ui.label_img->setText(tr("Select user"));
    }
    // 232 0 CMODE 0
    else if ((strDataList[0] == "232") && (strDataList.count() == 4))
    {
        // ignore
    }
    // 233 0 QUALITY_FACTOR 1
    else if ((strDataList[0] == "233") && (strDataList.count() == 4))
    {
        // ignore
    }
    // 250 12519 OK
    else if (strDataList[0] == "250")
    {
        // initial read users status
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
    else if ((strDataList[0] == "252") && (strDataList.count() == 4))
    {
        if (strDataList[1].toInt() > 0)
        {
            iBytes_need = strDataList[1].toInt();
            bText = false;

            QString strUser = strDataList[3];
            if (strUser == strNick)
                iCam_cmd = 252;
            else
                iCam_cmd = 0;
        }
    }
    // 253 0 USER_VOTES Delikatna 38
    else if ((strDataList[0] == "253") && (strDataList.count() == 5))
    {
        // ignore
    }
    // 254 1489 USER_COUNT_UPDATE
    else if (strDataList[0] == "254")
    {
        bText = false;
        iBytes_need = strDataList[1].toInt();
        iCam_cmd = 254;

        // check keepalive
        QDateTime dt = QDateTime::currentDateTime();
        qint64 iCurrentTime = (qint64)dt.toTime_t(); // seconds that have passed since 1970

        if (iCurrentTime - iLastKeepAlive > 30) // 30 sec
        {
            if ((strNick.isEmpty() == false) && (this->isHidden() == false))
                network_send(QString("KEEPALIVE_BIG %1").arg(strNick));
        }
    }
    // 264 0 CODE_ACCEPTED ffffffff 2147483647
    else if ((strDataList[0] == "264") && (strDataList.count() == 5))
    {
        // ignore
    }
    // 261 0 OK
    else if (strDataList[0] == "261")
    {
        // stopped
    }
    // 262 0 NEW_FAN ~matka_wariatka
    else if ((strDataList[0] == "262") && (strDataList.count() == 4))
    {
        QString strFunNick = strDataList[3];
        ui.listWidget_funs->addItem(strFunNick);
    }
    // 267 0 SENDMODE=0
    // 267 0 SENDMODE=1
    else if (strDataList[0] == "267")
    {
        // ignore
    }
    // 268 0 OK
    else if (strDataList[0] == "268")
    {
        QSettings settings;
        QString strUOKey = settings.value("uokey").toString();
        network_send(QString("AUTH %1 3.00.159").arg(strUOKey));
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
    else if ((strDataList[0] == "405") && (strDataList.count() == 4))
    {
        QString strUser = strDataList[3];
        if (strUser == strNick)
        {
            ui.label_img->setText(tr("The specified user has left the chat"));
            strNick.clear();
        }
    }
    // 408 0 NO_SUCH_USER_SUBSCRIBE LOLexx
    else if ((strDataList[0] == "408") && (strDataList.count() == 4))
    {
        QString strUser = strDataList[3];
        if (strUser == strNick)
        {
            ui.label_img->setText(tr("The specified user does not have a webcam enabled"));
            strNick.clear();
        }
    }
    // 410 0 FAN_GONE Merovingian
    else if ((strDataList[0] == "410") && (strDataList.count() == 4))
    {
        QString strFunNick = strDataList[3];

        int row = -1;
        for (int i = 0; i < ui.listWidget_funs->count(); i++)
        {
            if (ui.listWidget_funs->item(i)->text() == strFunNick)
                row = i;
        }

        if (row != -1)
            ui.listWidget_funs->takeItem(row);
    }
    // 412 0 SUBSCRIBE_FAILED olgusia32
    else if ((strDataList[0] == "412") && (strDataList.count() == 4))
    {
        QString strUser = strDataList[3];
        if (strUser == strNick)
        {
            ui.label_img->setText(tr("Failed to retrieve the image from the webcam"));
            strNick.clear();
        }
    }
    // 413 0 SUBSCRIBE_DENIED aliina
    else if ((strDataList[0] == "413") && (strDataList.count() == 4))
    {
        QString strUser = strDataList[3];
        if (strUser == strNick)
        {
            ui.label_img->setText(tr("Failed to retrieve the image from the webcam"));
            strNick.clear();
        }
    }
    // 418 0 QUIT_CZAT
    else if (strDataList[0] == "418")
    {
        network_disconnect();
    }
    // 502 0 SESSION_TIMED_OUT
    else if (strDataList[0] == "502")
    {
        network_disconnect();
    }
    // 504 0 UNKNOWN_COMMAND PUT2
    else if ((strDataList[0] == "504") && (strDataList.count() == 4))
    {
        // ignore
    }
    // 508 0 SESSION_OVERRIDEN
    else if (strDataList[0] == "508")
    {
        // ignore
    }
    // 520 0 INVALID_UOKEY 1q3j0llVg40cu2784j9EVoz8sRdfNl3w
    else if ((strDataList[0] == "520") && (strDataList.count() == 4))
    {
        ui.label_img->setText(tr("Invalid authorization key"));
        network_disconnect();
    }
    else
    {
        qDebug() << "Unknown CAM RAW:" << strData;
    }
}

void DlgCam::broadcast_start_stop()
{
    if (bBroadcasting == false)
    {
        bBroadcasting = true;
        if (bBroadcasting_pubpriv == false)
            ui.label_broadcast_status->setText("<span style=\"color:#ffff00;font-weight:bold;\">"+tr("Bradcasting public")+"</span>");
        else
            ui.label_broadcast_status->setText("<span style=\"color:#0000ff;font-weight:bold;\">"+tr("Bradcasting private")+"</span>");

        ui.pushButton_broadcast->setText(tr("Stop broadcast"));
    }
    else
    {
        bBroadcasting = false;
        network_send("STOP");
        ui.label_broadcast_status->setText("<span style=\"color:#ff0000;font-weight:bold;\">"+tr("No bradcasting")+"</span>");
        ui.pushButton_broadcast->setText(tr("Start broadcast"));
    }
}

void DlgCam::broadcast_public()
{
    if (bBroadcasting_pubpriv == true)
    {
        network_send("SENDMODE 0");
        bBroadcasting_pubpriv = false;

        ui.label_broadcast_status->setText("<span style=\"color:#ffff00;font-weight:bold;\">"+tr("Bradcasting public")+"</span>");
    }
}

void DlgCam::broadcast_private()
{
    if (bBroadcasting_pubpriv == false)
    {
        network_send("SENDMODE 1");
        bBroadcasting_pubpriv = true;

        ui.label_broadcast_status->setText("<span style=\"color:#0000ff;font-weight:bold;\">"+tr("Bradcasting private")+"</span>");
    }
}

void DlgCam::button_ok()
{
    this->hide();
}

void DlgCam::change_user(int row, int column)
{
    Q_UNUSED(column);

    // clear status
    ui.textEdit_desc->hide();
    ui.textEdit_desc->setText("");

    // clear img
    ui.label_img->clear();

    // clear desc
    ui.textEdit_desc->setText("");
    ui.textEdit_desc->hide();

    // read nick
    QString strNewNick = ui.tableWidget_nick_rank_spectators->item(row, 0)->text();

    // change user
    if (strNick.isEmpty() == true)
    {
        network_send(QString("SUBSCRIBE_BIG * %1").arg(strNewNick));
    }
    else
    {
        lLastCommand.append("UNSUBSCRIBE_BIG");
        network_send(QString("UNSUBSCRIBE_BIG %1").arg(strNick));
    }

    // set nick
    strNick = strNewNick;

    // display nick
    ui.label_nick->setText("<p style=\"font-weight:bold;\">"+strNick+"</p>");
}

void DlgCam::read_video()
{
#ifndef Q_WS_WIN
    // video device not exist any more
    if (exist_video_device() == false)
    {
        if (bCreatedCaptureCv == true)
        {
            cvReleaseCapture(&captureCv);
            bCreatedCaptureCv = false;
        }
        return;
    }

    // set image
    QPixmap pixmap = convert_cam2img(opencv_get_camera_image());
    ui.label_capture->setPixmap(pixmap.scaled(QSize(320,240)));

    // send image
    if (bBroadcasting == true)
    {
        // create data 1 (320x240)
        QPixmap pixmap1 = pixmap.scaled(QSize(320,240));
        QByteArray bData1;
        QBuffer buffer1(&bData1);
        buffer1.open(QIODevice::WriteOnly);
        pixmap1.save(&buffer1, "JPG");

        // create data 2 (160x120)
        QPixmap pixmap2 = pixmap.scaled(QSize(160,120));
        QByteArray bData2;
        QBuffer buffer2(&bData2);
        buffer2.open(QIODevice::WriteOnly);
        pixmap2.save(&buffer2, "JPG");

        QByteArray bHeader = (QString("PUT2 %1 %2\n").arg(bData1.size()).arg(bData2.size())).toAscii();

        QByteArray bPackage;
        bPackage.append(bHeader);
        bPackage.append(bData1);
        bPackage.append(bData2);

        QDateTime dt = QDateTime::currentDateTime();
        qint64 iCurrentTime = (qint64)dt.toTime_t(); // seconds that have passed since 1970

        if (bFirstSendPUT == false)
        {
            bFirstSendPUT = true;
            bReadySendPUT = false;
            lLastCommand.append("PUT2");
            network_sendb(bPackage);
        }

        if ((bReadySendPUT == true) && (iCurrentTime-iLastSendPUT > 5)) // send -> 5 sec
        {
            bReadySendPUT = false;
            lLastCommand.append("PUT2");
            network_sendb(bPackage);
        }
    }

    // read self video
    QTimer::singleShot(1000*1, this, SLOT(read_video())); // 1sec
#endif
}

#ifndef Q_WS_WIN
IplImage *DlgCam::opencv_get_camera_image()
{
    IplImage *img;

    if (bCreatedCaptureCv == true)
        img = cvQueryFrame(captureCv);

    return img;
}

QPixmap DlgCam::convert_cam2img(IplImage *img)
{
    if (bCreatedCaptureCv == false) return QPixmap(); // error?

    int height = img->height;
    int width = img->width;

    if (img->depth == IPL_DEPTH_8U && img->nChannels == 3)
    {
        const uchar *qImageBuffer = (const uchar*)img->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
        return QPixmap::fromImage(img.rgbSwapped());
    }
    else
    {
        qWarning() << "Camera image cannot be converted.";
        return QPixmap();
    }
}
#endif

void DlgCam::showEvent(QShowEvent *event)
{
    event->accept();
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

#ifndef Q_WS_WIN
    // connect or get img
    if (camSocket->state() == QAbstractSocket::ConnectedState)
    {
        if (strNick.isEmpty() == false)
        {
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->clear();
            network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
        }
        else
        {
            ui.label_nick->setText("<p style=\"font-weight:bold;\">"+strNick+"</p>");
            ui.label_img->setText(tr("Select user"));
        }
    }
    else
        network_connect();

    // detect and set broadcasting
    detect_broadcasting();
    set_broadcasting();
#endif
}

void DlgCam::hideEvent(QHideEvent *event)
{
    event->accept();

#ifndef Q_WS_WIN
    if (strNick.isEmpty() == false)
    {
        lLastCommand.append("HIDE_EVENT");
        network_send(QString("UNSUBSCRIBE_BIG %1").arg(strNick));
    }

    ui.textEdit_desc->setText("");
    ui.textEdit_desc->hide();
    ui.label_img->clear();
#endif
}

void DlgCam::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
