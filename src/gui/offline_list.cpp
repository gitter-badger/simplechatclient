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
#include "offline.h"
#include "offline_list.h"

DlgOfflineList::DlgOfflineList(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Offline messages"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgOfflineList::createGui()
{
    ui.pushButton_accept->setEnabled(false);
    ui.pushButton_reject->setEnabled(false);

    ui.pushButton_accept->setIcon(QIcon(":/images/oxygen/16x16/mail-mark-read.png"));
    ui.pushButton_reject->setIcon(QIcon(":/images/oxygen/16x16/mail-mark-junk.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_accept->setText(tr("Read"));
    ui.pushButton_reject->setText(tr("Reject"));
}

void DlgOfflineList::createSignals()
{
    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(buttonRead()));
    connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(buttonReject()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgOfflineList::itemClicked(QListWidgetItem *)
{
    if (!ui.pushButton_accept->isEnabled())
        ui.pushButton_accept->setEnabled(true);

    if (!ui.pushButton_reject->isEnabled())
        ui.pushButton_reject->setEnabled(true);
}

void DlgOfflineList::refresh()
{
    ui.listWidget->clear();

    QList<QString> list = Offline::instance()->getNicks();
    foreach (const QString &strOfflineNick, list)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(OfflineMessageNickRole, strOfflineNick);
        item->setText(QString(tr("Offline message from %1")).arg(strOfflineNick));

        ui.listWidget->addItem(item);
    }
}

void DlgOfflineList::buttonRead()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strNick = item->data(OfflineMessageNickRole).toString();

    Offline::instance()->removeNick(strNick);
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->network->send(QString("NS OFFLINE GET %1").arg(strNick));

    // dialog
    //DlgOfflineMessage(strNick, this).exec();
}

void DlgOfflineList::buttonReject()
{
    if (ui.listWidget->selectedItems().isEmpty())
        return;

    QListWidgetItem *item = ui.listWidget->selectedItems().at(0);
    QString strNick = item->data(OfflineMessageNickRole).toString();

    Offline::instance()->removeNick(strNick);
    ui.listWidget->takeItem(ui.listWidget->row(item));

    Core::instance()->network->send(QString("NS OFFLINE REJECT %1").arg(strNick));
}
