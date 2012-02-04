/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Dariusz Markowicz <dmarkowicz@alari.pl>             *
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

#include <QDebug>
#include <QRect>
#include <QStringList>

#include "my_avatar_model.h"

MyAvatarModel::MyAvatarModel()
{
    _angle = -1;
    _height = -1;
    _imgId = -1;
    _mApp = -1;
}

MyAvatarModel::~MyAvatarModel()
{
}

QString MyAvatarModel::getUrl() const
{
    if ((!_mSrv.isEmpty()) && (!_img.isEmpty()) && (_mApp >= 0) && (_angle >= 0) && (_imgId >= 0))
    {
        return QString("%1%2,1,%3,%4%5%6.jpg").arg(_mSrv, _img, QString::number(_mApp), _crop, _crop.isEmpty()?"":"-", QString::number(_angle));
    }
    else
    {
        return QString();
    }
}

QString MyAvatarModel::getRawUrl() const
{
    return getRawUrl(_angle);
}

QString MyAvatarModel::getRawUrl(int angle) const
{
    if ((!_mSrv.isEmpty()) && (!_img.isEmpty()) && (_mApp >= 0) && (_angle >= 0) && (_imgId >= 0))
    {
        return QString("%1%2,8,%3,%4.jpg").arg(_mSrv, _img, QString::number(_mApp), QString::number(angle));
    }
    else
    {
        return QString();
    }
}

int MyAvatarModel::rotateLeft()
{
    return rotate(LEFT);
}

int MyAvatarModel::rotateRight()
{
    return rotate(RIGHT);
}

int MyAvatarModel::angle() const
{
    return _angle;
}

int MyAvatarModel::mApp() const
{
    return _mApp;
}

QString MyAvatarModel::crop() const
{
    return _crop;
}

QString MyAvatarModel::desc() const
{
    return _desc;
}

int MyAvatarModel::height() const
{
    return _height;
}

QString MyAvatarModel::img() const
{
    return _img;
}

int MyAvatarModel::imgId() const
{
    return _imgId;
}

QString MyAvatarModel::mSrv() const
{
    return _mSrv;
}

int MyAvatarModel::width() const
{
    return _width;
}

void MyAvatarModel::setAngle(int angle)
{
    _angle = angle;
}

void MyAvatarModel::setMApp(int mApp)
{
    _mApp = mApp;
}

void MyAvatarModel::setCrop(QString crop)
{
    _crop = crop;
}

void MyAvatarModel::setDesc(QString desc)
{
    _desc = desc;
}

void MyAvatarModel::setHeight(int height)
{
    _height = height;
}

void MyAvatarModel::setImg(QString img)
{
    _img = img;
}

void MyAvatarModel::setImgId(int imgId)
{
    _imgId = imgId;
}

void MyAvatarModel::setMSrv(QString mSrv)
{
    _mSrv = mSrv;
}

void MyAvatarModel::setWidth(int width)
{
    _width = width;
}


int MyAvatarModel::rotate(Rotate i)
{
    _angle += i;

    if (_angle > 3)
        _angle = 0;
    if (_angle < 0)
        _angle = 3;

    return _angle;
}

QString MyAvatarModel::scaledCropToString(QRect crop, QSize scaled, QSize original, int angle, bool rotate)
{
    if (crop.isNull() || (crop.x() == 0 && crop.y() == 0 && crop.size() == scaled))
        return "";

    qreal scale = 1.0;
    if (angle % 2 == 0)
    {
        scale = (qreal) original.width() / (qreal) scaled.width();
    }
    else
    {
        scale = (qreal) original.width() / (qreal) scaled.height();
    }

    if (rotate)
    {
        int tmp;
        switch (angle)
        {
        case A_90:
            tmp = crop.x();
            crop.moveLeft(crop.y());
            crop.moveTop(scaled.width() - crop.width() - tmp);
            break;
        case A_180:
            crop.moveLeft(scaled.width() - crop.width() - crop.x());
            crop.moveTop(scaled.height() - crop.height() - crop.y());
            break;
        case A_270:
            tmp = crop.y();
            crop.moveTop(crop.x());
            crop.moveLeft(scaled.height() - crop.height() - tmp);
            break;
        }
    }

    crop.moveLeft((int) (crop.x() * scale));
    crop.moveTop((int) (crop.y() * scale));
    crop.setWidth((int) (crop.width() * scale));
    crop.setHeight((int) (crop.height() * scale));

    QString s = QString("%1-%2-%3-%4").arg(QString::number(crop.x()), QString::number(crop.y()),
        QString::number(crop.width()), QString::number(crop.height()));

    //qDebug() << "scaledCropToString: string:" << s << "crop:" << crop << "scale:" << scale
    //    << "orgW:" << original.width() << "orgH:" << original.height() << "scaledW:"
    //    << scaled.width() << "scaledH:" << scaled.height() << "angle:" << angle;

    return s;
}

QRect MyAvatarModel::stringToScaledCrop(QString s, QSize scaled, QSize original, int angle, bool rotate)
{
    QStringList result = s.split("-");
    if (result.size() != 4)
        return QRect();

    if (!scaled.isValid())
        return QRect();

    qreal scale = 1.0;
    if (angle % 2 == 0)
    {
        scale = (qreal) scaled.width() / (qreal) original.width();
    }
    else
    {
        scale = (qreal) scaled.height() / (qreal) original.width();
    }
    QRect crop = QRect(result[0].toInt(), result[1].toInt(), result[2].toInt(), result[3].toInt());

    if (rotate)
    {
        int tmp;
        switch (angle)
        {
        case A_90:
            tmp = crop.x();
            crop.moveLeft(original.height() - crop.y() - crop.height());
            crop.moveTop(tmp);
            break;
        case A_180:
            crop.moveLeft(original.width() - crop.x() - crop.width());
            crop.moveTop(original.height() - crop.y() - crop.height());
            break;
        case A_270:
            tmp = crop.y();
            crop.moveTop(original.width() - crop.x() - crop.width());
            crop.moveLeft(tmp);
            break;
        }
    }

    crop.moveLeft((int) (crop.x() * scale));
    crop.moveTop((int) (crop.y() * scale));
    crop.setWidth((int) (crop.width() * scale));
    crop.setHeight((int) (crop.height() * scale));

    //qDebug() << "stringToScaledCrop: string:" << s << "crop:" << crop << "scale:" << scale
    //    << "orgW:" << original.width() << "orgH:" << original.height() << "scaledW:"
    //    << scaled.width() << "scaledH:" << scaled.height() << "angle:" << angle;

    return crop;
}

QSize MyAvatarModel::size() const
{
    return QSize(_width, _height);
}

void MyAvatarModel::debug(QString title) const
{
    qDebug() << title << "angle:" << angle() << "crop:" << crop() << "height:" << height() << "img:" << img() << "imgId:" << imgId() << "width:" << width() << "desc:" << desc() << "mApp:" << mApp() << "mSrv:" << mSrv();
}
