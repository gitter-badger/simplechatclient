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

#include "nicklist.h"
#include "settings.h"
#include "nick.h"

Nick * Nick::Instance = 0;

Nick * Nick::instance()
{
    if (!Instance)
    {
        Instance = new Nick();
        Instance->clear();
    }

    return Instance;
}

Nick::Nick()
{
}

void Nick::clear()
{
    lNicks.clear();
    Nicklist::instance()->clearUsers();
}

void Nick::add(const QString &strNick, const QString &strChannel, QString strModes)
{
    if ((strModes.contains(FLAG_OWNER)) && (strModes.contains(FLAG_OP))) strModes.remove(FLAG_OP);
    int iMaxModes = createMaxModes(strModes);

    if (lNicks.contains(strNick))
    {
        if (lNicks[strNick].channels.contains(strChannel)) return;

        lNicks[strNick].channels.append(strChannel);
        lNicks[strNick].channel_modes[strChannel] = strModes;
        lNicks[strNick].channel_max_modes[strChannel] = iMaxModes;

        Nicklist::instance()->addUser(strNick, strChannel, strModes, iMaxModes, lNicks[strNick].avatar);
    }
    else
    {
        OnetNick nick;
        nick.avatar = QString::null;
        nick.channels.append(strChannel);
        nick.channel_modes[strChannel] = strModes;
        nick.channel_max_modes[strChannel] = iMaxModes;

        lNicks[strNick] = nick;

        Nicklist::instance()->addUser(strNick, strChannel, strModes, iMaxModes, QString::null);
    }
}

void Nick::remove(const QString &strNick, const QString &strChannel)
{
    if (!lNicks.contains(strNick)) return;

    Nicklist::instance()->delUser(strNick, strChannel);

    lNicks[strNick].channels.removeOne(strChannel);
    lNicks[strNick].channel_modes.remove(strChannel);
    lNicks[strNick].channel_max_modes.remove(strChannel);

    if (lNicks[strNick].channels.count() == 0)
        lNicks.remove(strNick);

    QString strMe = Settings::instance()->get("nick");
    if (strNick == strMe)
        removeFromChannel(strChannel);
}

void Nick::removeFromChannel(const QString &strChannel)
{
    QList<CaseIgnoreString> lNicksFromChannel = getFromChannel(strChannel);

    foreach (QString strNick, lNicksFromChannel)
    {
        lNicks[strNick].channels.removeOne(strChannel);
        lNicks[strNick].channel_modes.remove(strChannel);
        lNicks[strNick].channel_max_modes.remove(strChannel);

        if (lNicks[strNick].channels.size() == 0)
            lNicks.remove(strNick);
    }
}

void Nick::rename(const QString &strNick, const QString &strNewNick, const QString &strDisplay)
{
    if (!lNicks.contains(strNick)) return;

    Nicklist::instance()->renameUser(strNick, strNewNick, lNicks[strNick].channels, strDisplay);

    lNicks[strNewNick] = lNicks[strNick];
    lNicks.remove(strNick);
}

void Nick::quit(const QString &strNick, const QString &strDisplay)
{
    if (!lNicks.contains(strNick)) return;

    QList<QString> lChannels = lNicks[strNick].channels;
    Nicklist::instance()->quitUser(strNick, lChannels, strDisplay);
    lNicks.remove(strNick);

    QString strMe = Settings::instance()->get("nick");
    if (strNick == strMe)
    {
        foreach (QString strChannel, lChannels)
            removeFromChannel(strChannel);
    }
}

void Nick::changeFlag(const QString &strNick, const QString &strChannel, QString strFlag)
{
    if (!lNicks.contains(strNick)) return;

    QString strModes = lNicks[strNick].channel_modes.value(strChannel, QString::null);
    QString plusminus = strFlag.at(0);
    strFlag.remove(0, 1);

    QString strConvertFrom = "qaohXYv";
    QString strConvertTo = "`&@%!=+";

    for (int i = 0; i < strConvertFrom.size(); i++)
        strFlag.replace(strConvertFrom.at(i), strConvertTo.at(i));

    if (plusminus == "+")
    {
        // fix webcam flags
        if ((strFlag == FLAG_CAM_PUB) && (strModes.contains(FLAG_CAM_PRIV))) strModes.remove(FLAG_CAM_PRIV);
        if ((strFlag == FLAG_CAM_PRIV) && (strModes.contains(FLAG_CAM_PUB))) strModes.remove(FLAG_CAM_PUB);

        if (!strModes.contains(strFlag))
            strModes.append(strFlag);
    }
    else
    {
        if (strModes.contains(strFlag))
            strModes.remove(strFlag);
    }

    // if owner remove op
    if ((strModes.contains(FLAG_OWNER)) && (strModes.contains(FLAG_OP))) strModes.remove(FLAG_OP);

    int iMaxModes = createMaxModes(strModes);
    lNicks[strNick].channel_modes[strChannel] = strModes;
    lNicks[strNick].channel_max_modes[strChannel] = iMaxModes;

    Nicklist::instance()->setUserModes(strNick, strChannel, strModes, iMaxModes);
}

void Nick::changeFlag(const QString &strNick, const QString &strFlag)
{
    if (!lNicks.contains(strNick)) return;

    QList<QString> lChannels = lNicks[strNick].channels;
    foreach (QString strChannel, lChannels)
        changeFlag(strNick, strChannel, strFlag);
}

void Nick::setAvatar(const QString &strNick, const QString &strAvatar)
{
    if (!lNicks.contains(strNick)) return;

    Nicklist::instance()->setUserAvatar(strNick, lNicks[strNick].channels, strAvatar);

    lNicks[strNick].avatar = strAvatar;
}

QString Nick::getAvatar(const QString &strNick)
{
    if (!lNicks.contains(strNick)) return QString::null;

    return lNicks[strNick].avatar;
}

QString Nick::getModes(const QString &strNick, const QString &strChannel)
{
    if (!lNicks.contains(strNick)) return QString::null;

    return lNicks[strNick].channel_modes.value(strChannel, QString::null);
}

int Nick::getMaxModes(const QString &strNick, const QString &strChannel)
{
    if (!lNicks.contains(strNick)) return -1;

    return lNicks[strNick].channel_max_modes.value(strChannel, -1);
}

QList<CaseIgnoreString> Nick::getFromChannel(const QString &strChannel)
{
    QList<CaseIgnoreString> lNicksFromChannel;

    QHashIterator<QString, OnetNick> i(lNicks);
    while (i.hasNext())
    {
        i.next();

        QString strNick = i.key();

        if (lNicks[strNick].channels.contains(strChannel))
            lNicksFromChannel.append(strNick);
    }

    // sort
    qSort(lNicksFromChannel.begin(), lNicksFromChannel.end());

    return lNicksFromChannel;
}

int Nick::createMaxModes(const QString &strModes)
{
    if (strModes.contains(FLAG_DEV)) { return 64; }
    else if (strModes.contains(FLAG_ADMIN)) { return 32; }
    else if (strModes.contains(FLAG_OWNER)) { return 16; }
    else if (strModes.contains(FLAG_OP)) { return 8; }
    else if (strModes.contains(FLAG_HALFOP)) { return 4; }
    else if (strModes.contains(FLAG_MOD)) { return 2; }
    else if (strModes.contains(FLAG_SCREENER)) { return 1; }
    else if (strModes.contains(FLAG_VOICE)) { return 0; }
    else return -1;
}