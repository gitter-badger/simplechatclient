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

NickListWidget::NickListWidget(Network *param1, QString param2, QTcpSocket *param3, DlgUserProfile *param4)
{
    pNetwork = param1;
    strChannel = param2;
    camSocket = param3;
    pDlgUserProfile = param4;

    setAnimated(true);
    header()->hide();
    setSortingEnabled(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

NickListWidget::~NickListWidget()
{
    strOpenChannels.clear();

    for (int i = 0; i < Core::instance()->stlChannelNickStatus.size(); i++)
    {
        if (Core::instance()->stlChannelNickStatus.at(i).channel == strChannel)
        {
            Core::instance()->stlChannelNickStatus.removeAt(i);
            i--;
        }
    }
}

#ifndef Q_WS_WIN
void NickListWidget::set_dlg_cam(DlgCam *param1)
{
    pDlgCam = param1;
}
#endif

void NickListWidget::set_open_channels(QStringList param1)
{
    strOpenChannels = param1;
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

void NickListWidget::add(QString strNick, QString strPrefix, QString strSuffix)
{
    // add
    NickStatus add;
    add.channel = strChannel;
    add.nick = strNick;
    add.prefix = strPrefix;
    add.suffix = strSuffix;

    // add to nick list
    Core::instance()->stlChannelNickStatus.append(add);

    // add child to widget
    add_child(strNick, strPrefix, strSuffix);
}

void NickListWidget::remove(QString strNick)
{
    // remove from nick list
    for (int i = 0; i < Core::instance()->stlChannelNickStatus.size(); i++)
    {
        if ((Core::instance()->stlChannelNickStatus.at(i).nick == strNick) && (Core::instance()->stlChannelNickStatus.at(i).channel == strChannel))
        {
            Core::instance()->stlChannelNickStatus.removeAt(i);
            i--;
        }
    }

    // remove from widget
    remove_child(strNick);
}

bool NickListWidget::exist(QString strNick)
{
    for (int i = 0; i < Core::instance()->stlChannelNickStatus.size(); i++)
    {
        if ((Core::instance()->stlChannelNickStatus.at(i).nick == strNick) && (Core::instance()->stlChannelNickStatus.at(i).channel == strChannel))
            return true;
    }
    return false;
}

QStringList NickListWidget::get()
{
    QStringList strlResult;

    for (int i = 0; i < Core::instance()->stlChannelNickStatus.size(); i++)
    {
        if (Core::instance()->stlChannelNickStatus.at(i).channel == strChannel)
        {
            QString strKey = Core::instance()->stlChannelNickStatus.at(i).nick;
            strlResult.append(strKey);
        }
    }

    return strlResult;
}

void NickListWidget::refresh_avatars()
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        for (int x = 0; x < this->topLevelItem(i)->childCount(); x++)
        {
            QString strChild = this->topLevelItem(i)->child(x)->text(0);

            if ((strChild[0] != '~') && (this->topLevelItem(i)->child(x)->data(0, Qt::UserRole+1).isNull() == true) && (Core::instance()->mNickAvatar.contains(strChild) == true))
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

    sort_parent();
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

int NickListWidget::index_parent(QString strName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QString strParent = this->topLevelItem(i)->text(0);

        if (strParent == strName)
            return i;
    }
    return -1;
}

void NickListWidget::move_parent(int index, int top)
{
    // move
    this->insertTopLevelItem(top, this->takeTopLevelItem(index));

    // scroll
    this->topLevelItem(top)->setExpanded(true);
}

void NickListWidget::sort_parent()
{
    QStringList strlNames;
    strlNames << tr("User(s)") << tr("Cam(s)") << tr("Voice(s)") << tr("Screener(s)") << tr("Mod(s)") << tr("HalfOp(s)") << tr("Op(s)") << tr("Owner(s)") << tr("Admin(s)") << tr("Developer(s)");

    QStringListIterator strliNames(strlNames);
    while (strliNames.hasNext())
    {
        QString strName = strliNames.next();

        if (exist_parent(strName) == true)
        {
            int index = index_parent(strName);
            if ((index != 0) && (index != -1))
                move_parent(index, 0);
        }
    }

}

void NickListWidget::add_child(QString strNick, QString strPrefix, QString strSuffix)
{
    // add to widget
    if (strSuffix.contains("O") == true)
    {
        QPixmap icon = QPixmap(":/images/dev.png");
        if (exist_parent(tr("Developer(s)")) == false) add_parent(tr("Developer(s)"), icon);
        if (exist_child(strNick, tr("Developer(s)")) == false) add_child(tr("Developer(s)"), create_child(strNick, strSuffix, icon));
    }
    if (strSuffix.contains("o") == true)
    {
        QPixmap icon = QPixmap(":/images/admin.png");
        if (exist_parent(tr("Admin(s)")) == false) add_parent(tr("Admin(s)"), icon);
        if (exist_child(strNick, tr("Admin(s)")) == false) add_child(tr("Admin(s)"), create_child(strNick, strSuffix, icon));
    }
    if (strPrefix.contains("`") == true)
    {
        QPixmap icon = QPixmap(":/images/owner.png");
        if (exist_parent(tr("Owner(s)")) == false) add_parent(tr("Owner(s)"), icon);
        if (exist_child(strNick, tr("Owner(s)")) == false) add_child(tr("Owner(s)"), create_child(strNick, strSuffix, icon));
    }
    if (strPrefix.contains("@") == true)
    {
        QPixmap icon = QPixmap(":/images/op.png");
        if (exist_parent(tr("Op(s)")) == false) add_parent(tr("Op(s)"), icon);
        if (exist_child(strNick, tr("Op(s)")) == false) add_child(tr("Op(s)"), create_child(strNick, strSuffix, icon));
    }
    if (strPrefix.contains("%") == true)
    {
        QPixmap icon = QPixmap(":/images/halfop.png");
        if (exist_parent(tr("HalfOp(s)")) == false) add_parent(tr("HalfOp(s)"), icon);
        if (exist_child(strNick, tr("HalfOp(s)")) == false) add_child(tr("HalfOp(s)"), create_child(strNick, strSuffix, icon));
    }
    if (strPrefix.contains("!") == true)
    {
        QPixmap icon = QPixmap(":/images/mod.png");
        if (exist_parent(tr("Mod(s)")) == false) add_parent(tr("Mod(s)"), icon);
        if (exist_child(strNick, tr("Mod(s)")) == false) add_child(tr("Mod(s)"), create_child(strNick, strSuffix, icon));
    }
    if (strPrefix.contains("=") == true)
    {
        QPixmap icon = QPixmap(":/images/screener.png");
        if (exist_parent(tr("Screener(s)")) == false) add_parent(tr("Screener(s)"), icon);
        if (exist_child(strNick, tr("Screener(s)")) == false) add_child(tr("Screener(s)"), create_child(strNick, strSuffix, icon));
    }
    if (strPrefix.contains("+") == true)
    {
        QPixmap icon = QPixmap(":/images/voice.png");
        if (exist_parent(tr("Voice(s)")) == false) add_parent(tr("Voice(s)"), icon);
        if (exist_child(strNick, tr("Voice(s)")) == false) add_child(tr("Voice(s)"), create_child(strNick, strSuffix, icon));
    }
    if ((strSuffix.contains("W") == true) || (strSuffix.contains("V") == true))
    {
        QPixmap icon;
        if (strSuffix.contains("W") == true)
            icon = QPixmap(":/images/pubcam.png");
        else if (strSuffix.contains("V") == true)
            icon = QPixmap(":/images/privcam.png");

        if (exist_parent(tr("Cam(s)")) == false) add_parent(tr("Cam(s)"), QPixmap(":/images/pubcam.png"));
        if (exist_child(strNick, tr("Cam(s)")) == false) add_child(tr("Cam(s)"), create_child(strNick, strSuffix, icon));
    }
    if ((strSuffix.contains("O") == false) && (strSuffix.contains("o") == false) && (strPrefix.contains("`") == false) && (strPrefix.contains("@") == false) && (strPrefix.contains("%") == false) && (strPrefix.contains("!") == false) && (strPrefix.contains("=") == false) && (strPrefix.contains("+") == false) && (strSuffix.contains("W") == false) && (strSuffix.contains("V") == false))
    {
        QPixmap icon = QPixmap(":/images/user.png");
        if (exist_parent(tr("User(s)")) == false) add_parent(tr("User(s)"), icon);
        if (exist_child(strNick, tr("User(s)")) == false) add_child(tr("User(s)"), create_child(strNick, strSuffix, icon));
    }
}

void NickListWidget::add_child(QString strParentName, SortedTreeWidgetItem *item)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QString strParent = this->topLevelItem(i)->text(0);
        if (strParent == strParentName)
        {
            this->topLevelItem(i)->addChild(item);
            this->topLevelItem(i)->sortChildren(0, Qt::AscendingOrder);
        }
    }
}

bool NickListWidget::exist_child(QString strChildName, QString strParentName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QString strParent = this->topLevelItem(i)->text(0);
        for (int x = 0; x < this->topLevelItem(i)->childCount(); x++)
        {
            QString strChild = this->topLevelItem(i)->child(x)->text(0);
            if ((strParent == strParentName) && (strChild == strChildName))
                return true;
        }
    }
    return false;
}

void NickListWidget::remove_child(QString strName)
{
    QMultiMap <int, QTreeWidgetItem*> mRemoveChild;
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

SortedTreeWidgetItem* NickListWidget::create_child(QString strNick, QString strSuffix, QPixmap icon)
{
    SortedTreeWidgetItem *item = new SortedTreeWidgetItem();
    item->setData(0, Qt::UserRole, icon);
    item->setText(0, strNick);
    item->setData(0, Qt::DisplayRole, strNick);

    // read from cache when refresh
    if (Core::instance()->mNickAvatar.contains(strNick) == true)
    {
        QPixmap pixmap;
        pixmap.loadFromData(Core::instance()->mNickAvatar.value(strNick));
        item->setData(0, Qt::UserRole+1, pixmap);
    }

    item->setData(0, Qt::UserRole+10, strSuffix.indexOf("b") != -1 ? true : false);

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

    if ((ok == true) && (strText.isEmpty() == false))
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

    if ((ok == true) && (strText.isEmpty() == false))
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

    QString strPrefix;
    QString strSuffix;

    for (int i = 0; i < Core::instance()->stlChannelNickStatus.size(); i++)
    {
        if ((Core::instance()->stlChannelNickStatus.at(i).nick == strNick) && (Core::instance()->stlChannelNickStatus.at(i).channel == strChannel))
        {
            strPrefix = Core::instance()->stlChannelNickStatus.at(i).prefix;
            strSuffix = Core::instance()->stlChannelNickStatus.at(i).suffix;
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

    for (int i = 0; i < strOpenChannels.size(); ++i)
    {
        openChannelsActs[i]->setText(strOpenChannels[i]);
        openChannelsActs[i]->setData(strOpenChannels[i]);
        openChannelsActs[i]->setVisible(true);
    }
    for (int j = strOpenChannels.size(); j < maxOpenChannels; ++j)
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

    if (strPrefix.indexOf("@") == -1)
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(op_add()));
    else
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(op_del()));

    if (strPrefix.indexOf("%") == -1)
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfop_add()));
    else
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfop_del()));

    if (strPrefix.indexOf("!") == -1)
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderator_add()));
    else
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderator_del()));

    if (strPrefix.indexOf("+") == -1)
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
