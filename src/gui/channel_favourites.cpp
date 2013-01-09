/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDesktopWidget>
#include <QInputDialog>
#include <QMenu>
#include <QTimer>
#include "avatar.h"
#include "channel.h"
#include "core.h"
#include "channel_favourites_model.h"
#include "channel_favourites.h"

DlgChannelFavourites::DlgChannelFavourites(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Favorite channels"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgChannelFavourites::createGui()
{
    ui.toolButton_options->setEnabled(false);

    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/irc-close-channel.png"));
    ui.toolButton_options->setIcon(QIcon(":/images/oxygen/16x16/applications-system.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.toolButton_options->setText(tr("Options"));

    QMenu *optionsMenu = new QMenu(this);
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/legalmoves.png"), tr("Join"), this, SLOT(join()));

    ui.toolButton_options->setMenu(optionsMenu);
}

void DlgChannelFavourites::createSignals()
{
    connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(buttonAdd()));
    connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui.listWidget_channels, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
}

void DlgChannelFavourites::refresh()
{
    ui.listWidget_channels->clear();

    QHashIterator<QString, OnetChannelFavourites> i(ChannelFavouritesModel::instance()->getAll());
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();
        OnetChannelFavourites onetChannelFavourites = i.value();

        QString strAvatar = onetChannelFavourites.avatar;
        if (!strAvatar.isEmpty())
        {
            strAvatar = Avatar::instance()->getAvatarPath(strAvatar);
        }
        else
        {
            strAvatar = ":/images/channel_avatar.png";
            Core::instance()->network->send(QString("CS INFO %1 i").arg(strChannel));
        }

        ui.listWidget_channels->addItem(new QListWidgetItem(QIcon(strAvatar), strChannel));
    }
}

void DlgChannelFavourites::itemClicked(QListWidgetItem *)
{
    if (!ui.toolButton_options->isEnabled())
        ui.toolButton_options->setEnabled(true);
}

void DlgChannelFavourites::buttonAdd()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the new channel to add to favorites:"), QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->network->send(QString("NS FAVOURITES ADD %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgChannelFavourites::buttonRemove()
{
    QString strSelected;
    if (ui.listWidget_channels->selectedItems().size() != 0)
        strSelected = ui.listWidget_channels->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your favorite channels"), tr("Enter the name of the channel to remove from the favorites:"), QLineEdit::Normal, strSelected, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->network->send(QString("NS FAVOURITES DEL %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgChannelFavourites::join()
{
    if (ui.listWidget_channels->selectedItems().size() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();
        Core::instance()->network->send(QString("JOIN %1").arg(strChannel));
    }
}
