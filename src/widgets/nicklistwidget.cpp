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
#include <QMenu>
#include <QSettings>
#include "core.h"
#include "dlg_user_profile.h"
#include "network.h"
#include "nicklistwidget.h"

#ifdef Q_WS_WIN
    #include "kamerzysta.h"
#else
    #include "dlg_cam.h"
#endif

#ifdef Q_WS_X11
#include <QDebug>
#endif

NickListWidget::NickListWidget(Network *param1, QTcpSocket *param2, DlgUserProfile *param3)
{
    pNetwork = param1;
    camSocket = param2;
    pDlgUserProfile = param3;

    header()->hide();
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

// nicklist

// owner      `
// op         @
// halfop     %
// mod        !
// screener   =
// voice      +
// ?          &
// busy       b
// registered r
// encrypted  x
// publiccam  W
// privcam    V
// admin      o
// developer  O

void NickListWidget::add(QString strNick, QString strModes)
{
    // add child to widget
    add_child(strNick, strModes);
}

void NickListWidget::remove(QString strNick)
{
    // remove from widget
    remove_child(strNick);
}

void NickListWidget::update_avatar(QString strNick)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        for (int x = 0; x < this->topLevelItem(i)->childCount(); x++)
        {
            QString strChild = this->topLevelItem(i)->child(x)->text(0);

            if ((strChild[0] != '~') && (strChild == strNick) && (Core::instance()->mNickAvatar.contains(strChild)))
            {
                QPixmap pixmap;
                pixmap.loadFromData(Core::instance()->mNickAvatar.value(strChild));
                this->topLevelItem(i)->child(x)->setData(0, Qt::UserRole+1, pixmap);
            }
        }
    }
}

void NickListWidget::add_parent(QString strName, QPixmap px)
{
    SortedTreeWidgetItem *item = new SortedTreeWidgetItem(this);
    item->setText(0, strName);
    item->setData(0, Qt::UserRole, px);
    item->setData(0, Qt::UserRole+20, "parent");

    QList<QString> lNames;
    lNames << tr("Developer(s)") << tr("Admin(s)") << tr("Owner(s)") << tr("Op(s)") << tr("HalfOp(s)") << tr("Mod(s)") << tr("Screener(s)") << tr("Voice(s)") << tr("Cam(s)") << tr("User(s)");

    int iId;
    for (int i = 0; i < lNames.size(); i++)
    {
        if (lNames.at(i) == strName)
        {
            iId = i;
            break;
        }
    }

    item->setData(0, Qt::UserRole+21, iId);
}

bool NickListWidget::exist_parent(QString strName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QString strText = this->topLevelItem(i)->text(0);
        if (strText == strName)
            return true;
    }
    return false;
}

void NickListWidget::remove_parent(QString strName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QString strText = this->topLevelItem(i)->text(0);
        if (strText == strName)
        {
            delete this->takeTopLevelItem(i);
            i--;
        }
    }
}

void NickListWidget::add_child(QString strNick, QString strModes)
{
    // add to widget
    if (strModes.contains("O"))
    {
        QPixmap icon = QPixmap(":/images/dev.png");
        QString strParent = tr("Developer(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if (strModes.contains("o"))
    {
        QPixmap icon = QPixmap(":/images/admin.png");
        QString strParent = tr("Admin(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if (strModes.contains("`"))
    {
        QPixmap icon = QPixmap(":/images/owner.png");
        QString strParent = tr("Owner(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if (strModes.contains("@"))
    {
        QPixmap icon = QPixmap(":/images/op.png");
        QString strParent = tr("Op(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if (strModes.contains("%"))
    {
        QPixmap icon = QPixmap(":/images/halfop.png");
        QString strParent = tr("HalfOp(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if (strModes.contains("!"))
    {
        QPixmap icon = QPixmap(":/images/mod.png");
        QString strParent = tr("Mod(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if (strModes.contains("="))
    {
        QPixmap icon = QPixmap(":/images/screener.png");
        QString strParent = tr("Screener(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if (strModes.contains("+"))
    {
        QPixmap icon = QPixmap(":/images/voice.png");
        QString strParent = tr("Voice(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if ((strModes.contains("W")) || (strModes.contains("V")))
    {
        QPixmap icon;
        if (strModes.contains("W"))
            icon = QPixmap(":/images/pubcam.png");
        else if (strModes.contains("V"))
            icon = QPixmap(":/images/privcam.png");

        QString strParent = tr("Cam(s)");
        add_child(strNick, strModes, strParent, icon);
    }
    if ((!strModes.contains("O")) && (!strModes.contains("o")) && (!strModes.contains("`")) && (!strModes.contains("@")) && (!strModes.contains("%")) && (!strModes.contains("!")) && (!strModes.contains("=")) && (!strModes.contains("+")) && (!strModes.contains("W")) && (!strModes.contains("V")))
    {
        QPixmap icon = QPixmap(":/images/user.png");
        QString strParent = tr("User(s)");
        add_child(strNick, strModes, strParent, icon);
    }
}

void NickListWidget::add_child(QString strNick, QString strModes, QString strParent, QPixmap icon)
{
    // exist parent
    if (!exist_parent(strParent))
    {
        if (strModes.contains("V")) // fix for cam icon
            add_parent(strParent, QPixmap(":/images/pubcam.png"));
        else
            add_parent(strParent, icon);
    }

    // add
    SortedTreeWidgetItem *item = create_child(strNick, strModes, icon);
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QString strParentName = this->topLevelItem(i)->text(0);
        if (strParentName == strParent)
            this->topLevelItem(i)->addChild(item);
    }
}

void NickListWidget::remove_child(QString strName)
{
    QMultiMap<int,QTreeWidgetItem*> mRemoveChild;
    QStringList strlRemoveParent;

    // remove all childs
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QString strParent = this->topLevelItem(i)->text(0);
        for (int x = 0; x < this->topLevelItem(i)->childCount(); x++)
        {
            QString strChild = this->topLevelItem(i)->child(x)->text(0);
            if (strChild == strName)
            {
                // add remove child
                mRemoveChild.insert(i,this->topLevelItem(i)->child(x));

                // remove parent if empty
                if (this->topLevelItem(i)->childCount() == 1) // 1 = last
                    strlRemoveParent.append(strParent);
            }
        }
    }

    // remove childs
    QMapIterator <int, QTreeWidgetItem*> iter(mRemoveChild);
    while (iter.hasNext())
    {
        iter.next();
        int i = iter.key();
        QTreeWidgetItem *item = iter.value();

        this->topLevelItem(i)->removeChild(item);
    }

    // remove empty parents
    QStringListIterator strliRemoveParent(strlRemoveParent);
    while (strliRemoveParent.hasNext())
        remove_parent(strliRemoveParent.next());
}

SortedTreeWidgetItem* NickListWidget::create_child(QString strNick, QString strModes, QPixmap icon)
{
    SortedTreeWidgetItem *item = new SortedTreeWidgetItem();
    item->setText(0, strNick);
    item->setData(0, Qt::DisplayRole, strNick);
    item->setData(0, Qt::UserRole, icon);

    // read from cache when refresh
    if (Core::instance()->mNickAvatar.contains(strNick))
    {
        QPixmap pixmap;
        pixmap.loadFromData(Core::instance()->mNickAvatar.value(strNick));
        item->setData(0, Qt::UserRole+1, pixmap);
    }

    item->setData(0, Qt::UserRole+10, strModes.indexOf("b") != -1 ? true : false);

    item->setData(0, Qt::UserRole+20, "child");

    return item;
}

void NickListWidget::priv()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("PRIV %1").arg(strNick));
}

void NickListWidget::whois()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("WHOIS %1 :%1").arg(strNick));
}

void NickListWidget::profile()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    if (strNick[0] != '~')
    {
        pDlgUserProfile->set_nick(strNick);
        pDlgUserProfile->show();
    }
}

void NickListWidget::cam()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
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

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void NickListWidget::friends_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void NickListWidget::ignore_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void NickListWidget::ignore_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void NickListWidget::kick()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
}

void NickListWidget::ban()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::kban()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

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

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS BANIP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::op_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::op_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::halfop_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::halfop_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::moderator_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::moderator_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::voice_add()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::voice_del()
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::invite()
{
    if (this->selectedItems().size() == 0) return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        QString strNick = this->selectedItems().at(0)->text(0);
        pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

void NickListWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if (this->selectedItems().size() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    // return if not nick
    if ((strNick == tr("Dev(s)")) || (strNick == tr("Admin(s)")) || (strNick == tr("Owner(s)")) || (strNick == tr("Op(s)")) || (strNick == tr("HalfOp(s)")) || (strNick == tr("Mod(s)")) || (strNick == tr("Screener(s)")) || (strNick == tr("Voice(s)")) || (strNick == tr("Cam(s)")) || (strNick == tr("User(s)")))
        return;

    QString strModes;

    for (int i = 0; i < Core::instance()->lUsers.size(); i++)
    {
        if ((Core::instance()->lUsers.at(i).nick == strNick) && (Core::instance()->lUsers.at(i).channel == strChannel))
        {
            strModes = Core::instance()->lUsers.at(i).modes;
            break;
        }
    }

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

    if (strModes.indexOf("@") == -1)
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(op_add()));
    else
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(op_del()));

    if (strModes.indexOf("%") == -1)
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfop_add()));
    else
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfop_del()));

    if (strModes.indexOf("!") == -1)
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderator_add()));
    else
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderator_del()));

    if (strModes.indexOf("+") == -1)
        privilege->addAction(QIcon(":/images/voice.png"), tr("Give guest status"), this, SLOT(voice_add()));
    else
        privilege->addAction(QIcon(":/images/voice.png"), tr("Take guest status"), this, SLOT(voice_del()));

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
