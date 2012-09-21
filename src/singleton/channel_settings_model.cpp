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

#include "channel_settings_model.h"

ChannelSettingsModel * ChannelSettingsModel::Instance = 0;

ChannelSettingsModel * ChannelSettingsModel::instance()
{
    if (!Instance)
    {
        Instance = new ChannelSettingsModel();
        Instance->clear();
    }

    return Instance;
}

ChannelSettingsModel::ChannelSettingsModel()
{
}

void ChannelSettingsModel::clear()
{
    strChannel.clear();

    lSettingsInfo.clear();
    lSettingsPermissions.clear();
    bReadyInfo = false;

    lSettingsStats.clear();
    bReadyStats = false;
}

QString ChannelSettingsModel::getChannel()
{
    return strChannel;
}

void ChannelSettingsModel::setChannel(const QString &channel)
{
    clear();

    strChannel = channel;
}

QString ChannelSettingsModel::getInfo(const QString &key)
{
    return lSettingsInfo.value(key, QString::null);
}

QHash<QString,QString> ChannelSettingsModel::getAllInfo()
{
    return lSettingsInfo;
}

void ChannelSettingsModel::setInfo(const QString &key, const QString &value)
{
    lSettingsInfo[key] = value;
}

QMultiHash<QString,QString> ChannelSettingsModel::getAllPermission()
{
    return lSettingsPermissions;
}

void ChannelSettingsModel::setPermission(const QString &key, const QString &value)
{
    lSettingsPermissions.insert(key, value);
}

bool ChannelSettingsModel::containsPermission(const QString &key, const QString &value)
{
    return lSettingsPermissions.contains(key, value);
}

bool ChannelSettingsModel::getReadyInfo()
{
    return bReadyInfo;
}

void ChannelSettingsModel::setReadyInfo(bool bReady)
{
    bReadyInfo = bReady;
}

QString ChannelSettingsModel::getStats(const QString &key)
{
    return lSettingsStats.value(key, QString::null);
}

QHash<QString,QString> ChannelSettingsModel::getAllStats()
{
    return lSettingsStats;
}

void ChannelSettingsModel::setStats(const QString &key, const QString &value)
{
    lSettingsStats[key] = value;
}

bool ChannelSettingsModel::getReadyStats()
{
    return bReadyStats;
}

void ChannelSettingsModel::setReadyStats(bool bReady)
{
    bReadyStats = bReady;
}
