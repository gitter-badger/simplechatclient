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
#include "core.h"
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
    // save
    painter->save();

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

    QString nick = index.data(Qt::DisplayRole).toString();
    QString channel = index.data(Qt::UserRole+10).toString();

    bool busy = false;

    QString modes = Core::instance()->get_user_modes(nick, channel);

    QList<QIcon> icons;
    if (modes.contains("b")) { busy = true; }
    if (modes.contains("V")) { icons << QIcon(":/images/privcam.png"); }
    if (modes.contains("W")) { icons << QIcon(":/images/pubcam.png"); }
    if (modes.contains("+")) { icons << QIcon(":/images/voice.png"); }
    if (modes.contains("=")) { icons << QIcon(":/images/screener.png"); }
    if (modes.contains("!")) { icons << QIcon(":/images/mod.png"); }
    if (modes.contains("%")) { icons << QIcon(":/images/halfop.png"); }
    if (modes.contains("@")) { icons << QIcon(":/images/op.png"); }
    if (modes.contains("`")) { icons << QIcon(":/images/owner.png"); }
    if (modes.contains("o")) { icons << QIcon(":/images/admin.png"); }
    if (modes.contains("O")) { icons << QIcon(":/images/dev.png"); }

    // avatar
    if ((nick[0] != '~') && (strDisableAvatars == "off"))
    {
        QPixmap pAvatar;
        pAvatar.loadFromData(Core::instance()->mNickAvatar.value(nick));
        QIcon avatar(pAvatar);

        int x = option.rect.left();
        int y = option.rect.top();
        avatar.paint(painter, x, y, 35, 35);
    }

    // nick
    if (strDisableAvatars == "off")
    {
        // with avatars
        if ((busy) && (!(option.state & QStyle::State_Selected)) && (!(option.state & QStyle::State_MouseOver))) painter->setPen(busyPen); // gray
        painter->setFont(QFont(option.font.family(), option.font.pointSize(), busy ? QFont::Light : QFont::Normal, busy));

        int x = option.rect.left();
        if (nick[0] != '~') x += 40;
        int y = option.rect.top() + (option.rect.height() / 2) + (option.font.pointSize() / 2);
        painter->drawText(x, y, nick);
    }
    else
    {
        // without avatars
        if ((busy) && (!(option.state & QStyle::State_Selected)) && (!(option.state & QStyle::State_MouseOver))) painter->setPen(busyPen); // gray
        painter->setFont(QFont(option.font.family(), option.font.pointSize(), busy ? QFont::Light : QFont::Normal, busy));

        painter->drawText(option.rect, nick);
    }

    // status
    int x = option.rect.right();
    int y = option.rect.top() + (option.rect.height() - 16) / 2;

    foreach (QIcon icon, icons)
    {
        x -= 16;
        icon.paint(painter, x, y, 16, 16, Qt::AlignAbsolute, QIcon::Normal, QIcon::On);
    }

    // restore
    painter->restore();
}

QSize NickListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED (option);
    Q_UNUSED (index);

    QSettings settings;
    QString strDisableAvatars = settings.value("disable_avatars").toString();

    if (strDisableAvatars == "off") // with avatars
        return QSize(150, 35);
    else // without avatars
        return QSize(100, 16);
}
