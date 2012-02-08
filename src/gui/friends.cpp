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
#include "friends.h"

DlgFriends::DlgFriends(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Friends list"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgFriends::createGui()
{
    ui.pushButton_whois->setEnabled(false);

    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/list-add-user.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove-user.png"));
    ui.pushButton_whois->setIcon(QIcon(":/images/oxygen/16x16/user-properties.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.pushButton_whois->setText(tr("Whois"));
}

void DlgFriends::createSignals()
{
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(ui.listWidget_online, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(buttonAdd()));
    connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    connect(ui.pushButton_whois, SIGNAL(clicked()), this, SLOT(buttonWhois()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgFriends::refresh()
{
    ui.listWidget_online->clear();
    ui.listWidget_offline->clear();

    QHashIterator <QString, bool> i(Core::instance()->mFriends);
    while (i.hasNext())
    {
        i.next();
        SortedListWidgetItem *item = new SortedListWidgetItem();
        item->setData(Qt::UserRole+10, false); // is nicklist
        item->setText(i.key());
        item->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending.png"));

        if (i.value())
            ui.listWidget_online->addItem(item);
        else
            ui.listWidget_offline->addItem(item);
    }
}

void DlgFriends::tabChanged(int index)
{
    // online - show whois
    if ((index == 0) && (ui.pushButton_whois->isHidden()))
        ui.pushButton_whois->setHidden(false);
    // offline - hide whois
    else if ((index == 1) && (!ui.pushButton_whois->isHidden()))
        ui.pushButton_whois->setHidden(true);
}

void DlgFriends::itemClicked(QListWidgetItem *)
{
    if (!ui.pushButton_whois->isEnabled())
        ui.pushButton_whois->setEnabled(true);
}

void DlgFriends::buttonAdd()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing your friends list"), tr("Enter a nickname to be added:"), QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("NS FRIENDS ADD %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgFriends::buttonRemove()
{
    QString strSelected;
    if (ui.tabWidget->currentIndex() == 0)
    {
        if (ui.listWidget_online->selectedItems().size() != 0)
            strSelected = ui.listWidget_online->selectedItems().at(0)->text();
    }
    else if (ui.tabWidget->currentIndex() == 1)
    {
        if (ui.listWidget_offline->selectedItems().size() != 0)
            strSelected = ui.listWidget_offline->selectedItems().at(0)->text();
    }

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing your friends list"), tr("Enter a nickname for removal:"), QLineEdit::Normal, strSelected, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("NS FRIENDS DEL %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgFriends::buttonWhois()
{
    QString strSelected;
    if (ui.tabWidget->currentIndex() == 0)
    {
        if (ui.listWidget_online->selectedItems().size() != 0)
            strSelected = ui.listWidget_online->selectedItems().at(0)->text();
    }
    else if (ui.tabWidget->currentIndex() == 1)
    {
        if (ui.listWidget_offline->selectedItems().size() != 0)
            strSelected = ui.listWidget_offline->selectedItems().at(0)->text();
    }

    if (!strSelected.isEmpty())
        Core::instance()->pNetwork->send(QString("WHOIS %1 :%1").arg(strSelected));
}
