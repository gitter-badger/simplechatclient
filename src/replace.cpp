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

#include "replace.h"

Replace::Replace()
{
}

QString Replace::replace_emots(QString strData)
{
    strData.replace(":))", "%Ixhehe%");
    strData.replace(";))", "%Ixhehe%");

    strData.replace(":)", "%Ixhaha%");
    strData.replace(";)", "%Ixoczko%");

    strData.replace(":((", "%Ixbuu%");
    strData.replace(";((", "%Ixbuu%");

    strData.replace(":(", "%Ixsmutek%");
    strData.replace(";(", "%Ixbuu%");

    strData.replace(":p", "%Ijezor%");
    strData.replace(";p", "%Ijezyk%");
    strData.replace(":P", "%Ijezor%");
    strData.replace(";P", "%Ijezyk%");

    strData.replace(":d", "%Ixhehe%");
    strData.replace(";d", "%Ixhehe%");
    strData.replace(":D", "%Ixhehe%");
    strData.replace(";D", "%Ixhehe%");

    strData.replace(":x", "%Ixanimru%");
    strData.replace(";x", "%Ixanimru%");
    strData.replace(":X", "%Ixanimru%");
    strData.replace(";X", "%Ixanimru%");

    strData.replace(":o", "%Ixpanda%");
    strData.replace(";o", "%Ixpanda%");
    strData.replace(":O", "%Ixpanda%");
    strData.replace(";O", "%Ixpanda%");

    strData.replace(":>", "%Ixluzak%");
    strData.replace(";>", "%Ixluzak%");
    strData.replace(":<", "%Ixsmutek%");
    strData.replace(";<", "%Ixbuu%");

    if ((strData.indexOf("http://") == -1) && (strData.indexOf("https://") == -1))
        strData.replace(":/", "%Ixmm%");
    strData.replace(";/", "%Ixmm%");
    strData.replace(":\\", "%Ixmm%");
    strData.replace(";\\", "%Ixmm%");

    strData.replace(":]", "%Ixhaha%");
    strData.replace(";]", "%Ixhaha%");
    strData.replace(":[", "%Ixsmutek%");
    strData.replace(";[", "%Ixbuu%");

    strData.replace(":|", "%Iooo%");
    strData.replace(";|", "%Iooo%");

    strData.replace(":*", "%Ixcalus%");
    strData.replace(";*", "%Icmok2%");

    strData.replace(":s", "%Iskwaszony%");
    strData.replace(";s", "%Iskwaszony%");
    strData.replace(":S", "%Iskwaszony%");
    strData.replace(";S", "%Iskwaszony%");

    strData.replace("]:->", "%Ixdiabel%");
    strData.replace("];->", "%Ixdiabel%");

    strData.replace("xD", "%Ilol%");
    strData.replace("XD", "%Ilol%");

    strData.replace(";?", "%Ixco%");
    strData.replace("??", "%Ipytanie%");
    strData.replace("!!", "%Iwykrzyknik%");
    strData.replace("?!", "%Ipytanie%%Iwykrzyknik%");

    return strData;
}
