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

#ifndef DLG_PRIVILEGE_H
#define DLG_PRIVILEGE_H

#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTableWidget>
#include "network.h"
#include "ui_privilege.h"

class dlg_privilege : public QDialog
{
    Q_OBJECT
public:
    dlg_privilege(network *, QSettings *, QString, QString, QString, QTableWidget *, QTableWidget *, QTableWidget *, QTableWidget *);

private:
    Ui::uiPrivilege ui;
    network *pNetwork;
    QSettings *settings;
    QString strStatus;
    QString strChannel;
    QString strAddDel;
    QTableWidget *opList;
    QTableWidget *halfopList;
    QTableWidget *banList;
    QTableWidget *inviteList;

    void clear();

private slots:
    void button_ok();
    void button_cancel();

};

#endif // DLG_PRIVILEGE_H
