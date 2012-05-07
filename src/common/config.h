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

class Xml;
class QDomElement;
#include <QHash>
#include <QObject>

/**
 * Class for config read/write
 */
class Config : public QObject
{
    Q_OBJECT
public:
    Config(bool _bProfileConfig = true, QString _strForceProfile = QString::null);
    virtual ~Config();

    QString getValue(const QString &strKey);
    void setValue(const QString &strKey, const QString &strValue);
    QHash<QString,QString> readToHash();

private:
    QString strConfigFile;
    bool bProfileConfig;
    QString strForceProfile;
    QHash<QString,QString> lDefaultValues;
    Xml *xml;

    void fixConfig();
    QHash<QString,QString> getDefaultValues();
};

#endif // CONFIG_H
