/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#ifndef FIND_NICK_MODEL_H
#define FIND_NICK_MODEL_H

#include <QObject>

class FindNick : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FindNick)
    static FindNick *Instance;
public:
    static FindNick *instance();

    FindNick();
    void add(const QString &nick);
    QList<QString> get();
    void clear();
    void setReady(bool ready);
    bool getReady();

private:
    QList<QString> lNickList;
    bool bNickListReady;
};

#endif // FIND_NICK_MODEL_H
