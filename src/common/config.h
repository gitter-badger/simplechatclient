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

#include <QDomDocument>
#include <QFile>
#include <QMap>
#include <QObject>

/**
 * Class for config read/write
 */
class Config : public QObject
{
    Q_OBJECT
public:
    Config();
    virtual ~Config();

    /**
     * Get config value
     * @param QString key
     * @return QString value
     */
    QString getValue(QString);
    /**
     * Set config key value
     * @param QString key
     * @param QString value
     */
    void setValue(QString, QString);
    /**
     * Read config to QMap
     * @return QMap all keys,values
     */
    QMap<QString,QString> readConfig();

private:
    QString strConfigFile;
    QFile *file;
    QDomDocument doc;

    void removeValue(QString);
    void fixConfig();
    void createNewConfig();
    QMap<QString,QString> getDefaultValues();
    void addConfigValue(QDomDocument *, QDomElement *, QString, QString);
    void save();
};

#endif // CONFIG_H
