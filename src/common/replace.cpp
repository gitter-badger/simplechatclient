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

#include <QRegExp>
#include "settings.h"
#include "replace.h"

void Replace::replaceEmots(QString &strData)
{
    // return if disable replaces == on
    if (Settings::instance()->get("disable_replaces") == "true")
        return;

    // applet
    strData.replace(":*", "%Icmok%");
    strData.replace(";)", "%Ioczko%");
    strData.replace(":P", "%Ijezor%");
    strData.replace(";P", "%Ixjezyk%");
    strData.replace(":)", "%Ihaha%");
    strData.replace(":(", "%Izal%");
    strData.replace(":x", "%Inie_powiem%");
    strData.replace(":?", "%Inie_rozumiem%");
    strData.replace(":((", "%Ibardzo_smutny%");
    strData.replace(":|", "%Ixdepresja%");
    strData.replace(":]", "%Iusmieszek%");
    strData.replace(":>", "%Ixluzak%");
    strData.replace(";>", "%Iwazniak%");
    strData.replace(":<", "%Iumm%");
    strData.replace(":$", "%Iskwaszony%");
    strData.replace(";$", "%Ixkwas%");
    strData.replace(";/", "%Ixsceptyk%");
    if (!strData.contains(QRegExp("(ftp:|http:|https:)//"))) strData.replace(":/", "%Isceptyczny%");
    strData.replace(";D", "%Ixhehe%");
    strData.replace(":D", "%Ihehe%");
    strData.replace("o_O", "%Iswir%");
    strData.replace("!!", "%Iwykrzyknik%");
    strData.replace("??", "%Ipytanie%");
    strData.replace("xD", "%Ilol%");
    strData.replace("-_-", "%Iwrr%");
    strData.replace(";(", "%Iszloch%");

    // scc
    strData.replace(":))", "%Ihaha%");
    strData.replace(";))", "%Ioczko%");
    strData.replace(";((", "%Iszloch%");
    strData.replace(";(", "%Iszloch%");
    strData.replace(":p", "%Ijezyk%");
    strData.replace(";p", "%Ijezor%");
    strData.replace(":d", "%Ihehe%");
    strData.replace(";d", "%Ihehe%");
    strData.replace(";x", "%Inie_powiem%");
    strData.replace(":o", "%Ipanda%");
    strData.replace(";o", "%Ipanda%");
    strData.replace(";<", "%Ibuu%");
    strData.replace(";]", "%Ioczko%");
    strData.replace(":[", "%Izal%");
    strData.replace(";[", "%Iszloch%");
    strData.replace(";|", "%Ixdepresja%");
    strData.replace(";*", "%Icmok2%");
    strData.replace(":s", "%Iskwaszony%");
    strData.replace(";s", "%Iskwaszony%");
    strData.replace("]:->", "%Ixdiabel%");
    strData.replace("];->", "%Ixdiabel%");
    strData.replace(";?", "%Ixco%");
    strData.replace("?!", "%Ipytanie%%Iwykrzyknik%");
}
