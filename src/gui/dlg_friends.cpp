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

#include "dlg_friends.h"

DlgFriends::DlgFriends(QWidget *parent, Network *param1, QSettings *param2, QMap <QString, QByteArray> *param3) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Friends list"));

    myparent = parent;
    pNetwork = param1;
    settings = param2;
    mNickAvatar = param3;

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));
    ui.pushButton_whois->setText(tr("Whois"));

    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.pushButton_whois, SIGNAL(clicked()), this, SLOT(button_whois()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgFriends::set_friend(QString strNick, bool bStatus)
{
    friends[strNick] = bStatus;
}

void DlgFriends::remove_friend(QString strNick)
{
    friends.remove(strNick);
}

void DlgFriends::refresh()
{
    ui.listWidget_online->clear();
    ui.listWidget_offline->clear();

    QMap<QString, bool>::const_iterator i = friends.constBegin();
    while (i != friends.constEnd())
    {
        QListWidgetItem *item;

        if (mNickAvatar->contains(i.key()) == true)
        {
            QPixmap pixmap;
            pixmap.loadFromData(mNickAvatar->value(i.key()));
            item = new QListWidgetItem(QIcon(pixmap), i.key());
        }
        else
        {
            item = new QListWidgetItem(QIcon(":/3rdparty/images/people.png"), i.key());
            if (settings->value("disable_avatars").toString() == "off")
                pNetwork->send(QString("NS INFO %1 s").arg(i.key()));
        }

        if (i.value() == true)
            ui.listWidget_online->addItem(item);
        else
            ui.listWidget_offline->addItem(item);

        ++i;
    }
}

void DlgFriends::clear()
{
    friends.clear();
}

void DlgFriends::button_add()
{
    (new DlgFriendsAd(myparent, pNetwork, settings, "add", ""))->show();
}

void DlgFriends::button_remove()
{
    QString strSelected;
    if (ui.tabWidget->currentIndex() == 0)
    {
        if (ui.listWidget_online->selectedItems().count() != 0)
            strSelected = ui.listWidget_online->selectedItems().at(0)->text();
    }
    else if (ui.tabWidget->currentIndex() == 1)
    {
        if (ui.listWidget_offline->selectedItems().count() != 0)
            strSelected = ui.listWidget_offline->selectedItems().at(0)->text();
    }

    (new DlgFriendsAd(myparent, pNetwork, settings, "remove", strSelected))->show();
}

void DlgFriends::button_whois()
{
    QString strSelected;
    if (ui.tabWidget->currentIndex() == 0)
    {
        if (ui.listWidget_online->selectedItems().count() != 0)
            strSelected = ui.listWidget_online->selectedItems().at(0)->text();
    }
    else if (ui.tabWidget->currentIndex() == 1)
    {
        if (ui.listWidget_offline->selectedItems().count() != 0)
            strSelected = ui.listWidget_offline->selectedItems().at(0)->text();
    }

    if (strSelected.isEmpty() == false)
        pNetwork->send(QString("WHOIS %1 :%1").arg(strSelected));
}

void DlgFriends::button_ok()
{
    this->hide();
}

void DlgFriends::button_cancel()
{
    this->hide();
}

void DlgFriends::showEvent(QShowEvent *event)
{
    event->accept();

    refresh();
}

void DlgFriends::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
