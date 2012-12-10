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

#include <QDateTime>
#include <QDesktopWidget>
#include <QTimer>
#include "core.h"
#include "channel.h"
#include "message.h"
#include "settings.h"
#include "moderation.h"

DlgModeration::DlgModeration(const QString &_strCurrentChannel, QWidget *parent) : QDialog(parent), strCurrentChannel(_strCurrentChannel)
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
    QList<OnetModerateMessage> lModerateMessages = Channel::instance()->getModerateMessages(strCurrentChannel);
    int listWidgetCount = ui.listWidget_msg->count();
    int moderateMessagesCount = lModerateMessages.size();

    for (int i = listWidgetCount; i < listWidgetCount + (moderateMessagesCount - listWidgetCount); i++)
    {
        OnetModerateMessage omessage = lModerateMessages.at(i);

        QString strID = omessage.id;
        qint64 iTime = omessage.datetime;
        QString strNick = omessage.nick;
        QString strMessage = omessage.message;

        QString strData = QString("[%1] <%2> %3").arg(QDateTime::fromMSecsSinceEpoch(iTime).toString("hh:mm:ss"), strNick, strMessage);

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(ModerationIdRole, strID);
        item->setData(ModerationChannelRole, strCurrentChannel);
        item->setData(ModerationTimeRole, iTime);
        item->setData(ModerationNickRole, strNick);
        item->setData(ModerationMessageRole, strMessage);
        item->setText(strData);

        ui.listWidget_msg->addItem(item);
    }

    // refresh
    QTimer::singleShot(1000*1, this, SLOT(refreshMessages())); // 1 sec
}

void DlgModeration::removeSelected()
{
    if (ui.listWidget_msg->selectedItems().isEmpty())
        return;

    QList<QListWidgetItem*> list = ui.listWidget_msg->selectedItems();

    // remove from list
    foreach (QListWidgetItem *item, list)
    {
        QString strChannel = item->data(ModerationChannelRole).toString();
        QString strID = item->data(ModerationIdRole).toString();

        Channel::instance()->removeModerateMessage(strChannel, strID);

        ui.listWidget_msg->takeItem(ui.listWidget_msg->row(item));
    }
}

void DlgModeration::buttonAccept()
{
    if (ui.listWidget_msg->selectedItems().isEmpty())
        return;

    QList<QListWidgetItem*> list = ui.listWidget_msg->selectedItems();

    // display
    foreach (QListWidgetItem *item, list)
    {
        QString strChannel = item->data(ModerationChannelRole).toString();
        qint64 iTime = item->data(ModerationTimeRole).toLongLong();
        QString strNick = item->data(ModerationNickRole).toString();
        QString strMessage = item->data(ModerationMessageRole).toString();

        QString strSend = QString("MODERMSG %1 - %2 :%3").arg(strNick, strChannel, strMessage);
        Core::instance()->network->send(strSend);

        QString strMe = Settings::instance()->get("nick");
        QString strDisplay = QString("%1 [%2 %3]").arg(strMessage, tr("Moderated by"), strMe);
        Message::instance()->showMessage(strChannel, strDisplay, MessageDefault, strNick, iTime);
    }

    // remove
    removeSelected();
}

void DlgModeration::buttonRemove()
{
    // remove
    removeSelected();
}
