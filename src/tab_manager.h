/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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
#include <Phonon/AudioOutput>
#include <Phonon/MediaObject>
#include <QSettings>
#include <QStringList>
#include <QTabBar>
#include <QTabWidget>
#include <QTimer>

class tab_manager : public QTabWidget
{
    Q_OBJECT
public:
    tab_manager(QWidget *, QSettings *);
    void set_hilight(QString);
    void set_alert(QString);
    int tab_pos(QString);

    QStringList *alert_list;

private:
    QTabBar *tab;
    QTimer *timer;
    QColor color;
    QWidget *mainWin;
    QSettings *settings;

    QString get_settings_key(QString);

private slots:
    void current_tab_changed(int);
    void flash_tab();

};

#endif // TAB_MANAGER_H
