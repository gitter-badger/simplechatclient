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

#include "qnicklistdelegate.h"

qnicklistdelegate::qnicklistdelegate(QObject *parent)
{
}

qnicklistdelegate::~qnicklistdelegate()
{
}

void qnicklistdelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect r = option.rect;

    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);

    //Color: #005A83
    QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);

    //Color: #333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    //Color: #fff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);

    if(option.state & QStyle::State_Selected)
    {
        QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
        gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
        gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
        gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
        painter->setBrush(gradientSelected);
        painter->drawRect(r);

        //BORDER
        painter->setPen(lineMarkedPen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontMarkedPen);
    }
    else
    {
        //BACKGROUND
        //ALTERNATING COLORS
        painter->setBrush((index.row() % 2) ? Qt::white : QColor(252,252,252));
        painter->drawRect(r);

        //BORDER
        painter->setPen(linePen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontPen);
    }

    //GET STATUS, AVATAR, NICK, DESCRIPTION
    QIcon status = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QIcon avatar = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole+2)));
    QString nick = index.data(Qt::UserRole).toString();
    //QString description = index.data(Qt::UserRole+1).toString();

    int imageSpace = 10;

    // STATUS
    r = option.rect.adjusted(imageSpace, 5, -5, -5);
    status.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
    imageSpace += 20;

    // AVATAR
    r = option.rect.adjusted(imageSpace, 0, -10, 0);
    avatar.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
    imageSpace += 30;

    // NICK
    r = option.rect.adjusted(imageSpace, -8, -10, -8);
    painter->setFont(QFont("Verdana", 9, QFont::Normal));
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, nick, &r);

    // DESCRIPTION
    //r = option.rect.adjusted(imageSpace, 35, -10, 0);
    //painter->setFont(QFont("Lucida Grande", 5, QFont::Normal));
    //painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);
}

QSize qnicklistdelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(200, 30);
}
