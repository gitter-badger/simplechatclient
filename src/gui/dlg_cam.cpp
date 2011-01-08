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

    // params
    pNetwork = param1;
    camSocket = param2;

    // init rank
    simpleRankWidget = new SimpleRankWidget(this);
    ui.horizontalLayout->insertWidget(1, simpleRankWidget);

    // default text
    ui.label_nick->setText(strNick);
    ui.tabWidget->setTabText(0, tr("Viewing"));
    ui.tabWidget->setTabText(1, tr("Broadcasting"));
    ui.tabWidget->setTabText(2, tr("About me"));
    ui.tabWidget->setTabText(3, tr("Settings"));
    ui.label_fun->setText(tr("Funs"));
    ui.label_broadcast_status->setText(QString("<span style=\"color:#ff0000;\">%1</span>").arg(tr("No broadcasting")));
    ui.pushButton_broadcast->setText(tr("Start broadcast"));
    ui.radioButton_broadcast_public->setText(tr("Public"));
    ui.radioButton_broadcast_private->setText(tr("Private"));
    ui.label_status->setText(tr("My status:"));
    ui.pushButton_set_status->setText(tr("Apply"));
    ui.groupBox_about_me->setTitle(tr("About me"));
    ui.pushButton_set_about_me->setText(tr("Apply"));
    ui.groupBox_my_homepage->setTitle(tr("My homepage"));
    ui.groupBox_photos->setTitle(tr("My photos"));
    ui.pushButton_set_homepage->setText(tr("Apply"));
    ui.pushButton_add_img0->setText(tr("Add"));
    ui.pushButton_remove_img0->setText(tr("Remove"));
    ui.pushButton_add_img1->setText(tr("Add"));
    ui.pushButton_remove_img1->setText(tr("Remove"));
    ui.pushButton_add_img2->setText(tr("Add"));
    ui.pushButton_remove_img2->setText(tr("Remove"));
    ui.pushButton_add_img3->setText(tr("Add"));
    ui.pushButton_remove_img3->setText(tr("Remove"));
    ui.groupBox_settings->setTitle(tr("Settings"));
    ui.label_device->setText(tr("Device:"));

    // icons
    ui.pushButton_set_about_me->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_homepage->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_add_img0->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.pushButton_remove_img0->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_add_img1->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.pushButton_remove_img1->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_add_img2->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.pushButton_remove_img2->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.pushButton_add_img3->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.pushButton_remove_img3->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));

    ui.pushButton_set_status->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.toolButton_vote_minus->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.toolButton_vote_plus->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.tableWidget_nick_rank_spectators->resizeColumnsToContents();

    // clear
    ui.label_img->setText(tr("Starting the service webcams"));
    ui.textEdit_desc->setText("");
    ui.textEdit_desc->hide();
    ui.tableWidget_nick_rank_spectators->clear();
    ui.listWidget_funs->clear();
    mNickChannels.clear();
    bBroadcasting = false;
    bBroadcasting_pubpriv = false;
    bFirstSendPUT = false;
    bReadySendPUT = true;
    iLastSendPUT = 0;
    iLastKeepAlive = 0;
    iLastActive = 0;
#ifndef Q_WS_WIN
    bCreatedCaptureCv = false;
#endif

    QSettings settings;
    timerPingPong = new QTimer();
    timerPingPong->setInterval(1*60*1000); // 1 min

    // set labels
    QStringList strlLabels;
    strlLabels << tr("Nick") << tr("Rank") << tr("Spectators");
    ui.tableWidget_nick_rank_spectators->setHorizontalHeaderLabels(strlLabels);

    // set about me
    if (settings.value("cam_about_me").toString().isEmpty() == false)
        ui.textEdit_about_me->setPlainText(settings.value("cam_about_me").toString());

    // set homepage
    if (settings.value("cam_homepage").toString().isEmpty() == false)
        ui.lineEdit_homepage->setText(settings.value("cam_homepage").toString());

    // set img0
    QString strImg0 = settings.value("cam_img0").toString();
    if ((strImg0.isEmpty() == false) && (QFile::exists(strImg0) == true))
    {
        QPixmap pixmap;
        pixmap.load(strImg0);
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img0->setPixmap(pixmap);
    }

    // set img1
    QString strImg1 = settings.value("cam_img1").toString();
    if ((strImg1.isEmpty() == false) && (QFile::exists(strImg1) == true))
    {
        QPixmap pixmap;
        pixmap.load(strImg1);
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img1->setPixmap(pixmap);
    }

    // set img2
    QString strImg2 = settings.value("cam_img2").toString();
    if ((strImg2.isEmpty() == false) && (QFile::exists(strImg2) == true))
    {
        QPixmap pixmap;
        pixmap.load(strImg2);
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img2->setPixmap(pixmap);
    }

    // set img3
    QString strImg3 = settings.value("cam_img3").toString();
    if ((strImg3.isEmpty() == false) && (QFile::exists(strImg3) == true))
    {
        QPixmap pixmap;
        pixmap.load(strImg3);
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img3->setPixmap(pixmap);
    }

    //camSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    //camSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    QObject::connect(ui.tableWidget_nick_rank_spectators, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(change_user(int,int)));
    QObject::connect(ui.toolButton_vote_minus, SIGNAL(clicked()), this, SLOT(vote_minus()));
    QObject::connect(ui.toolButton_vote_plus, SIGNAL(clicked()), this, SLOT(vote_plus()));
    QObject::connect(ui.pushButton_broadcast, SIGNAL(clicked()), this, SLOT(broadcast_start_stop()));
    QObject::connect(ui.radioButton_broadcast_public, SIGNAL(clicked()), this, SLOT(broadcast_public()));
    QObject::connect(ui.radioButton_broadcast_private, SIGNAL(clicked()), this, SLOT(broadcast_private()));
    QObject::connect(ui.pushButton_set_status, SIGNAL(clicked()), this, SLOT(set_status()));
    QObject::connect(ui.pushButton_add_img0, SIGNAL(clicked()), this, SLOT(add_img0()));
    QObject::connect(ui.pushButton_remove_img0, SIGNAL(clicked()), this, SLOT(remove_img0()));
    QObject::connect(ui.pushButton_add_img1, SIGNAL(clicked()), this, SLOT(add_img1()));
    QObject::connect(ui.pushButton_remove_img1, SIGNAL(clicked()), this, SLOT(remove_img1()));
    QObject::connect(ui.pushButton_add_img2, SIGNAL(clicked()), this, SLOT(add_img2()));
    QObject::connect(ui.pushButton_remove_img2, SIGNAL(clicked()), this, SLOT(remove_img2()));
    QObject::connect(ui.pushButton_add_img3, SIGNAL(clicked()), this, SLOT(add_img3()));
    QObject::connect(ui.pushButton_remove_img3, SIGNAL(clicked()), this, SLOT(remove_img3()));
    QObject::connect(ui.pushButton_set_about_me, SIGNAL(clicked()), this, SLOT(set_about_me()));
    QObject::connect(ui.pushButton_set_homepage, SIGNAL(clicked()), this, SLOT(set_homepage()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
#ifndef Q_WS_WIN
    QObject::connect(camSocket, SIGNAL(connected()), this, SLOT(network_connected()));
    QObject::connect(camSocket, SIGNAL(disconnected()), this, SLOT(network_disconnected()));
    QObject::connect(camSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(network_error(QAbstractSocket::SocketError)));
    QObject::connect(camSocket, SIGNAL(readyRead()), this, SLOT(network_read()));
#endif
    QObject::connect(timerPingPong, SIGNAL(timeout()), this, SLOT(timeout_pingpong()));
}

DlgCam::~DlgCam()
{
    delete simpleRankWidget;

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

void DlgCam::set_nick(QString n)
{
    strNick = n;
    ui.label_nick->setText(strNick);
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
    ui.tabWidget->setTabEnabled(2, false);
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
                ui.tabWidget->setTabEnabled(1, true);
                ui.tabWidget->setTabEnabled(2, true);
                ui.comboBox_device->setEnabled(true);
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
            QTimer::singleShot(1000*1, this, SLOT(read_video())); // 1 sec
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

        // set last active
        QDateTime dt = QDateTime::currentDateTime();
        iLastActive = (int)dt.toTime_t();

        // timer
        timerPingPong->start();
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
            strNick.clear();
        }
        camSocket->disconnectFromHost();
    }
}

void DlgCam::network_read()
{
    // set last active
    QDateTime dt = QDateTime::currentDateTime();
    iLastActive = (int)dt.toTime_t();

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
    ui.tableWidget_nick_rank_spectators->clear();
    ui.label_nick->setText(strNick);
    ui.textEdit_desc->setText("");
    ui.textEdit_desc->hide();
    ui.label_img->setText(ui.label_img->text()+"<br>"+tr("Disconnected from server webcams"));
    simpleRankWidget->set_rank(0);
    ui.textEdit_channels->clear();

    ui.listWidget_funs->clear();
    bBroadcasting = false;
    //network_send("STOP");
    ui.label_broadcast_status->setText(QString("<span style=\"color:#ff0000;\">%1</span>").arg(tr("No broadcasting")));
    ui.pushButton_broadcast->setText(tr("Start broadcast"));

    bFirstSendPUT = false;
    bReadySendPUT = true;
    lLastCommand.clear();
    mNickChannels.clear();

    // set labels
    QStringList strlLabels;
    strlLabels << tr("Nick") << tr("Rank") << tr("Spectators");
    ui.tableWidget_nick_rank_spectators->setHorizontalHeaderLabels(strlLabels);

    // timer
    if (timerPingPong->isActive() == true)
        timerPingPong->stop();

    // reconnect
    QTimer::singleShot(1000*10, this, SLOT(slot_network_connect())); // 10 sec
}

void DlgCam::network_error(QAbstractSocket::SocketError err)
{
    Q_UNUSED (err);

    ui.label_img->setText(QString(tr("Disconnected from server [%1]")).arg(camSocket->errorString()));

    if (camSocket->state() == QAbstractSocket::ConnectedState)
        network_disconnect();
    else if (camSocket->state() == QAbstractSocket::UnconnectedState)
    {
        // timer
        if (timerPingPong->isActive() == true)
            timerPingPong->stop();

        // reconnect
        QTimer::singleShot(1000*10, this, SLOT(slot_network_connect())); // 10 sec
    }
}

void DlgCam::slot_network_connect()
{
    if (pNetwork->is_connected() == true)
        network_connect();
    else
    {
        QTimer::singleShot(1000*60, this, SLOT(slot_network_connect())); // 60 sec
    }
}

void DlgCam::data_kernel()
{
    // 202 14283 IMAGE_UPDATE_BIG psotnica2603
    if (iCam_cmd == 202)
    {
        show_img(bData);
    }
    // osa1987:1:-2/-2/osa1987/1:0::0
    // ToWiemTylkoJa:1:2/0/#Relax/0,-2/-2/ToWiemTylkoJa/1:2::13
    // scc_test:1:2/0/#Quiz/0,2/0/#Relax/0,2/0/#Scrabble/0,4/0/#scc/0:0::0
    else if (iCam_cmd == 250)
    {
        // init data
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
                QString strCamOnOff = strLineList[1]; // 1 = on; 2 = off
                QString strChannelsParams = strLineList[2];
                QString strSpectators = strLineList[3];
                QString strUdget = strLineList[4]; // udget (012345)
                QString strRank = strLineList[5]; // -500 to 500

                if (strChannelsParams.isEmpty() == false)
                {
                    QString strAllChannels;

                    QStringList strlChannelsParams = strChannelsParams.split(",");
                    foreach (QString strChannelParams, strlChannelsParams)
                    {
                        QStringList strlChannelParams = strChannelParams.split("/");
                        if (strlChannelParams.count() == 4)
                        {
                            QString strChannelCategory = strlChannelParams[0];
                            QString strUnknown = strlChannelParams[1]; // always 0 (?)
                            QString strChannelName = strlChannelParams[2];
                            QString strCamPubPriv = strlChannelParams[3]; // 0 = public; 1 = private

                            if ((strChannelName[0] == '#') || (strChannelName[0] == '^'))
                                strAllChannels += strChannelName+" ";
                        }
                    }

                    // insert into map
                    mNickChannels[strUser] = strAllChannels;
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
    // osa1987:1:-2/-2/osa1987/1:0::0
    // ToWiemTylkoJa:1:2/0/#Relax/0,-2/-2/ToWiemTylkoJa/1:2::13
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
                int iCamOnOff = strLineList[1].toInt(); // 1 = on; 2 = off
                QString strChannelsParams = strLineList[2];
                QString strSpectators = strLineList[3];
                QString strUdget = strLineList[4]; // udget (012345)
                int iRank = strLineList[5].toInt(); // -500 to 500

                if (strChannelsParams.isEmpty() == false)
                {
                    QString strAllChannels;

                    QStringList strlChannelsParams = strChannelsParams.split(",");
                    foreach (QString strChannelParams, strlChannelsParams)
                    {
                        QStringList strlChannelParams = strChannelParams.split("/");
                        if (strlChannelParams.count() == 4)
                        {
                            QString strChannelCategory = strlChannelParams[0];
                            QString strUnknown = strlChannelParams[1]; // always 0 (?)
                            QString strChannelName = strlChannelParams[2];
                            QString strCamPubPriv = strlChannelParams[3]; // 0 = public; 1 = private

                            if ((strChannelName[0] == '#') || (strChannelName[0] == '^'))
                                strAllChannels += strChannelName+" ";
                        }
                    }

                    // update map
                    mNickChannels[strUser] = strAllChannels;
                }

                // if current nick
                if (strUser == strNick)
                {
                    // update rank
                    simpleRankWidget->set_rank(iRank);
                    // update channels
                    ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:")).arg(mNickChannels[strUser]));
                }

                // remove user if cam off
                if (iCamOnOff == 0)
                    mNickChannels.remove(strUser);
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
    // multi-line
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
                if (strLineList.count() == 3) // is correct ?
                {
                    QString strUser = strLineList[0];
                    QString strUsers = strLineList[1];
                    QString strRank = strLineList[2];

                    // add to table
                    ui.tableWidget_nick_rank_spectators->setItem(row, 0, new QTableWidgetItem(strUser));
                    ui.tableWidget_nick_rank_spectators->setItem(row, 1, new QTableWidgetItem(strRank));
                    ui.tableWidget_nick_rank_spectators->setItem(row, 2, new QTableWidgetItem(strUsers));

                    // if current nick
                    if (strUser == strNick)
                        simpleRankWidget->set_rank(strRank.toInt());
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
            ui.label_img->setText(tr("Downloading image"));
            network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));

            // update channels
            ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:")).arg(mNickChannels[strNick]));
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
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->setText(tr("This user does not send data"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->clear();
        }
    }
    // 211 19995 Noemi_01@0
    // 211 13584 Noemi_01@1
    // 211 13584 Noemi_01@2
    // 211 13584 Noemi_01@3
    // 211 29 my_dwoje@4
    // 211 17 myszka29brunetka@5
    else if ((strDataList[0] == "211") && (strDataList.count() == 3))
    {
        if (strDataList[1].toInt() > 0)
        {
            bText = false;
            iBytes_need = strDataList[1].toInt();
            iCam_cmd = 211;
        }
    }
    // 221 0 UDPUT_OK
    else if (strDataList[0] == "221")
    {
        // ignore
    }
    // 231 0 OK scc_test
    else if ((strDataList[0] == "231") && (strDataList.count() == 4))
    {
        // ignore
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
        QString strUser = strDataList[3];
        int iRank = strDataList[4].toInt();

        if (strUser == strNick) // is current nick
            simpleRankWidget->set_rank(iRank);
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
    // successfully logged in
    // 264 0 CODE_ACCEPTED ffffffff 2147483647
    else if ((strDataList[0] == "264") && (strDataList.count() == 5))
    {
        ui.label_img->setText(tr("Setting mode for viewing only"));
        network_send("SENDMODE 0");
        ui.radioButton_broadcast_public->setChecked(true);
        bBroadcasting_pubpriv = false;

        if (strNick.isEmpty() == false)
        {
            ui.label_img->setText(tr("Downloading image"));
            network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));

            // update channels
            ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:")).arg(mNickChannels[strNick]));
        }
        else
        {
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->setText(tr("Select user"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->clear();
        }

        // send all options
        send_all_my_options();
    }
    // 261 0 OK
    else if (strDataList[0] == "261")
    {
        // stopped
    }
    // 262 0 NEW_FAN ~matka_wariatka
    else if ((strDataList[0] == "262") && (strDataList.count() == 4))
    {
        // new fun
        QString strFunNick = strDataList[3];
        ui.listWidget_funs->addItem(strFunNick);
    }
    // 263 0 VOTE_OK alicja17 3
    else if ((strDataList[0] == "263") && (strDataList.count() == 5))
    {
        // vote ok
        ui.toolButton_vote_minus->setEnabled(false);
        ui.toolButton_vote_plus->setEnabled(false);

        QTimer::singleShot(1000*5, this, SLOT(enable_vote())); // 5 sec
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
        // CAUTH ok
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
            strNick.clear();
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->setText(tr("The specified user has left the chat"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->clear();
        }
    }
    // 406 0 NO_SUCH_USER_UDGET 0@4
    else if ((strDataList[0] == "406") && (strDataList.count() == 4))
    {
        // ignore
    }
    // 408 0 NO_SUCH_USER_SUBSCRIBE LOLexx
    else if ((strDataList[0] == "408") && (strDataList.count() == 4))
    {
        QString strUser = strDataList[3];
        if (strUser == strNick)
        {
            strNick.clear();
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->setText(tr("The specified user does not have a webcam enabled"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->clear();
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
    // 411 0 VOTE_DENIED soneja
    else if ((strDataList[0] == "412") && (strDataList.count() == 4))
    {
        // ignore
    }
    // 412 0 SUBSCRIBE_FAILED olgusia32
    else if ((strDataList[0] == "412") && (strDataList.count() == 4))
    {
        QString strUser = strDataList[3];
        if (strUser == strNick)
        {
            strNick.clear();
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->setText(tr("Failed to retrieve the image from the webcam"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->clear();
        }
    }
    // 413 0 SUBSCRIBE_DENIED aliina
    else if ((strDataList[0] == "413") && (strDataList.count() == 4))
    {
        QString strUser = strDataList[3];
        if (strUser == strNick)
        {
            strNick.clear();
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->setText(tr("Failed to retrieve the image from the webcam"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->clear();
        }
    }
    // 418 0 QUIT_CZAT
    else if (strDataList[0] == "418")
    {
        strNick.clear();
        network_disconnect();
    }
    // udget with -1
    // 501 0 INVALID_USER_DATA_NUM
    else if (strDataList[0] == "501")
    {
        // ignore
    }
    // 502 0 SESSION_TIMED_OUT
    else if (strDataList[0] == "502")
    {
        strNick.clear();
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
    // 515 0 USER_DATA_TOO_LARGE
    else if (strDataList[0] == "515")
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

void DlgCam::send_all_my_options()
{
    QSettings settings;

    // about me
    QString strAboutMe = settings.value("cam_about_me").toString();
    if (strAboutMe.isEmpty() == false)
    {
        network_send(QString("UDPUT 4 %1").arg(strAboutMe.length()));
        network_sendb(strAboutMe.toAscii());
    }

    // homepage
    QString strHomePage = settings.value("cam_homepage").toString();
    if (strHomePage.isEmpty() == false)
    {
        network_send(QString("UDPUT 5 %1").arg(strHomePage.length()));
        network_sendb(strHomePage.toAscii());
    }

    // img0
    QString strImg0 = settings.value("cam_img0").toString();
    if ((strImg0.isEmpty() == false) && (QFile::exists(strImg0) == true))
    {
        // read and scale
        QPixmap pixmap;
        pixmap.load(strImg0);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // send
        network_send(QString("UDPUT 0 %1").arg(bData.length()));
        network_sendb(bData);
    }

    // img1
    QString strImg1 = settings.value("cam_img1").toString();
    if ((strImg1.isEmpty() == false) && (QFile::exists(strImg1) == true))
    {
        // read and scale
        QPixmap pixmap;
        pixmap.load(strImg1);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // send
        network_send(QString("UDPUT 1 %1").arg(bData.length()));
        network_sendb(bData);
    }

    // img2
    QString strImg2 = settings.value("cam_img2").toString();
    if ((strImg2.isEmpty() == false) && (QFile::exists(strImg2) == true))
    {
        // read and scale
        QPixmap pixmap;
        pixmap.load(strImg2);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // send
        network_send(QString("UDPUT 2 %1").arg(bData.length()));
        network_sendb(bData);
    }

    // img3
    QString strImg3 = settings.value("cam_img3").toString();
    if ((strImg3.isEmpty() == false) && (QFile::exists(strImg3) == true))
    {
        // read and scale
        QPixmap pixmap;
        pixmap.load(strImg3);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // send
        network_send(QString("UDPUT 3 %1").arg(bData.length()));
        network_sendb(bData);
    }
}

void DlgCam::broadcast_start_stop()
{
    if (bBroadcasting == false)
    {
        bBroadcasting = true;
        if (bBroadcasting_pubpriv == false)
            ui.label_broadcast_status->setText(QString("<span style=\"color:#ffff00;\">%1</span>").arg(tr("Bradcasting public")));
        else
            ui.label_broadcast_status->setText(QString("<span style=\"color:#0000ff;\">%1</span>").arg(tr("Bradcasting private")));

        ui.pushButton_broadcast->setText(tr("Stop broadcast"));
    }
    else
    {
        bFirstSendPUT = false;
        bReadySendPUT = true;
        bBroadcasting = false;
        network_send("STOP");
        ui.label_broadcast_status->setText(QString("<span style=\"color:#ff0000;\">%1</span>").arg(tr("No broadcasting")));
        ui.pushButton_broadcast->setText(tr("Start broadcast"));
        ui.listWidget_funs->clear();
    }
}

void DlgCam::broadcast_public()
{
    if (bBroadcasting_pubpriv == true)
    {
        network_send("SENDMODE 0");
        bBroadcasting_pubpriv = false;

        if (bBroadcasting == true)
            ui.label_broadcast_status->setText(QString("<span style=\"color:#ffff00;\">%1</span>").arg(tr("Bradcasting public")));
    }
}

void DlgCam::broadcast_private()
{
    if (bBroadcasting_pubpriv == false)
    {
        network_send("SENDMODE 1");
        bBroadcasting_pubpriv = true;

        if (bBroadcasting == true)
            ui.label_broadcast_status->setText(QString("<span style=\"color:#0000ff;\">%1</span>").arg(tr("Bradcasting private")));
    }
}

void DlgCam::set_status()
{
    QString strStatus = ui.lineEdit_status->text();

    if (strStatus.isEmpty() == false)
    {
        lLastCommand.append("SETSTATUS");
        network_send(QString("SETSTATUS %1").arg(strStatus));
    }
}

void DlgCam::set_about_me()
{
    QString strAboutMe = ui.textEdit_about_me->toPlainText();

    if (strAboutMe.isEmpty() == false)
    {
        // save
        QSettings settings;
        Config *pConfig = new Config();
        pConfig->set_value("cam_about_me", strAboutMe);
        settings.setValue("cam_about_me", strAboutMe);
        delete pConfig;

        // send
        network_send(QString("UDPUT 4 %1").arg(strAboutMe.length()));
        network_sendb(strAboutMe.toAscii());
    }
}

void DlgCam::set_homepage()
{
    QString strHomePage = ui.lineEdit_homepage->text();

    if (strHomePage.isEmpty() == false)
    {
        // save
        QSettings settings;
        Config *pConfig = new Config();
        pConfig->set_value("cam_homepage", strHomePage);
        settings.setValue("cam_homepage", strHomePage);
        delete pConfig;

        // send
        network_send(QString("UDPUT 5 %1").arg(strHomePage.length()));
        network_sendb(strHomePage.toAscii());
    }
}

void DlgCam::add_img0()
{
    QString selectedFilter;
    QString strFileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     "",
                                     tr("JPEG Files (*.jpg);;Bitmap Files (*.bmp)"),
                                     &selectedFilter,
                                     0);

    if (strFileName.isEmpty() == false)
    {
        // save
        QSettings settings;
        Config *pConfig = new Config();
        pConfig->set_value("cam_img0", strFileName);
        settings.setValue("cam_img0", strFileName);
        delete pConfig;

        // read and scale
        QPixmap pixmap;
        pixmap.load(strFileName);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // display
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img0->setPixmap(pixmap);

        // send
        network_send(QString("UDPUT 0 %1").arg(bData.length()));
        network_sendb(bData);
    }
}

void DlgCam::remove_img0()
{
    // save
    QSettings settings;
    Config *pConfig = new Config();
    pConfig->set_value("cam_img0", "");
    settings.setValue("cam_img0", "");
    delete pConfig;

    // display
    ui.label_img0->clear();

    // send
    network_send("UDPUT 0 0");
}

void DlgCam::add_img1()
{
    QString selectedFilter;
    QString strFileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     "",
                                     tr("JPEG Files (*.jpg);;Bitmap Files (*.bmp)"),
                                     &selectedFilter,
                                     0);

    if (strFileName.isEmpty() == false)
    {
        // save
        QSettings settings;
        Config *pConfig = new Config();
        pConfig->set_value("cam_img1", strFileName);
        settings.setValue("cam_img1", strFileName);
        delete pConfig;

        // read and scale
        QPixmap pixmap;
        pixmap.load(strFileName);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // display
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img1->setPixmap(pixmap);

        // send
        network_send(QString("UDPUT 1 %1").arg(bData.length()));
        network_sendb(bData);
    }
}

void DlgCam::remove_img1()
{
    // save
    QSettings settings;
    Config *pConfig = new Config();
    pConfig->set_value("cam_img1", "");
    settings.setValue("cam_img1", "");
    delete pConfig;

    // display
    ui.label_img1->clear();

    // send
    network_send("UDPUT 1 0");
}

void DlgCam::add_img2()
{
    QString selectedFilter;
    QString strFileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     "",
                                     tr("JPEG Files (*.jpg);;Bitmap Files (*.bmp)"),
                                     &selectedFilter,
                                     0);

    if (strFileName.isEmpty() == false)
    {
        // save
        QSettings settings;
        Config *pConfig = new Config();
        pConfig->set_value("cam_img2", strFileName);
        settings.setValue("cam_img2", strFileName);
        delete pConfig;

        // read and scale
        QPixmap pixmap;
        pixmap.load(strFileName);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // display
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img2->setPixmap(pixmap);

        // send
        network_send(QString("UDPUT 2 %1").arg(bData.length()));
        network_sendb(bData);
    }
}

void DlgCam::remove_img2()
{
    // save
    QSettings settings;
    Config *pConfig = new Config();
    pConfig->set_value("cam_img2", "");
    settings.setValue("cam_img2", "");
    delete pConfig;

    // display
    ui.label_img2->clear();

    // send
    network_send("UDPUT 2 0");
}

void DlgCam::add_img3()
{
    QString selectedFilter;
    QString strFileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     "",
                                     tr("JPEG Files (*.jpg);;Bitmap Files (*.bmp)"),
                                     &selectedFilter,
                                     0);

    if (strFileName.isEmpty() == false)
    {
        // save
        QSettings settings;
        Config *pConfig = new Config();
        pConfig->set_value("cam_img3", strFileName);
        settings.setValue("cam_img3", strFileName);
        delete pConfig;

        // read and scale
        QPixmap pixmap;
        pixmap.load(strFileName);
        if ((pixmap.width() > 320) || (pixmap.height() > 240))
            pixmap = pixmap.scaled(320,240);
        QByteArray bData;
        QBuffer buffer(&bData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "JPG");

        // display
        if ((pixmap.width() > 160) || (pixmap.height() > 120))
            pixmap = pixmap.scaled(160,120);
        ui.label_img3->setPixmap(pixmap);

        // send
        network_send(QString("UDPUT 3 %1").arg(bData.length()));
        network_sendb(bData);
    }
}

void DlgCam::remove_img3()
{
    // save
    QSettings settings;
    Config *pConfig = new Config();
    pConfig->set_value("cam_img3", "");
    settings.setValue("cam_img3", "");
    delete pConfig;

    // display
    ui.label_img3->clear();

    // send
    network_send("UDPUT 3 0");
}

void DlgCam::vote_minus()
{
    if (strNick.isEmpty() == false)
        network_send(QString("VOTE %1 -").arg(strNick));
}

void DlgCam::vote_plus()
{
    if (strNick.isEmpty() == false)
        network_send(QString("VOTE %1 +").arg(strNick));
}

void DlgCam::button_ok()
{
    this->hide();
}

void DlgCam::enable_vote()
{
    ui.toolButton_vote_minus->setEnabled(true);
    ui.toolButton_vote_plus->setEnabled(true);
}

void DlgCam::change_user(int row, int column)
{
    Q_UNUSED(column);

    // img
    ui.label_img->setText(tr("Downloading image"));

    // clear desc
    ui.textEdit_desc->setText("");
    ui.textEdit_desc->hide();

    // clear rank
    simpleRankWidget->set_rank(0);

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
    ui.label_nick->setText(strNick);

    // update channels
    ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:")).arg(mNickChannels[strNick]));
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
    QTimer::singleShot(1000*1, this, SLOT(read_video())); // 1 sec
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

void DlgCam::timeout_pingpong()
{
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent = (int)dt.toTime_t();

    if (iLastActive+301 < iCurrent)
    {
        network_disconnect();
        iLastActive = iCurrent;
    }
}

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
            ui.label_img->setText(tr("Downloading image"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:")).arg(mNickChannels[strNick]));

            network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
        }
        else
        {
            ui.label_nick->setText(strNick);
            ui.textEdit_desc->setText("");
            ui.textEdit_desc->hide();
            ui.label_img->setText(tr("Select user"));
            simpleRankWidget->set_rank(0);
            ui.textEdit_channels->clear();
        }
    }
    else
        network_connect();

    // detect and set broadcasting
    detect_broadcasting();
    set_broadcasting();

    // switch to first tab
    ui.tabWidget->setCurrentIndex(0);
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

    // clear
    ui.textEdit_desc->setText("");
    ui.textEdit_desc->hide();
    ui.label_img->clear();
    simpleRankWidget->set_rank(0);
    ui.textEdit_channels->clear();
#endif
}

void DlgCam::closeEvent(QCloseEvent *event)
{
    event->ignore();
    // hide
    this->hide();
}
