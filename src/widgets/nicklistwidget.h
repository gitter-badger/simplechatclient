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
#include <QTcpSocket>
#include <QTreeWidget>

class NickListWidget : public QTreeWidget
{
    Q_OBJECT
public:
    NickListWidget(Network *, QString, QTcpSocket *, DlgUserProfile *);
    virtual ~NickListWidget();
#ifndef Q_WS_WIN
    void set_dlg_cam(DlgCam *);
#endif
    void set_open_channels(QStringList);
    void add(QString, QString, QString);
    void remove(QString);
    bool exist(QString);
    QStringList get();
    void refresh_avatars();
    void update_avatar(QString, QByteArray);

private:
    // params
    Network *pNetwork;
    QString strChannel;
    QStringList strOpenChannels;
    QTcpSocket *camSocket;
    DlgUserProfile *pDlgUserProfile;
#ifndef Q_WS_WIN
    DlgCam *pDlgCam;
#endif
    // other
    enum { maxOpenChannels = 30 };
    QAction *openChannelsActs[maxOpenChannels];

    void add_parent(QString, QPixmap);
    bool exist_parent(QString);
    void remove_parent(QString);
    int index_parent(QString);
    void move_parent(int, int);
    void sort_parent();
    void add_child(QString, SortedTreeWidgetItem *);
    bool exist_child(QString, QString);
    void remove_child(QString);
    SortedTreeWidgetItem* create_child(QString, QString, QPixmap);

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
