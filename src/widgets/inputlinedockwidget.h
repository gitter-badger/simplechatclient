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

#ifndef INPUTLINEDOCKWIDGET_H
#define INPUTLINEDOCKWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include "inputwidget.h"
#include "log.h"
#include "network.h"
#include "toolwidget.h"

class InputLineDockWidget : public QWidget
{
    Q_OBJECT
public:
    InputLineDockWidget(QWidget *, Network *, DlgChannelSettings *, DlgModeration *);
    void enable_moderation();
    void disable_moderation();
    void hide_toolwidget();
    void show_toolwidget();
    inline void hide_channel_settings() { pToolWidget->set_channel_settings(false); }
    inline void show_channel_settings() { pToolWidget->set_channel_settings(true); }
    inline void set_active(QString strChannel) {  pInputWidget->set_active(strChannel); }
    inline QString get_active() { return pInputWidget->get_active(); }
    inline void set_userslist(QTreeWidget *treeWidget) { pInputWidget->set_userslist(treeWidget); }

public slots:
    void slot_update_nick(QString);
    void slot_display_message(QString, QString, int);
    void slot_show_msg(QString, QString, int);
    void slot_show_hide_toolwidget();
    void slot_change_font_size(QString);
    void slot_clear_content(QString);
    void slot_set_scroll(QString, bool);

private:
    // params
    QWidget *myparent;
    Network *pNetwork;
    DlgChannelSettings *dlgchannel_settings;
    DlgModeration *dlgmoderation;
    // other
    InputWidget *pInputWidget;
    ToolWidget *pToolWidget;

signals:
    void update_nick(QString);
    void display_message(QString,QString,int);
    void show_msg(QString,QString,int);
    void change_font_size(QString);
    void clear_content(QString);
    void set_scroll(QString, bool);

};

#endif // INPUTLINEDOCKWIDGET_H
