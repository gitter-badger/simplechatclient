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

DlgChannelFavourites::DlgChannelFavourites(QWidget *parent, Network *param1, QSettings *param2, QMap <QString, QByteArray> *param3) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Favorite channels"));

    myparent = parent;
    pNetwork = param1;
    settings = param2;
    mChannelAvatar = param3;

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));

    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
}

void DlgChannelFavourites::add_channel(QString strChannel)
{
    if (mChannelAvatar->contains(strChannel) == true)
    {
        QPixmap pixmap;
        pixmap.loadFromData(mChannelAvatar->value(strChannel));
        ui.listWidget_channels->addItem(new QListWidgetItem(QIcon(pixmap), strChannel));
    }
    else
    {
        ui.listWidget_channels->addItem(new QListWidgetItem(QIcon(":/images/channel_avatar.png"), strChannel));
        pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
    }
}

void DlgChannelFavourites::clear()
{
    ui.listWidget_channels->clear();
}

void DlgChannelFavourites::button_add()
{
    ui.listWidget_channels->clear();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the new channel to add to favorites:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
        pNetwork->send(QString("NS FAVOURITES ADD %1").arg(strText));

    pNetwork->send("NS FAVOURITES");
}

void DlgChannelFavourites::button_remove()
{
    QString strSelected;
    if (ui.listWidget_channels->selectedItems().count() != 0)
        strSelected = ui.listWidget_channels->selectedItems().at(0)->text();

    ui.listWidget_channels->clear();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the channel to remove from the favorites:"), QLineEdit::Normal, strSelected, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
        pNetwork->send(QString("NS FAVOURITES DEL %1").arg(strText));

    pNetwork->send("NS FAVOURITES");
}

void DlgChannelFavourites::button_close()
{
    ui.listWidget_channels->clear();
    this->hide();
}

void DlgChannelFavourites::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget_channels->clear();
    pNetwork->send("NS FAVOURITES");
}

void DlgChannelFavourites::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
