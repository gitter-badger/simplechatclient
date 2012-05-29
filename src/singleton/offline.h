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

#ifndef OFFLINE_H
#define OFFLINE_H

#include "defines.h"
#include <QAction>
#include <QObject>

class Offline : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Offline)
    static Offline *Instance;
public:
    static Offline *instance();

    Offline();
    void init();
    void addMsg(int iTime, const QString &strType, const QString &strNick, const QString &strMessage);
    void removeMsg(const QString &strNick);
    void clearMsg();
    QList<OfflineMsg> getMsg();
    bool isEmptyMsg();

    void addNick(const QString &strNick);
    void removeNick(const QString &strNick);
    void clearNicks();
    QList<QString> getNicks();

    QAction *offlineMsgAction;

private:
    QList<OfflineMsg> lOfflineMsg;
    QList<QString> lOfflineNicks;
};

#endif // OFFLINE_H
