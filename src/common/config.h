/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

class QDomElement;
#include <QHash>
#include <QDomDocument>
#include <QObject>

#include "scc-config.h"

/**
 * Class for config read/write
 */
class Config : public QObject
{
    Q_OBJECT
public:
    Config(bool _bProfileConfig = true, QString _strForceProfile = QString::null);

    /**
     * Get config value
     * @param QString key
     * @return QString value
     */
    QString getValue(const QString &strKey);
    /**
     * Set config key value
     * @param QString key
     * @param QString value
     */
    void setValue(const QString &strKey, const QString &strValue);
    /**
     * Read config to QHash
     * @return QHash all keys,values
     */
    QHash<QString,QString> readConfig();

private:
    QString strConfigFile;
    QDomDocument doc;
    bool bProfileConfig;
    QString strForceProfile;

    void removeValue(const QString &strKey);
    void fixConfig();
    void createNewConfig();
    QHash<QString,QString> getDefaultValues();
    void addConfigValue(QDomDocument *doc, QDomElement *root, const QString &strKey, const QString &strValue);
    void save();
};

#endif // CONFIG_H
