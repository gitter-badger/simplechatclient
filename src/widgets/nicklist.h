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

#ifndef NICKLIST_H
#define NICKLIST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "dlg_user_profile.h"

#include <QContextMenuEvent>
#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QMenu>
#include <QQueue>
#include <QSettings>
#include <QTreeWidget>
#include "dlg_cam.h"
#include "dlg_kick.h"
#ifdef Q_WS_WIN
#include "kamerzysta.h"
#endif
#include "network.h"

struct NickStatus
{
    QString nick;
    QString status;
};
typedef QQueue <NickStatus> sNickStatus;


class Nicklist : public QTreeWidget
{
    Q_OBJECT
public:
    Nicklist(QWidget *, Network *, QString, QMap <QString, QByteArray> *, QTcpSocket *);
    ~Nicklist();
    void set_open_channels(QStringList);
    void set_user_info(QString, QString, QString);
    void nicklist_add(QString, QString, sNickStatus *);
    void nicklist_remove(QString, sNickStatus *);
    bool nicklist_exist(QString, sNickStatus *);
    QStringList nicklist_get(sNickStatus *);
    void nicklist_refresh_avatars();
    void update_avatar(QString, QByteArray);

private:
    QWidget *myparent;
    Network *pNetwork;
    QString strChannel;
    QStringList strOpenChannels;
    QMap <QString, QByteArray> *mNickAvatar;
    QTcpSocket *camSocket;
    enum { maxOpenChannels = 50 };
    QAction *openChannelsActs[maxOpenChannels];
    sNickInfo sCurrentUserInfo;

    void add_parent(QString, QPixmap);
    bool exist_parent(QString);
    void remove_parent(QString);
    int index_parent(QString);
    void move_parent(int, int);
    void sort_parent();
    void add_child(QString, QTreeWidgetItem *);
    bool exist_child(QString, QString);
    void remove_child(QString);
    QTreeWidgetItem* create_child(QString, QString, QString, QPixmap);

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
    void op_add();
    void op_del();
    void halfop_add();
    void halfop_del();
    void moderator_add();
    void moderator_del();
    void voice_add();
    void voice_del();
    void invite();
    void item_selected();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *);

};

#endif // NICKLIST_H
