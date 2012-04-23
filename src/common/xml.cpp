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

#include <QDir>
#include <QFile>
#include <QTextStream>
#include "core.h"
#include "xml.h"

Xml::Xml(const QString &_strFile, const QString &_strRootName, const QHash<QString,QString> &_lDefaultValues) : strFile(_strFile), strRootName(_strRootName), lDefaultValues(_lDefaultValues)
{
    QFile file(strFile);

    // if not exist - create new
    if (!file.exists())
        createNewFile();

    // try read
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream ts(&file);
            QString strData = ts.readAll();
            file.close();

            if (!doc.setContent(strData))
            {
                if (Core::instance()->settings.value("debug") == "true")
                    qDebug() << QString(tr("Error: Cannot set content from xml file %1")).arg(strFile);
            }
        }
        else
        {
            if (Core::instance()->settings.value("debug") == "true")
                qDebug() << QString(tr("Error: Cannot read xml file %1")).arg(strFile);
        }
    }
    else
    {
        if (Core::instance()->settings.value("debug") == "true")
            qDebug() << QString(tr("Error: Cannot open xml file %1")).arg(strFile);
    }
}

QString Xml::getValue(const QString &strKey)
{
    if (doc.isNull())
    {
        if (Core::instance()->settings.value("debug") == "true")
            qDebug() << QString(tr("Error: Cannot get xml value for key %1 from file %2")).arg(strKey, strFile);
        return QString::null;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while (!n.isNull())
    {
        QDomElement e = n.toElement();
        if (!e.isNull())
        {
            if (e.tagName() == strKey)
                return e.text();
        }
        n = n.nextSibling();
    }

    return QString::null;
}

void Xml::setValue(const QString &strKey, const QString &strValue)
{
    if (doc.isNull())
    {
        if (Core::instance()->settings.value("debug") == "true")
            qDebug() << QString(tr("Error: Cannot set xml value for key %1 from file %2")).arg(strKey, strFile);
        return;
    }

    // remove
    removeKey(strKey);

    // add value
    QDomElement docElem = doc.documentElement();
    QDomElement eSetKey = doc.createElement(strKey);
    docElem.appendChild(eSetKey);
    QDomText tSetValue = doc.createTextNode(strValue);
    eSetKey.appendChild(tSetValue);

    // save
    save();
}

QHash<QString,QString> Xml::readToHash()
{
    QHash<QString, QString> lResult;

    if (!doc.isNull())
    {
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();

        while (!n.isNull())
        {
            QDomElement e = n.toElement();

            // save to map
            if (!e.isNull())
                lResult.insert(e.tagName(), e.text());

            n = n.nextSibling();
        }
    }

    return lResult;
}

void Xml::createNewFile()
{
    doc.clear();
    QDomElement root = doc.createElement(strRootName);
    doc.appendChild(root);

    QHashIterator<QString, QString> i(lDefaultValues);
    while (i.hasNext())
    {
        i.next();
        QString strKey = i.key();
        QString strValue = i.value();

        addKeyValue(&doc, &root, strKey, strValue);
    }

    // save
    save();
}

void Xml::removeKey(const QString &strKey)
{
    if (doc.isNull())
    {
        if (Core::instance()->settings.value("debug") == "true")
            qDebug() << QString(tr("Error: Cannot remove xml value for key %1 from file %2")).arg(strKey, strFile);
        return;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    // remove if exist
    while (!n.isNull())
    {
        QDomElement e = n.toElement();
        if (!e.isNull())
        {
            if (e.tagName() == strKey)
            {
                docElem.removeChild(e);
                docElem.removeChild(e);
            }
        }
        n = n.nextSibling();
    }

    // save
    save();
}

void Xml::addKeyValue(QDomDocument *doc, QDomElement *root, const QString &strKey, const QString &strValue)
{
    QDomElement eKey = doc->createElement(strKey);
    root->appendChild(eKey);
    QDomText tValue = doc->createTextNode(strValue);
    eKey.appendChild(tValue);
}

void Xml::save()
{
    QString xml = doc.toString();

    QFile f(strFile);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&f);
        out << xml;

        f.close();
    }
    else
    {
        if (Core::instance()->settings.value("debug") == "true")
            qDebug() << QString(tr("Error: Cannot save xml file %1")).arg(strFile);
    }
}
