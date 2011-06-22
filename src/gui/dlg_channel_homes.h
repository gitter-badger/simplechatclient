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

#ifndef DLG_CHANNEL_HOMES_H
#define DLG_CHANNEL_HOMES_H

class DlgChannelSettings;
class Network;
#include <QDialog>
#include "ui_channel_homes.h"

class DlgChannelHomes : public QDialog
{
    Q_OBJECT
public:
    DlgChannelHomes(QWidget *, Network *, DlgChannelSettings *);

private:
    Ui::uiChannelHomes ui;
    Network *pNetwork;
    DlgChannelSettings *pDlgChannelSettings;

    void create_gui();
    void create_signals();

private slots:
    void refresh();
    void list_clicked(QModelIndex);
    void item_clicked(QListWidgetItem*);
    void button_create();
    void button_remove();
    void button_join();
    void button_settings();
};

#endif // DLG_CHANNEL_HOMES_H
