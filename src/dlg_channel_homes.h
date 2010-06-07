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

#ifndef DLG_CHANNEL_HOMES_H
#define DLG_CHANNEL_HOMES_H

#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QDialog>
#include "dlg_channel_homes_ad.h"
#include "dlg_channel_settings.h"
#include "network.h"
#include "tab_container.h"
#include "ui_channel_homes.h"

class dlg_channel_homes : public QDialog
{
    Q_OBJECT
public:
    dlg_channel_homes(network *, QSettings *, tab_container *, dlg_channel_settings *);
    void add_channel(QString);
    void clear();

private:
    Ui::uiChannelHomes ui;
    network *pNetwork;
    QSettings *settings;
    tab_container *tabc;
    dlg_channel_settings *dlgchannel_settings;
    dlg_channel_homes_ad *dlgchannel_homes_ad;

protected:
    virtual void showEvent(QShowEvent *event);

private slots:
    void list_clicked(QModelIndex);
    void button_create();
    void button_remove();
    void button_ok();
    void button_cancel();

};

#endif // DLG_CHANNEL_HOMES_H
