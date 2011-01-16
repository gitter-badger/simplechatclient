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

Config::Config(bool bCreate)
{
    bCreateConfig = bCreate;

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
    if ((file->exists() == false) && (bCreateConfig == true))
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
    }
    else
    {
#ifdef Q_WS_X11
        if (bCreateConfig == true)
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
    // if config file was opened successfully
    if ((doc.isNull() == false) && (file->isOpen() == true))
    {
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
    }

    // do not create config
    if (bCreateConfig == false)
        return QString::null;

    // not exist value - save default, return default

    QString path = QCoreApplication::applicationDirPath();
    QString strSoundBeep = path+"/3rdparty/sounds/beep.wav";
    QString strSoundQuery = path+"/3rdparty/sounds/query.wav";

    QStringList strlKeys;
    strlKeys << "nick" << "pass" << "language" << "auto_busy" << "disable_autojoin_favourites" << "show_zuo" << "hide_formating" << "hide_join_part" << "hide_join_part_200" << "disable_avatars" << "disable_emots" << "disable_replaces" << "style" << "background_color" << "my_bold" << "my_italic" << "my_font" << "my_color" << "font_size" << "default_font_color" << "font_color_level_1" << "font_color_level_2" << "font_color_level_3" << "font_color_level_4" << "font_color_level_5" << "font_color_level_6" << "font_color_level_7" << "font_color_level_9" << "channel_font_color" << "nicklist_nick_color" << "nicklist_selected_nick_color" << "nicklist_busy_nick_color" << "nicklist_gradient_1_color" << "nicklist_gradient_2_color" << "disable_logs" << "sound_beep" << "sound_query" << "disable_sounds";
    QStringList strlValues;
    strlValues << "~test" << "" << "pl" << "off" << "off" << "off" << "off" << "off" << "on" << "on" << "off" << "off" << "modern" << "#ffffff" << "off" << "off" << "Verdana" << "#000000" << "11px" << "#000000" << "#009300" << "#4733FF" << "#00007F" << "#00007F" << "#009300" << "#0066FF" << "#666666" << "#ff0000" << "#0000ff" << "#333333" << "#ffffff" << "#a0a0a4" << "#77d5f7" << "#1b86b7" << "off" << strSoundBeep << strSoundQuery << "off";

    for (int i = 0; i < strlKeys.count(); i++)
    {
        if (strKey == strlKeys.at(i))
        {
            set_value(strlKeys.at(i), strlValues.at(i));
            return strlValues.at(i);
        }
    }

#ifdef Q_WS_X11
    // exception: iv
    if ((bCreateConfig == true) && (strKey != "iv"))
        qDebug() << tr("Error: config: Cannot get value: ") << strKey;
#endif

    return QString::null;
}

QMap <QString, QString> Config::read_config()
{
    QMap <QString, QString> mResult;

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

    QString path = QCoreApplication::applicationDirPath();
    QString strSoundBeep = path+"/3rdparty/sounds/beep.wav";
    QString strSoundQuery = path+"/3rdparty/sounds/query.wav";

    QStringList strlKeys;
    strlKeys << "nick" << "pass" << "language" << "auto_busy" << "disable_autojoin_favourites" << "show_zuo" << "hide_formating" << "hide_join_part" << "hide_join_part_200" << "disable_avatars" << "disable_emots" << "disable_replaces" << "style" << "background_color" << "my_bold" << "my_italic" << "my_font" << "my_color" << "font_size" << "default_font_color" << "font_color_level_1" << "font_color_level_2" << "font_color_level_3" << "font_color_level_4" << "font_color_level_5" << "font_color_level_6" << "font_color_level_7" << "font_color_level_9" << "channel_font_color" << "nicklist_nick_color" << "nicklist_selected_nick_color" << "nicklist_busy_nick_color" << "nicklist_gradient_1_color" << "nicklist_gradient_2_color" << "disable_logs" << "sound_beep" << "sound_query" << "disable_sounds";
    QStringList strlValues;
    strlValues << "~test" << "" << "pl" << "off" << "off" << "off" << "off" << "off" << "on" << "on" << "off" << "off" << "modern" << "#ffffff" << "off" << "off" << "Verdana" << "#000000" << "11px" << "#000000" << "#009300" << "#4733FF" << "#00007F" << "#00007F" << "#009300" << "#0066FF" << "#666666" << "#ff0000" << "#0000ff" << "#333333" << "#ffffff" << "#a0a0a4" << "#77d5f7" << "#1b86b7" << "off" << strSoundBeep << strSoundQuery << "off";

    for (int i = 0; i < strlKeys.count(); i++)
        add_config_value(&doc, &root, strlKeys.at(i), strlValues.at(i));

    save();
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
