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

class MyTreeWidgetItem : public QTreeWidgetItem
{
public:
    MyTreeWidgetItem() {}
    MyTreeWidgetItem(QTreeWidget *parent) : QTreeWidgetItem(parent) {}

    virtual bool operator< ( const QTreeWidgetItem & o ) const
    {
        int column =  treeWidget() ? treeWidget()->sortColumn() : 0; // column

        QVariant my = (data(column, Qt::DisplayRole)); // compare value 1
        QVariant other = (o.data(column, Qt::DisplayRole)); // compare value 2
        if (!my.isValid() || !other.isValid() || my.type() != other.type()) // valid compare
            return QTreeWidgetItem::operator< (o);
        if (my.canConvert(QVariant::String)) // my compare
            return text(column).toLower() < o.text(column).toLower();
        else
            return QTreeWidgetItem::operator< (o); // other compare
    }
};


class NickListWidget : public QTreeWidget
{
    Q_OBJECT
public:
    NickListWidget(QWidget *, Network *, QString, QMap <QString, QByteArray> *, QTcpSocket *, sChannelNickStatus *, DlgUserProfile *);
    ~NickListWidget();
#ifndef Q_WS_WIN
    void set_dlg_cam(DlgCam *);
#endif
    void set_open_channels(QStringList);
    void add(QString, QString, QString, sChannelNickStatus *mChannelNickStatus);
    void remove(QString, sChannelNickStatus *mChannelNickStatus);
    bool exist(QString, sChannelNickStatus *mChannelNickStatus);
    QStringList get(sChannelNickStatus *mChannelNickStatus);
    void refresh_avatars();
    void update_avatar(QString, QByteArray);

private:
    // params
    QWidget *myparent;
    Network *pNetwork;
    QString strChannel;
    QStringList strOpenChannels;
    QMap <QString, QByteArray> *mNickAvatar;
    QTcpSocket *camSocket;
    sChannelNickStatus *mChannelNickStatus;
    DlgUserProfile *pDlg_user_profile;
#ifndef Q_WS_WIN
    DlgCam *pDlg_cam;
#endif
    // other
    enum { maxOpenChannels = 50 };
    QAction *openChannelsActs[maxOpenChannels];

    void add_parent(QString, QPixmap);
    bool exist_parent(QString);
    void remove_parent(QString);
    int index_parent(QString);
    void move_parent(int, int);
    void sort_parent();
    void add_child(QString, MyTreeWidgetItem *);
    bool exist_child(QString, QString);
    void remove_child(QString);
    MyTreeWidgetItem* create_child(QString, QString, QPixmap);

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
