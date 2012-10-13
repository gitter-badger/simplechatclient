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
#include <QInputDialog>
#include <QMenu>
#include <QTimer>
#include "core.h"
#include "channel.h"
#include "friends_model.h"
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
    ui.toolButton_options->setEnabled(false);

    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/list-add-user.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove-user.png"));
    ui.toolButton_options->setIcon(QIcon(":/images/oxygen/16x16/applications-system.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.toolButton_options->setText(tr("Options"));

    QMenu *optionsMenu = new QMenu(this);
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/list-add-user.png"), tr("Priv"), this, SLOT(priv()));
    optionsMenu->addAction(QIcon(":/images/oxygen/16x16/user-properties.png"), tr("Whois"), this, SLOT(whois()));

    QMenu *mInvite = new QMenu(tr("Invite"));
    mInvite->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));

    QList<QString> lChannelsCleared = Channel::instance()->getCleared();
    for (int i = 0; i < lChannelsCleared.size(); ++i)
    {
        QString strOpenChannel = lChannelsCleared[i];
        if (strOpenChannel[0] == '^')
            strOpenChannel = Channel::instance()->getPriv(strOpenChannel);

        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
        openChannelsActs[i]->setVisible(false);
        openChannelsActs[i]->setText(strOpenChannel);
        openChannelsActs[i]->setData(lChannelsCleared[i]);
        openChannelsActs[i]->setVisible(true);

        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
        mInvite->addAction(openChannelsActs[i]);
    }
    optionsMenu->addMenu(mInvite);

    ui.toolButton_options->setMenu(optionsMenu);
}

void DlgFriends::createSignals()
{
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(ui.listWidget_online, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
    connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(buttonAdd()));
    connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgFriends::refresh()
{
    ui.listWidget_online->clear();
    ui.listWidget_offline->clear();

    QHashIterator <QString, bool> i(FriendsModel::instance()->getAll());
    while (i.hasNext())
    {
        i.next();
        SortedListWidgetItem *item = new SortedListWidgetItem();
        item->setData(SortedListWidgetNicklistRole, false); // is nicklist
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
    // online - show options
    if ((index == 0) && (ui.toolButton_options->isHidden()))
        ui.toolButton_options->setHidden(false);
    // offline - hide options
    else if ((index == 1) && (!ui.toolButton_options->isHidden()))
        ui.toolButton_options->setHidden(true);
}

void DlgFriends::itemClicked(QListWidgetItem *)
{
    if (!ui.toolButton_options->isEnabled())
        ui.toolButton_options->setEnabled(true);
}

void DlgFriends::buttonAdd()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing your friends list"), tr("Enter a nickname to be added:"), QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->network->send(QString("NS FRIENDS ADD %1").arg(strText));
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
        Core::instance()->network->send(QString("NS FRIENDS DEL %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgFriends::priv()
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
        Core::instance()->network->send(QString("PRIV %1").arg(strSelected));
}

void DlgFriends::whois()
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
        Core::instance()->network->send(QString("WHOIS %1 :%1").arg(strSelected));
}

void DlgFriends::invite()
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
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            QString strInviteChannel = action->data().toString();
            Core::instance()->network->send(QString("INVITE %1 %2").arg(strSelected, strInviteChannel));
        }
    }
}
