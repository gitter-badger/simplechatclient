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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include <QStringList>

/**
 * Class for irc commands
 * @param QString channel name
 * @param QString command
 */
class Commands : public QObject
{
    Q_OBJECT
public:
    Commands(const QString &_strChan, const QString &_strData);
    QString execute();

private:
    QString strChan;
    QString strData;
    QStringList strDataList;

    QString cmdRaw();
    QString cmdAll();
    QString cmdJoin();
    QString cmdPriv();
    QString cmdIgnore();
    QString cmdFriend();
    QString cmdWhereis();
    QString cmdBusy();
    QString cmdAway();
    QString cmdOffmsg();
    QString cmdMp3();
    QString cmdQuit();
    QString cmdHelp();
    QString cmdCycle();
    QString cmdMe();
    QString cmdTopic();
    QString cmdPart();
    QString cmdInvite();
    QString cmdKick();
    QString cmdBan();
    QString cmdBanip();
    QString cmdKBan();
    QString cmdKBanip();
    QString cmdSop();
    QString cmdOp();
    QString cmdModer();
    QString cmdVip();
};

#endif // COMMANDS_H
