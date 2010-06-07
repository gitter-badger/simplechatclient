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

#ifndef DLG_PRIV_H
#define DLG_PRIV_H

#include <QDialog>
#include "network.h"
#include "tab_container.h"
#include "ui_priv.h"

class dlg_priv : public QDialog
{
    Q_OBJECT
public:
    dlg_priv(network *, QSettings *, tab_container *, QString, QString);

private:
    Ui::uiPriv ui;
    network *pNetwork;
    QSettings *settings;
    tab_container *tabc;
    QString strNick;
    QString strChannel;
    QString strTimerChannel;
    QString strTimerNick;

private slots:
    void timer_timeout();
    void button_whois();
    void button_reject();
    void button_ignore();
    void button_accept();

};

#endif // DLG_PRIV_H
