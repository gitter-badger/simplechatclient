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

#ifndef DLG_EMOTICONS_H
#define DLG_EMOTICONS_H

class InputWidget;
class MainWindow;
#include <QDialog>
#include "ui_emoticons.h"

class DlgEmoticons : public QDialog
{
    Q_OBJECT
public:
    DlgEmoticons(MainWindow *, InputWidget *);

private:
    Ui::uiEmoticons ui;
    InputWidget *pInputWidget;
    bool bDoneStandard;
    bool bDoneExtended;

    void create_gui();
    void set_default_values();
    void create_signals();

    void get_emoticons_standard();
    void get_emoticons_extended();

private slots:
    void current_tab_changed(int);
    void clicked_standard(QModelIndex);
    void clicked_extended(QModelIndex);
    void button_insert();
    void button_close();
};

#endif // DLG_EMOTICONS_H
