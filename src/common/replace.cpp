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

#include <QSettings>
#include "replace.h"

Replace::Replace()
{
}

QString Replace::replace_emots(QString strData)
{
    // return if disable replaces == on
    QSettings settings;
    if (settings.value("disable_replaces").toString() == "on") return strData;

    strData.replace(":))", "%Ihaha%");
    strData.replace(";))", "%Ioczko%");

    strData.replace(":)", "%Ihaha%");
    strData.replace(";)", "%Ioczko%");

    strData.replace(":((", "%Ibardzo_smutny%");
    strData.replace(";((", "%Iszloch%");

    strData.replace(":(", "%Izal%");
    strData.replace(";(", "%Iszloch%");

    strData.replace(":p", "%Ijezyk%");
    strData.replace(";p", "%Ijezor%");
    strData.replace(":P", "%Ijezyk%");
    strData.replace(";P", "%Ijezor%");

    strData.replace(":d", "%Ihehe%");
    strData.replace(";d", "%Ihehe%");
    strData.replace(":D", "%Ihehe%");
    strData.replace(";D", "%Ihehe%");

    strData.replace(":x", "%Inie_powiem%");
    strData.replace(";x", "%Inie_powiem%");
    strData.replace(":X", "%Inie_powiem%");
    strData.replace(";X", "%Inie_powiem%");

    strData.replace(":o", "%Ipanda%");
    strData.replace(";o", "%Ipanda%");
    strData.replace(":O", "%Ipanda%");
    strData.replace(";O", "%Ipanda%");

    strData.replace(":>", "%Ixluzak%");
    strData.replace(";>", "%Ixluzak%");
    strData.replace(":<", "%Iumm%");
    strData.replace(";<", "%Ibuu%");

    if ((!strData.contains("http://")) && (!strData.contains("https://")))
        strData.replace(":/", "%Isceptyczny%");
    strData.replace(";/", "%Isceptyczny%");
    strData.replace(":\\", "%Isceptyczny%");
    strData.replace(";\\", "%Isceptyczny%");

    strData.replace(":]", "%Iusmieszek%");
    strData.replace(";]", "%Ioczko%");
    strData.replace(":[", "%Izal%");
    strData.replace(";[", "%Iszloch%");

    strData.replace(":|", "%Ixdepresja%");
    strData.replace(";|", "%Ixdepresja%");

    strData.replace(":*", "%Icmok%");
    strData.replace(";*", "%Icmok2%");

    strData.replace(":s", "%Iskwaszony%");
    strData.replace(";s", "%Iskwaszony%");
    strData.replace(":S", "%Iskwaszony%");
    strData.replace(";S", "%Iskwaszony%");

    strData.replace("]:->", "%Ixdiabel%");
    strData.replace("];->", "%Ixdiabel%");

    strData.replace(";?", "%Ixco%");
    strData.replace("??", "%Ipytanie%");
    strData.replace("!!", "%Iwykrzyknik%");
    strData.replace("?!", "%Ipytanie%%Iwykrzyknik%");

    return strData;
}
