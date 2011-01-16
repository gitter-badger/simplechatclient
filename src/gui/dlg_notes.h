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

#ifndef DLG_NOTES_H
#define DLG_NOTES_H

#include <QDialog>
#include "ui_notes.h"

class DlgNotes : public QDialog
{
    Q_OBJECT
public:
    DlgNotes(QWidget *);

private:
    Ui::uiNotes ui;
    QWidget *myparent;
    QString path;
    QString strNotesFile;

    void read_path();
    void read();
    void save();

private slots:
    void button_ok();
    void button_cancel();
};

#endif // DLG_NOTES_H
