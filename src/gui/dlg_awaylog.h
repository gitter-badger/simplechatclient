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

#ifndef DLG_AWAYLOG_H
#define DLG_AWAYLOG_H

#include <QDialog>
#include "ui_awaylog.h"

class DlgAwaylog : public QDialog
{
    Q_OBJECT
public:
    DlgAwaylog(QWidget *, QAction *);

private:
    Ui::uiAwaylog ui;
    QAction *awaylogAct;

    void create_gui();
    void create_signals();

    void refresh();
    void button_reset();
    void button_close();

private slots:
    void buttons(QAbstractButton*);
};

#endif // DLG_AWAYLOG_H
