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

#include "moderation_model.h"

ModerationModel * ModerationModel::Instance = 0;

ModerationModel * ModerationModel::instance()
{
    if (!Instance)
    {
        Instance = new ModerationModel();
        Instance->clear();
    }

    return Instance;
}

ModerationModel::ModerationModel()
{
}

void ModerationModel::clear()
{
    moderateMessages.clear();
}

QMultiHash<QString,OnetModerateMessage> ModerationModel::getAll()
{
    return moderateMessages;
}

void ModerationModel::set(const QString &id, const QString &channel, qint64 time, const QString &nick, const QString &message)
{
    OnetModerateMessage item;
    item.channel = channel;
    item.datetime = time;
    item.nick = nick;
    item.message = message;

    moderateMessages.insert(id, item);
}

void ModerationModel::remove(const QString &id)
{
    moderateMessages.remove(id);
}
