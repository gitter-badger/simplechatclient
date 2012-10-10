/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include "settings.h"
#include "themes_model.h"

ThemesModel * ThemesModel::Instance = 0;

ThemesModel * ThemesModel::instance()
{
    if (!Instance)
    {
        Instance = new ThemesModel();
        Instance->init();
    }

    return Instance;
}

ThemesModel::ThemesModel()
{
    add("Standard", false, false);
    add("Alhena", false, false);
    add("Origin", false, true);
    add("Adara", true, false);
}

void ThemesModel::init()
{
    updateCurrent();
}

void ThemesModel::add(const QString &name, bool withChannelAvatar, bool withNicklistAvatar)
{
    Theme add;
    add.name = name;
    add.withChannelAvatar = withChannelAvatar;
    add.withNicklistAvatar = withNicklistAvatar;

    lThemes.append(add);
}

Theme ThemesModel::get()
{
    QString currentName = Settings::instance()->get("themes");

    foreach (Theme _Theme, lThemes)
    {
        if (_Theme.name == currentName)
            return _Theme;
    }

    return lThemes.at(0);
}

void ThemesModel::updateCurrent()
{
    current = get();
}

QList<QString> ThemesModel::getAll()
{
    QList<QString> lThemesName;

    foreach (Theme _Theme, lThemes)
    {
        lThemesName.append(_Theme.name);
    }

    return lThemesName;
}

bool ThemesModel::isCurrentWithAvatar()
{
    if ((current.withChannelAvatar) || (current.withNicklistAvatar))
        return true;
    else
        return false;
}

bool ThemesModel::isCurrentWithChannelAvatar()
{
    return current.withChannelAvatar;
}

bool ThemesModel::isCurrentWithNicklistAvatar()
{
    return current.withNicklistAvatar;
}
