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

#ifndef MY_STATS_MODEL_H
#define MY_STATS_MODEL_H

#include <QHash>
#include <QObject>

class MyStatsModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MyStatsModel)
    static MyStatsModel *Instance;
public:
    static MyStatsModel *instance();

    MyStatsModel();
    void clear();
    QString get(const QString &key);
    QHash<QString,QString> getAll();
    void set(const QString &key, const QString &value);

private:
    QHash<QString,QString> myStats;
};

#endif // MY_STATS_MODEL_H
