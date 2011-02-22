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

#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QPixmap>

// opencv
#ifndef Q_WS_WIN
#include <opencv/highgui.h>
#include <opencv/cv.h>
#endif

class Video : public QObject
{
    Q_OBJECT
public:
    Video();
    ~Video();
    void create();
    void destroy();
    QPixmap get_image();
    bool exist_video_device();
    QList<QString> get_video_devices();

private:
    bool bCreatedCaptureCv;
    CvCapture *captureCv; // camera

    IplImage *opencv_get_camera_image(); // get camera image
    QPixmap convert_cam2img(IplImage *); // convert camera image to pixmap

};

#endif // VIDEO_H
