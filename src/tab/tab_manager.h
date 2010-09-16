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

#ifndef TAB_MANAGER_H
#define TAB_MANAGER_H

#include <QCoreApplication>
#include <QKeyEvent>
#include <QSettings>
#include <QStringList>
#include <QTabBar>
#include <QTabWidget>
#include <QTimer>

class TabManager : public QTabWidget
{
    Q_OBJECT
public:
    TabManager(QWidget *);
    void set_hilight(QString);
    void set_alert(QString, QColor);
    int tab_pos(QString);
    QString tab_name(int);

private:
    QWidget *myparent;
    QTabBar *tab;

private slots:
    void current_tab_changed(int);

};

#endif // TAB_MANAGER_H
