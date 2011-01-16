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

#endif // DEFINES_H
