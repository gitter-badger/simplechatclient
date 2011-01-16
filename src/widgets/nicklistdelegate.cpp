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

#include <QPainter>
#include <QSettings>
#include "nicklistdelegate.h"

NickListDelegate::NickListDelegate(QObject *parent)
{
    Q_UNUSED (parent);
}

NickListDelegate::~NickListDelegate()
{
}

void NickListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QRect r = option.rect;

    QSettings settings;
    QString strNicklistNickColor = settings.value("nicklist_nick_color").toString();
    QString strNicklistSelectedNickColor = settings.value("nicklist_selected_nick_color").toString();
    QString strNicklistBusyNickColor = settings.value("nicklist_busy_nick_color").toString();
    QString strNicklistGradient1Color = settings.value("nicklist_gradient_1_color").toString();
    QString strNicklistGradient2Color = settings.value("nicklist_gradient_2_color").toString();
    QString strDisableAvatars = settings.value("disable_avatars").toString();

    QPen fontPen(QColor(strNicklistNickColor), 1, Qt::SolidLine);
    QPen selectedFontPen(QColor(strNicklistSelectedNickColor), 1, Qt::SolidLine);
    QPen busyPen(QColor(strNicklistBusyNickColor), 1, Qt::SolidLine);
    QColor cGradient1 = QColor(strNicklistGradient1Color);
    QColor cGradient2 = QColor(strNicklistGradient2Color);

    if (option.state & QStyle::State_Selected)
    {
        QLinearGradient backgroundGradient(option.rect.left(), option.rect.top(), option.rect.left(), option.rect.height()+option.rect.top());
        backgroundGradient.setColorAt(0.0, cGradient1);
        backgroundGradient.setColorAt(1.0, cGradient2);
        painter->fillRect(option.rect, QBrush(backgroundGradient));
        painter->setPen(selectedFontPen);
    }
#ifndef Q_WS_WIN
    else if (option.state & QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setPen(selectedFontPen);
    }
#endif
    else
        painter->setPen(fontPen);

    // get status, avatar, nick
    QIcon status = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole)));
    QIcon avatar = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole+1)));
    QString nick = index.data(Qt::DisplayRole).toString();
    bool bBusy = index.data(Qt::UserRole+10).toBool();

    int imageSpace = 0;

    // status
    if (status.isNull() == false)
    {
        if (strDisableAvatars == "off")
        {
            // with avatars
            r = option.rect.adjusted(imageSpace, -5, 5, 5);
            status.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace += 20;
        }
        else
        {
            // without avatars
            r = option.rect.adjusted(imageSpace, -4, 0, 4);
            status.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace += 20;
        }
    }

    // avatar
    if ((nick[0] != '~') && (strDisableAvatars == "off"))
    {
        if ((nick != tr("Developer(s)")) && (nick != tr("Admin(s)")) && (nick != tr("Owner(s)")) && (nick != tr("Op(s)")) && (nick != tr("HalfOp(s)")) && (nick != tr("Mod(s)")) && (nick != tr("Screener(s)")) && (nick != tr("Voice(s)")) && (nick != tr("Cam(s)")) && (nick != tr("User(s)")))
        {
            r = option.rect.adjusted(imageSpace, 0, 10, 0);
            avatar.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
            imageSpace += 35;

            r = option.rect.adjusted(imageSpace, 0, 10, 10);
            imageSpace += 10;
        }
    }

    // nick
    if (strDisableAvatars == "off")
    {
        // with avatars
        r = option.rect.adjusted(imageSpace, -10, -10, -10);
        if ((bBusy == true) && (!(option.state & QStyle::State_Selected))) painter->setPen(busyPen); // gray
        painter->setFont(QFont(option.font.family(), option.font.pointSize(), bBusy == true ? QFont::Light : QFont::Normal, bBusy));
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, nick, &r);
    }
    else
    {
        // without avatars
        r = option.rect.adjusted(imageSpace, 0, 0, 0);
        if ((bBusy == true) && (!(option.state & QStyle::State_Selected))) painter->setPen(busyPen); // gray
        painter->setFont(QFont(option.font.family(), option.font.pointSize(), bBusy == true ? QFont::Light : QFont::Normal, bBusy));
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, nick, &r);
    }

    painter->restore();
}

QSize NickListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED (option);
    Q_UNUSED (index);

    QSettings settings;
    QString strDisableAvatars = settings.value("disable_avatars").toString();

    if (strDisableAvatars == "off") // with avatars
        return QSize(180, 35);
    else // without avatars
        return QSize(100, 16);
}
