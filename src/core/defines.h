#ifndef DEFINES_H
#define DEFINES_H

#include <QList>
#include <QString>

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

#endif // DEFINES_H
