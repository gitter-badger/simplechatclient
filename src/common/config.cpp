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

#include <QCoreApplication>
#include <QDir>
#ifndef Q_WS_X11
#include <QSettings>
#endif
#include "config.h"

#ifdef Q_WS_X11
#include <QDebug>
#endif

Config::Config()
{
    QString path;
#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
    settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    path = settings.value("Personal").toString();
    path += "/scc";
#endif

    // create dir if not exist
    QDir d(path);
    if (d.exists(path) == false)
        d.mkdir(path);

    strConfigFile = path+"/scc.conf";
    file = new QFile(strConfigFile);

    // if not exist - create new
    if (file->exists() == false)
        create_new_config();

    // try read
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
        // fix config
        fix_config();
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

    // remove
    remove_value(strKey);

    // add value
    QDomElement docElem = doc.documentElement();
    QDomElement eSetKey = doc.createElement(strKey);
    docElem.appendChild(eSetKey);
    QDomText tSetValue = doc.createTextNode(strValue);
    eSetKey.appendChild(tSetValue);

    // save
    save();
}

void Config::remove_value(QString strKey)
{
    if ((doc.isNull() == true) || (file->isOpen() == false))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: config: Cannot remove value: ") << strKey;
#endif
        return;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    // remove if exist
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

    // save
    save();
}

void Config::fix_config()
{
    QMap<QString,QString> mDefaultValues = get_default_values();
    QMapIterator<QString, QString> i(mDefaultValues);
    while (i.hasNext())
    {
        i.next();
        QString strDefaultKey = i.key();
        QString strDefaultValue = i.value();

        QString strValue = get_value(strDefaultKey);
        if (strValue.isEmpty() == true)
        {
            if (strDefaultKey != "pass") // ignore pass
                set_value(strDefaultKey, strDefaultValue);
        }
    }
}

void Config::create_new_config()
{
    doc.clear();
    QDomElement root = doc.createElement("config");
    doc.appendChild(root);

    QMap<QString,QString> mDefaultValues = get_default_values();
    QMapIterator<QString, QString> i(mDefaultValues);
    while (i.hasNext())
    {
        i.next();
        QString strDefaultKey = i.key();
        QString strDefaultValue = i.value();

        add_config_value(&doc, &root, strDefaultKey, strDefaultValue);
    }

    // save
    save();
}

QMap<QString,QString> Config::get_default_values()
{
    QString path = QCoreApplication::applicationDirPath();
    QString strSoundBeep = path+"/3rdparty/sounds/beep.wav";
    QString strSoundQuery = path+"/3rdparty/sounds/query.wav";
    QString strBackgroundImage = path+"/images/wallpaper/default.jpg";

    QMap<QString,QString> mDefaultValues;

    mDefaultValues.insert("first_run", "true");
    mDefaultValues.insert("nick", "~test");
    mDefaultValues.insert("pass", "");
    mDefaultValues.insert("language", "pl");
    mDefaultValues.insert("auto_busy", "off");
    mDefaultValues.insert("disable_autojoin_favourites", "off");
    mDefaultValues.insert("show_zuo", "off");
    mDefaultValues.insert("hide_formating", "off");
    mDefaultValues.insert("hide_join_part", "off");
    mDefaultValues.insert("hide_join_part_200", "on");
    mDefaultValues.insert("disable_avatars", "on");
    mDefaultValues.insert("disable_emots", "off");
    mDefaultValues.insert("disable_replaces", "off");
    mDefaultValues.insert("style", "modern");
    mDefaultValues.insert("background_color", "#ffffff");
    mDefaultValues.insert("my_bold", "off");
    mDefaultValues.insert("my_italic", "off");
    mDefaultValues.insert("my_font", "Verdana");
    mDefaultValues.insert("my_color", "#000000");
    mDefaultValues.insert("font_size", "11px");
    mDefaultValues.insert("default_font_color", "#000000");
    mDefaultValues.insert("font_color_level_1", "#009300");
    mDefaultValues.insert("font_color_level_2", "#4733FF");
    mDefaultValues.insert("font_color_level_3", "#00007F");
    mDefaultValues.insert("font_color_level_4", "#00007F");
    mDefaultValues.insert("font_color_level_5", "#009300");
    mDefaultValues.insert("font_color_level_6", "#0066FF");
    mDefaultValues.insert("font_color_level_7", "#666666");
    mDefaultValues.insert("font_color_level_9", "#ff0000");
    mDefaultValues.insert("channel_font_color", "#0000ff");
    mDefaultValues.insert("nicklist_nick_color", "#333333");
    mDefaultValues.insert("nicklist_selected_nick_color", "#ffffff");
    mDefaultValues.insert("nicklist_busy_nick_color", "#a0a0a4");
    mDefaultValues.insert("nicklist_gradient_1_color", "#77d5f7");
    mDefaultValues.insert("nicklist_gradient_2_color", "#1b86b7");
    mDefaultValues.insert("disable_logs", "off");
    mDefaultValues.insert("sound_beep", strSoundBeep);
    mDefaultValues.insert("sound_query", strSoundQuery);
    mDefaultValues.insert("disable_sounds", "off");
    mDefaultValues.insert("background_image", strBackgroundImage);
    mDefaultValues.insert("disable_background_image", "off");
    mDefaultValues.insert("spellchecker", "off");

    return mDefaultValues;
}

QMap<QString,QString> Config::read_config()
{
    QMap<QString, QString> mResult;

    if ((doc.isNull() == false) && (file->isOpen() == true))
    {
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();

        while (n.isNull() == false)
        {
            QDomElement e = n.toElement();

            // save to map
            if (e.isNull() == false)
                mResult.insert(e.tagName(), e.text());

            n = n.nextSibling();
        }
    }

    return mResult;
}

void Config::add_config_value(QDomDocument *doc, QDomElement *root, QString strKey, QString strValue)
{
    QDomElement eKey = doc->createElement(strKey);
    root->appendChild(eKey);
    QDomText tValue = doc->createTextNode(strValue);
    eKey.appendChild(tValue);
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
