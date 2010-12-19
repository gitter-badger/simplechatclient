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

#ifndef DLG_CAM_H
#define DLG_CAM_H

#include <QBuffer>
#include <QDesktopWidget>
#include <QDialog>
#include <QFile>
#include <QHideEvent>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QShowEvent>
#include <QStringList>
#include <QTcpSocket>
#include <QTimer>
#include "network.h"
#include "ui_cam.h"
// opencv
#ifndef Q_WS_WIN
#include <opencv/highgui.h>
#include <opencv/cv.h>
#endif

class DlgCam : public QDialog
{
    Q_OBJECT
public:
    DlgCam(QWidget *, Network *, QTcpSocket *);
    ~DlgCam();
    void set_nick(QString);

private:
    Ui::uiCam ui;
    Network *pNetwork;
    QString strNick;
    QTcpSocket *camSocket;
    bool bText;
    int iCam_cmd;
    QByteArray bData;
    int iBytes_need;
    int iBytes_recv;
#ifndef Q_WS_WIN
    CvCapture *captureCv; // camera
    bool bCreatedCaptureCv;
#endif
    bool bBroadcasting;
    bool bBroadcasting_pubpriv; // false = public; true = private;
    QByteArray bPackage;
    bool bFirstSend;
    bool bReadySend;
    qint64 iLastSend;
    QList <QString> lLastCommand;
    qint64 iLastKeepAlive;

#ifndef Q_WS_WIN
    IplImage *opencv_get_camera_image(); // get camera image
    QPixmap convert_cam2img(IplImage *); // convert camera image to pixmap

    bool exist_video_device();
    void detect_broadcasting();
    void set_broadcasting();
#endif
    void show_img(QByteArray);
    void network_connect();
    void network_send(QString);
    void network_sendb(QByteArray);
    void network_disconnect();
    void data_kernel();
    void text_kernel(QString);

private slots:
    void broadcast_start_stop();
    void broadcast_public();
    void broadcast_private();
    void button_ok();
    void change_user(int,int);
    void read_video();
    void network_read();
    void network_connected();
    void network_disconnected();
    void network_error(QAbstractSocket::SocketError);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

};

#endif // DLG_CAM_H
