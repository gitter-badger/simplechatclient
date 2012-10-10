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

#ifndef THEMES_MODEL_H
#define THEMES_MODEL_H

#include "defines.h"
#include <QObject>

class ThemesModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ThemesModel)
    static ThemesModel *Instance;
public:
    static ThemesModel *instance();

    ThemesModel();
    void updateCurrent();
    QList<QString> getAll();
    bool isCurrentWithAvatar();
    bool isCurrentWithChannelAvatar();
    bool isCurrentWithNicklistAvatar();

private:
    QList<Theme> lThemes;
    Theme current;

    void add(const QString &name, bool withNicklistAvatar, bool withChannelAvatar);
    Theme get();
};

#endif // THEMES_MODEL_H
