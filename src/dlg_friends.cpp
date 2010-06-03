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

#include "dlg_friends.h"

dlg_friends::dlg_friends(QSettings *param1, network *param2, tab_container *param3)
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

void dlg_friends::set_friend(QString strNick, bool bStatus)
{
    friends[strNick] = bStatus;
    dlg_friends::refresh();
}

void dlg_friends::remove_friend(QString strNick)
{
    friends.remove(strNick);
    dlg_friends::refresh();
}

void dlg_friends::refresh()
{
    ui.listWidget->clear();
    ui.listWidget_2->clear();

    QMap<QString, bool>::const_iterator i = friends.constBegin();
    while (i != friends.constEnd())
    {
        QListWidgetItem *item;
        item = new QListWidgetItem(QIcon(":/3rdparty/images/people.png"), i.key());
        if (i.value() == true)
            ui.listWidget->addItem(item);
        else
            ui.listWidget_2->addItem(item);

        ++i;
    }
}

void dlg_friends::clear()
{
    friends.clear();
}

void dlg_friends::button_add()
{
    (new dlg_friends_ad(pNetwork, tabc, settings, "add"))->show();
}

void dlg_friends::button_remove()
{
    (new dlg_friends_ad(pNetwork, tabc, settings, "remove"))->show();
}

void dlg_friends::button_ok()
{
    this->hide();
}

void dlg_friends::button_cancel()
{
    this->hide();
}
