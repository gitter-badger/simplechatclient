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
#include "core.h"
#include "invite_model.h"
#include "invite_list.h"

DlgInviteList::DlgInviteList(QWidget *parent) : QDialog(parent)
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

void DlgInviteList::createGui()
{
    ui.pushButton_whois->setIcon(QIcon(":/images/oxygen/16x16/user-properties.png"));
    ui.pushButton_reject->setIcon(QIcon(":/images/oxygen/16x16/user-invisible.png"));
    ui.pushButton_ignore->setIcon(QIcon(":/images/oxygen/16x16/user-busy.png"));
    ui.pushButton_accept->setIcon(QIcon(":/images/oxygen/16x16/user-online.png"));

    ui.pushButton_whois->setText(tr("Whois"));
    ui.pushButton_reject->setText(tr("Reject"));
    ui.pushButton_ignore->setText(tr("Ignore"));
    ui.pushButton_accept->setText(tr("Accept"));
}

void DlgInviteList::createSignals()
{
    connect(ui.pushButton_whois, SIGNAL(clicked()), this, SLOT(buttonWhois()));
    connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(buttonReject()));
    connect(ui.pushButton_ignore, SIGNAL(clicked()), this, SLOT(buttonIgnore()));
    connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(buttonAccept()));
}

void DlgInviteList::createList()
{
    QHashIterator<QString, QString> i(Invite::instance()->get());
    while (i.hasNext())
    {
        i.next();
        QString strNick = i.key();
        QString strChannel = i.value();

        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::UserRole, strNick);
        item->setData(Qt::UserRole+1, strChannel);

        if (strChannel[0] == '^')
            item->setText(QString(tr("%1 invites you to priv")).arg(strNick));
        else
            item->setText(QString(tr("%1 invites you to channel %2")).arg(strNick, strChannel));

        ui.listWidget->addItem(item);
    }
}

void DlgInviteList::buttonWhois()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strNick = item->data(Qt::UserRole).toString();

    Core::instance()->pNetwork->send(QString("WHOIS %1 %1").arg(strNick));
}

void DlgInviteList::buttonReject()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strNick = item->data(Qt::UserRole).toString();
    QString strChannel = item->data(Qt::UserRole+1).toString();
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->pNetwork->send(QString("INVREJECT %1 %2").arg(strNick, strChannel));
    Invite::instance()->remove(strNick, strChannel);
}

void DlgInviteList::buttonIgnore()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strNick = item->data(Qt::UserRole).toString();
    QString strChannel = item->data(Qt::UserRole+1).toString();
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->pNetwork->send(QString("INVIGNORE %1 %2").arg(strNick, strChannel));
    Invite::instance()->remove(strNick, strChannel);
}

void DlgInviteList::buttonAccept()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strNick = item->data(Qt::UserRole).toString();
    QString strChannel = item->data(Qt::UserRole+1).toString();
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
    Invite::instance()->remove(strNick, strChannel);
}
