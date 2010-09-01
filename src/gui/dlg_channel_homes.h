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

#ifndef DLG_CHANNEL_HOMES_H
#define DLG_CHANNEL_HOMES_H

#include <QDialog>
#include <QInputDialog>
#include "dlg_channel_settings.h"
#include "network.h"
#include "ui_channel_homes.h"

class DlgChannelHomes : public QDialog
{
    Q_OBJECT
public:
    DlgChannelHomes(QWidget *, Network *, QSettings *, QMap <QString, QByteArray> *, DlgChannelSettings *);
    void add_channel(QString);
    void clear();

private:
    Ui::uiChannelHomes ui;
    QWidget *myparent;
    Network *pNetwork;
    QSettings *settings;
    QMap <QString, QByteArray> *mChannelAvatar;
    DlgChannelSettings *dlgchannel_settings;

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);

private slots:
    void list_clicked(QModelIndex);
    void button_create();
    void button_remove();
    void button_join();
    void button_close();

};

#endif // DLG_CHANNEL_HOMES_H
