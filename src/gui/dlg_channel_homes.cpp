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

#include "dlg_channel_homes.h"

DlgChannelHomes::DlgChannelHomes(QWidget *parent, Network *param1, QMap <QString, QByteArray> *param2, DlgChannelSettings *param3) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Your channels"));

    myparent = parent;
    pNetwork = param1;
    mChannelAvatar = param2;
    dlgchannel_settings = param3;

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
}

void DlgChannelHomes::add_channel(QString strChannel)
{
    strChannel = strChannel.right(strChannel.length()-1); // remove status
    if (exist_channel(strChannel) == true) return; // already exist in list

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

void DlgChannelHomes::clear()
{
    ui.listWidget_channels->clear();
}

bool DlgChannelHomes::exist_channel(QString strChannel)
{
    for (int i = 0; i < ui.listWidget_channels->count(); i++)
    {
        if (ui.listWidget_channels->item(i)->text() == strChannel)
            return true;
    }
    return false;
}

void DlgChannelHomes::list_clicked(QModelIndex index)
{
    int i = index.row();
    QString strChannel = ui.listWidget_channels->item(i)->text();
    dlgchannel_settings->set_channel(strChannel);
    dlgchannel_settings->show();
    ui.listWidget_channels->clear();
    this->hide();
}

void DlgChannelHomes::button_create()
{
    ui.listWidget_channels->clear();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), tr("Enter the name of the new channel:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
        pNetwork->send(QString("CS REGISTER %1").arg(strText));
    else
        pNetwork->send("CS HOMES");
}

void DlgChannelHomes::button_remove()
{
    ui.listWidget_channels->clear();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing channels"), "<p style=\"font-weight:bold;\">"+tr("The removal of the channel operation is irreversible!")+"</p><p>"+tr("Enter the name of the channel to remove:")+"</p>", QLineEdit::Normal, QString::null, &ok);

    if ((ok == true) && (strText.isEmpty() == false))
        pNetwork->send(QString("CS DROP %1").arg(strText));
    else
        pNetwork->send("CS HOMES");
}

void DlgChannelHomes::button_join()
{
    if (ui.listWidget_channels->selectedItems().count() != 0)
    {
        QString strChannel = ui.listWidget_channels->selectedItems().at(0)->text();
        pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
}

void DlgChannelHomes::button_close()
{
    ui.listWidget_channels->clear();
    this->hide();
}

void DlgChannelHomes::showEvent(QShowEvent *event)
{
    event->accept();
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    ui.listWidget_channels->clear();

    pNetwork->send("CS HOMES");
}

void DlgChannelHomes::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
