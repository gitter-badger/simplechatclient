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

#ifndef DLG_FRIENDS_H
#define DLG_FRIENDS_H

class Network;
#include <QDialog>
#include "ui_friends.h"

class DlgFriends : public QDialog
{
    Q_OBJECT
public:
    DlgFriends(QWidget *, Network *, QMap <QString, QByteArray> *, QMap <QString, bool> *);

private:
    Ui::uiFriends ui;
    QWidget *myparent;
    Network *pNetwork;
    QMap <QString, QByteArray> *mNickAvatar;
    QMap <QString, bool> *mFriends;

private slots:
    void refresh();
    void tab_changed(int);
    void button_add();
    void button_remove();
    void button_whois();
    void button_close();

protected:
    virtual void showEvent(QShowEvent *);
};

#endif // DLG_FRIENDS_H
