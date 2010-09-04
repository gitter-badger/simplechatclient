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

Config::Config(bool bCreate)
{
    bCreateConfig = bCreate;

    QString path = QCoreApplication::applicationDirPath();
    strConfigFile = path+"/scc.conf";
    file = new QFile(strConfigFile);

    if ((file->exists() == false) && (bCreateConfig == true))
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

    // not create config
    if (bCreateConfig == false)
        return QString::null;

    // not exist value - save default, return default

    if (strKey == "login-nick")
    {
        set_value("login-nick", "~test");
        return "~test";
    }
    else if (strKey == "login-pass")
    {
        set_value("login-pass", QString::null);
        return QString::null;
    }
    else if (strKey == "language")
    {
        set_value("language", "pl");
        return "pl";
    }
    else if (strKey == "auto_busy")
    {
        set_value("auto_busy", "off");
        return "off";
    }
    else if (strKey == "show_zuo")
    {
        set_value("show_zuo", "off");
        return "off";
    }
    else if (strKey == "hide_formating")
    {
        set_value("hide_formating", "off");
        return "off";
    }
    else if (strKey == "hide_join_part")
    {
        set_value("hide_join_part", "off");
        return "off";
    }
    else if (strKey == "hide_join_part_200")
    {
        set_value("hide_join_part_200", "on");
        return "on";
    }
    else if (strKey == "disable_avatars")
    {
        set_value("disable_avatars", "on");
        return "on";
    }
    else if (strKey == "disable_logs")
    {
        set_value("disable_logs", "off");
        return "off";
    }
    else if (strKey == "disable_sounds")
    {
        set_value("disable_sounds", "off");
        return "off";
    }
    else if (strKey == "style")
    {
        set_value("style", "modern");
        return "modern";
    }
    else if (strKey == "background_color")
    {
        set_value("background_color", "#ffffff");
        return "#ffffff";
    }
    else if (strKey == "my_bold")
    {
        set_value("my_bold", "off");
        return "off";
    }
    else if (strKey == "my_italic")
    {
        set_value("my_italic", "off");
        return "off";
    }
    else if (strKey == "my_font")
    {
        set_value("my_font", "Verdana");
        return "Verdana";
    }
    else if (strKey == "my_color")
    {
        set_value("my_color", "#000000");
        return "#000000";
    }
    else if (strKey == "default_font_color")
    {
        set_value("default_font_color", "#000000");
        return "#000000";
    }
    else if (strKey == "font_color_level_1")
    {
        set_value("font_color_level_1", "#009300");
        return "#009300";
    }
    else if (strKey == "font_color_level_2")
    {
        set_value("font_color_level_2", "#4733FF");
        return "#4733FF";
    }
    else if (strKey == "font_color_level_3")
    {
        set_value("font_color_level_3", "#00007F");
        return "#00007F";
    }
    else if (strKey == "font_color_level_4")
    {
        set_value("font_color_level_4", "#00007F");
        return "#00007F";
    }
    else if (strKey == "font_color_level_5")
    {
        set_value("font_color_level_5", "#009300");
        return "#009300";
    }
    else if (strKey == "font_color_level_6")
    {
        set_value("font_color_level_6", "#0066FF");
        return "#0066FF";
    }
    else if (strKey == "font_color_level_7")
    {
        set_value("font_color_level_7", "#666666");
        return "#666666";
    }
    else if (strKey == "font_color_level_9")
    {
        set_value("font_color_level_9", "#ff0000");
        return "#ff0000";
    }
    else if (strKey == "channel_font_color")
    {
        set_value("channel_font_color", "#0000ff");
        return "#0000ff";
    }
    else if (strKey == "nicklist_background_color")
    {
        set_value("nicklist_background_color", "#ffffff");
        return "#ffffff";
    }
    else if (strKey == "nicklist_line_color")
    {
        set_value("nicklist_line_color", "#c4c4c4");
        return "#c4c4c4";
    }
    else if (strKey == "nicklist_selected_line_color")
    {
        set_value("nicklist_selected_line_color", "#005a83");
        return "#005a83";
    }
    else if (strKey == "nicklist_nick_color")
    {
        set_value("nicklist_nick_color", "#333333");
        return "#333333";
    }
    else if (strKey == "nicklist_selected_nick_color")
    {
        set_value("nicklist_selected_nick_color", "#ffffff");
        return "#ffffff";
    }
    else if (strKey == "nicklist_busy_nick_color")
    {
        set_value("nicklist_busy_nick_color", "#a0a0a4");
        return "#a0a0a4";
    }
    else if (strKey == "nicklist_gradient_1_color")
    {
        set_value("nicklist_gradient_1_color", "#77d5f7");
        return "#77d5f7";
    }
    else if (strKey == "nicklist_gradient_2_color")
    {
        set_value("nicklist_gradient_2_color", "#1b86b7");
        return "#1b86b7";
    }

#ifdef Q_WS_X11
    // exception: iv
    if ((bCreateConfig == true) && (strKey != "iv"))
        qDebug() << tr("Error: config: Cannot get value: ") << strKey;
#endif

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
    add_config_value(&doc, &root, "language", "pl");
    add_config_value(&doc, &root, "auto_busy", "off");
    add_config_value(&doc, &root, "show_zuo", "off");
    add_config_value(&doc, &root, "hide_formating", "off");
    add_config_value(&doc, &root, "hide_join_part", "off");
    add_config_value(&doc, &root, "hide_join_part_200", "on");
    add_config_value(&doc, &root, "disable_avatars", "on");
    add_config_value(&doc, &root, "disable_logs", "off");
    add_config_value(&doc, &root, "disable_sounds", "off");
    add_config_value(&doc, &root, "style", "modern");
    add_config_value(&doc, &root, "my_bold", "off");
    add_config_value(&doc, &root, "my_italic", "off");
    add_config_value(&doc, &root, "my_font", "Verdana");
    add_config_value(&doc, &root, "my_color", "#000000");
    add_config_value(&doc, &root, "background_color", "#ffffff");
    add_config_value(&doc, &root, "default_font_color", "#000000");
    add_config_value(&doc, &root, "font_color_level_1", "#009300");
    add_config_value(&doc, &root, "font_color_level_2", "#4733FF");
    add_config_value(&doc, &root, "font_color_level_3", "#00007F");
    add_config_value(&doc, &root, "font_color_level_4", "#00007F");
    add_config_value(&doc, &root, "font_color_level_5", "#009300");
    add_config_value(&doc, &root, "font_color_level_6", "#0066FF");
    add_config_value(&doc, &root, "font_color_level_7", "#666666");
    add_config_value(&doc, &root, "font_color_level_9", "#ff0000");
    add_config_value(&doc, &root, "channel_font_color", "#0000ff");
    add_config_value(&doc, &root, "nicklist_background_color", "#ffffff");
    add_config_value(&doc, &root, "nicklist_line_color", "#c4c4c4");
    add_config_value(&doc, &root, "nicklist_selected_line_color", "#005a83");
    add_config_value(&doc, &root, "nicklist_nick_color", "#333333");
    add_config_value(&doc, &root, "nicklist_selected_nick_color", "#ffffff");
    add_config_value(&doc, &root, "nicklist_busy_nick_color", "#a0a0a4");
    add_config_value(&doc, &root, "nicklist_gradient_1_color", "#77d5f7");
    add_config_value(&doc, &root, "nicklist_gradient_2_color", "#1b86b7");

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
