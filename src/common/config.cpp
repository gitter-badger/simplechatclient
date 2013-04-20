/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QDir>
#include <QUuid>
#include "settings.h"
#include "xml.h"
#include "config.h"

#ifdef Q_WS_WIN
    #include <QCoreApplication>
    #include <QDesktopServices>
#else
    #include "scc-config.h"
#endif

Config::Config(bool _bProfileConfig, QString _strForceProfile) : bProfileConfig(_bProfileConfig), strForceProfile(_strForceProfile)
{
    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc/";
#else
    path = QDir::homePath()+"/.scc/";
#endif

    if (bProfileConfig)
    {
        QString user = (strForceProfile.isEmpty() ? Settings::instance()->get("current_profile") : strForceProfile);
        if (user.isEmpty()) user = "~test";

        path += "profiles/"+user+"/";
        strConfigFile = path+"profile.xml";
    }
    else
        strConfigFile = path+"scc.xml";

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
    fix();
}

Config::~Config()
{
    delete xml;
}

QString Config::get(const QString &strKey)
{
    return xml->get(strKey);
}

void Config::set(const QString &strKey, const QString &strValue)
{
    xml->set(strKey, strValue);
}

void Config::remove(const QString &strKey)
{
    xml->remove(strKey);
}

QHash<QString, QString> Config::read()
{
    return xml->read();
}

void Config::fix()
{
    QHashIterator<QString, QString> i(lDefaultValues);
    while (i.hasNext())
    {
        i.next();
        QString strDefaultKey = i.key();
        QString strDefaultValue = i.value();

        QString strValue = xml->get(strDefaultKey);
        if (strValue.isEmpty())
        {
            if (strDefaultKey != "pass") // ignore pass
                xml->set(strDefaultKey, strDefaultValue);
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

    QString uuidStr = QUuid::createUuid().toString();
    uuidStr.remove("{"); uuidStr.remove("}");

    QString strSoundBeep = path+"/sounds/beep.wav";
    QString strSoundQuery = path+"/sounds/query.wav";
    QString strBackgroundImage = path+"/images/wallpaper/default.jpg";

    QString strPunishReason = "Bluzg;Flood;Spam;";

    QHash<QString,QString> lDefaultValues;

    if (!bProfileConfig)
    {
        lDefaultValues.insert("first_run", "true");
        lDefaultValues.insert("current_profile", "~test");
        lDefaultValues.insert("unique_id", uuidStr);
    }
    else
    {
        lDefaultValues.insert("nick", "~test");
        lDefaultValues.insert("pass", QString::null);
        lDefaultValues.insert("themes", "Standard");
        lDefaultValues.insert("language", "pl");

        lDefaultValues.insert("highlight", QString::null);

        lDefaultValues.insert("punish_reason", strPunishReason);

        lDefaultValues.insert("auto_busy", "false");
        lDefaultValues.insert("auto_away", "true");
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
        lDefaultValues.insert("default_color", "#000000");
        lDefaultValues.insert("message_join_color", "#009300");
        lDefaultValues.insert("message_part_color", "#4733FF");
        lDefaultValues.insert("message_quit_color", "#00007F");
        lDefaultValues.insert("message_kick_color", "#00007F");
        lDefaultValues.insert("message_mode_color", "#009300");
        lDefaultValues.insert("message_notice_color", "#0066FF");
        lDefaultValues.insert("message_info_color", "#666666");
        lDefaultValues.insert("message_me_color", "#800080");
        lDefaultValues.insert("message_error_color", "#ff0000");

        lDefaultValues.insert("channel_color", "#0000ff");
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

