/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XML_H
#define XML_H

class QDomElement;
#include <QHash>
#include <QDomDocument>
#include <QObject>

class Xml : public QObject
{
    Q_OBJECT
public:
    Xml(const QString &_strFile, const QString &_strRootName, const QHash<QString, QString> &_lDefaultValues);
    QString getValue(const QString &strKey);
    void setValue(const QString &strKey, const QString &strValue);
    QHash<QString,QString> readToHash();

private:
    QString strFile;
    QString strRootName;
    QHash<QString, QString> lDefaultValues;
    QDomDocument doc;

    void createNewFile();
    void removeKey(const QString &strKey);
    void addKeyValue(QDomDocument *doc, QDomElement *root, const QString &strKey, const QString &strValue);
    void save();

};

#endif // XML_H
