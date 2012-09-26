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
#include "punish_reason.h"

PunishReason * PunishReason::Instance = 0;

PunishReason * PunishReason::instance()
{
    if (!Instance)
    {
        Instance = new PunishReason();
        Instance->init();
    }

    return Instance;
}

PunishReason::PunishReason()
{
}

void PunishReason::init()
{
    lPunishReason.clear();
    read();
}

void PunishReason::add(QString &strPunishReason)
{
    fix(strPunishReason);

    if (!lPunishReason.contains(strPunishReason))
    {
        lPunishReason.append(strPunishReason);
        save();
    }
}

void PunishReason::remove(const QString &strPunishReason)
{
    if (lPunishReason.contains(strPunishReason))
    {
        lPunishReason.removeOne(strPunishReason);
        save();
    }
}

bool PunishReason::contains(const QString &strPunishReason)
{
    return lPunishReason.contains(strPunishReason);
}

void PunishReason::clear()
{
    lPunishReason.clear();
}

QStringList PunishReason::get()
{
    return lPunishReason;
}

void PunishReason::read()
{
    QString strPunishReason;

    Config *pConfig = new Config();
    strPunishReason = pConfig->get("punish_reason");
    delete pConfig;

    lPunishReason = strPunishReason.split(";", QString::SkipEmptyParts);
}

void PunishReason::save()
{
    QString strPunishReason;

    foreach (const QString &strNewPunishReason, lPunishReason)
        strPunishReason.append(strNewPunishReason+";");

    Config *pConfig = new Config();
    pConfig->set("punish_reason", strPunishReason);
    delete pConfig;
}

void PunishReason::fix(QString &strPunishReason)
{
    strPunishReason.remove(QRegExp("[&<>\"'\\;]"));
}
