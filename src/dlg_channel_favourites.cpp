/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "dlg_channel_favourites.h"

DlgChannelFavourites::DlgChannelFavourites(Network *param1, QSettings *param2, TabContainer *param3, QMap <QString, QByteArray> *param4)
{
    ui.setupUi(this);

    pNetwork = param1;
    settings = param2;
    tabc = param3;
    mChannelAvatar = param4;

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgChannelFavourites::add_channel(QString strChannel)
{
    if (mChannelAvatar->contains(strChannel) == true)
    {
        QPixmap pixmap;
        pixmap.loadFromData(mChannelAvatar->value(strChannel));
        ui.listWidget->addItem(new QListWidgetItem(QIcon(pixmap), strChannel));
    }
    else
    {
        ui.listWidget->addItem(new QListWidgetItem(QIcon(":/images/channel_avatar.png"), strChannel));
        pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
    }
}

void DlgChannelFavourites::clear()
{
    ui.listWidget->clear();
}

void DlgChannelFavourites::button_add()
{
    ui.listWidget->clear();
    (new DlgChannelFavouritesAd(pNetwork, settings, tabc, "add", ""))->show();
}

void DlgChannelFavourites::button_remove()
{
    QString strSelected;
    if (ui.listWidget->selectedItems().count() != 0)
        strSelected = ui.listWidget->selectedItems().at(0)->text();

    ui.listWidget->clear();
    (new DlgChannelFavouritesAd(pNetwork, settings, tabc, "remove", strSelected))->show();
}

void DlgChannelFavourites::button_ok()
{
    ui.listWidget->clear();
    this->hide();
}

void DlgChannelFavourites::button_cancel()
{
    ui.listWidget->clear();
    this->hide();
}

void DlgChannelFavourites::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget->clear();
    pNetwork->send("NS FAVOURITES");
}
