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

#ifndef TAB_CONTAINER_H
#define TAB_CONTAINER_H

#include <QObject>
#include <QSettings>
#include "config.h"
#include "log.h"
#include "network.h"
#include "dlg_channel_settings.h"
#include "dlg_moderation.h"
#include "notify.h"
#include "tab_manager.h"
#include "tab_widget.h"

class TabContainer : public QObject
{
    Q_OBJECT
public:
    TabContainer(QWidget *, Network *, TabManager *, Notify *, QMap <QString, QByteArray> *, QMap <QString, QByteArray> *, QTcpSocket *);
    ~TabContainer();
    void set_dlg(DlgChannelSettings *, DlgModeration *);
    bool exist_tab(QString);
    void remove_tab(QString);
    bool rename_tab(QString, QString);
    void show_msg(QString, QString, QString, int);
    void show_msg(QString, QString, int);
    void show_msg_all(QString, int);
    void show_msg_active(QString, int);
    void set_topic(QString, QString);
    void enable_topic(QString);
    void disable_topic(QString);
    void author_topic(QString, QString);
    void set_link(QString, QString);
    void add_user(QString, QString, QString);
    void del_user(QString, QString);
    void quit_user(QString, QString);
    void change_flag(QString, QString, QString);
    void change_flag(QString, QString);
    void update_nick(QString);
    void clear_nicklist(QString);
    void clear_all_nicklist();
    void clear_channel_all_nick_avatars(QString);
    void refresh_nicklist(QString);
    int get_nick_channels(QString);
    void set_user_info(QString, QString, QString);
    void create_channel_names();
    QStringList get_open_channels();

public slots:
    void update_nick_avatar(QString);
    void update_channel_avatar(QString);
    void sshow_msg(QString, QString, int);
    void refresh_colors();

private:
    QWidget *myparent;
    Network *pNetwork;
    TabManager *tabm;
    TabWidget *tw[50];
    QString strFreeUsedList;
    Notify *pNotify;
    QMap <QString, QByteArray> *mNickAvatar;
    QMap <QString, QByteArray> *mChannelAvatar;
    DlgChannelSettings *dlgchannel_settings;
    DlgModeration *dlgmoderation;
    QTcpSocket *camSocket;

    void add_tab(QString);
    void update_open_channels();
    int free_list_get();
    int get_index(QString);

};

#endif // TAB_CONTAINER_H
