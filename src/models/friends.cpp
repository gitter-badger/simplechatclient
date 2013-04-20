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

#include "friends.h"

FriendsModel * FriendsModel::Instance = 0;

FriendsModel * FriendsModel::instance()
{
    if (!Instance)
    {
        Instance = new FriendsModel();
        Instance->clear();
    }

    return Instance;
}

FriendsModel::FriendsModel()
{
}

void FriendsModel::clear()
{
    friends.clear();
}

bool FriendsModel::get(const QString &key)
{
    return friends.value(key, false);
}

QHash<QString,bool> FriendsModel::getAll()
{
    return friends;
}

void FriendsModel::set(const QString &key, bool value)
{
    friends[key] = value;
}

void FriendsModel::remove(const QString &key)
{
    friends.remove(key);
}

bool FriendsModel::contains(const QString &key)
{
    return friends.contains(key);
}
