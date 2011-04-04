/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include <QSettings>
#include "notify.h"

Notify * Notify::Instance = 0;

Notify * Notify::instance()
{
    if (!Instance)
    {
        Instance = new Notify();
        Instance->init();
    }

    return Instance;
}

Notify::Notify()
{
    eCurrentCategory = Beep;
}

Notify::~Notify()
{
    delete music;
}

void Notify::init()
{
    QSettings settings;
    QString strSoundBeep = settings.value("sound_beep").toString();
    music = Phonon::createPlayer(Phonon::NotificationCategory, Phonon::MediaSource(strSoundBeep));
}

void Notify::play(NotifyCategory eCategory)
{
    music->stop();

    QSettings settings;
    if (eCategory == Query)
    {
        if (eCurrentCategory != Query)
        {
            QString strSoundQuery = settings.value("sound_query").toString();
            music->setCurrentSource(strSoundQuery);
            eCurrentCategory = Query;
        }
    }
    else
    {
        if (eCurrentCategory != Beep)
        {
            QString strSoundBeep = settings.value("sound_beep").toString();
            music->setCurrentSource(strSoundBeep);
            eCurrentCategory = Beep;
        }
    }

    music->play();
}

void Notify::quit()
{
    if (!Instance)
        return;

    delete Instance;
    Instance = 0;
}
