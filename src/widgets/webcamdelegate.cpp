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

#include <QBrush>
#include <QPainter>
#include <QPen>
#include "webcamdelegate.h"

WebcamDelegate::WebcamDelegate(QObject *parent)
{
    Q_UNUSED (parent);
}

void WebcamDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // save
    painter->save();

    QString nick = index.data(Qt::DisplayRole).toString();
    nick = nick.left(19); // short nick
    int rank = index.data(Qt::UserRole).toInt();
    QString spectators = index.data(Qt::UserRole+1).toString();

    // state
    if (option.state & QStyle::State_MouseOver)
        painter->fillRect(option.rect, option.palette.highlight());

    // draw nick
    painter->drawText(option.rect, nick);

    // x y
    int x = option.rect.right();
    int y = option.rect.top() + (option.rect.height() / 2) + (option.font.pointSize() / 2);

    // spectators
    painter->drawText(x-80,y,spectators);

    // rank
    x -= 50;
    y = option.rect.top();
    int w = 50;
    int half_w = x+w/2;
    int h = option.rect.height();

    // rank background
    painter->setBrush(Qt::white);
    painter->setPen(Qt::white);
    painter->drawRect(x,y,w,h);

    // correct rank
    if (rank > 500) rank = 500;
    if (rank < -500) rank = -500;

    if (rank < 0)
    {
        int new_w = qAbs(rank)/20;

        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(half_w - new_w, y, new_w, h);
    }
    else if (rank > 0)
    {
        int new_w = rank/20;

        painter->setBrush(Qt::green);
        painter->setPen(Qt::green);
        painter->drawRect(half_w, y, new_w, h);
    }

    // zero
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->drawLine(half_w, y, half_w, h);

    // border
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->drawLine(x,y,x,h); // left
    painter->drawLine(x+w-1,y,x+w-1,h-1); // right
    painter->drawLine(x,y,x+w-1,y); // top
    painter->drawLine(x,y+h-1,x+w-1,y+w-1); // bottom

    // restore
    painter->restore();
}

QSize WebcamDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED (option);
    Q_UNUSED (index);

    return QSize(230,16);
}
