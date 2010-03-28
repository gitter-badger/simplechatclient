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

#include "tab_manager.h"

tab_manager::tab_manager(QWidget *pMainWin, QSettings *param1) : QTabWidget(pMainWin)
{
    mainWin = pMainWin;
    settings = param1;
    tab = tabBar();
    this->setTabsClosable(true);
    alert_list = new QStringList();
    color = QColor(255, 0, 0, 255);
    timer = new QTimer(this);
    timer->setInterval(500);
    timer->start();
    QObject::connect(tab, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(flash_tab()));
}

void tab_manager::set_hilight(QString strName)
{
    if (tab_manager::get_settings_key(strName).isEmpty() == false)
        strName = tab_manager::get_settings_key(strName);

    int index = tab_manager::tab_pos(strName);
    tab->setTabTextColor(index, QColor(138, 0, 184, 255));

    // beep
    QString apath = QCoreApplication::applicationDirPath();
    Phonon::MediaObject *mediaObject = new Phonon::MediaObject(this);
    mediaObject->setCurrentSource(Phonon::MediaSource(apath+"/3rdparty/sounds/beep.wav"));
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::Path path = Phonon::createPath(mediaObject, audioOutput);
    mediaObject->play();
}

void tab_manager::set_alert(QString strName)
{
    if (tab_manager::get_settings_key(strName).isEmpty() == false)
        strName = tab_manager::get_settings_key(strName);

    if (tab_manager::tab_pos(strName) != -1)
    {
        if (alert_list->contains(strName) == false)
            alert_list->append(strName);
    }
}

void tab_manager::current_tab_changed(int index)
{
    QString strName = tab->tabText(index);
    mainWin->setWindowTitle(QString("Simple Chat Client - [%1]").arg(strName));

    // if strName not exist convert strName to ^c1f1..
    if (alert_list->contains(strName) == false)
    {
        if (settings->value("priv"+strName).toString().isEmpty() == false)
            strName = settings->value("priv"+strName).toString();
    }

    color = QColor(0,0,0);
    alert_list->removeAll(strName);
    tab->setTabTextColor(tab_manager::tab_pos(strName), color);
}

void tab_manager::flash_tab()
{
    if (color == QColor(255, 0, 0, 255))
        color = QColor(0, 0, 0, 255);
    else if (color == QColor(0, 0, 0, 255))
        color = QColor(255, 0, 0, 255);

    for (int i = 0; i < alert_list->count(); i++)
    {
        int index = tab_manager::tab_pos(alert_list->at(i));
        if (tab->tabTextColor(index) != QColor(138, 0, 184, 255))
            tab->setTabTextColor(index, color);
    }
}

int tab_manager::tab_pos(QString strName)
{
    if (tab_manager::get_settings_key(strName).isEmpty() == false)
        strName = tab_manager::get_settings_key(strName);

    for (int i = 0; i < tab->count(); i++)
    {
        if (tab->tabText(i) == strName)
            return i;
    }
    return -1;
}

QString tab_manager::get_settings_key(QString strFind)
{
    QStringList keys = settings->allKeys();
    for (int i = 0; i < keys.count(); i++)
    {
        if ((keys.at(i).length() > 4) && (settings->value(keys.at(i)).toString() == strFind))
            return keys.at(i).right(keys.at(i).length()-4);
    }
    return QString::null;
}
