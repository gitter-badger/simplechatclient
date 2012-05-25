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

#include <QCoreApplication>
#include <QDir>
#include "core.h"
#include "xml.h"
#include "config.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#else
    #include <QDebug>
#endif

Config::Config(bool _bProfileConfig, QString _strForceProfile) : bProfileConfig(_bProfileConfig), strForceProfile(_strForceProfile)
{
    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    if (bProfileConfig)
    {
        QString user = (strForceProfile.isEmpty() ? Core::instance()->settings.value("current_profile") : strForceProfile);
        if (user.isEmpty()) user = "~test";

        path += "/profiles/"+user;
        strConfigFile = path+"/profile.xml";
    }
    else
        strConfigFile = path+"/scc.xml";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    // root name
    QString strRootName = (bProfileConfig  == true ? "profile" : "settings");

    // default values
    lDefaultValues = getDefaultValues();

    // open
    xml = new Xml(strConfigFile, strRootName, lDefaultValues);

    // fix
    fixConfig();
}

Config::~Config()
{
    delete xml;
}

QString Config::getValue(const QString &strKey)
{
    return xml->getValue(strKey);
}

void Config::setValue(const QString &strKey, const QString &strValue)
{
    xml->setValue(strKey, strValue);
}

QHash<QString, QString> Config::readToHash()
{
    return xml->readToHash();
}

void Config::fixConfig()
{
    QHashIterator<QString, QString> i(lDefaultValues);
    while (i.hasNext())
    {
        i.next();
        QString strDefaultKey = i.key();
        QString strDefaultValue = i.value();

        QString strValue = xml->getValue(strDefaultKey);
        if (strValue.isEmpty())
        {
            if (strDefaultKey != "pass") // ignore pass
                xml->setValue(strDefaultKey, strDefaultValue);
        }
    }
}

QHash<QString,QString> Config::getDefaultValues()
{
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = SCC_DATA_DIR;
#endif

    QString strSoundBeep = path+"/3rdparty/sounds/beep.wav";
    QString strSoundQuery = path+"/3rdparty/sounds/query.wav";
    QString strBackgroundImage = path+"/images/wallpaper/default.jpg";

    QHash<QString,QString> lDefaultValues;

    if (!bProfileConfig)
    {
        lDefaultValues.insert("first_run", "true");
        lDefaultValues.insert("current_profile", "~test");
    }
    else
    {
        lDefaultValues.insert("nick", "~test");
        lDefaultValues.insert("pass", "");
        lDefaultValues.insert("themes", "Standard");
        lDefaultValues.insert("language", "pl");
        lDefaultValues.insert("highlight", "");
        lDefaultValues.insert("auto_busy", "false");
        lDefaultValues.insert("minimize_to_tray", "false");
        lDefaultValues.insert("disable_autojoin_favourites", "false");
        lDefaultValues.insert("show_zuo_and_ip", "false");
        lDefaultValues.insert("hide_formating", "false");
        lDefaultValues.insert("hide_join_part", "false");
        lDefaultValues.insert("hide_join_part_200", "true");
        lDefaultValues.insert("disable_emots", "false");
        lDefaultValues.insert("disable_replaces", "false");
        lDefaultValues.insert("background_color", "#ffffff");
        lDefaultValues.insert("my_bold", "false");
        lDefaultValues.insert("my_italic", "false");
        lDefaultValues.insert("my_font", "Verdana");
        lDefaultValues.insert("my_color", "#000000");
        lDefaultValues.insert("font_size", "11px");
        lDefaultValues.insert("default_font_color", "#000000");
        lDefaultValues.insert("font_color_level_1", "#009300");
        lDefaultValues.insert("font_color_level_2", "#4733FF");
        lDefaultValues.insert("font_color_level_3", "#00007F");
        lDefaultValues.insert("font_color_level_4", "#00007F");
        lDefaultValues.insert("font_color_level_5", "#009300");
        lDefaultValues.insert("font_color_level_6", "#0066FF");
        lDefaultValues.insert("font_color_level_7", "#666666");
        lDefaultValues.insert("font_color_level_8", "#800080");
        lDefaultValues.insert("font_color_level_9", "#ff0000");
        lDefaultValues.insert("channel_font_color", "#0000ff");
        lDefaultValues.insert("nicklist_nick_color", "#333333");
        lDefaultValues.insert("nicklist_selected_nick_color", "#ffffff");
        lDefaultValues.insert("nicklist_busy_nick_color", "#a0a0a4");
        lDefaultValues.insert("nicklist_gradient_1_color", "#77d5f7");
        lDefaultValues.insert("nicklist_gradient_2_color", "#1b86b7");
        lDefaultValues.insert("save_logs_by_date", "true");
        lDefaultValues.insert("disable_logs", "false");
        lDefaultValues.insert("sound_beep", strSoundBeep);
        lDefaultValues.insert("sound_query", strSoundQuery);
        lDefaultValues.insert("disable_sounds", "false");
        lDefaultValues.insert("background_image", strBackgroundImage);
        lDefaultValues.insert("disable_background_image", "false");
        lDefaultValues.insert("winamp", "$song [$position/$length] //muzyka");
        lDefaultValues.insert("tray_message", "true");
        lDefaultValues.insert("always_quit", "false");
    }

    return lDefaultValues;
}

