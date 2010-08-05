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

TabManager::TabManager(QWidget *pMainWin, QSettings *param1) : QTabWidget(pMainWin)
{
    mainWin = pMainWin;
    settings = param1;
    tab = tabBar();
    this->setTabsClosable(true);
    color = QColor(255, 0, 0, 255);
    timer = new QTimer(this);
    timer->setInterval(500);
    timer->start();
    QObject::connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(close_requested(int)));
    QObject::connect(tab, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(flash_tab()));
}

TabManager::~TabManager()
{
    timer->stop();
    alert_list.clear();
}

void TabManager::close_requested(int index)
{
    // remove from tab
    //tab->removeTab(index);

    // remove from settings and alert list
    QStringList keys = settings->allKeys();
    for (int i = 0; i < keys.count(); i++)
    {
        if ((keys.at(i).length() > 4) && (keys.at(i).left(4) == "priv"))
        {
            alert_list.removeAll(settings->value(keys.at(i)).toString());
            settings->remove(keys.at(i));
        }
    }
}

void TabManager::set_hilight(QString strName)
{
    if (get_settings_key(strName).isEmpty() == false)
        strName = get_settings_key(strName);

    alert_list.removeAll(strName);
    int index = tab_pos(strName);
    tab->setTabTextColor(index, QColor(138, 0, 184, 255));
}

void TabManager::set_alert(QString strName)
{
    if (get_settings_key(strName).isEmpty() == false)
        strName = get_settings_key(strName);

    if (tab_pos(strName) != -1)
    {
        if (alert_list.contains(strName) == false)
            alert_list.append(strName);
    }
}

void TabManager::current_tab_changed(int index)
{
    QString strName = tab->tabText(index);
    mainWin->setWindowTitle(QString("Simple Chat Client - [%1]").arg(strName));

    // if strName not exist convert strName to ^c1f1..
    if (alert_list.contains(strName) == false)
    {
        if (settings->value("priv"+strName).toString().isEmpty() == false)
            strName = settings->value("priv"+strName).toString();
    }

    color = QColor(0,0,0);
    alert_list.removeAll(strName);
    tab->setTabTextColor(tab_pos(strName), color);
}

void TabManager::flash_tab()
{
    if (color == QColor(255, 0, 0, 255))
        color = QColor(0, 0, 0, 255);
    else if (color == QColor(0, 0, 0, 255))
        color = QColor(255, 0, 0, 255);

    for (int i = 0; i < alert_list.count(); i++)
    {
        int index = tab_pos(alert_list.at(i));
        if (tab->tabTextColor(index) != QColor(138, 0, 184, 255))
            tab->setTabTextColor(index, color);
    }
}

int TabManager::tab_pos(QString strName)
{
    if (get_settings_key(strName).isEmpty() == false)
        strName = get_settings_key(strName);

    for (int i = 0; i < tab->count(); i++)
    {
        if (tab->tabText(i) == strName)
            return i;
    }
    return -1;
}

QString TabManager::get_settings_key(QString strFind)
{
    QStringList keys = settings->allKeys();
    for (int i = 0; i < keys.count(); i++)
    {
        if ((keys.at(i).length() > 4) && (settings->value(keys.at(i)).toString() == strFind))
            return keys.at(i).right(keys.at(i).length()-4);
    }
    return QString::null;
}
