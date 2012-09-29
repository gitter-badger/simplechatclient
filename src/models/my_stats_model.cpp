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

#include "my_stats_model.h"

MyStatsModel * MyStatsModel::Instance = 0;

MyStatsModel * MyStatsModel::instance()
{
    if (!Instance)
    {
        Instance = new MyStatsModel();
        Instance->clear();
    }

    return Instance;
}

MyStatsModel::MyStatsModel()
{
}

void MyStatsModel::clear()
{
    myStats.clear();
}

QString MyStatsModel::get(const QString &key)
{
    return myStats.value(key, QString::null);
}

QHash<QString,QString> MyStatsModel::getAll()
{
    return myStats;
}

void MyStatsModel::set(const QString &key, const QString &value)
{
    myStats[key] = value;
}
