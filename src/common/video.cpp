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

#include <QFile>
#include <QDebug>
#include "video.h"

Video::Video()
{
    bCreatedCaptureCv = false;
}

Video::~Video()
{
    destroy();
}

void Video::create()
{
    if (bCreatedCaptureCv == false)
    {
        // create capture
        captureCv = cvCreateCameraCapture(CV_CAP_ANY);
        if (!captureCv)
            qWarning() << "Error: cannot create camera capture!";
        else
            bCreatedCaptureCv = true;
    }
}

void Video::destroy()
{
    // destroy capture
    if (bCreatedCaptureCv == true)
    {
        cvReleaseCapture(&captureCv);
        bCreatedCaptureCv = false;
    }
}

QPixmap Video::get_image()
{
    return convert_cam2img(opencv_get_camera_image());
}

bool Video::exist_video_device()
{
    // search video device
    for (int i = 0; i < 99; i++)
    {
        if (QFile::exists("/dev/video"+QString::number(i)) == true)
            return true;
    }
    return false;
}

QList<QString> Video::get_video_devices()
{
    QList<QString> lVideoDevices;

    // get video devices
    for (int i = 0; i < 99; i++)
    {
        QString strDevice = "/dev/video"+QString::number(i);
        if (QFile::exists(strDevice) == true)
            lVideoDevices.append(strDevice);
    }
    return lVideoDevices;
}

IplImage *Video::opencv_get_camera_image()
{
    IplImage *img;

    if (bCreatedCaptureCv == true)
        img = cvQueryFrame(captureCv);

    return img;
}

QPixmap Video::convert_cam2img(IplImage *img)
{
    if (bCreatedCaptureCv == false) return QPixmap(); // error?

    int height = img->height;
    int width = img->width;

    if (img->depth == IPL_DEPTH_8U && img->nChannels == 3)
    {
        const uchar *qImageBuffer = (const uchar*)img->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
        return QPixmap::fromImage(img.rgbSwapped());
    }
    else
    {
        qWarning() << "Camera image cannot be converted.";
        return QPixmap();
    }
}
