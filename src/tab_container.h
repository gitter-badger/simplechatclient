/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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
#include <QTcpSocket>
#include "config.h"
#include "dlg_channel_settings.h"
#include "dlg_moderation.h"
#include "tab_manager.h"
#include "tab_widget.h"

class tab_container : public QObject
{
    Q_OBJECT
public:
    tab_container(tab_manager *, QWidget *, QTcpSocket *, QSettings *);
    ~tab_container();
    void set_dlg(dlg_channel_settings *, dlg_moderation *);
    bool exist_tab(QString);
    void add_tab(QString, QWidget *);
    void remove_tab(QString);
    void rename_tab(QString, QString);
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
    void set_logo(QString, QByteArray);
    QStringList get_open_channels();

private:
    tab_manager *tabm;
    tab_widget *tw[50];
    QString free_list;
    QWidget *mainWin;
    QTcpSocket *socket;
    QSettings *settings;
    dlg_channel_settings *dlgchannel_settings;
    dlg_moderation *dlgmoderation;

    void update_open_channels();
    int free_list_get();

};

#endif // TAB_CONTAINER_H
