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

#ifndef DLG_PRIV_H
#define DLG_PRIV_H

class MainWindow;
#include <QDialog>
#include "ui_invite.h"

class DlgInvite : public QDialog
{
    Q_OBJECT
public:
    DlgInvite(MainWindow *, QString, QString);

private:
    Ui::uiInvite ui;
    QString strNick;
    QString strChannel;

    void createGui();
    void createSignals();

private slots:
    void buttonWhois();
    void buttonReject();
    void buttonIgnore();
    void buttonAccept();
};

#endif // DLG_PRIV_H
