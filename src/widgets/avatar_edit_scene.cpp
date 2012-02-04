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
#include <QtGui>
#include <QPainter>
#include <QRubberBand>

#include "avatar_edit_scene.h"

// TODO: redesign in Qt way ;)
// i.e. crop as opacity mask not a copy
AvatarEditScene::AvatarEditScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 480, 360);

    isSizing = false;
    isDragged = false;

    crop = QRect(0, 0, 100, 100);

    // cropper image
    QPixmap c = QPixmap(":/images/crop.png");
    pixCropper = QPixmap(c.size());
    pixCropper.fill(Qt::white);
    QPainter painter(&pixCropper);
    painter.drawPixmap(c.rect(), c);
    // cropper position
    cropper = pixCropper.rect();

    // background
    setBackgroundBrush(QPixmap(":/images/transparent.png"));
    // scaled photo
    gpiPhoto = addPixmap(QPixmap());
    // gray it down
    griPhotoBlur = addRect(QRect(), Qt::NoPen, QBrush(QColor(255, 255, 255, 127)));
    // crop
    gpiCrop = addPixmap(QPixmap());
    // cropper
    gpiCropper = addPixmap(pixCropper);
    gpiCropper->setOpacity(0.5);
}

AvatarEditScene::~AvatarEditScene()
{
}

void AvatarEditScene::setPhoto(const QPixmap &pixPhoto, const QRect &crop)
{
    this->crop = crop;
    this->pixPhoto = pixPhoto;

    // scaled photo
    gpiPhoto->setPixmap(this->pixPhoto);
    // gray it down
    griPhotoBlur->setRect(gpiPhoto->boundingRect());

    redraw();
}

void AvatarEditScene::redraw()
{
    // crop
    QPixmap c = pixPhoto.copy(crop);
    // crop border
    QPainter cp(&c);
    cp.setPen(Qt::DotLine);
    cp.setCompositionMode(QPainter::CompositionMode_Clear);
    cp.drawRect(0, 0, crop.width() - 1, crop.height() - 1);
    gpiCrop->setPixmap(c);
    gpiCrop->setPos(crop.topLeft());
    // cropper
    cropper.moveCenter(crop.bottomRight());
    gpiCropper->setPos(cropper.topLeft());
}

void AvatarEditScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QGraphicsView *view = qobject_cast<QGraphicsView*>(event->widget()->parent());
    Q_ASSERT(view);
    QPoint p(view->mapFromScene(event->scenePos()));

    if (!isSizing && crop.contains(p))
    {
        isDragged = true;
        view->setCursor(Qt::ClosedHandCursor);
        origin = p;
    }
    else if (!isDragged && cropper.contains(p))
    {
        isSizing = true;
        view->setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsScene::mousePressEvent(event);
}

void AvatarEditScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    QGraphicsView *view = qobject_cast<QGraphicsView*>(event->widget()->parent());
    Q_ASSERT(view);
    QPoint p(view->mapFromScene(event->scenePos()));

    if (isDragged)
    {
        crop.moveLeft(qMax(0, crop.x() + (p.x() - origin.x())));
        crop.moveTop(qMax(0, crop.y() + (p.y() - origin.y())));
        crop.moveLeft(qMin(pixPhoto.width() - crop.width(), crop.x()));
        crop.moveTop(qMin(pixPhoto.height() - crop.height(), crop.y()));
        origin = p;

        redraw();
        emit cropChanged(crop);
    }
    else if (isSizing)
    {
        crop.setWidth(qMax(1, p.x() - crop.x()));
        crop.setWidth(
            qMin(qMin(pixPhoto.width() - crop.x(), pixPhoto.height() - crop.y()), crop.width()));
        crop.setHeight(crop.width());

        redraw();
        emit cropChanged(crop);
    }
}

void AvatarEditScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QGraphicsView *view = qobject_cast<QGraphicsView*>(event->widget()->parent());
    Q_ASSERT(view);

    view->setCursor(Qt::ArrowCursor);

    isDragged = false;
    isSizing = false;

    QGraphicsScene::mouseMoveEvent(event);
}

