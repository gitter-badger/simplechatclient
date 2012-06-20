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

#include "channel_list_model.h"

ChannelList * ChannelList::Instance = 0;

ChannelList * ChannelList::instance()
{
    if (!Instance)
    {
        Instance = new ChannelList();
        Instance->init();
    }

    return Instance;
}

ChannelList::ChannelList()
{
}

void ChannelList::init()
{
    clear();
}

void ChannelList::clear()
{
    lChannelList.clear();
    bChannelListReady = false;
    iChannelListTime = 0;
}

void ChannelList::add(const OnetChannelList &channel)
{
    lChannelList.append(channel);
}

QList<OnetChannelList> ChannelList::get()
{
    return lChannelList;
}

void ChannelList::setTime(qint64 time)
{
    iChannelListTime = time;
}

qint64 ChannelList::getTime()
{
    return iChannelListTime;
}

void ChannelList::setReady(bool ready)
{
    bChannelListReady = ready;
}

bool ChannelList::getReady()
{
    return bChannelListReady;
}
