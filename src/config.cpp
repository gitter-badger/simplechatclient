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

#include "config.h"

Config::Config()
{
    QString path = QCoreApplication::applicationDirPath();
    strConfigFile = path+"/scc.conf";
    file = new QFile(strConfigFile);

    if (file->exists() == false)
        create_new_config();

    if (file->exists() == true)
    {
        if (file->open(QIODevice::ReadWrite) == false)
        {
#ifdef Q_WS_X11
            qDebug() << tr("Error: config: Cannot read config file!");
#endif
            return;
        }
        if (doc.setContent(file) == false)
        {
#ifdef Q_WS_X11
            qDebug() << tr("Error: config: Cannot set content from file!");
#endif
            return;
        }
    }
    else
    {
#ifdef Q_WS_X11
            qDebug() << tr("Error: config: Cannot open config file!");
#endif
            return;
    }
}

Config::~Config()
{
    file->close();
    delete file;
}

void Config::save()
{
    QString xml = doc.toString();
    QByteArray baSave = xml.toAscii();

    QFile *fs = new QFile(strConfigFile);
    fs->open(QIODevice::WriteOnly | QIODevice::Truncate);
    fs->write(baSave);
    fs->close();
    delete fs;
}

QString Config::get_value(QString strKey)
{
    if ((doc.isNull() == true) || (file->isOpen() == false))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: config: Cannot get value: ") << strKey;
#endif
        return QString::null;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while (n.isNull() == false)
    {
        QDomElement e = n.toElement();
        if (e.isNull() == false)
        {
            if (e.tagName() == strKey)
                return e.text();
        }
        n = n.nextSibling();
    }

    return QString::null;
}

void Config::set_value(QString strKey, QString strValue)
{
    if ((doc.isNull() == true) || (file->isOpen() == false))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: config: Cannot set value: ") << strKey;
#endif
        return;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while (n.isNull() == false)
    {
        QDomElement e = n.toElement();
        if (e.isNull() == false)
        {
            if (e.tagName() == strKey)
            {
                docElem.removeChild(e);
                docElem.removeChild(e);
            }
        }
        n = n.nextSibling();
    }

    QDomElement eSetKey = doc.createElement(strKey);
    docElem.appendChild(eSetKey);
    QDomText tSetValue = doc.createTextNode(strValue);
    eSetKey.appendChild(tSetValue);

    save();
}

void Config::create_new_config()
{
    doc.clear();
    QDomElement root = doc.createElement("config");
    doc.appendChild(root);

    add_config_value(&doc, &root, "login-nick", "~test");
    add_config_value(&doc, &root, "login-pass", QString::null);
    add_config_value(&doc, &root, "auto_busy", "off");
    add_config_value(&doc, &root, "debug_all", "off");
    add_config_value(&doc, &root, "show_zuo", "off");
    add_config_value(&doc, &root, "hide_formating", "off");
    add_config_value(&doc, &root, "hide_join_part", "off");
    add_config_value(&doc, &root, "disable_avatars", "off");

    save();
}

void Config::add_config_value(QDomDocument *doc, QDomElement *root, QString strKey, QString strValue)
{
    QDomElement eKey = doc->createElement(strKey);
    root->appendChild(eKey);
    QDomText tValue = doc->createTextNode(strValue);
    eKey.appendChild(tValue);
}
