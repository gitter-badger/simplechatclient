/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QPainter>
#include "avatar.h"
#include "core.h"
#include "defines.h"
#include "settings.h"
#include "themes.h"
#include "nicklist_delegate.h"

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

    QString strNicklistNickColor = Settings::instance()->get("nicklist_nick_color");
    QString strNicklistSelectedNickColor = Settings::instance()->get("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = Settings::instance()->get("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = Settings::instance()->get("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = Settings::instance()->get("nicklist_gradient_2_color");

    QPen fontPen(QColor(strNicklistNickColor), 1, Qt::SolidLine);
    QPen selectedFontPen(QColor(strNicklistSelectedNickColor), 1, Qt::SolidLine);
    QPen busyPen(QColor(strNicklistBusyNickColor), 1, Qt::SolidLine);
    QColor cGradient1 = QColor(strNicklistGradient1Color);
    QColor cGradient2 = QColor(strNicklistGradient2Color);

    bool selected;
    if (option.state & QStyle::State_Selected)
    {
        QLinearGradient backgroundGradient(option.rect.left(), option.rect.top(), option.rect.left(), option.rect.bottom());
        backgroundGradient.setColorAt(0.0, cGradient1);
        backgroundGradient.setColorAt(1.0, cGradient2);
        painter->fillRect(option.rect, QBrush(backgroundGradient));
        painter->setPen(selectedFontPen);
        selected = true;
    }
/*
#ifndef Q_WS_WIN
    else if (option.state & QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setPen(selectedFontPen);
        selected = true;
    }
#endif
*/
    else
    {
        painter->setPen(fontPen);
        selected = false;
    }

    QString nick = index.data(Qt::DisplayRole).toString();
    QString modes = index.data(NickListModesRole).toString();
    QString userAvatar = index.data(NickListAvatarUrlRole).toString();

    bool busy = false;
    bool statusIcons = true;

    QList<QIcon> icons;
    if (modes.contains(FLAG_BUSY)) { busy = true; }
    if (modes.contains(FLAG_CAM_PRIV)) { icons << QIcon(":/images/privcam.png"); }
    if (modes.contains(FLAG_CAM_PUB)) { icons << QIcon(":/images/pubcam.png"); }
    if (modes.contains(FLAG_VOICE)) { icons << QIcon(":/images/voice.png"); }
    if (modes.contains(FLAG_SCREENER)) { icons << QIcon(":/images/screener.png"); }
    if (modes.contains(FLAG_MOD)) { icons << QIcon(":/images/mod.png"); }
    if (modes.contains(FLAG_HALFOP)) { icons << QIcon(":/images/halfop.png"); }
    if (modes.contains(FLAG_OP)) { icons << QIcon(":/images/op.png"); }
    if (modes.contains(FLAG_OWNER)) { icons << QIcon(":/images/owner.png"); }
    if (modes.contains(FLAG_ADMIN)) { icons << QIcon(":/images/admin.png"); }
    if (modes.contains(FLAG_DEV)) { icons << QIcon(":/images/dev.png"); }

    // avatar
    if ((!nick.startsWith('~')) && (Themes::instance()->isCurrentWithNicklistAvatar()))
    {
        // is valid avatar
        if (!userAvatar.isEmpty())
            userAvatar = Avatar::instance()->getAvatarPath(userAvatar);
        else
            userAvatar = Avatar::instance()->getEmptyUserAvatar();

        QIcon avatar(userAvatar);

        int x = option.rect.left();
        int y = option.rect.top();
        int w = 35;
        int h = 35;
        avatar.paint(painter, x, y, w, h);
    }

    // nick
    if (Themes::instance()->isCurrentWithNicklistAvatar())
    {
        // with avatars
        if ((busy) && (!selected)) painter->setPen(busyPen); // gray

        QFont font = option.font;
        font.setWeight(busy ? QFont::Light : QFont::Normal);
        font.setItalic(busy ? true : false);
        painter->setFont(font);

        QRect rect = option.rect;
        if (!nick.startsWith('~')) rect.setX(rect.x()+40);
        painter->drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, nick);
    }
    else
    {
        // without avatars
        if ((busy) && (!selected)) painter->setPen(busyPen); // gray

        QFont font = option.font;
        font.setWeight(busy ? QFont::Light : QFont::Normal);
        font.setItalic(busy ? true : false);
        painter->setFont(font);

        painter->drawText(option.rect, Qt::AlignVCenter | Qt::AlignLeft, nick);
    }

    // status icons
    if (statusIcons)
    {
        QRect rect = option.rect;
        rect.setX(rect.right());

        foreach (QIcon icon, icons)
        {
            rect.setX(rect.x() - 16);
            icon.paint(painter, rect, Qt::AlignVCenter | Qt::AlignAbsolute, QIcon::Normal, QIcon::On);
        }
    }

    // restore
    painter->restore();
}

QSize NickListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED (index);

    if (Themes::instance()->isCurrentWithNicklistAvatar()) // with avatars
    {
        int w = 200;
        int h = 35;
        return QSize(w, h);
    }
    else // without avatars
    {
        int w = 170;
        int h = (option.fontMetrics.height() > 16 ? option.fontMetrics.height() : 16);
        return QSize(w, h);
    }
}
