/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "core.h"
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

void Notify::init()
{
#if WITH_PHONON
    QString strSoundBeep = Core::instance()->settings.value("sound_beep");
    music = Phonon::createPlayer(Phonon::NotificationCategory, Phonon::MediaSource(strSoundBeep));
#endif
}

Notify::Notify()
{
    eCurrentCategory = Beep;
}

Notify::~Notify()
{
#if WITH_PHONON
    delete music;
#endif
}

void Notify::play(NotifyCategory eCategory)
{
#if WITH_PHONON
    music->stop();

    if (eCategory == Query)
    {
        if (eCurrentCategory != Query)
        {
            QString strSoundQuery = Core::instance()->settings.value("sound_query");
            music->setCurrentSource(strSoundQuery);
            eCurrentCategory = Query;
        }
    }
    else
    {
        if (eCurrentCategory != Beep)
        {
            QString strSoundBeep = Core::instance()->settings.value("sound_beep");
            music->setCurrentSource(strSoundBeep);
            eCurrentCategory = Beep;
        }
    }

    music->play();
#endif
}

void Notify::quit()
{
    if (!Instance)
        return;

    delete Instance;
    Instance = 0;
}
