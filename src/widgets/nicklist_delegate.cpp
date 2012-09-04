/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include "core.h"
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

    QString strNicklistNickColor = Core::instance()->settings.value("nicklist_nick_color");
    QString strNicklistSelectedNickColor = Core::instance()->settings.value("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = Core::instance()->settings.value("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = Core::instance()->settings.value("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = Core::instance()->settings.value("nicklist_gradient_2_color");
    QString strThemes = Core::instance()->settings.value("themes");

    QPen fontPen(QColor(strNicklistNickColor), 1, Qt::SolidLine);
    QPen selectedFontPen(QColor(strNicklistSelectedNickColor), 1, Qt::SolidLine);
    QPen busyPen(QColor(strNicklistBusyNickColor), 1, Qt::SolidLine);
    QColor cGradient1 = QColor(strNicklistGradient1Color);
    QColor cGradient2 = QColor(strNicklistGradient2Color);

    bool selected;
    if (option.state & QStyle::State_Selected)
    {
        QLinearGradient backgroundGradient(option.rect.left(), option.rect.top(), option.rect.left(), option.rect.height()+option.rect.top());
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
    QString modes = index.data(Qt::UserRole+12).toString();
    QString userAvatarPath = index.data(Qt::UserRole+13).toString();

    bool busy = false;

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
    if ((nick[0] != '~') && (strThemes == "Origin"))
    {
        QIcon avatar(userAvatarPath);

        int x = option.rect.left();
        int y = option.rect.top();
        avatar.paint(painter, x, y, 35, 35);
    }

    // nick
    if (strThemes == "Origin")
    {
        // with avatars
        if ((busy) && (!selected)) painter->setPen(busyPen); // gray
        painter->setFont(QFont(option.font.family(), option.font.pointSize(), busy ? QFont::Light : QFont::Normal, busy));

        int x = option.rect.left();
        if (nick[0] != '~') x += 40;
        int y = option.rect.top() + (option.rect.height() / 2) + (option.font.pointSize() / 2);
        painter->drawText(x, y, nick);
    }
    else
    {
        // without avatars
        if ((busy) && (!selected)) painter->setPen(busyPen); // gray
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

    QString strThemes = Core::instance()->settings.value("themes");

    if (strThemes == "Origin") // with avatars
        return QSize(200, 35);
    else // without avatars
        return QSize(170, 16);
}
