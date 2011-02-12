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

#include "tab_manager.h"

TabManager::TabManager(QWidget *parent) : QTabWidget(parent)
{
    myparent = parent;
    tab = tabBar();
    setTabsClosable(true);
}

void TabManager::set_hilight(int index)
{
    tab->setTabTextColor(index, QColor(138, 0, 184, 255));
}

void TabManager::set_alert(int index, QColor color)
{
    // if not hilighted and not current color
    if ((tab->tabTextColor(index) != QColor(138, 0, 184, 255)) && (tab->tabTextColor(index) != color))
        tab->setTabTextColor(index, color);
}

void TabManager::set_color(int index, QColor color)
{
    tab->setTabTextColor(index, color);
}

void TabManager::hide_close_button(int index)
{
    tab->setTabButton(index, QTabBar::RightSide, 0);
}

void TabManager::tabInserted(int index)
{
    if (index == 0) // hide close on status
        hide_close_button(index);
}
