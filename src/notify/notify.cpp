/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 ****************************************************************************/

#include "notify.h"

Notify::Notify()
{
    apath = QCoreApplication::applicationDirPath();
    music = Phonon::createPlayer(Phonon::NotificationCategory, Phonon::MediaSource(apath+"/3rdparty/sounds/beep.wav"));
}

Notify::~Notify()
{
    delete music;
}

void Notify::play(QString strWhat)
{
    music->stop();

    if (strWhat == "query")
        music->setCurrentSource(apath+"/3rdparty/sounds/query.wav");
    else
        music->setCurrentSource(apath+"/3rdparty/sounds/beep.wav");

    music->play();
}
