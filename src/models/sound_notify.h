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

#ifndef SOUND_NOTIFY_H
#define SOUND_NOTIFY_H

#include "scc-config.h"
#if (QT_VERSION >= 0x050000)
    #include <QMediaPlayer>
#else
    #if WITH_PHONON
        #include <phonon/MediaObject>
    #endif
#endif
#include <QObject>
#include <QThread>

enum NotifyCategory {Beep, Query};

class SoundThread : public QThread
{
public:
    void run() { music->stop(); music->play(); exec(); }

#if (QT_VERSION >= 0x050000)
    void setMedia(QMediaPlayer *m) { music = m; }
private:
    QMediaPlayer *music;
#else
    #if WITH_PHONON
        void setMedia(Phonon::MediaObject *m) { music = m; }
private:
        Phonon::MediaObject *music;
    #endif
#endif
};

/**
 * Notify using Phonon
 */
class SoundNotify : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SoundNotify)
    static SoundNotify *Instance;

public:
    static SoundNotify *instance();

    SoundNotify();
    virtual ~SoundNotify();
    void play(NotifyCategory);

private:
    QString apath;
#if (QT_VERSION >= 0x050000)
    QMediaPlayer *music;
#else
    #if WITH_PHONON
        Phonon::MediaObject *music;
    #endif
#endif
    NotifyCategory eCurrentCategory;
    SoundThread thread;

    void init();
};

#endif // SOUND_NOTIFY_H
