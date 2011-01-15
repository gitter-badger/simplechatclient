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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QIcon>
#include <QMessageBox>
#include <QObject>
#include <QSettings>
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
    Commands(QString, QString);
    QString execute();

private:
    QString strChan;
    QString strData;
    QStringList strDataList;

    QString cmd_join();
    QString cmd_priv();
    QString cmd_ignore();
    QString cmd_friend();
    QString cmd_whereis();
    QString cmd_busy();
    QString cmd_away();
    QString cmd_quit();
    QString cmd_help();
    QString cmd_cycle();
    QString cmd_me();
    QString cmd_topic();
    QString cmd_part();
    QString cmd_invite();
    QString cmd_kick();
    QString cmd_ban();
    QString cmd_banip();
    QString cmd_sop();
    QString cmd_op();
    QString cmd_moder();
    QString cmd_vip();

};

#endif // COMMANDS_H
