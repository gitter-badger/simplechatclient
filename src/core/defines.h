#ifndef DEFINES_H
#define DEFINES_H

#include <QList>
#include <QListWidgetItem>
#include <QString>
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

class SortedTreeWidgetItem : public QTreeWidgetItem
{
public:
    SortedTreeWidgetItem() {}
    SortedTreeWidgetItem(QTreeWidget *parent) : QTreeWidgetItem(parent) {}

    virtual bool operator< ( const QTreeWidgetItem & o ) const
    {
        int column =  treeWidget() ? treeWidget()->sortColumn() : 0; // column

        QVariant my = (data(column, Qt::DisplayRole)); // compare value 1
        QVariant other = (o.data(column, Qt::DisplayRole)); // compare value 2
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
        QVariant my = (data(Qt::DisplayRole)); // compare value 1
        QVariant other = (o.data(Qt::DisplayRole)); // compare value 2
        if (!my.isValid() || !other.isValid() || my.type() != other.type()) // valid compare
            return QListWidgetItem::operator< (o);
        if (my.canConvert(QVariant::String)) // my compare
            return text().toLower() < o.text().toLower();
        else
            return QListWidgetItem::operator< (o); // other compare
    }
};

#endif // DEFINES_H
