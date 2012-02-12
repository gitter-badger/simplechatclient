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
#include <QTimer>
#include "core.h"
#include "moderation.h"

DlgModeration::DlgModeration(QWidget *parent, const QString &_strCurrentChannel) : QDialog(parent), strCurrentChannel(_strCurrentChannel)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Moderation"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    // refresh
    QTimer::singleShot(200, this, SLOT(refreshMessages())); // 0.2 sec
}

void DlgModeration::createGui()
{
    ui.pushButton_accept->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_accept->setText(tr("Accept"));
    ui.pushButton_remove->setText(tr("Remove"));

    ui.label_channel->setText(strCurrentChannel);
}

void DlgModeration::createSignals()
{
    connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(buttonAccept()));
    connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgModeration::refreshMessages()
{
    ui.listWidget_msg->clear();

    QHashIterator <QString, ModerateMsg> i(Core::instance()->mModerateMessages);
    while (i.hasNext())
    {
        i.next();

        QString strID = i.key();
        ModerateMsg msg = i.value();
        QString strChannel = msg.channel;
        QString strDT = msg.datetime;
        QString strNick = msg.nick;
        QString strMessage = msg.message;

        if (strChannel == strCurrentChannel)
        {
            QString strData = QString("[%1] <%2> %3").arg(strDT, strNick, strMessage);

            QListWidgetItem *item = new QListWidgetItem();
            item->setData(Qt::UserRole, strID);
            item->setData(Qt::UserRole+1, strChannel);
            item->setData(Qt::UserRole+2, strDT);
            item->setData(Qt::UserRole+3, strNick);
            item->setData(Qt::UserRole+4, strMessage);
            item->setText(strData);

            ui.listWidget_msg->addItem(item);
        }
    }

    // refresh
    QTimer::singleShot(1000*10, this, SLOT(refreshMessages())); // 10 sec
}

void DlgModeration::removeSelected()
{
    if (ui.listWidget_msg->selectedItems().isEmpty())
        return;

    QList<QListWidgetItem*> list = ui.listWidget_msg->selectedItems();

    // remove from list
    for (int i = 0; i < list.size(); i++)
    {
        QListWidgetItem *item = list.at(i);
        QString strID = item->data(Qt::UserRole).toString();

        Core::instance()->mModerateMessages.remove(strID);
    }
}

void DlgModeration::buttonAccept()
{
    if (ui.listWidget_msg->selectedItems().isEmpty())
        return;

    QList<QListWidgetItem*> list = ui.listWidget_msg->selectedItems();

    // display
    for (int i = 0; i < list.size(); i++)
    {
        QListWidgetItem *item = list.at(i);
        QString strChannel = item->data(Qt::UserRole+1).toString();
        QString strNick = item->data(Qt::UserRole+3).toString();
        QString strMessage = item->data(Qt::UserRole+4).toString();

        QString strSend = QString("MODERMSG %1 - %2 :%3").arg(strNick, strChannel, strMessage);
        Core::instance()->pNetwork->send(strSend);

        QString strMe = Core::instance()->settings.value("nick");
        QString strDisplay = QString("%1 [%2 %3]").arg(strMessage, tr("Moderated by"), strMe);
        Core::instance()->showMessage(strChannel, strDisplay, DefaultMessage, QString::null, strNick);
    }

    // remove
    removeSelected();
}

void DlgModeration::buttonRemove()
{
    // remove
    removeSelected();
}
