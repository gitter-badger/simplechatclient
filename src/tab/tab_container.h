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

#ifndef TAB_CONTAINER_H
#define TAB_CONTAINER_H

#ifndef Q_WS_WIN
    class DlgCam;
#endif
class DlgChannelSettings;
class DlgModeration;
class DlgUserProfile;
class Network;
class TabManager;
class TabWidget;
#include <QMap>
#include <QObject>
#include <QTcpSocket>

/**
 * Class for displaying messages in tabs or change main window settings
 */
class TabContainer : public QObject
{
    Q_OBJECT
public:
    TabContainer(Network *, TabManager *, QTcpSocket *);
    virtual ~TabContainer();
    void set_dlg(DlgUserProfile *);
#ifndef Q_WS_WIN
    void set_dlg_cam(DlgCam *);
#endif
    QString get_name(int);
    bool exist_tab(QString);
    void add_tab(QString);
    void remove_tab(QString);
    bool rename_tab(QString, QString);
    void part_tab(int);
    void show_msg(QString, QString, QString, int);
    void show_msg(QString, QString, int);
    void show_msg_all(QString, int);
    void show_msg_active(QString, int);
    void set_topic(QString, QString);
    void author_topic(QString, QString);
    void set_link(QString, QString);

public slots:
    void slot_update_nick_avatar(QString);
    void slot_update_channel_avatar(QString);
    void slot_show_msg(QString, QString, int);
    void slot_show_msg_active(QString, int);
    void slot_show_msg_all(QString, int);
    void slot_display_message(QString, QString, int);
    void slot_change_font_size(QString);
    void slot_clear_content(QString);
    void refresh_colors();
    void refresh_background_image();

private:
    // params
    Network *pNetwork;
    TabManager *pTabM;
    DlgChannelSettings *pDlgChannelSettings;
    DlgModeration *pDlgModeration;
    QTcpSocket *camSocket;
    DlgUserProfile *pDlgUserProfile;
#ifndef Q_WS_WIN
    DlgCam *pDlgCam;
#endif
    // other
    QList<TabWidget*> tw;

    int get_index(QString);

signals:
    void clear_channel_all_nick_avatars(QString);
    void clear_nicklist(QString);
    void update_nick_avatar(QString);
    void update_awaylog_status();
};

#endif // TAB_CONTAINER_H
