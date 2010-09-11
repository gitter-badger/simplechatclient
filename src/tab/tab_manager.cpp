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

#include "tab_manager.h"

TabManager::TabManager(QWidget *parent) : QTabWidget(parent)
{
    myparent = parent;
    tab = tabBar();
    setTabsClosable(true);
    QObject::connect(tab, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
}

TabManager::~TabManager()
{
}

void TabManager::set_hilight(QString strName)
{
    int index = tab_pos(strName);
    tab->setTabTextColor(index, QColor(138, 0, 184, 255));
}

void TabManager::set_alert(QString strName, QColor cColor)
{
    int index = tab_pos(strName);

    // if not hilighted and not current color
    if ((tab->tabTextColor(index) != QColor(138, 0, 184, 255)) && (tab->tabTextColor(index) != cColor))
        tab->setTabTextColor(index, cColor);
}

int TabManager::tab_pos(QString strName)
{
    QSettings settings;
    QStringList strlChannelNames = settings.value("channel_names").toStringList();

    int i = 0;
    QStringListIterator strliChannelNames (strlChannelNames);
    while (strliChannelNames.hasNext())
    {
        if (strliChannelNames.next() == strName)
            return i;
        i++;
    }

    return -1;
}

QString TabManager::tab_name(int index)
{
    if (index == -1) return QString::null; // prevent crash

    QSettings settings;
    QStringList strlChannelNames = settings.value("channel_names").toStringList();

    if (index < strlChannelNames.size())
        return strlChannelNames.at(index);
    else
        return QString::null;
}

void TabManager::current_tab_changed(int index)
{
    QString strTabText = tabText(index);
    myparent->setWindowTitle(QString("Simple Chat Client - [%1]").arg(strTabText));

    QString strName = tab_name(index);
    tab->setTabTextColor(tab_pos(strName), QColor(0,0,0));
}
