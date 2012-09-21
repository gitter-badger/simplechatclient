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

#include "my_profile_model.h"

MyProfileModel * MyProfileModel::Instance = 0;

MyProfileModel * MyProfileModel::instance()
{
    if (!Instance)
    {
        Instance = new MyProfileModel();
        Instance->clear();
    }

    return Instance;
}

MyProfileModel::MyProfileModel()
{
}

void MyProfileModel::clear()
{
    myProfile.clear();
}

QString MyProfileModel::get(const QString &key)
{
    return myProfile.value(key, QString::null);
}

QHash<QString,QString> MyProfileModel::getAll()
{
    return myProfile;
}

void MyProfileModel::set(const QString &key, const QString &value)
{
    myProfile[key] = value;
}
