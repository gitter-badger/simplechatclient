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

#ifndef DLG_USER_AVATAR_H
#define DLG_USER_AVATAR_H

class MainWindow;
#include <QDialog>
#include "ui_user_avatar.h"

class DlgUserAvatar : public QDialog
{
    Q_OBJECT
public:
    DlgUserAvatar(MainWindow *, QPixmap);

private:
    Ui::uiUserAvatar ui;
    QPixmap avatar;

    void create_gui();
    void set_default_values();
    void create_signals();

private slots:
    void button_zoom_out();
    void slider_value_changed(int);
    void button_zoom_in();
    void button_close();
};

#endif // DLG_USER_AVATAR_H
