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

#include "dlg_ignore.h"

dlg_ignore::dlg_ignore(QSettings *param1, network *param2, tab_container *param3)
{
    ui.setupUi(this);

    settings = param1;
    pNetwork = param2;
    tabc = param3;

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void dlg_ignore::add_ignore(QString strNick)
{
    ui.listWidget->addItem(new QListWidgetItem(QIcon(":/3rdparty/images/people.png"), strNick));
}

void dlg_ignore::clear()
{
    ui.listWidget->clear();
}

void dlg_ignore::button_add()
{
    ui.listWidget->clear();
    (new dlg_ignore_ad(pNetwork, tabc, settings, "add"))->show();
}

void dlg_ignore::button_remove()
{
    ui.listWidget->clear();
    (new dlg_ignore_ad(pNetwork, tabc, settings, "remove"))->show();
}

void dlg_ignore::button_ok()
{
    ui.listWidget->clear();
    this->hide();
}

void dlg_ignore::button_cancel()
{
    ui.listWidget->clear();
    this->hide();
}

void dlg_ignore::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget->clear();
    pNetwork->send("NS IGNORE");
}
