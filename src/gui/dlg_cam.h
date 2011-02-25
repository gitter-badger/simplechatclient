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

#ifndef DLG_CAM_H
#define DLG_CAM_H

class DlgCamNetwork;
class Network;
class SimpleRankWidget;
class Video;
#include <QDialog>
#include <QTcpSocket>
#include "ui_cam.h"

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
    SimpleRankWidget *simpleRankWidget;
    int iCam_cmd;

    DlgCamNetwork *camNetwork;
    QImage captured_frame;
    QTimer *video_frame_timer;

    bool bBroadcasting;
    bool bBroadcasting_pubpriv; // false = public; true = private;

    bool bFirstSendPUT;
    bool bReadySendPUT;
    qint64 iLastSendPUT;
    QList <QString> lLastCommand;
    QMap <QString, QString> mNickChannels;

    QString get_cauth();
    Video *video;

    bool exist_video_device();
    void detect_broadcasting();
    void set_broadcasting();

    // options
    void send_all_my_options();
    // items
    void add_item(int, QString, QString, QString);
    void add_item(QString, QString, QString);
    bool exist_item(QString);
    void remove_item(QString);
    int get_item(QString);
    void update_item(QString, QString, QString);

private slots:
    // video frame timer
    void get_frame();
    // kernel
    void data_kernel(QByteArray);
    void text_kernel(QString);
    // from network
    void nconnected();
    void ndisconnected();
    void set_label(QString); // set label img text
    // broadcast
    void broadcast_start_stop();
    void broadcast_public();
    void broadcast_private();
    // my images
    void set_status();
    void set_about_me();
    void set_homepage();
    void add_img0();
    void remove_img0();
    void add_img1();
    void remove_img1();
    void add_img2();
    void remove_img2();
    void add_img3();
    void remove_img3();
    // standard buttons
    void vote_minus();
    void vote_plus();
    void button_ok();
    // vote
    void enable_vote();
    // change user
    void change_user(int,int);
    // read video image
    void read_video();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);
};

class DlgCamNetwork : public QObject
{
    Q_OBJECT
public:
    DlgCamNetwork(Network *, QTcpSocket *);
    inline void set_bytes_need(int i) { iBytes_need = i; }
    inline void set_btext(bool b) { bText = b; }
    inline void set_last_keep_alive(qint64 i) { iLastKeepAlive = i; }
    inline qint64 get_last_keep_alive() { return iLastKeepAlive; }

    void clear_all();
    bool is_connected();
    void network_connect();
    void network_send(QString);
    void network_sendb(QByteArray);
    void network_disconnect();

private:
    Network *pNetwork;
    QTcpSocket *socket;
    QString strData;
    QByteArray bData;
    int iBytes_need;
    int iBytes_recv;
    QTimer *timerPingPong;
    int iLastActive;
    qint64 iLastKeepAlive;
    bool bText;

private slots:
    void network_read();
    void network_connected();
    void network_disconnected();
    void network_error(QAbstractSocket::SocketError);
    void slot_network_connect();
    void timeout_pingpong();

signals:
    void data_kernel(QByteArray);
    void text_kernel(QString);
    void set_label(QString);
    void nconnected();
    void ndisconnected();
};

#endif // DLG_CAM_H
