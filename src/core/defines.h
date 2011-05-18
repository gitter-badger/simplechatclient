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

#ifndef DEFINES_H
#define DEFINES_H

#include <QList>
#include <QListWidgetItem>
#include <QString>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>

enum MessageCategory {DefaultMessage, JoinMessage, PartMessage, QuitMessage, KickMessage, ModeMessage, NoticeMessage, InfoMessage, MeMessage, ErrorMessage, HilightMessage};

struct User
{
    QString nick;
    QString channel;
    QString modes;
};
typedef QList<User> sUsers;

struct ChannelList
{
    QString name;
    QString people;
    QString cat;
    QString type;
};
typedef QList<ChannelList> sChannelList;

struct ModerateMsg
{
    QString datetime;
    QString id;
    QString channel;
    QString nick;
    QString message;
};
typedef QList<ModerateMsg> sModerateMsg;

struct OfflineMsg
{
    QString datetime;
    QString nick;
    QString message;
};
typedef QList<OfflineMsg> sOfflineMsg;

class SortedListWidgetItem : public QListWidgetItem
{
public:
    SortedListWidgetItem() {}
    SortedListWidgetItem(QListWidget *parent) : QListWidgetItem(parent) {}

    virtual bool operator< ( const QListWidgetItem & o ) const
    {
        QVariant my = data(Qt::DisplayRole); // compare value 1
        QVariant other = o.data(Qt::DisplayRole); // compare value 2
        QVariant isNickList = data(Qt::UserRole+11); // compare value 2
        if (!my.isValid() || !other.isValid() || my.type() != other.type()) // valid compare
            return QListWidgetItem::operator< (o);
        if (my.canConvert(QVariant::String)) // my compare
        {
            if (isNickList.toBool() == true) // sorting by status
            {
                int mModes = data(Qt::UserRole+12).toInt();
                int oModes = o.data(Qt::UserRole+12).toInt();
                if (mModes != oModes)
                {
                    if (mModes < oModes)
                        return false;
                    else
                        return true;
                }
                else
                    return text().toLower() < o.text().toLower();
            }
            else
                return text().toLower() < o.text().toLower();
        }
        else
            return QListWidgetItem::operator< (o); // other compare
    }
};

class SortedTableWidgetItem : public QTableWidgetItem
{
public:
    SortedTableWidgetItem() {}
    SortedTableWidgetItem(QString name) : QTableWidgetItem(name) {}

    virtual bool operator< ( const QTableWidgetItem & o ) const
    {
        QVariant my = data(Qt::DisplayRole);
        QVariant other = o.data(Qt::DisplayRole);
        if (!my.isValid() || !other.isValid() || my.type() != other.type())
            return QTableWidgetItem::operator< (o);

        if (QVariant::String == my.type())
            return my.toString().toLower() < other.toString().toLower();
        else
            return QTableWidgetItem::operator< (o);
    }
};

class SortedChannelListTableWidgetItem : public QTableWidgetItem
{
public:
    SortedChannelListTableWidgetItem() {}
    SortedChannelListTableWidgetItem(QString name) : QTableWidgetItem(name) {}

    virtual bool operator< ( const QTableWidgetItem & o ) const
    {
        QVariant my = data(Qt::DisplayRole);
        QVariant other = o.data(Qt::DisplayRole);
        if (!my.isValid() || !other.isValid() || my.type() != other.type())
            return QTableWidgetItem::operator< (o);

        if (QVariant::String == my.type())
            return my.toString().toInt() < other.toString().toInt();
        else
            return QTableWidgetItem::operator< (o);
    }
};

#endif // DEFINES_H
