/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#include "qnotify.h"

qnotify::qnotify()
{
}

void qnotify::play()
{
    QString apath = QCoreApplication::applicationDirPath();
    Phonon::MediaObject *mediaObject = new Phonon::MediaObject();
    mediaObject->setTickInterval(1*1000); // 1 sec
    mediaObject->setCurrentSource(Phonon::MediaSource(apath+"/3rdparty/sounds/beep.wav"));
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::NotificationCategory);
    Phonon::Path audioPath = Phonon::createPath(mediaObject, audioOutput);

    mediaObject->play();

#ifdef Q_WS_X11
    QEventLoop loop;
    QObject::connect(mediaObject, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
#endif
    delete audioOutput;
    delete mediaObject;
}
