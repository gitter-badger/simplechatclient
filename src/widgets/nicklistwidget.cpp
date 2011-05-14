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

#include <QContextMenuEvent>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QSettings>
#include "core.h"
#include "defines.h"
#include "dlg_user_profile.h"
#include "network.h"
#include "nicklistwidget.h"

#ifdef Q_WS_WIN
    #include "kamerzysta.h"
#else
    #include "dlg_cam.h"
#endif

NickListWidget::NickListWidget(Network *param1, QTcpSocket *param2, DlgUserProfile *param3)
{
    pNetwork = param1;
    camSocket = param2;
    pDlgUserProfile = param3;

    setSortingEnabled(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

NickListWidget::~NickListWidget()
{
}

#ifndef Q_WS_WIN
void NickListWidget::set_dlg_cam(DlgCam *param1)
{
    pDlgCam = param1;
}
#endif

void NickListWidget::set_channel(QString param1)
{
    strChannel = param1;
}

void NickListWidget::add(QString strNick)
{
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setText(strNick);
    item->setData(Qt::UserRole+10, strChannel);
    item->setData(Qt::UserRole+11, true); // is nicklist
    item->setData(Qt::UserRole+12, Core::instance()->get_user_max_modes(strNick, strChannel));

    this->addItem(item);
}

void NickListWidget::remove(QString strNick)
{
    for (int i = 0; i < this->count(); i++)
    {
        if (this->item(i)->text() == strNick)
        {
            this->takeItem(i);
            break;
        }
    }
}

void NickListWidget::priv()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("PRIV %1").arg(strNick));
}

void NickListWidget::whois()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("WHOIS %1 :%1").arg(strNick));
}

void NickListWidget::profile()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();

    if (strNick[0] != '~')
    {
        pDlgUserProfile->set_nick(strNick);
        pDlgUserProfile->show();
    }
}

void NickListWidget::cam()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
#ifdef Q_WS_WIN
    (new Kamerzysta(camSocket))->show(strNick);
#else
    pDlgCam->set_nick(strNick);
    pDlgCam->show();
#endif
}

void NickListWidget::friends_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void NickListWidget::friends_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void NickListWidget::ignore_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void NickListWidget::ignore_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void NickListWidget::kick()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
}

void NickListWidget::ban()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::kban()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick & Ban"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
    }
}

void NickListWidget::ipban()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS BANIP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::op_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::op_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::halfop_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::halfop_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::moderator_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::moderator_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::voice_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::voice_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::invite()
{
    if (this->selectedItems().size() == 0) return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        QString strNick = this->selectedItems().at(0)->text();
        pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

void NickListWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();

    QString strModes = Core::instance()->get_user_modes(strNick, strChannel);

    QMenu *minvite = new QMenu(tr("Invite"));
    minvite->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));

    for (int i = 0; i < maxOpenChannels; ++i)
    {
        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
        openChannelsActs[i]->setVisible(false);
        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
     }

    for (int i = 0; i < maxOpenChannels; ++i)
        minvite->addAction(openChannelsActs[i]);

    QList<QString> lOpenChannels = Core::instance()->lOpenChannels;
    for (int i = 0; i < lOpenChannels.size(); ++i)
    {
        openChannelsActs[i]->setText(lOpenChannels[i]);
        openChannelsActs[i]->setData(lOpenChannels[i]);
        openChannelsActs[i]->setVisible(true);
    }
    for (int j = lOpenChannels.size(); j < maxOpenChannels; ++j)
        openChannelsActs[j]->setVisible(false);

    QMenu *friends = new QMenu(tr("Friends list"));
    friends->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending.png"));
    friends->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to friends"), this, SLOT(friends_add()));
    friends->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from friends"), this, SLOT(friends_del()));

    QMenu *ignore = new QMenu(tr("Ignore list"));
    ignore->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"));
    ignore->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to Ignore list"), this, SLOT(ignore_add()));
    ignore->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from Ignore list"), this, SLOT(ignore_del()));

    QMenu *privilege = new QMenu(tr("Actions"));
    privilege->setIcon(QIcon(":/images/oxygen/16x16/irc-operator.png"));

    if (strModes.contains("@"))
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(op_del()));
    else
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(op_add()));

    if (strModes.contains("%"))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfop_del()));
    else
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfop_add()));

    if (strModes.contains("!"))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderator_del()));
    else
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderator_add()));

    if (strModes.contains("+"))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Take guest status"), this, SLOT(voice_del()));
    else
        privilege->addAction(QIcon(":/images/voice.png"), tr("Give guest status"), this, SLOT(voice_add()));

    QAction *nickAct = new QAction(strNick, this);
    nickAct->setIcon(QIcon(":/images/oxygen/16x16/user-identity.png"));
    nickAct->setDisabled(true);

    QMenu *menu = new QMenu(strNick);
    menu->addAction(nickAct);
    menu->addSeparator();
    menu->addAction(QIcon(":/images/oxygen/16x16/list-add-user.png"), tr("Priv"), this, SLOT(priv()));
    menu->addAction(QIcon(":/images/oxygen/16x16/text-field.png"), tr("Whois"), this, SLOT(whois()));
    if (strNick[0] != '~')
    {
        menu->addAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"), tr("Profile"), this, SLOT(profile()));
        if ((strModes.contains("W")) || (strModes.contains("V")))
            menu->addAction(QIcon(":/images/pubcam.png"), tr("Webcam"), this, SLOT(cam()));
    }
    menu->addMenu(minvite);
    menu->addMenu(friends);
    menu->addMenu(ignore);
    menu->addSeparator();
    menu->addAction(QIcon(":/images/oxygen/16x16/im-kick-user.png"), tr("Kick From Channel"), this, SLOT(kick()));
    menu->addAction(QIcon(":/images/oxygen/16x16/im-ban-user.png"), tr("Ban From Channel"), this, SLOT(ban()));
    menu->addAction(QIcon(":/images/oxygen/16x16/im-ban-kick-user.png"), tr("Kick & Ban"), this, SLOT(kban()));
    menu->addAction(QIcon(":/images/oxygen/16x16/im-user-busy.png"), tr("IP Ban"), this, SLOT(ipban()));
    menu->addSeparator();
    menu->addMenu(privilege);

    menu->popup(e->globalPos());
}
