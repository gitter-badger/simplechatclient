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
#include <QFile>
#include <QTextStream>
#include "core.h"
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
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    if (bProfileConfig)
    {
        path += "/profiles";

        // create dir if not exist
        if (!QDir().exists(path))
            QDir().mkdir(path);

        QString user = (strForceProfile.isEmpty() ? Core::instance()->settings.value("current_profile") : strForceProfile);
        if (user.isEmpty()) user = "~test";

        strConfigFile = path+"/"+user+".xml";
    }
    else
        strConfigFile = path+"/scc.xml";

    QFile file(strConfigFile);

    // if not exist - create new
    if (!file.exists())
        createNewConfig();

    // try read
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream ts(&file);
            QString strData = ts.readAll();
            file.close();

            if (doc.setContent(strData))
                fixConfig();
            else
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
            qDebug() << tr("Error: config: Cannot read config file!");
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

QString Config::getValue(QString strKey)
{
    if (doc.isNull())
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: config: Cannot get value: ") << strKey;
#endif
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

void Config::setValue(QString strKey, QString strValue)
{
    if (doc.isNull())
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: config: Cannot set value: ") << strKey;
#endif
        return;
    }

    // remove
    removeValue(strKey);

    // add value
    QDomElement docElem = doc.documentElement();
    QDomElement eSetKey = doc.createElement(strKey);
    docElem.appendChild(eSetKey);
    QDomText tSetValue = doc.createTextNode(strValue);
    eSetKey.appendChild(tSetValue);

    // save
    save();
}

void Config::removeValue(QString strKey)
{
    if (doc.isNull())
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: config: Cannot remove value: ") << strKey;
#endif
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

void Config::fixConfig()
{
    QMap<QString,QString> mDefaultValues = getDefaultValues();
    QMapIterator<QString, QString> i(mDefaultValues);
    while (i.hasNext())
    {
        i.next();
        QString strDefaultKey = i.key();
        QString strDefaultValue = i.value();

        QString strValue = getValue(strDefaultKey);
        if (strValue.isEmpty())
        {
            if (strDefaultKey != "pass") // ignore pass
                setValue(strDefaultKey, strDefaultValue);
        }
    }
}

void Config::createNewConfig()
{
    doc.clear();
    QString strRootName = (bProfileConfig  == true ? "profile" : "settings");
    QDomElement root = doc.createElement(strRootName);
    doc.appendChild(root);

    QMap<QString,QString> mDefaultValues = getDefaultValues();
    QMapIterator<QString, QString> i(mDefaultValues);
    while (i.hasNext())
    {
        i.next();
        QString strDefaultKey = i.key();
        QString strDefaultValue = i.value();

        addConfigValue(&doc, &root, strDefaultKey, strDefaultValue);
    }

    // save
    save();
}

QMap<QString,QString> Config::getDefaultValues()
{
    QString strPath;
#ifdef Q_WS_WIN
    strPath = QCoreApplication::applicationDirPath();
#else
    strPath = "/usr/share/scc";
#endif
    QString strSoundBeep = strPath+"/3rdparty/sounds/beep.wav";
    QString strSoundQuery = strPath+"/3rdparty/sounds/query.wav";
    QString strBackgroundImage = strPath+"/images/wallpaper/default.jpg";

    QMap<QString,QString> mDefaultValues;

    if (!bProfileConfig)
    {
        mDefaultValues.insert("first_run", "true");
        mDefaultValues.insert("current_profile", "~test");
    }
    else
    {
        mDefaultValues.insert("nick", "~test");
        mDefaultValues.insert("pass", "");
        mDefaultValues.insert("themes", "Standard");
        mDefaultValues.insert("language", "pl");
        mDefaultValues.insert("auto_busy", "off");
        mDefaultValues.insert("disable_autojoin_favourites", "off");
        mDefaultValues.insert("show_zuo_and_ip", "off");
        mDefaultValues.insert("hide_formating", "off");
        mDefaultValues.insert("hide_join_part", "off");
        mDefaultValues.insert("hide_join_part_200", "on");
        mDefaultValues.insert("disable_emots", "off");
        mDefaultValues.insert("disable_replaces", "off");
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
        mDefaultValues.insert("font_color_level_8", "#800080");
        mDefaultValues.insert("font_color_level_9", "#ff0000");
        mDefaultValues.insert("channel_font_color", "#0000ff");
        mDefaultValues.insert("nicklist_nick_color", "#333333");
        mDefaultValues.insert("nicklist_selected_nick_color", "#ffffff");
        mDefaultValues.insert("nicklist_busy_nick_color", "#a0a0a4");
        mDefaultValues.insert("nicklist_gradient_1_color", "#77d5f7");
        mDefaultValues.insert("nicklist_gradient_2_color", "#1b86b7");
        mDefaultValues.insert("save_logs_by_date", "on");
        mDefaultValues.insert("disable_logs", "off");
        mDefaultValues.insert("sound_beep", strSoundBeep);
        mDefaultValues.insert("sound_query", strSoundQuery);
        mDefaultValues.insert("disable_sounds", "off");
        mDefaultValues.insert("background_image", strBackgroundImage);
        mDefaultValues.insert("disable_background_image", "off");
        mDefaultValues.insert("winamp", "Winamp $version odtwarza teraz $song [$position/$length] //muzyka");
    }

    return mDefaultValues;
}

QMap<QString,QString> Config::readConfig()
{
    QMap<QString, QString> mResult;

    if (!doc.isNull())
    {
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();

        while (!n.isNull())
        {
            QDomElement e = n.toElement();

            // save to map
            if (!e.isNull())
                mResult.insert(e.tagName(), e.text());

            n = n.nextSibling();
        }
    }

    return mResult;
}

void Config::addConfigValue(QDomDocument *doc, QDomElement *root, QString strKey, QString strValue)
{
    QDomElement eKey = doc->createElement(strKey);
    root->appendChild(eKey);
    QDomText tValue = doc->createTextNode(strValue);
    eKey.appendChild(tValue);
}

void Config::save()
{
    QString xml = doc.toString();

    QFile f(strConfigFile);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&f);
        out << xml;

        f.close();
    }
}
