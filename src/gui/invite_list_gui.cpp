/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include <QMenu>
#include "core.h"
#include "invite.h"
#include "invite_list_gui.h"

InviteListGui::InviteListGui(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Invitations"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();
    createList();
}

void InviteListGui::createGui()
{
    ui.pushButton_accept->setEnabled(false);
    ui.pushButton_reject->setEnabled(false);
    ui.pushButton_ignore->setEnabled(false);
    ui.toolButton_options->setEnabled(false);

    ui.pushButton_accept->setIcon(QIcon(":/images/oxygen/16x16/user-online.png"));
    ui.pushButton_reject->setIcon(QIcon(":/images/oxygen/16x16/user-invisible.png"));
    ui.pushButton_ignore->setIcon(QIcon(":/images/oxygen/16x16/user-busy.png"));
    ui.toolButton_options->setIcon(QIcon(":/images/oxygen/16x16/applications-system.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_accept->setText(tr("Accept"));
    ui.pushButton_reject->setText(tr("Reject"));
    ui.pushButton_ignore->setText(tr("Ignore"));

    ui.toolButton_options->setText(tr("Options"));

    QMenu *optionsMenu = new QMenu(this);
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/user-properties.png"), tr("Whois"), this, SLOT(whois()));

    ui.toolButton_options->setMenu(optionsMenu);
}

void InviteListGui::createSignals()
{
    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(buttonReject()));
    connect(ui.pushButton_ignore, SIGNAL(clicked()), this, SLOT(buttonIgnore()));
    connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(buttonAccept()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void InviteListGui::createList()
{
    QList<OnetInvite> lOnetInvite = Invite::instance()->get();
    foreach (const OnetInvite &oInvite, lOnetInvite)
    {
        QString strId = oInvite.id;
        qint64 iDateTime = oInvite.datetime;
        QString strNick = oInvite.nick;
        QString strChannel = oInvite.channel;

        QListWidgetItem *item = new QListWidgetItem;
        item->setData(InviteIdRole, strId);
        item->setData(InviteDateTimeRole, iDateTime);
        item->setData(InviteNickRole, strNick);
        item->setData(InviteChannelRole, strChannel);

        if (strChannel.at(0) == '^')
            item->setText(QString(tr("%1 %2 invites you to priv")).arg(QDateTime::fromMSecsSinceEpoch(iDateTime).toString("[hh:mm:ss]"), strNick));
        else
            item->setText(QString(tr("%1 %2 invites you to channel %3")).arg(QDateTime::fromMSecsSinceEpoch(iDateTime).toString("[hh:mm:ss]"), strNick, strChannel));

        ui.listWidget->addItem(item);
    }
}

void InviteListGui::itemClicked(QListWidgetItem *)
{
    if (!ui.pushButton_accept->isEnabled())
        ui.pushButton_accept->setEnabled(true);

    if (!ui.pushButton_reject->isEnabled())
        ui.pushButton_reject->setEnabled(true);

    if (!ui.pushButton_ignore->isEnabled())
        ui.pushButton_ignore->setEnabled(true);

    if (!ui.toolButton_options->isEnabled())
        ui.toolButton_options->setEnabled(true);
}

void InviteListGui::buttonAccept()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strId = item->data(InviteIdRole).toString();
    QString strChannel = item->data(InviteChannelRole).toString();
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->network->send(QString("JOIN %1").arg(strChannel));
    Invite::instance()->remove(strId);
}

void InviteListGui::buttonReject()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strId = item->data(InviteIdRole).toString();
    QString strNick = item->data(InviteNickRole).toString();
    QString strChannel = item->data(InviteChannelRole).toString();
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->network->send(QString("INVREJECT %1 %2").arg(strNick, strChannel));
    Invite::instance()->remove(strId);
}

void InviteListGui::buttonIgnore()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strId = item->data(InviteIdRole).toString();
    QString strNick = item->data(InviteNickRole).toString();
    QString strChannel = item->data(InviteChannelRole).toString();
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->network->send(QString("INVIGNORE %1 %2").arg(strNick, strChannel));
    Invite::instance()->remove(strId);
}

void InviteListGui::whois()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strNick = item->data(InviteNickRole).toString();

    Core::instance()->network->send(QString("WHOIS %1 %1").arg(strNick));
}
