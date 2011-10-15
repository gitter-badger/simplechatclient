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

#include <QDesktopWidget>
#include <QInputDialog>
#include <QTimer>
#include "core.h"
#include "channel_favourites.h"

DlgChannelFavourites::DlgChannelFavourites(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Favorite channels"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgChannelFavourites::createGui()
{
    ui.pushButton_join->setEnabled(false);

    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/irc-close-channel.png"));
    ui.pushButton_join->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.pushButton_join->setText(tr("Join"));
}

void DlgChannelFavourites::createSignals()
{
    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(buttonAdd()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    QObject::connect(ui.pushButton_join, SIGNAL(clicked()), this, SLOT(buttonJoin()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    QObject::connect(ui.listWidget_channels, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
}

void DlgChannelFavourites::refresh()
{
    ui.listWidget_channels->clear();

    foreach (QString strChannel, Core::instance()->lChannelFavourites)
    {
        if (Core::instance()->mChannelAvatar.contains(strChannel))
        {
            QPixmap pixmap;
            pixmap.loadFromData(Core::instance()->mChannelAvatar.value(strChannel));
            ui.listWidget_channels->addItem(new QListWidgetItem(QIcon(pixmap), strChannel));
        }
        else
        {
            ui.listWidget_channels->addItem(new QListWidgetItem(QIcon(":/images/channel_avatar.png"), strChannel));
            Core::instance()->pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
        }
    }
}

void DlgChannelFavourites::itemClicked(QListWidgetItem *)
{
    if (!ui.pushButton_join->isEnabled())
        ui.pushButton_join->setEnabled(true);
}

void DlgChannelFavourites::buttonAdd()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the new channel to add to favorites:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("NS FAVOURITES ADD %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelFavourites::buttonRemove()
{
    QString strSelected;
    if (ui.listWidget_channels->selectedItems().size() != 0)
        strSelected = ui.listWidget_channels->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the channel to remove from the favorites:"), QLineEdit::Normal, strSelected, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("NS FAVOURITES DEL %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelFavourites::buttonJoin()
{
    if (ui.listWidget_channels->selectedItems().size() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();
        Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
}
