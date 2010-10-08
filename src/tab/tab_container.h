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
#include "inputlinedockwidget.h"
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
    TabContainer(QWidget *, Network *, TabManager *, Notify *, QMap <QString, QByteArray> *, QTcpSocket *, InputLineDockWidget *, sChannelNickStatus *);
    ~TabContainer();
    inline QString get_name(int i) { if ((i < 0) || (i > tw.count())) return QString::null; else return tw[i]->get_name(); }
    void set_dlg(DlgChannelSettings *, DlgModeration *);
    bool exist_tab(QString);
    void remove_tab(QString);
    bool rename_tab(QString, QString);
    void part_tab(int);
    void show_msg(QString, QString, QString, int);
    void show_msg(QString, QString, int);
    void show_msg_all(QString, int);
    void show_msg_active(QString, int);
    void set_topic(QString, QString);
    void author_topic(QString, QString);
    void enable_moderation(QString);
    void disable_moderation(QString);
    void set_user_info(QString, QString, QString);
    QStringList get_open_channels();

public slots:
    void update_channel_avatar(QString);
    void slot_show_msg(QString, QString, int);
    void slot_show_msg_active(QString, int);
    void slot_show_msg_all(QString, int);
    void slot_display_message(QString, QString, int);
    void refresh_colors();

private:
    QWidget *myparent;
    Network *pNetwork;
    TabManager *pTabM;
    QList <TabWidget *> tw;
    Notify *pNotify;
    QMap <QString, QByteArray> *mChannelAvatar;
    DlgChannelSettings *dlgchannel_settings;
    DlgModeration *dlgmoderation;
    QTcpSocket *camSocket;
    InputLineDockWidget *inputlinewidget;
    sChannelNickStatus *mChannelNickStatus;

    void add_tab(QString);
    void update_open_channels();
    int get_index(QString);

signals:
    void create_nicklist(QString);
    void remove_nicklist(QString);
    void currentChanged(int);

};

#endif // TAB_CONTAINER_H
