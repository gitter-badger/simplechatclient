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
#include "channel_settings.h"
#include "channel_homes.h"

DlgChannelHomes::DlgChannelHomes(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Your channels"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgChannelHomes::createGui()
{
    ui.pushButton_join->setEnabled(false);
    ui.pushButton_settings->setEnabled(false);

    ui.pushButton_create->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/irc-close-channel.png"));
    ui.pushButton_join->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));
    ui.pushButton_settings->setIcon(QIcon(":/images/oxygen/16x16/configure.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_create->setText(tr("Create"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.pushButton_join->setText(tr("Join"));
    ui.pushButton_settings->setText(tr("Settings"));
}

void DlgChannelHomes::createSignals()
{
    QObject::connect(ui.listWidget_channels, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(listClicked(QModelIndex)));
    QObject::connect(ui.listWidget_channels, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    QObject::connect(ui.pushButton_create, SIGNAL(clicked()), this, SLOT(buttonCreate()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    QObject::connect(ui.pushButton_join, SIGNAL(clicked()), this, SLOT(buttonJoin()));
    QObject::connect(ui.pushButton_settings, SIGNAL(clicked()), this, SLOT(buttonSettings()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgChannelHomes::refresh()
{
    ui.listWidget_channels->clear();

    foreach (QString strChannel, Core::instance()->lChannelHomes)
    {
        strChannel = strChannel.right(strChannel.length()-1); // remove status

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

void DlgChannelHomes::listClicked(QModelIndex index)
{
    int i = index.row();
    QString strChannel = ui.listWidget_channels->item(i)->text();

    DlgChannelSettings(this, strChannel).exec();
}

void DlgChannelHomes::itemClicked(QListWidgetItem *item)
{
    Q_UNUSED (item);

    if (!ui.pushButton_join->isEnabled())
        ui.pushButton_join->setEnabled(true);
    if (!ui.pushButton_settings->isEnabled())
        ui.pushButton_settings->setEnabled(true);
}

void DlgChannelHomes::buttonCreate()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), tr("Enter the name of the new channel:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("CS REGISTER %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelHomes::buttonRemove()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), "<p style=\"font-weight:bold;\">"+tr("The removal of the channel operation is irreversible!")+"</p><p>"+tr("Enter the name of the channel to remove:")+"</p>", QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("CS DROP %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelHomes::buttonJoin()
{
    if (ui.listWidget_channels->selectedItems().size() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();
        Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
}

void DlgChannelHomes::buttonSettings()
{
    if (ui.listWidget_channels->selectedItems().size() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();

        DlgChannelSettings(this, strChannel).exec();
    }
}
