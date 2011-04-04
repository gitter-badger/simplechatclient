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

#ifndef NOTIFY_H
#define NOTIFY_H

#include <Phonon/MediaObject>
#include <QObject>

enum NotifyCategory {Beep, Query};

/**
 * Notify using Phonon
 */
class Notify : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Notify)
    static Notify *Instance;

public:
    static Notify *instance();

    Notify();
    virtual ~Notify();
    void play(NotifyCategory);

public slots:
    void quit();

private:
    QString apath;
    Phonon::MediaObject *music;
    NotifyCategory eCurrentCategory;

    void init();
};

#endif // NOTIFY_H
