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

#include "nicklist.h"

Nicklist::Nicklist(QWidget *parent, Network *param1, QSettings *param2, QString param3, QMap <QString, QByteArray> *param4)
{
    myparent = parent;
    pNetwork = param1;
    settings = param2;
    strChannel = param3;
    mNickAvatar = param4;

    QObject::connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(item_selected()));
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

void Nicklist::profile()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();

    if (strNick == sCurrentUserInfo.nick)
    {
        (new DlgUserProfile(myparent, pNetwork, settings, sCurrentUserInfo))->show();
    }
}

void Nicklist::cam()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    (new DlgCam(myparent, pNetwork, settings, strNick))->show();
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
    (new DlgKick(myparent, pNetwork, settings, strNick, strChannel))->show();
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
        {
            nick_status->removeAt(i);
            break;
        }
    }

    for (int i = 0; i < this->count(); i++)
    {
        if (this->item(i)->data(Qt::UserRole).toString() == strNick)
        {
            this->takeItem(i);
            break;
        }
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

QStringList Nicklist::nicklist_get(sNickStatus *nick_status)
{
    QStringList slResult;

    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus listNickStatus(nick_status->at(i));

        QString strKey = listNickStatus.nick;
        slResult.append(strKey);
    }

    return slResult;
}

void Nicklist::nicklist_sort(sNickStatus *nick_status)
{
    sNickStatus nick_status_sorted;

    nicklist_quicksort("dev", nick_status, &nick_status_sorted);
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

        if ((strValue == strStatus) || (strValue == strStatus+"cam") ||  (strValue == strStatus+"busy") || (strValue == strStatus+"cambusy"))
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

// sort
    nicklist_sort(nick_status);

// insert
    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus listNickStatus(nick_status->at(i));

        QString strNick = listNickStatus.nick;
        QString strStatus = listNickStatus.status;
        QPixmap icon;

        bool bCam = false;
        bool bBusy = false;

        if (strStatus.indexOf("cam") != -1)
        {
            strStatus.replace("cam","");
            bCam = true;
        }
        if (strStatus.indexOf("busy") != -1)
        {
            strStatus.replace("busy","");
            bBusy = true;
        }

        if (strStatus == "dev") icon = QPixmap(":/images/dev.png");
        else if (strStatus == "admin") icon = QPixmap(":/images/admin.png");
        else if (strStatus == "owner") icon = QPixmap(":/images/owner.png");
        else if (strStatus == "op") icon = QPixmap(":/images/op.png");
        else if (strStatus == "halfop") icon = QPixmap(":/images/halfop.png");
        else if (strStatus == "mod") icon = QPixmap(":/images/mod.png");
        else if (strStatus == "vip") icon = QPixmap(":/images/vip.png");

        QListWidgetItem *item = new QListWidgetItem(this);
        item->setData(Qt::UserRole+10, bCam);
        item->setData(Qt::UserRole+11, bBusy);
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
            {
                this->setCurrentItem(this->item(i));
                break;
            }
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

void Nicklist::set_user_info(QString strNick, QString strKey, QString strValue)
{
    if (this->selectedItems().count() == 0) return;

    QString strNickSelected = this->selectedItems().at(0)->data(Qt::UserRole).toString();

    if (strNick == strNickSelected)
    {
        sCurrentUserInfo.nick = strNick;

        if (strKey == "avatar")
            sCurrentUserInfo.avatar = strValue;
        else if (strKey == "birthdate")
            sCurrentUserInfo.birthdate = strValue;
        else if (strKey == "city")
            sCurrentUserInfo.city = strValue;
        else if (strKey == "country")
            sCurrentUserInfo.country = strValue;
        else if (strKey == "email")
            sCurrentUserInfo.email = strValue;
        else if (strKey == "longDesc")
            sCurrentUserInfo.longDesc = strValue;
        else if (strKey == "offmsg")
            sCurrentUserInfo.offmsg = strValue;
        else if (strKey == "prefs")
            sCurrentUserInfo.prefs = strValue;
        else if (strKey == "rank")
            sCurrentUserInfo.rank = strValue;
        else if (strKey == "sex")
            sCurrentUserInfo.sex = strValue;
        else if (strKey == "shortDesc")
            sCurrentUserInfo.shortDesc = strValue;
        else if (strKey == "tags")
            sCurrentUserInfo.tags = strValue;
        else if (strKey == "type")
            sCurrentUserInfo.type = strValue;
        else if (strKey == "vEmail")
            sCurrentUserInfo.vEmail = strValue;
        else if (strKey == "www")
            sCurrentUserInfo.www = strValue;
    }
}

void Nicklist::item_selected()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();

    if (strNick[0] != '~')
    {
        // clear user info
        sCurrentUserInfo.avatar = QString::null;
        sCurrentUserInfo.birthdate = QString::null;
        sCurrentUserInfo.city = QString::null;
        sCurrentUserInfo.country = QString::null;
        sCurrentUserInfo.email = QString::null;
        sCurrentUserInfo.longDesc = QString::null;
        sCurrentUserInfo.nick = QString::null;
        sCurrentUserInfo.offmsg = QString::null;
        sCurrentUserInfo.prefs = QString::null;
        sCurrentUserInfo.rank = QString::null;
        sCurrentUserInfo.sex = QString::null;
        sCurrentUserInfo.shortDesc = QString::null;
        sCurrentUserInfo.tags = QString::null;
        sCurrentUserInfo.type = QString::null;
        sCurrentUserInfo.vEmail = QString::null;
        sCurrentUserInfo.www = QString::null;

        // get new user info
        pNetwork->send(QString("NS INFO %1").arg(strNick));
    }
}

void Nicklist::contextMenuEvent(QContextMenuEvent *e)
{
    if (this->selectedItems().count() == 0) return;

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

    QString strNick = this->selectedItems().at(0)->data(Qt::UserRole).toString();
    QAction *nickAct = new QAction(strNick, this);
    nickAct->setDisabled(true);

    QMenu *menu = new QMenu(this);
    menu->addAction(nickAct);
    menu->addSeparator();
    menu->addAction(tr("Priv"), this, SLOT(priv()));
    menu->addAction(tr("Whois"), this, SLOT(whois()));
    if (strNick[0] != '~')
    {
        menu->addAction(tr("Profile"), this, SLOT(profile()));
        menu->addAction(tr("Webcam"), this, SLOT(cam()));
    }
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
