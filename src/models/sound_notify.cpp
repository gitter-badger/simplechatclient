/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include "sound_notify.h"

SoundNotify * SoundNotify::Instance = 0;

SoundNotify * SoundNotify::instance()
{
    if (!Instance)
    {
        Instance = new SoundNotify();
        Instance->init();
    }

    return Instance;
}

void SoundNotify::init()
{
#if (QT_VERSION >= 0x050000)
    music = new QMediaPlayer;
#else
    #if WITH_PHONON
        QString strSoundBeep = Settings::instance()->get("sound_beep");
        music = Phonon::createPlayer(Phonon::NotificationCategory, Phonon::MediaSource(strSoundBeep));
    #endif
#endif
}

SoundNotify::SoundNotify()
{
    eCurrentCategory = Beep;
}

SoundNotify::~SoundNotify()
{
#if (QT_VERSION >= 0x050000)
    delete music;
#else
    #if WITH_PHONON
        delete music;
    #endif
#endif
}

void SoundNotify::play(NotifyCategory eCategory)
{
#if (QT_VERSION >= 0x050000)
    if (eCategory == Query)
    {
        if (eCurrentCategory != Query)
        {
            QString strSoundQuery = Settings::instance()->get("sound_query");
            music->setMedia(QUrl::fromLocalFile(strSoundQuery));
            eCurrentCategory = Query;
        }
    }
    else
    {
        if (eCurrentCategory != Beep)
        {
            QString strSoundBeep = Settings::instance()->get("sound_beep");
            music->setMedia(QUrl::fromLocalFile(strSoundBeep));
            eCurrentCategory = Beep;
        }
    }

    thread.setMedia(music);
    thread.start();
    thread.quit();
    //thread.wait();
#else
    #if WITH_PHONON
        if (eCategory == Query)
        {
            if (eCurrentCategory != Query)
            {
                QString strSoundQuery = Settings::instance()->get("sound_query");
                music->setCurrentSource(strSoundQuery);
                eCurrentCategory = Query;
            }
        }
        else
        {
            if (eCurrentCategory != Beep)
            {
                QString strSoundBeep = Settings::instance()->get("sound_beep");
                music->setCurrentSource(strSoundBeep);
                eCurrentCategory = Beep;
            }
        }

        thread.setMedia(music);
        thread.start();
        thread.quit();
        //thread.wait();
    #else
        Q_UNUSED(eCategory)
    #endif
#endif
}

void SoundNotify::quit()
{
    if (!Instance)
        return;

    delete Instance;
    Instance = 0;
}
