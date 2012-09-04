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

#include "core.h"
#include "mainwindow.h"
#include "channel.h"

Channel * Channel::Instance = 0;

Channel * Channel::instance()
{
    if (!Instance)
    {
        Instance = new Channel();
        Instance->init();
    }

    return Instance;
}

Channel::Channel()
{
}

void Channel::init()
{
    lChannels.clear();
    lAvatar.clear();
    lPriv.clear();
}

void Channel::add(const QString &channel)
{
    lChannels.append(channel);
}

void Channel::remove(const QString &channel)
{
    lChannels.removeOne(channel);
}

QList<QString> Channel::get()
{
    return lChannels;
}

void Channel::move(int from, int to)
{
    lChannels.move(from, to);
}

bool Channel::contains(const QString &channel)
{
    return lChannels.contains(channel);
}

QList<QString> Channel::getCleared()
{
    QList<QString> lChannelsCleared = lChannels;
    lChannelsCleared.removeOne(DEBUG);
    lChannelsCleared.removeOne(STATUS);
    return lChannelsCleared;
}

QString Channel::getFromIndex(int index)
{
    return lChannels.value(index, QString::null);
}

int Channel::getIndex(const QString &channel)
{
    return lChannels.indexOf(channel);
}

QString Channel::getCurrent()
{
    int index = Core::instance()->mainWindow()->getCurrentTabIndex();
    return lChannels.value(index, QString::null);
}

QList<CaseIgnoreString> Channel::getSorted()
{
    QList<CaseIgnoreString> lChannelsCaseIgnore;

    // copy to new list
    QList<QString> lChannels = getCleared();

    foreach (QString strChannel, lChannels)
        lChannelsCaseIgnore.append(strChannel);

    // sort
    qSort(lChannelsCaseIgnore.begin(), lChannelsCaseIgnore.end());

    return lChannelsCaseIgnore;
}

// avatar
QString Channel::getAvatar(const QString &channel)
{
    return lAvatar.value(channel, QString::null);
}

void Channel::setAvatar(const QString &channel, const QString &path)
{
    lAvatar[channel] = path;
}

// priv
void Channel::setPriv(const QString &channel, const QString &name)
{
    lPriv[channel] = name;
}

QString Channel::getPriv(const QString &channel)
{
    return lPriv.value(channel, channel);
}

bool Channel::containsPriv(const QString &channel)
{
    return lPriv.contains(channel);
}
