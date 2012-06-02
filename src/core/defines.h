/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <QList>
#include <QListWidgetItem>
#include <QString>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>

#include "debug_block.h"

enum MessageCategory {MessageDefault, MessageJoin, MessagePart, MessageQuit, MessageKick, MessageMode, MessageNotice, MessageInfo, MessageMe, MessageError, MessageHighlight, MessageModerNotice};

enum ChannelColor {ChannelGreen, ChannelRed, ChannelHighlight};

enum { maxOpenChannels = 30 };

const int HOURS_IN_DAY = 24;
const int MINUTES_IN_HOUR = 60;
const int SECONDS_IN_MINUTE = 60;

#define STATUS "Status"
#define DEBUG "Debug"

struct ChannelList
{
    QString name;
    int people;
    int cat;
    int type;
    bool moderated;
    bool recommended;
};

struct ModerateMsg
{
    QString channel;
    int datetime;
    QString nick;
    QString message;
};

struct OfflineMsg
{
    int datetime;
    QString type;
    QString nick;
    QString message;
};

class CaseIgnoreString : public QString
{
public:
    CaseIgnoreString(const QString& other = QString()) : QString(other) {}

    bool operator<(const QString & other) const {
        return toLower() < other.toLower();
    }
    bool operator==(const QString& other) const {
        return toLower() == other.toLower();
    }
};

class SortedListWidgetItem : public QListWidgetItem
{
public:
    SortedListWidgetItem() {}
    SortedListWidgetItem(QListWidget *parent) : QListWidgetItem(parent) {}

    virtual bool operator< ( const QListWidgetItem & o ) const
    {
        QVariant my = data(Qt::DisplayRole); // compare value 1
        QVariant other = o.data(Qt::DisplayRole); // compare value 2
        QVariant isNickList = data(Qt::UserRole+10); // compare value 2
        if (!my.isValid() || !other.isValid() || my.type() != other.type()) // valid compare
            return QListWidgetItem::operator< (o);
        if (my.canConvert(QVariant::String)) // my compare
        {
            if (isNickList.toBool() == true) // sorting by status
            {
                int mModes = data(Qt::UserRole+11).toInt();
                int oModes = o.data(Qt::UserRole+11).toInt();
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
