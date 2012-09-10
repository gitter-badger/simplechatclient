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

#include "config.h"
#include "highlight.h"

Highlight * Highlight::Instance = 0;

Highlight * Highlight::instance()
{
    if (!Instance)
    {
        Instance = new Highlight();
        Instance->init();
    }

    return Instance;
}

Highlight::Highlight()
{
}

void Highlight::init()
{
    lHighlight.clear();
    read();
}

void Highlight::add(QString &strHighlight)
{
    fix(strHighlight);

    if (lHighlight.contains(strHighlight))
        return;

    lHighlight.append(strHighlight);
    save();
}

void Highlight::remove(const QString &strHighlight)
{
    if (!lHighlight.contains(strHighlight))
        return;

    lHighlight.removeOne(strHighlight);
    save();
}

bool Highlight::contains(const QString &strHighlight)
{
    return lHighlight.contains(strHighlight);
}

void Highlight::clear()
{
    lHighlight.clear();
}

QStringList Highlight::get()
{
    return lHighlight;
}

void Highlight::read()
{
    QString strHighlight;

    Config *pConfig = new Config();
    strHighlight = pConfig->get("highlight");
    delete pConfig;

    lHighlight = strHighlight.split(";", QString::SkipEmptyParts);
}

void Highlight::save()
{
    QString strHighlight;

    foreach (const QString &strNewHighlight, lHighlight)
        strHighlight.append(strNewHighlight+";");

    Config *pConfig = new Config();
    pConfig->set("highlight", strHighlight);
    delete pConfig;
}

void Highlight::fix(QString &strHighlight)
{
    strHighlight.remove(QRegExp("[&<>\"'\\;]"));
}
