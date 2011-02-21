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

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QShowEvent>
#include "network.h"
#include "dlg_channel_favourites.h"

DlgChannelFavourites::DlgChannelFavourites(QWidget *parent, Network *param1, QMap <QString, QByteArray> *param2, QList<QString> *param3) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Favorite channels"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    myparent = parent;
    pNetwork = param1;
    mChannelAvatar = param2;
    lChannelFavourites = param3;

    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/irc-close-channel.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));

    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    refresh();
}

void DlgChannelFavourites::refresh()
{
    ui.listWidget_channels->clear();

    for (int i = 0; i < lChannelFavourites->count(); i++)
    {
        QString strChannel = lChannelFavourites->at(i);

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
}

void DlgChannelFavourites::button_add()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the new channel to add to favorites:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
    {
        pNetwork->send(QString("NS FAVOURITES ADD %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelFavourites::button_remove()
{
    QString strSelected;
    if (ui.listWidget_channels->selectedItems().count() != 0)
        strSelected = ui.listWidget_channels->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the channel to remove from the favorites:"), QLineEdit::Normal, strSelected, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
    {
        pNetwork->send(QString("NS FAVOURITES DEL %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelFavourites::button_close()
{
    this->close();
}
