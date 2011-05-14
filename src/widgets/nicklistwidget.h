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

#ifndef NICKLISTTREEWIDGET_H
#define NICKLISTTREEWIDGET_H

#include "defines.h"
#ifndef Q_WS_WIN
    class DlgCam;
#endif
class DlgUserProfile;
class Network;
#include <QListWidget>
#include <QTcpSocket>

class NickListWidget : public QListWidget
{
    Q_OBJECT
public:
    NickListWidget(Network *, QTcpSocket *, DlgUserProfile *);
    virtual ~NickListWidget();
#ifndef Q_WS_WIN
    void set_dlg_cam(DlgCam *);
#endif
    void set_channel(QString);
    QString get_channel() { return strChannel; }
    void add(QString);
    void remove(QString);

private:
    // params
    Network *pNetwork;
    QTcpSocket *camSocket;
    DlgUserProfile *pDlgUserProfile;
#ifndef Q_WS_WIN
    DlgCam *pDlgCam;
#endif
    // other
    QString strChannel;
    enum { maxOpenChannels = 30 };
    QAction *openChannelsActs[maxOpenChannels];

private slots:
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
    virtual void contextMenuEvent(QContextMenuEvent *);
};

#endif // NICKLISTTREEWIDGET_H
