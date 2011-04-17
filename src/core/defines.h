#ifndef DEFINES_H
#define DEFINES_H

#include <QList>
#include <QListWidgetItem>
#include <QString>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>

struct NickStatus
{
    QString channel;
    QString nick;
    QString prefix;
    QString suffix;
};
typedef QList<NickStatus> sChannelNickStatus;

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

class SortedTreeWidgetItem : public QTreeWidgetItem
{
public:
    SortedTreeWidgetItem() {}
    SortedTreeWidgetItem(QTreeWidget *parent) : QTreeWidgetItem(parent) {}

    virtual bool operator< ( const QTreeWidgetItem & o ) const
    {
        int column =  treeWidget() ? treeWidget()->sortColumn() : 0; // column

        QVariant my = data(column, Qt::DisplayRole); // compare value 1
        QVariant other = o.data(column, Qt::DisplayRole); // compare value 2
        if (!my.isValid() || !other.isValid() || my.type() != other.type()) // valid compare
            return QTreeWidgetItem::operator< (o);
        if (my.canConvert(QVariant::String)) // my compare
            return text(column).toLower() < o.text(column).toLower();
        else
            return QTreeWidgetItem::operator< (o); // other compare
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
        if (!my.isValid() || !other.isValid() || my.type() != other.type()) // valid compare
            return QListWidgetItem::operator< (o);
        if (my.canConvert(QVariant::String)) // my compare
            return text().toLower() < o.text().toLower();
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
