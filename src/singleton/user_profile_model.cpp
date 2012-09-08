/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr £Åuczko <piotr.luczko@gmail.com>
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

#include "user_profile_model.h"

UserProfileModel * UserProfileModel::Instance = 0;

UserProfileModel * UserProfileModel::instance()
{
    if (!Instance)
    {
        Instance = new UserProfileModel();
        Instance->init();
    }

    return Instance;
}

UserProfileModel::UserProfileModel()
{
}

void UserProfileModel::init()
{
    clear();
}

void UserProfileModel::clear()
{
    nick = QString::null;
    userProfile.clear();
    ready = false;
}

QString UserProfileModel::getNick()
{
    return nick;
}

void UserProfileModel::setNick(const QString &newNick)
{
    clear();

    nick = newNick;
}

QString UserProfileModel::get(const QString &key)
{
    return userProfile.value(key, QString::null);
}

QHash<QString,QString> UserProfileModel::getAll()
{
    return userProfile;
}

void UserProfileModel::set(const QString &key, const QString &value)
{
    userProfile[key] = value;
}

bool UserProfileModel::getReady()
{
    return ready;
}

void UserProfileModel::setReady(bool readyState)
{
    ready = readyState;
}
