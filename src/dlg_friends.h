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

#ifndef DLG_FRIENDS_H
#define DLG_FRIENDS_H

#include <QDialog>
#include "dlg_friends_ad.h"
#include "network.h"
#include "tab_container.h"
#include "ui_friends.h"

class dlg_friends : public QDialog
{
    Q_OBJECT
public:
    dlg_friends(QSettings *, network *, tab_container *);
    void set_friend(QString, bool);
    void remove_friend(QString);
    void clear();

private:
    Ui::uiFriends ui;
    QSettings *settings;
    network *pNetwork;
    tab_container *tabc;
    QMap<QString, bool> friends;

    void refresh();

private slots:
    void button_add();
    void button_remove();
    void button_ok();
    void button_cancel();

};

#endif // DLG_FRIENDS_H
