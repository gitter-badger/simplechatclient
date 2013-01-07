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

#include "channel_homes_model.h"

ChannelHomes * ChannelHomes::Instance = 0;

ChannelHomes * ChannelHomes::instance()
{
    if (!Instance)
    {
        Instance = new ChannelHomes();
        Instance->clear();
    }

    return Instance;
}

ChannelHomes::ChannelHomes()
{
}

void ChannelHomes::clear()
{
    lChannelHomes.clear();

    channelHomesStatus = StatusAwaiting;
}

void ChannelHomes::add(const QString &channel)
{
    if (!lChannelHomes.contains(channel))
        lChannelHomes.append(channel);
}

QList<QString> ChannelHomes::get()
{
    return lChannelHomes;
}

QList<QString> ChannelHomes::getCleared()
{
    QList<QString> lChannelHomesCleared;

    QList<QString> lChannelHomes = get();
    foreach (QString strChannel, lChannelHomes)
    {
        if (strChannel[0] != '#')
            strChannel = strChannel.right(strChannel.length()-1); // remove status

        lChannelHomesCleared.append(strChannel);
    }

    return lChannelHomesCleared;
}

bool ChannelHomes::contains(const QString &channel)
{
    QList<QString> lChannelHomesCleared = getCleared();
    return lChannelHomesCleared.contains(channel);
}

void ChannelHomes::setStatus(ObjectStatus status)
{
    channelHomesStatus = status;
}

ObjectStatus ChannelHomes::getStatus()
{
    return channelHomesStatus;
}
