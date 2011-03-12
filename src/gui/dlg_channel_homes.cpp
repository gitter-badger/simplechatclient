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
#include "dlg_channel_settings.h"
#include "network.h"
#include "dlg_channel_homes.h"

DlgChannelHomes::DlgChannelHomes(QWidget *parent, Network *param1, QMap <QString, QByteArray> *param2, QList<QString> *param3, DlgChannelSettings *param4) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Your channels"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    myparent = parent;
    pNetwork = param1;
    mChannelAvatar = param2;
    lChannelHomes = param3;
    dlgchannel_settings = param4;

    ui.pushButton_create->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/irc-close-channel.png"));
    ui.pushButton_join->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_create->setText(tr("Create"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.pushButton_join->setText(tr("Join"));

    QObject::connect(ui.listWidget_channels, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(list_clicked(QModelIndex)));
    QObject::connect(ui.pushButton_create, SIGNAL(clicked()), this, SLOT(button_create()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.pushButton_join, SIGNAL(clicked()), this, SLOT(button_join()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    refresh();
}

void DlgChannelHomes::refresh()
{
    ui.listWidget_channels->clear();

    for (int i = 0; i < lChannelHomes->size(); i++)
    {
        QString strChannel = lChannelHomes->at(i);
        strChannel = strChannel.right(strChannel.length()-1); // remove status

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

void DlgChannelHomes::list_clicked(QModelIndex index)
{
    int i = index.row();
    QString strChannel = ui.listWidget_channels->item(i)->text();

    dlgchannel_settings->set_channel(strChannel);
    dlgchannel_settings->show();

    this->close();
}

void DlgChannelHomes::button_create()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), tr("Enter the name of the new channel:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
    {
        pNetwork->send(QString("CS REGISTER %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelHomes::button_remove()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), "<p style=\"font-weight:bold;\">"+tr("The removal of the channel operation is irreversible!")+"</p><p>"+tr("Enter the name of the channel to remove:")+"</p>", QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
    {
        pNetwork->send(QString("CS DROP %1").arg(strText));
        QTimer::singleShot(1000*2, this, SLOT(refresh())); // 2 sec
    }
}

void DlgChannelHomes::button_join()
{
    if (ui.listWidget_channels->selectedItems().size() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();
        pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
}

void DlgChannelHomes::button_close()
{
    this->close();
}
