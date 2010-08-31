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

#include "nicklistdelegate.h"

NicklistDelegate::NicklistDelegate(QObject *parent)
{
    Q_UNUSED (parent);
}

NicklistDelegate::~NicklistDelegate()
{
}

void NicklistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect r = option.rect;

    Config *pConfigInit = new Config();
    QString strNicklistBackgroundColor = pConfigInit->get_value("nicklist_background_color");
    QString strNicklistLineColor = pConfigInit->get_value("nicklist_line_color");
    QString strNicklistSelectedLineColor = pConfigInit->get_value("nicklist_selected_line_color");
    QString strNicklistNickColor = pConfigInit->get_value("nicklist_nick_color");
    QString strNicklistSelectedNickColor = pConfigInit->get_value("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = pConfigInit->get_value("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = pConfigInit->get_value("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = pConfigInit->get_value("nicklist_gradient_2_color");
    QString strNicklistGradient3Color = pConfigInit->get_value("nicklist_gradient_3_color");
    delete pConfigInit;

    // default #c4c4c4
    QPen linePen(QColor(strNicklistLineColor), 1, Qt::SolidLine);

    // default #005a83
    QPen selectedLinePen(QColor(strNicklistSelectedLineColor), 1, Qt::SolidLine);

    // default #333333
    QPen fontPen(QColor(strNicklistNickColor), 1, Qt::SolidLine);

    // default #ffffff
    QPen selectedFontPen(QColor(strNicklistSelectedNickColor), 1, Qt::SolidLine);

    // default #a0a0a4
    QPen busyPen(QColor(strNicklistBusyNickColor), 1, Qt::SolidLine);

    QColor cGradient1 = QColor(strNicklistGradient1Color);
    QColor cGradient2 = QColor(strNicklistGradient2Color);

    if (option.state & QStyle::State_Selected)
    {
        QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
        gradientSelected.setColorAt(0.0, cGradient1);
        gradientSelected.setColorAt(1.0, cGradient2);
        painter->setBrush(gradientSelected);
        painter->drawRect(r);

        // border
        painter->setPen(selectedLinePen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(selectedFontPen);
    }
    else
    {
        // background
        if (QColor(strNicklistBackgroundColor) != Qt::white)
            painter->setBrush(QColor(strNicklistBackgroundColor));
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

    // get status, cam, avatar, nick, description, cam, busy
    QIcon status = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QIcon cam = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole+1)));
    QIcon avatar = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole+2)));
    QString nick = index.data(Qt::UserRole).toString();
    //QString description = index.data(Qt::UserRole+3).toString();
    bool bCam = index.data(Qt::UserRole+10).toBool();
    bool bBusy = index.data(Qt::UserRole+11).toBool();
    Q_UNUSED (bCam);

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
        if ((bBusy == true) && (!(option.state & QStyle::State_Selected))) painter->setPen(busyPen); // gray
        painter->setFont(QFont("Verdana", 9, bBusy == true ? QFont::Light : QFont::Normal, bBusy));
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, nick, &r);
    }
    else if (strStyle == "classic")
    {
        r = option.rect.adjusted(imageSpace, -4, 0, -4);
        if ((bBusy == true) && (!(option.state & QStyle::State_Selected))) painter->setPen(busyPen); // gray
        painter->setFont(QFont("Verdana", 9, bBusy == true ? QFont::Light : QFont::Normal, bBusy));
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, nick, &r);
    }

    // description
    //r = option.rect.adjusted(imageSpace, 35, -10, 0);
    //painter->setFont(QFont("Lucida Grande", 5, QFont::Normal));
    //painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);
}

QSize NicklistDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED (option);
    Q_UNUSED (index);

    Config *pConfig = new Config();
    QString strDisableAvatars = pConfig->get_value("disable_avatars");
    QString strStyle = pConfig->get_value("style");
    delete pConfig;

    if (strStyle == "modern")
    {
        if (strDisableAvatars == "off") // modern with avatars
            return QSize(180, 35);
        else // modern no avatars
            return QSize(140, 35);
    }
    else if (strStyle == "classic")
        return QSize(100, 24);
    else // default modern no avatars
        return QSize(140, 35);
}
