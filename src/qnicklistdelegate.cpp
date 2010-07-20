/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

NicklistDelegate::NicklistDelegate(QObject *parent)
{
}

NicklistDelegate::~NicklistDelegate()
{
}

void NicklistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect r = option.rect;

    // #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);

    // #005A83
    QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);

    // #333333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    // #ffffff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);

    if(option.state & QStyle::State_Selected)
    {
        QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
        gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
        gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
        gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
        painter->setBrush(gradientSelected);
        painter->drawRect(r);

        // border
        painter->setPen(lineMarkedPen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontMarkedPen);
    }
    else
    {
        // background

        Config *pConfig = new Config();
        QString strBackgroundColor = pConfig->get_value("background_color");
        delete pConfig;

        if (QColor("#"+strBackgroundColor) != Qt::white)
            painter->setBrush(QColor("#"+strBackgroundColor));
        else
            painter->setBrush((index.row() % 2) ? Qt::white : QColor(252,252,252));
        painter->drawRect(r);

        // border
        painter->setPen(linePen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontPen);
    }

    // get status, cam, avatar, nick, description
    QIcon status = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QIcon cam = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole+1)));
    QIcon avatar = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole+2)));
    QString nick = index.data(Qt::UserRole).toString();
    //QString description = index.data(Qt::UserRole+3).toString();

    Config *pConfig = new Config();
    QString strDisableAvatars = pConfig->get_value("disable_avatars");
    QString strStyle = pConfig->get_value("style");
    delete pConfig;

    int imageSpace = 10;

    // status
    if (status.isNull() == false)
    {
        if (strStyle == "modern")
        {
            r = option.rect.adjusted(imageSpace, -5, 5, 5);
            status.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace += 20;
        }
        else if (strStyle == "classic")
        {
            imageSpace = 5;
            r = option.rect.adjusted(imageSpace, -4, 0, 4);
            status.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace += 10;
        }
        r = option.rect.adjusted(imageSpace, 0, 10, 10);
        imageSpace += 10;
    }

    // cam
    if (cam.isNull() == false)
    {
        if (strStyle == "modern")
        {
            r = option.rect.adjusted(imageSpace, 0, 20, 0);
            cam.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace += 20;
        }
        else if (strStyle == "classic")
        {
            r = option.rect.adjusted(imageSpace, 0, 20, 0);
            cam.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace += 10;
        }

        r = option.rect.adjusted(imageSpace, 0, 10, 10);
        imageSpace += 10;
    }

    // avatar
    if ((nick[0] != '~') && (strStyle == "modern") && (strDisableAvatars == "off"))
    {
        r = option.rect.adjusted(imageSpace, 0, 10, 0);
        avatar.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
        imageSpace += 35;

        r = option.rect.adjusted(imageSpace, 0, 10, 10);
        imageSpace += 10;
    }

    // nick
    if (strStyle == "modern")
    {
        r = option.rect.adjusted(imageSpace, -8, -10, -8);
        painter->setFont(QFont("Verdana", 9, QFont::Normal));
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, nick, &r);
    }
    else if (strStyle == "classic")
    {
        r = option.rect.adjusted(imageSpace, -4, 0, -4);
        painter->setFont(QFont("Verdana", 9, QFont::Normal));
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, nick, &r);
    }

    // description
    //r = option.rect.adjusted(imageSpace, 35, -10, 0);
    //painter->setFont(QFont("Lucida Grande", 5, QFont::Normal));
    //painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);
}

QSize NicklistDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Config *pConfig = new Config();
    QString strStyle = pConfig->get_value("style");
    delete pConfig;

    if (strStyle == "modern")
        return QSize(180, 35);
    else if (strStyle == "classic")
        return QSize(100, 24);
}
