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

#ifndef MAINTEXTEDIT_H
#define MAINTEXTEDIT_H

#include "defines.h"
class DlgCam;
class DlgUserProfile;
class Network;
#include <QTcpSocket>
#include <QTextEdit>

class MainTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MainTextEdit(QWidget *, Network *, QString, QTcpSocket *, sChannelNickStatus *, DlgUserProfile *, DlgCam *);
    ~MainTextEdit();
    void update_background_image();
    void set_open_channels(QStringList);
    inline QString get_current_nick() { return strNick; }

private:
    // params
    QWidget *myparent;
    Network *pNetwork;
    QString strChannel;
    QTcpSocket *camSocket;
    sChannelNickStatus *mChannelNickStatus;
    DlgUserProfile *pDlg_user_profile;
    DlgCam *pDlg_cam;
    // other
    QString strNick;
    QStringList strOpenChannels;
    enum { maxOpenChannels = 50 };
    QAction *openChannelsActs[maxOpenChannels];

private slots:
    void join_channel();
    void priv();
    void whois();
    void profile();
    void cam();
    void friends_add();
    void friends_del();
    void ignore_add();
    void ignore_del();
    void kick();
    void ban();
    void kban();
    void ipban();
    void op_add();
    void op_del();
    void halfop_add();
    void halfop_del();
    void moderator_add();
    void moderator_del();
    void voice_add();
    void voice_del();
    void invite();

protected:
    virtual void mousePressEvent(QMouseEvent *);
};

#endif // MAINTEXTEDIT_H
