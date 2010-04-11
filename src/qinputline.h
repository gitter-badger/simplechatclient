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

#ifndef QINPUTLINE_H
#define QINPUTLINE_H

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListWidget>

class qinputline : public QLineEdit
{
    Q_OBJECT
public:
    qinputline(QWidget*);
    ~qinputline();
    void set_userslist(QListWidget *);

private:
    int index;
    QString strLastWord;
    QListWidget *users_list;
    QList<QListWidgetItem *> find;

    QString get_word();
    void set_word(QString);

protected:
    virtual bool event(QEvent *);

};

#endif // QINPUTLINE_H
