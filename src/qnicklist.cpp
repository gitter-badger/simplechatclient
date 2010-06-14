/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

Nicklist::Nicklist(Network *param1, QSettings *param2, QString param3)
{
    pNetwork = param1;
    settings = param2;
    strChannel = param3;
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

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("PRIV %1").arg(strNick));
}

void Nicklist::whois()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("WHOIS %1 %1").arg(strNick));
}

void Nicklist::cam()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    (new DlgCam(pNetwork, settings, strNick))->show();
}

void Nicklist::friends_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void Nicklist::friends_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void Nicklist::ignore_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void Nicklist::ignore_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void Nicklist::kick()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    (new DlgKick(pNetwork, settings, strNick, strChannel))->show();
}

void Nicklist::ban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::kban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    QString strReason = "Zachowuj siê! Byle jak ale siê zachowuj!";
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
    pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strReason));
}

void Nicklist::op_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::op_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::halfop_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::halfop_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::moderator_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::moderator_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::voice_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void Nicklist::voice_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void Nicklist::invite()
{
    if (this->selectedItems().count() == 0) return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        QString strNick = this->selectedItems().at(0)->text();
        pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

// nicklist

void Nicklist::nicklist_add(QString strNick, QString strStatus, QHash <QString, QString> *nicklist, QQueue <QString> *new_nicklist1, QQueue <QString> *new_nicklist2)
{
    nicklist->insert(strNick, strStatus);
}

void Nicklist::nicklist_remove(QString strNick, QHash <QString, QString> *nicklist, QQueue <QString> *new_nicklist1, QQueue <QString> *new_nicklist2)
{
    nicklist->remove(strNick);
    nicklist_refresh(nicklist, new_nicklist1, new_nicklist2);
}

bool Nicklist::nicklist_exist(QString strNick, QHash <QString, QString> *nicklist)
{
    return nicklist->contains(strNick);
}

void Nicklist::nicklist_clear(QHash <QString, QString> *nicklist, QQueue <QString> *new_nicklist1, QQueue <QString> *new_nicklist2)
{
    nicklist->clear();
    nicklist_refresh(nicklist, new_nicklist1, new_nicklist2);
}

void Nicklist::nicklist_refresh(QHash <QString, QString> *nicklist, QQueue <QString> *new_nicklist1, QQueue <QString> *new_nicklist2)
{
    this->clear();

    new_nicklist1->clear();
    new_nicklist2->clear();

    nicklist_sort(nicklist, new_nicklist1, new_nicklist2);

    while (!new_nicklist1->isEmpty())
    {
        QString strNick = new_nicklist1->dequeue();
        QString strStatus = new_nicklist2->dequeue();
        QIcon icon;
        //if (strStatus == "admincam") icon = QIcon(":/3rdparty/images/admincam.png");
        //else if (strStatus == "ownercam") icon = QIcon(":/3rdparty/images/ownercam.png");
        //else if (strStatus == "opcam") icon = QIcon(":/3rdparty/images/opcam.png");
        //else if (strStatus == "halfopcam") icon = QIcon(":/3rdparty/images/halfopcam.png");
        //else if (strStatus == "modcam") icon = QIcon(":/3rdparty/images/modcam.png");
        //else if (strStatus == "vipcam") icon = QIcon(":/3rdparty/images/vipcam.png");
        if (strStatus == "usercam") icon = QIcon(":/images/cam.png");
        else if (strStatus == "admin") icon = QIcon(":/images/admin.png");
        else if (strStatus == "owner") icon = QIcon(":/images/owner.png");
        else if (strStatus == "op") icon = QIcon(":/images/op.png");
        else if (strStatus == "halfop") icon = QIcon(":/images/halfop.png");
        else if (strStatus == "mod") icon = QIcon(":/images/mod.png");
        else if (strStatus == "vip") icon = QIcon(":/images/vip.png");
        else if (strStatus == "user") icon = QIcon(":/images/user.png");

        this->addItem(new QListWidgetItem(icon, strNick));
    }
}

void Nicklist::nicklist_sort(QHash <QString, QString> *nicklist, QQueue <QString> *new_nicklist1, QQueue <QString> *new_nicklist2)
{
    nicklist_quicksort("admin", nicklist, new_nicklist1, new_nicklist2);
    nicklist_quicksort("owner", nicklist, new_nicklist1, new_nicklist2);
    nicklist_quicksort("op", nicklist, new_nicklist1, new_nicklist2);
    nicklist_quicksort("halfop", nicklist, new_nicklist1, new_nicklist2);
    nicklist_quicksort("mod", nicklist, new_nicklist1, new_nicklist2);
    nicklist_quicksort("vip", nicklist, new_nicklist1, new_nicklist2);
    nicklist_quicksort("user", nicklist, new_nicklist1, new_nicklist2);
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

void Nicklist::nicklist_quicksort(QString strStatus, QHash <QString, QString> *nicklist, QQueue <QString> *new_nicklist1, QQueue <QString> *new_nicklist2)
{
    QHash <QString, QString> status_nicklist;

    QHash <QString, QString>::const_iterator i1 = nicklist->constBegin();
    while (i1 != nicklist->constEnd())
    {
        if ((i1.value() == strStatus) || (i1.value() == strStatus+"cam"))
            status_nicklist.insert(i1.key(), i1.value());

        ++i1;
    }

    QStringList newlist;

    QHash <QString, QString>::const_iterator i2 = status_nicklist.constBegin();
    while (i2 != status_nicklist.constEnd())
    {
        newlist.insert(newlist.count(), i2.key());
        ++i2;
    }

    qStableSort(newlist.begin(), newlist.end(), caseInsensitiveLessThan);

    for (int i = 0; i < newlist.count(); i++)
    {
        QString strNick = newlist.at(i);
        new_nicklist1->insert(new_nicklist1->count(), strNick);
        new_nicklist2->insert(new_nicklist2->count(), status_nicklist[strNick]);
    }
}

void Nicklist::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *minvite = new QMenu("Zapro¶");

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

    QMenu *friends = new QMenu("Lista przyjació³");
    friends->addAction("Dodaj do listy przyjació³", this, SLOT(friends_add()));
    friends->addAction("Usuñ z listy przyjació³", this, SLOT(friends_del()));

    QMenu *ignore = new QMenu("Lista ignorowanych");
    ignore->addAction("Dodaj do listy ignorowanych", this, SLOT(ignore_add()));
    ignore->addAction("Usuñ z listy ignorowanych", this, SLOT(ignore_del()));

    QMenu *privilege = new QMenu("Uprawnienia");
    privilege->addAction("Nadaj prawa super operatora", this, SLOT(op_add()));
    privilege->addAction("Odbierz prawa super operatora", this, SLOT(op_del()));
    privilege->addSeparator();
    privilege->addAction("Nadaj prawa operatora", this, SLOT(halfop_add()));
    privilege->addAction("Odbierz prawa operatora", this, SLOT(halfop_del()));
    privilege->addSeparator();
    privilege->addAction("Nadaj prawa moderatora", this, SLOT(moderator_add()));
    privilege->addAction("Odbierz prawa moderatora", this, SLOT(moderator_del()));
    privilege->addSeparator();
    privilege->addAction("Nadaj prawa go¶cia", this, SLOT(voice_add()));
    privilege->addAction("Odbierz prawa go¶cia", this, SLOT(voice_del()));

    QMenu *menu = new QMenu(this);

    menu->addAction("Rozmowa prywatna", this, SLOT(priv()));
    menu->addAction("Whois", this, SLOT(whois()));
    menu->addAction("Kamerka", this, SLOT(cam()));
    menu->addMenu(minvite);
    menu->addMenu(friends);
    menu->addMenu(ignore);
    menu->addSeparator();
    menu->addAction("Wyrzuæ", this, SLOT(kick()));
    menu->addAction("Cichy ban", this, SLOT(ban()));
    menu->addAction("Banuj i wyrzuæ", this, SLOT(kban()));
    menu->addSeparator();
    menu->addMenu(privilege);

    menu->popup(e->globalPos());

    QEventLoop loop;
    QObject::connect(menu, SIGNAL(triggered(QAction*)), &loop, SLOT(quit()));
    loop.exec();

    delete menu;
    delete privilege;
    delete ignore;
    delete friends;
    delete minvite;
}
