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

#ifndef CONFIG_H
#define CONFIG_H

#include <QCoreApplication>
#include <QDomDocument>
#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QFile>
#include <QObject>

class Config : public QObject
{
    Q_OBJECT
public:
    Config();
    ~Config();

    QString get_value(QString);
    void set_value(QString, QString);

private:
    QString strConfigFile;
    QFile *file;
    QDomDocument doc;

    void create_new_config();
    void save();

};

#endif // CONFIG_H
