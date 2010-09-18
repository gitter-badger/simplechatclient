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

#include "nicklistwidget.h"

NickListWidget::NickListWidget(QWidget *parent, Network *param1, QString param2, QMap <QString, QByteArray> *param3, QTcpSocket *param4, sNickStatus *param5)
{
    myparent = parent;
    pNetwork = param1;
    strChannel = param2;
    mNickAvatar = param3;
    camSocket = param4;
    nickStatus = param5;

    setAnimated(true);
    header()->hide();
    setSortingEnabled(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QObject::connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(item_selected()));
}

NickListWidget::~NickListWidget()
{
    strOpenChannels.clear();
}

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

// busy       b
// registered r
// encrypted  x
// publiccam  W
// privcam    V
// admin      o
// developer  O

void NickListWidget::add(QString strNick, QString strPrefix, QString strSuffix, sNickStatus *nick_status)
{
// add
    NickStatus add;
    add.nick = strNick;
    add.prefix = strPrefix;
    add.suffix = strSuffix;

// add to nick list
    nick_status->append(add);

// add to widget
    if (strSuffix.indexOf("O") != -1)
    {
        QPixmap icon = QPixmap(":/images/dev.png");
        if (exist_parent(tr("Developer(s)")) == false) add_parent(tr("Developer(s)"), icon);
        if (exist_child(strNick, tr("Developer(s)")) == false) add_child(tr("Developer(s)"), create_child(strNick, strSuffix, tr("Developer(s)"), icon));
    }
    if (strSuffix.indexOf("o") != -1)
    {
        QPixmap icon = QPixmap(":/images/admin.png");
        if (exist_parent(tr("Admin(s)")) == false) add_parent(tr("Admin(s)"), icon);
        if (exist_child(strNick, tr("Admin(s)")) == false) add_child(tr("Admin(s)"), create_child(strNick, strSuffix, tr("Admin(s)"), icon));
    }
    if (strPrefix.indexOf("`") != -1)
    {
        QPixmap icon = QPixmap(":/images/owner.png");
        if (exist_parent(tr("Owner(s)")) == false) add_parent(tr("Owner(s)"), icon);
        if (exist_child(strNick, tr("Owner(s)")) == false) add_child(tr("Owner(s)"), create_child(strNick, strSuffix, tr("Owner(s)"), icon));
    }
    if (strPrefix.indexOf("@") != -1)
    {
        QPixmap icon = QPixmap(":/images/op.png");
        if (exist_parent(tr("Op(s)")) == false) add_parent(tr("Op(s)"), icon);
        if (exist_child(strNick, tr("Op(s)")) == false) add_child(tr("Op(s)"), create_child(strNick, strSuffix, tr("Op(s)"), icon));
    }
    if (strPrefix.indexOf("%") != -1)
    {
        QPixmap icon = QPixmap(":/images/halfop.png");
        if (exist_parent(tr("HalfOp(s)")) == false) add_parent(tr("HalfOp(s)"), icon);
        if (exist_child(strNick, tr("HalfOp(s)")) == false) add_child(tr("HalfOp(s)"), create_child(strNick, strSuffix, tr("HalfOp(s)"), icon));
    }
    if (strPrefix.indexOf("!") != -1)
    {
        QPixmap icon = QPixmap(":/images/mod.png");
        if (exist_parent(tr("Mod(s)")) == false) add_parent(tr("Mod(s)"), icon);
        if (exist_child(strNick, tr("Mod(s)")) == false) add_child(tr("Mod(s)"), create_child(strNick, strSuffix, tr("Mod(s)"), icon));
    }
    if (strPrefix.indexOf("=") != -1)
    {
        QPixmap icon = QPixmap(":/images/screener.png");
        if (exist_parent(tr("Screener(s)")) == false) add_parent(tr("Screener(s)"), icon);
        if (exist_child(strNick, tr("Screener(s)")) == false) add_child(tr("Screener(s)"), create_child(strNick, strSuffix, tr("Screener(s)"), icon));
    }
    if (strPrefix.indexOf("+") != -1)
    {
        QPixmap icon = QPixmap(":/images/voice.png");
        if (exist_parent(tr("Voice(s)")) == false) add_parent(tr("Voice(s)"), icon);
        if (exist_child(strNick, tr("Voice(s)")) == false) add_child(tr("Voice(s)"), create_child(strNick, strSuffix, tr("Voice(s)"), icon));
    }
    if ((strSuffix.indexOf("W") != -1) || (strSuffix.indexOf("V") != -1))
    {
        QPixmap icon = QPixmap(":/images/pubcam.png");
        if (exist_parent(tr("Cam(s)")) == false) add_parent(tr("Cam(s)"), icon);
        if (exist_child(strNick, tr("Cam(s)")) == false) add_child(tr("Cam(s)"), create_child(strNick, strSuffix, tr("Cam(s)"), icon));
    }
    if ((strSuffix.indexOf("O") == -1) && (strSuffix.indexOf("o") == -1) && (strPrefix.indexOf("`") == -1) && (strPrefix.indexOf("@") == -1) && (strPrefix.indexOf("%") == -1) && (strPrefix.indexOf("!") == -1) && (strPrefix.indexOf("+") == -1))
    {
        QPixmap icon = QPixmap(":/images/user.png");
        if (exist_parent(tr("User(s)")) == false) add_parent(tr("User(s)"), icon);
        if (exist_child(strNick, tr("User(s)")) == false) add_child(tr("User(s)"), create_child(strNick, strSuffix, tr("User(s)"), icon));
    }
}

void NickListWidget::remove(QString strNick, sNickStatus *nick_status)
{
// remove from nick list
    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus check = nick_status->at(i);
        if (check.nick == strNick)
        {
            nick_status->removeAt(i);
            break;
        }
    }

// remove from widget
    remove_child(strNick);
}

bool NickListWidget::exist(QString strNick, sNickStatus *nick_status)
{
    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus check = nick_status->at(i);
        if (check.nick == strNick)
            return true;
    }
    return false;
}

QStringList NickListWidget::get(sNickStatus *nick_status)
{
    QStringList strlResult;

    for (int i = 0; i < nick_status->count(); i++)
    {
        NickStatus listNickStatus(nick_status->at(i));

        QString strKey = listNickStatus.nick;
        strlResult.append(strKey);
    }

    return strlResult;
}

void NickListWidget::refresh_avatars()
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *parent_item = this->topLevelItem(i);
        for (int x = 0; x < parent_item->childCount(); x++)
        {
            QTreeWidgetItem *child_item = parent_item->child(x);
            QString strChild = child_item->text(0);

            if ((strChild[0] != '~') && (child_item->data(0, Qt::UserRole+2).isNull() == true) && (mNickAvatar->contains(strChild) == true))
            {
                QPixmap pixmap;
                pixmap.loadFromData(mNickAvatar->value(strChild));
                child_item->setData(0, Qt::UserRole+2, pixmap);
            }
        }
    }
}

void NickListWidget::set_user_info(QString strNick, QString strKey, QString strValue)
{
    if (this->selectedItems().count() == 0) return;

    QString strNickSelected = this->selectedItems().at(0)->text(0);

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

void NickListWidget::add_parent(QString strName, QPixmap px)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setText(0, strName);
    item->setIcon(0, px);

    sort_parent();
}

bool NickListWidget::exist_parent(QString strName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = this->topLevelItem(i);
        QString strText = item->text(0);
        if (strText == strName)
            return true;
    }
    return false;
}

void NickListWidget::remove_parent(QString strName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = this->topLevelItem(i);
        QString strText = item->text(0);
        if (strText == strName)
            delete this->takeTopLevelItem(i);
    }
}

int NickListWidget::index_parent(QString strName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *parent_item = this->topLevelItem(i);
        QString strParent = parent_item->text(0);

        if (strParent == strName)
            return i;
    }
    return -1;
}

void NickListWidget::move_parent(int index, int top)
{
    // move
    QTreeWidgetItem *parent_item = this->takeTopLevelItem(index);
    this->insertTopLevelItem(top, parent_item);

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

void NickListWidget::add_child(QString strParentName, QTreeWidgetItem *item)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *parent_item = this->topLevelItem(i);
        QString strParent = parent_item->text(0);
        if (strParent == strParentName)
        {
            parent_item->addChild(item);
            parent_item->sortChildren(0, Qt::AscendingOrder);
        }
    }
}

bool NickListWidget::exist_child(QString strChildName, QString strParentName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *parent_item = this->topLevelItem(i);
        QString strParent = parent_item->text(0);
        for (int x = 0; x < parent_item->childCount(); x++)
        {
            QTreeWidgetItem *child_item = parent_item->child(x);
            QString strChild = child_item->text(0);
            if ((strParent == strParentName) && (strChild == strChildName))
                return true;
        }
    }
    return false;
}

void NickListWidget::remove_child(QString strName)
{
    for (int i = 0; i < this->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *parent_item = this->topLevelItem(i);
        QString strParent = parent_item->text(0);
        for (int x = 0; x < parent_item->childCount(); x++)
        {
            QTreeWidgetItem *child_item = parent_item->child(x);
            QString strChild = child_item->text(0);
            if (strChild == strName)
            {
                parent_item->removeChild(child_item);
                if (parent_item->childCount() == 0)
                    remove_parent(strParent);
            }
        }
    }
}

QTreeWidgetItem* NickListWidget::create_child(QString strNick, QString strSuffix, QString strParent, QPixmap icon)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, strNick);
    item->setIcon(0, icon);
    item->setData(0, Qt::UserRole, icon);

    if (strSuffix.indexOf("W") != -1)
    {
        QPixmap newicon = QPixmap(":/images/pubcam.png");
        item->setData(0, Qt::UserRole+1, newicon);
        if (strParent == tr("Cam(s)"))
            item->setIcon(0, newicon);
    }
    if (strSuffix.indexOf("V") != -1)
    {
        QPixmap newicon = QPixmap(":/images/privcam.png");
        item->setData(0, Qt::UserRole+1, newicon);
        if (strParent == tr("Cam(s)"))
            item->setIcon(0, newicon);
    }

    item->setData(0, Qt::DisplayRole, strNick);

    // read from cache when refresh
    if (mNickAvatar->contains(strNick) == true)
    {
        QPixmap pixmap;
        pixmap.loadFromData(mNickAvatar->value(strNick));
        item->setData(0, Qt::UserRole+2, pixmap);
    }

    item->setData(0, Qt::UserRole+10, strSuffix.indexOf("b") != -1 ? false : true);

    return item;
}

void NickListWidget::priv()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("PRIV %1").arg(strNick));
}

void NickListWidget::whois()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("WHOIS %1 %1").arg(strNick));
}

void NickListWidget::profile()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    if (strNick == sCurrentUserInfo.nick)
    {
        (new DlgUserProfile(myparent, pNetwork, sCurrentUserInfo))->show();
    }
}

void NickListWidget::cam()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
#ifdef Q_WS_WIN
    QSettings settings;
    QString strUOKey = settings.value("uokey").toString();
    (new Kamerzysta(camSocket))->show(strNick, strUOKey);
#else
    (new DlgCam(myparent, pNetwork, strNick))->show();
#endif
}

void NickListWidget::friends_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void NickListWidget::friends_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void NickListWidget::ignore_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void NickListWidget::ignore_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void NickListWidget::kick()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok == true) && (strText.isEmpty() == false))
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
}

void NickListWidget::ban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::kban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick & Ban"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok == true) && (strText.isEmpty() == false))
    {
        pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
    }
}

void NickListWidget::op_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::op_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::halfop_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::halfop_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::moderator_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::moderator_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::voice_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void NickListWidget::voice_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);
    pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void NickListWidget::invite()
{
    if (this->selectedItems().count() == 0) return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        QString strNick = this->selectedItems().at(0)->text(0);
        pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

void NickListWidget::item_selected()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    // return if not nick
    if ((strNick == tr("Dev(s)")) || (strNick == tr("Admin(s)")) || (strNick == tr("Owner(s)")) || (strNick == tr("Op(s)")) || (strNick == tr("HalfOp(s)")) || (strNick == tr("Mod(s)")) || (strNick == tr("Screener(s)")) || (strNick == tr("Voice(s)")) || (strNick == tr("Cam(s)")) || (strNick == tr("User(s)")))
        return;

    if ((strNick[0] != '~') && (sCurrentUserInfo.nick != strNick))
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

void NickListWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text(0);

    // return if not nick
    if ((strNick == tr("Dev(s)")) || (strNick == tr("Admin(s)")) || (strNick == tr("Owner(s)")) || (strNick == tr("Op(s)")) || (strNick == tr("HalfOp(s)")) || (strNick == tr("Mod(s)")) || (strNick == tr("Screener(s)")) || (strNick == tr("Voice(s)")) || (strNick == tr("Cam(s)")) || (strNick == tr("User(s)")))
        return;

    QString strPrefix;
    QString strSuffix;

    for (int i = 0; i < nickStatus->count(); i++)
    {
        if (nickStatus->at(i).nick == strNick)
        {
            strPrefix = nickStatus->at(i).prefix;
            strSuffix = nickStatus->at(i).suffix;
            break;
        }
    }

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

    if (strPrefix.indexOf("@") == -1)
        privilege->addAction(tr("Give super operator status"), this, SLOT(op_add()));
    else
        privilege->addAction(tr("Take super operator status"), this, SLOT(op_del()));

    if (strPrefix.indexOf("%") == -1)
        privilege->addAction(tr("Give operator status"), this, SLOT(halfop_add()));
    else
        privilege->addAction(tr("Take operator status"), this, SLOT(halfop_del()));

    if (strPrefix.indexOf("!") == -1)
        privilege->addAction(tr("Give moderator status"), this, SLOT(moderator_add()));
    else
        privilege->addAction(tr("Take moderator status"), this, SLOT(moderator_del()));

    if (strPrefix.indexOf("+") == -1)
        privilege->addAction(tr("Give guest status"), this, SLOT(voice_add()));
    else
        privilege->addAction(tr("Take guest status"), this, SLOT(voice_del()));

    QAction *nickAct = new QAction(strNick, this);
    nickAct->setDisabled(true);

    QMenu *menu = new QMenu(strNick);
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
