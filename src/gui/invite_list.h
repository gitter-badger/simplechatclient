/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INVITE_LIST_H
#define INVITE_LIST_H

#include <QDialog>
#include "ui_invite_list.h"

class DlgInviteList : public QDialog
{
    Q_OBJECT
public:
    DlgInviteList(QWidget *parent = 0);

private:
    Ui::uiInviteList ui;   

    void createGui();
    void createSignals();
    void createList();

private slots:
    void buttonWhois();
    void buttonReject();
    void buttonIgnore();
    void buttonAccept();
};

#endif // INVITE_LIST_H
