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

#ifndef CONFIG_H
#define CONFIG_H

#include <QCoreApplication>
#include <QDomDocument>
#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QDir>
#include <QFile>
#include <QObject>
#ifndef Q_WS_X11
#include <QSettings>
#endif

/**
 * Class for config read/write
 * @param bool create config
 */
class Config : public QObject
{
    Q_OBJECT
public:
    Config(bool bCreate = true);
    ~Config();

    /**
     * Get config value
     * @param QString key
     * @return QString value
     */
    QString get_value(QString);
    /**
     * Read config to QMap
     * @return QMap all keys,values
     */
    QMap <QString, QString> read_config();
    /**
     * Set config key value
     * @param QString key
     * @param QString value
     */
    void set_value(QString, QString);

private:
    bool bCreateConfig;
    QString strConfigFile;
    QFile *file;
    QDomDocument doc;

    void create_new_config();
    void add_config_value(QDomDocument *, QDomElement *, QString, QString);
    void save();

};

#endif // CONFIG_H
