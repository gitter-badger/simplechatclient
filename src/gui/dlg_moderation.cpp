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

#include <QDateTime>
#include <QDesktopWidget>
#include <QSettings>
#include <QShowEvent>
#include "dlg_moderation.h"

DlgModeration::DlgModeration(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Moderation"));

    create_gui();
    create_signals();
}

void DlgModeration::create_gui()
{
    ui.pushButton_accept->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.label_channel->setText(tr("Channel: "));
    ui.pushButton_accept->setText(tr("Accept"));
    ui.pushButton_remove->setText(tr("Remove"));
}

void DlgModeration::create_signals()
{
    QObject::connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(button_accept()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(hide()));
    QObject::connect(ui.comboBox_channels, SIGNAL(currentIndexChanged(QString)), this, SLOT(combo_changed(QString)));
}

void DlgModeration::refresh()
{
    for (int i = 0; i < mModerateMessages.size(); i++)
    {
        ModerateMsg item = mModerateMessages.at(i);
        QString strDT = item.datetime;
        QString strID = item.id;
        QString strChannel = item.channel;
        QString strNick = item.nick;
        QString strMessage = item.message;

        if (strChannel == strCurrentChannel)
        {
            QString strData = QString("[%1] <%2> %3").arg(strDT).arg(strNick).arg(strMessage);

            QListWidgetItem *item = new QListWidgetItem();
            item->setData(Qt::UserRole, strDT);
            item->setData(Qt::UserRole+1, strID);
            item->setData(Qt::UserRole+2, strChannel);
            item->setData(Qt::UserRole+3, strNick);
            item->setData(Qt::UserRole+4, strMessage);
            item->setText(strData);

            ui.listWidget_msg->insertItem(ui.listWidget_msg->count()-1, item);
        }
    }
}

void DlgModeration::set_current_channel(QString strName)
{
    // set combobox
    ui.comboBox_channels->setCurrentIndex(ui.comboBox_channels->findText(strName));

    // combo changed
    combo_changed(strName);
}

void DlgModeration::add_msg(QString strID, QString strChannel, QString strNick, QString strMessage)
{
    if (ui.comboBox_channels->findText(strChannel) == -1)
        ui.comboBox_channels->addItem(strChannel);

    QDateTime dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("hh:mm:ss");

    ModerateMsg addItem;
    addItem.datetime = strDT;
    addItem.id = strID;
    addItem.channel = strChannel;
    addItem.nick = strNick;
    addItem.message = strMessage;

    mModerateMessages.append(addItem);

    // active window - display
    if (!this->isHidden())
    {
        QString strData = QString("[%1] <%2> %3").arg(strDT).arg(strNick).arg(strMessage);

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, strDT);
        item->setData(Qt::UserRole+1, strID);
        item->setData(Qt::UserRole+2, strChannel);
        item->setData(Qt::UserRole+3, strNick);
        item->setData(Qt::UserRole+4, strMessage);
        item->setText(strData);

        ui.listWidget_msg->insertItem(ui.listWidget_msg->count()-1, item);
    }
}

void DlgModeration::combo_changed(QString strComboName)
{
    ui.listWidget_msg->clear();
    strCurrentChannel = strComboName;

    refresh();
}

void DlgModeration::remove_selected()
{
    QList<QListWidgetItem*> list = ui.listWidget_msg->selectedItems();

    // remove from display
    for (int i = 0; i < list.size(); i++)
    {
        QListWidgetItem *item = list.at(i);
        int row = ui.listWidget_msg->row(item);
        ui.listWidget_msg->takeItem(row);
    }

    // remove from list
    for (int i = 0; i < list.size(); i++)
    {
        QListWidgetItem *item = list.at(i);
        QString strID = item->data(Qt::UserRole+1).toString();

        for (int x = 0; x < mModerateMessages.size(); x++)
        {
            if (strID == mModerateMessages.at(x).id)
            {
                mModerateMessages.takeAt(x);
                break;
            }
        }

    }
}

void DlgModeration::button_accept()
{
    QList<QListWidgetItem*> list = ui.listWidget_msg->selectedItems();

    // display
    for (int i = 0; i < list.size(); i++)
    {
        QListWidgetItem *item = list.at(i);
        QString strChannel = item->data(Qt::UserRole+2).toString();
        QString strNick = item->data(Qt::UserRole+3).toString();
        QString strMessage = item->data(Qt::UserRole+4).toString();

        QString strSend = QString("MODERMSG %1 - %2 :%3").arg(strNick).arg(strChannel).arg(strMessage);
        emit send(strSend);

        QString strDisplay = QString("<%1> %2").arg(strNick).arg(strMessage);
        emit display_msg(strChannel, strDisplay, DefaultMessage);
    }

    // remove
    remove_selected();
}

void DlgModeration::button_remove()
{
    // remove
    remove_selected();
}

void DlgModeration::showEvent(QShowEvent *event)
{
    event->accept();

    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

void DlgModeration::hideEvent(QHideEvent *event)
{
    event->accept();

    ui.comboBox_channels->setCurrentIndex(-1);
    ui.listWidget_msg->clear();
    strCurrentChannel.clear();
}

void DlgModeration::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
