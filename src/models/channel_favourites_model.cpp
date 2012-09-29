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

#include "channel_favourites_model.h"

ChannelFavouritesModel * ChannelFavouritesModel::Instance = 0;

ChannelFavouritesModel * ChannelFavouritesModel::instance()
{
    if (!Instance)
    {
        Instance = new ChannelFavouritesModel();
        Instance->clear();
    }

    return Instance;
}

ChannelFavouritesModel::ChannelFavouritesModel()
{
}

void ChannelFavouritesModel::clear()
{
    favourites.clear();
}

QList<QString> ChannelFavouritesModel::getAll()
{
    return favourites;
}

void ChannelFavouritesModel::add(const QString &key)
{
    if (!favourites.contains(key))
        favourites.append(key);
}

void ChannelFavouritesModel::remove(const QString &key)
{
    if (favourites.contains(key))
        favourites.removeOne(key);
}

bool ChannelFavouritesModel::contains(const QString &key)
{
    return favourites.contains(key);
}
