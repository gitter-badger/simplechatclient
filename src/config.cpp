/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

config::config()
{
    QString path = QCoreApplication::applicationDirPath();
    strConfigFile = path+"/scc.conf";
    file = new QFile(strConfigFile);

    if (file->exists() == false)
        config::create_new_config();

    if (file->exists() == true)
    {
        if (file->open(QIODevice::ReadWrite) == false)
        {
#ifdef Q_WS_X11
            qDebug() << "Error: config: Cannot read config file!";
#endif
            return;
        }
        if (doc.setContent(file) == false)
        {
#ifdef Q_WS_X11
            qDebug() << "Error: config: Cannot set content from file!";
#endif
            return;
        }
    }
    else
    {
#ifdef Q_WS_X11
            qDebug() << "Error: config: Cannot open config file!";
#endif
            return;
    }
}

config::~config()
{
    file->close();
}

void config::save()
{
    QString xml = doc.toString();
    QByteArray baSave = xml.toAscii();

    QFile *fs = new QFile(strConfigFile);
    fs->open(QIODevice::WriteOnly | QIODevice::Truncate);
    fs->write(baSave);
    fs->close();
}

QString config::get_value(QString strKey)
{
    if ((doc.isNull() == true) || (file->isOpen() == false))
    {
#ifdef Q_WS_X11
        qDebug() << "Error: config: Cannot get value: " << strKey;
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

void config::set_value(QString strKey, QString strValue)
{
    if ((doc.isNull() == true) || (file->isOpen() == false))
    {
#ifdef Q_WS_X11
        qDebug() << "Error: config: Cannot set value: " << strKey;
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

    config::save();
}

void config::create_new_config()
{
    doc.clear();
    QDomElement root = doc.createElement("config");
    doc.appendChild(root);

    QDomElement eLoginNick = doc.createElement("login-nick");
    root.appendChild(eLoginNick);
    QDomText tLoginNick = doc.createTextNode("~test");
    eLoginNick.appendChild(tLoginNick);

    QDomElement eLoginPass = doc.createElement("login-pass");
    root.appendChild(eLoginPass);
    QDomText tLoginPass = doc.createTextNode(QString::null);
    eLoginPass.appendChild(tLoginPass);

    QDomElement eDebugAll = doc.createElement("debug_all");
    root.appendChild(eDebugAll);
    QDomText tDebugAll = doc.createTextNode("off");
    eDebugAll.appendChild(tDebugAll);

    QDomElement eShowZuo = doc.createElement("show_zuo");
    root.appendChild(eShowZuo);
    QDomText tShowZuo = doc.createTextNode("off");
    eShowZuo.appendChild(tShowZuo);

    QDomElement eHideFormating = doc.createElement("hide_formating");
    root.appendChild(eHideFormating);
    QDomText tHideFormating = doc.createTextNode("off");
    eHideFormating.appendChild(tHideFormating);

    QDomElement eHideJoinPart = doc.createElement("hide_join_part");
    root.appendChild(eHideJoinPart);
    QDomText tHideJoinPart = doc.createTextNode("off");
    eHideJoinPart.appendChild(tHideJoinPart);

    config::save();
}
