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

#include "qnicklist.h"

Nicklist::Nicklist(Network *param1, QSettings *param2, QString param3, QMap <QString, QByteArray> *param4)
{
    pNetwork = param1;
    settings = param2;
    strChannel = param3;
    mNickAvatar = param4;
}

Nicklist::~Nicklist()
{
    strOpenChannels.clear();
}

void Nicklist::set_open_channels(QStringList param1)
{
    strOpenChannels = param1;
}

void Nicklist::priv()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("PRIV %1").arg(strNick));
}

void Nicklist::whois()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("WHOIS %1 %1").arg(strNick));
}

void Nicklist::cam()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    (new DlgCam(pNetwork, settings, strNick))->show();
}

void Nicklist::friends_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void Nicklist::friends_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void Nicklist::ignore_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void Nicklist::ignore_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void Nicklist::kick()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    (new DlgKick(pNetwork, settings, strNick, strChannel))->show();
}

void Nicklist::ban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::kban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    QString strReason = tr("No reason");
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
    pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strReason));
}

void Nicklist::op_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::op_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::halfop_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::halfop_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::moderator_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::moderator_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::voice_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::voice_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::invite()
{
    if (this->selectedItems().count() == 0) return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
        pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

// nicklist

void Nicklist::nicklist_add(QString strNick, QString strStatus, int iRefresh, sNickStatus *nick_status)
{
// add
    NickStatus add;
    add.nick = strNick;
    add.status = strStatus;

    nick_status->append(add);

// return if dont refresh
    if (iRefresh == 0)
        return;
    else
        nicklist_refresh(nick_status);
}

void Nicklist::nicklist_remove(QString strNick, sNickStatus *nick_status)
{
    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus check = nick_status->at(i);
        if (check.nick == strNick)
            nick_status->removeAt(i);
    }

    for (int i = 0; i < this->count(); i++)
    {
        if (this->item(i)->data(Qt::UserRole).toString() == strNick)
            this->takeItem(i);
    }
}

bool Nicklist::nicklist_exist(QString strNick, sNickStatus *nick_status)
{
    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus check = nick_status->at(i);
        if (check.nick == strNick)
            return true;
    }
    return false;
}

void Nicklist::nicklist_clear(sNickStatus *nick_status)
{
    nick_status->clear();
}

void Nicklist::nicklist_sort(sNickStatus *nick_status)
{
    sNickStatus nick_status_sorted;

    nicklist_quicksort("admin", nick_status, &nick_status_sorted);
    nicklist_quicksort("owner", nick_status, &nick_status_sorted);
    nicklist_quicksort("op", nick_status, &nick_status_sorted);
    nicklist_quicksort("halfop", nick_status, &nick_status_sorted);
    nicklist_quicksort("mod", nick_status, &nick_status_sorted);
    nicklist_quicksort("vip", nick_status, &nick_status_sorted);
    nicklist_quicksort("user", nick_status, &nick_status_sorted);

// clear
    nick_status->clear();

// copy
    for (int i = 0; i < nick_status_sorted.count(); i++)
        nick_status->enqueue(nick_status_sorted.at(i));
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

void Nicklist::nicklist_quicksort(QString strStatus, sNickStatus *nick_status, sNickStatus *nick_status_sorted)
{
// create hash from nick_status by status
    QHash <QString, QString> hNick_status;

    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus listNickStatus(nick_status->at(i));

        QString strKey = listNickStatus.nick;
        QString strValue = listNickStatus.status;

        if ((strValue == strStatus) || (strValue == strStatus+"cam"))
            hNick_status.insert(strKey, strValue);
    }

// change hash to QStringList
    QStringList slNicklist;

    QHash <QString, QString>::const_iterator i2 = hNick_status.constBegin();
    while (i2 != hNick_status.constEnd())
    {
        slNicklist.insert(slNicklist.count(), i2.key());
        ++i2;
    }

// sort slNicklist
    qStableSort(slNicklist.begin(), slNicklist.end(), caseInsensitiveLessThan);

// insert
    for (int i = 0; i < slNicklist.count(); i++)
    {
        QString strNick = slNicklist.at(i);

        NickStatus insert;
        insert.nick = strNick;
        insert.status = hNick_status[strNick];

        nick_status_sorted->enqueue(insert);
    }
}

void Nicklist::nicklist_refresh(sNickStatus *nick_status)
{
// get selected item

    QString strCurrentItemSelected;

    QList <QListWidgetItem*> lSelectedItems = this->selectedItems();
    if (lSelectedItems.isEmpty() == false)
        strCurrentItemSelected = this->currentItem()->data(Qt::UserRole).toString();

// clear

    this->clear();

    nicklist_sort(nick_status);

    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus listNickStatus(nick_status->at(i));

        QString strNick = listNickStatus.nick;
        QString strStatus = listNickStatus.status;
        QPixmap icon;

        bool bCam = false;
        if (strStatus.indexOf("cam") != -1)
        {
            strStatus.replace("cam","");
            bCam = true;
        }

        if (strStatus == "admin") icon = QPixmap(":/images/admin.png");
        else if (strStatus == "owner") icon = QPixmap(":/images/owner.png");
        else if (strStatus == "op") icon = QPixmap(":/images/op.png");
        else if (strStatus == "halfop") icon = QPixmap(":/images/halfop.png");
        else if (strStatus == "mod") icon = QPixmap(":/images/mod.png");
        else if (strStatus == "vip") icon = QPixmap(":/images/vip.png");

        QListWidgetItem *item = new QListWidgetItem(this);
        item->setData(Qt::UserRole, strNick);
        item->setData(Qt::DecorationRole, icon);
        if (bCam == true)
            item->setData(Qt::UserRole+1, QPixmap(":/images/cam.png"));

        // read from cache when refresh
        if (mNickAvatar->contains(strNick) == true)
        {
            QPixmap pixmap;
            pixmap.loadFromData(mNickAvatar->value(strNick));
            item->setData(Qt::UserRole+2, pixmap);
        }

        this->addItem(item);
    }

// set selected item

    if (strCurrentItemSelected.isEmpty() == false)
    {
        for (int i = 0; i < this->count(); i++)
        {
            if (this->item(i)->data(Qt::UserRole).toString() == strCurrentItemSelected)
                this->setCurrentItem(this->item(i));
        }
    }
}

void Nicklist::nicklist_refresh_avatars()
{
    for (int i = 0; i < this->count(); i++)
    {
        QString strNick = this->item(i)->data(Qt::UserRole).toString();

        if (this->item(i)->data(Qt::UserRole+2).isNull() == true)
        {
            if (mNickAvatar->contains(strNick) == true)
            {
                QPixmap pixmap;
                pixmap.loadFromData(mNickAvatar->value(strNick));
                this->item(i)->setData(Qt::UserRole+2, pixmap);
            }
        }
    }
}

void Nicklist::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *minvite = new QMenu(tr("Invite"));

    for (int i = 0; i < maxOpenChannels; ++i)
    {
        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setVisible(false);
        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
     }

    for (int i = 0; i < maxOpenChannels; ++i)
        minvite->addAction(openChannelsActs[i]);

    for (int i = 0; i < strOpenChannels.count(); ++i)
    {
        openChannelsActs[i]->setText(strOpenChannels[i]);
        openChannelsActs[i]->setData(strOpenChannels[i]);
        openChannelsActs[i]->setVisible(true);
    }
    for (int j = strOpenChannels.count(); j < maxOpenChannels; ++j)
        openChannelsActs[j]->setVisible(false);

    QMenu *friends = new QMenu(tr("Friends list"));
    friends->addAction(tr("Add to friends"), this, SLOT(friends_add()));
    friends->addAction(tr("Remove from friends"), this, SLOT(friends_del()));

    QMenu *ignore = new QMenu(tr("Ignore list"));
    ignore->addAction(tr("Add to Ignore list"), this, SLOT(ignore_add()));
    ignore->addAction(tr("Remove from Ignore list"), this, SLOT(ignore_del()));

    QMenu *privilege = new QMenu(tr("Actions"));
    privilege->addAction(tr("Give super operator status"), this, SLOT(op_add()));
    privilege->addAction(tr("Take super operator status"), this, SLOT(op_del()));
    privilege->addSeparator();
    privilege->addAction(tr("Give operator status"), this, SLOT(halfop_add()));
    privilege->addAction(tr("Take operator status"), this, SLOT(halfop_del()));
    privilege->addSeparator();
    privilege->addAction(tr("Give moderator status"), this, SLOT(moderator_add()));
    privilege->addAction(tr("Take moderator status"), this, SLOT(moderator_del()));
    privilege->addSeparator();
    privilege->addAction(tr("Give guest status"), this, SLOT(voice_add()));
    privilege->addAction(tr("Take guest status"), this, SLOT(voice_del()));

    QMenu *menu = new QMenu(this);

    menu->addAction(tr("Priv"), this, SLOT(priv()));
    menu->addAction(tr("Whois"), this, SLOT(whois()));
    menu->addAction(tr("Webcam"), this, SLOT(cam()));
    menu->addMenu(minvite);
    menu->addMenu(friends);
    menu->addMenu(ignore);
    menu->addSeparator();
    menu->addAction(tr("Kick From Channel"), this, SLOT(kick()));
    menu->addAction(tr("Ban From Channel"), this, SLOT(ban()));
    menu->addAction(tr("Kick & Ban"), this, SLOT(kban()));
    menu->addSeparator();
    menu->addMenu(privilege);

    menu->popup(e->globalPos());
}
