/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include "ignore.h"

IgnoreModel * IgnoreModel::Instance = 0;

IgnoreModel * IgnoreModel::instance()
{
    if (!Instance)
    {
        Instance = new IgnoreModel();
        Instance->clear();
    }

    return Instance;
}

IgnoreModel::IgnoreModel()
{
}

void IgnoreModel::clear()
{
    ignore.clear();
}

QList<QString> IgnoreModel::getAll()
{
    return ignore;
}

void IgnoreModel::add(const QString &key)
{
    if (!ignore.contains(key))
        ignore.append(key);
}

void IgnoreModel::remove(const QString &key)
{
    if (ignore.contains(key))
        ignore.removeOne(key);
}

bool IgnoreModel::contains(const QString &key)
{
    return ignore.contains(key);
}
