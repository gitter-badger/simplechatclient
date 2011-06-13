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

#ifndef DLG_FIND_TEXT_H
#define DLG_FIND_TEXT_H

class MainWindow;
class MainTextEdit;
#include <QDialog>
#include "ui_find_text.h"

class DlgFindText : public QDialog
{
    Q_OBJECT
public:
    DlgFindText(MainWindow *, MainTextEdit *);

private:
    Ui::uiFindText ui;
    MainTextEdit *textEdit;

    void create_gui();
    void create_signals();

private slots:
    void button_find_next();
};

#endif // DLG_FIND_TEXT_H
