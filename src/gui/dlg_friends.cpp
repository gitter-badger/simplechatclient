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
#include <QSettings>
#include <QShowEvent>
#include <QTimer>
#include "core.h"
#include "network.h"
#include "dlg_friends.h"

DlgFriends::DlgFriends(QWidget *parent, Network *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Friends list"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    pNetwork = param1;

    create_gui();
    create_signals();

    refresh();
}

void DlgFriends::create_gui()
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

void DlgFriends::create_signals()
{
    QObject::connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tab_changed(int)));
    QObject::connect(ui.listWidget_online, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(item_clicked(QListWidgetItem*)));
    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.pushButton_whois, SIGNAL(clicked()), this, SLOT(button_whois()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgFriends::refresh()
{
    ui.listWidget_online->clear();
    ui.listWidget_offline->clear();

    QMap<QString, bool>::const_iterator i = Core::instance()->mFriends.constBegin();
    while (i != Core::instance()->mFriends.constEnd())
    {
        SortedListWidgetItem *item = new SortedListWidgetItem();
        item->setData(Qt::UserRole+11, false); // is nicklist

        if (Core::instance()->mNickAvatar.contains(i.key()))
        {
            QPixmap pixmap;
            pixmap.loadFromData(Core::instance()->mNickAvatar.value(i.key()));

            item->setText(i.key());
            item->setIcon(QIcon(pixmap));
        }
        else
        {
            item->setText(i.key());
            item->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending.png"));

            QSettings settings;
            if (settings.value("disable_avatars").toString() == "off") // with avatars
                pNetwork->send(QString("NS INFO %1 s").arg(i.key()));
        }

        if (i.value())
            ui.listWidget_online->addItem(item);
        else
            ui.listWidget_offline->addItem(item);

        ++i;
    }
}

void DlgFriends::tab_changed(int index)
{
    // online - show whois
    if ((index == 0) && (ui.pushButton_whois->isHidden()))
        ui.pushButton_whois->setHidden(false);
    // offline - hide whois
    else if ((index == 1) && (!ui.pushButton_whois->isHidden()))
        ui.pushButton_whois->setHidden(true);
}

void DlgFriends::item_clicked(QListWidgetItem *item)
{
    Q_UNUSED (item);

    if (!ui.pushButton_whois->isEnabled())
        ui.pushButton_whois->setEnabled(true);
}

void DlgFriends::button_add()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing your friends list"), tr("Enter a nickname to be added:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        pNetwork->send(QString("NS FRIENDS ADD %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgFriends::button_remove()
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

    if ((ok) && (!strText.isEmpty()))
    {
        pNetwork->send(QString("NS FRIENDS DEL %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgFriends::button_whois()
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
        pNetwork->send(QString("WHOIS %1 :%1").arg(strSelected));
}
