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

DlgIgnore::DlgIgnore(Network *param1, QSettings *param2, TabContainer *param3)
{
    ui.setupUi(this);

    pNetwork = param1;
    settings = param2;
    tabc = param3;

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgIgnore::add_ignore(QString strNick)
{
    ui.listWidget->addItem(new QListWidgetItem(QIcon(":/3rdparty/images/people.png"), strNick));
}

void DlgIgnore::clear()
{
    ui.listWidget->clear();
}

void DlgIgnore::button_add()
{
    ui.listWidget->clear();
    (new DlgIgnoreAd(pNetwork, settings, tabc, "add", ""))->show();
}

void DlgIgnore::button_remove()
{
    QString strSelected;
    if (ui.listWidget->selectedItems().count() != 0)
        strSelected = ui.listWidget->selectedItems().at(0)->text();

    ui.listWidget->clear();

    (new DlgIgnoreAd(pNetwork, settings, tabc, "remove", strSelected))->show();
}

void DlgIgnore::button_ok()
{
    ui.listWidget->clear();
    this->hide();
}

void DlgIgnore::button_cancel()
{
    ui.listWidget->clear();
    this->hide();
}

void DlgIgnore::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget->clear();
    pNetwork->send("NS IGNORE");
}
