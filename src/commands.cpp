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

#include "commands.h"

Commands::Commands(QString param1, QString param2, QSettings *param3)
{
    strChan = param1;
    strData = param2;
    settings = param3;
    strDataList = strData.split(" ");
}

QString Commands::execute()
{
    QString strResult = QString::null;

    if (strDataList.value(0).isEmpty() == false)
    {
        if ((strDataList[0].toLower() == "join") || (strDataList[0].toLower() == "j"))
            strResult = cmd_join();
        else if (strDataList[0].toLower() == "priv")
            strResult = cmd_priv();
        else if (strDataList[0].toLower() == "ignore")
            strResult = cmd_ignore();
        else if (strDataList[0].toLower() == "friend")
            strResult = cmd_friend();
        else if ((strDataList[0].toLower() == "whereis") || (strDataList[0].toLower() == "whois")  || (strDataList[0].toLower() == "wi")  || (strDataList[0].toLower() == "wii"))
            strResult = cmd_whereis();
        else if (strDataList[0].toLower() == "busy")
            strResult = cmd_busy();
        else if (strDataList[0].toLower() == "away")
            strResult = cmd_away();
        else if ((strDataList[0].toLower() == "logout") || (strDataList[0].toLower() == "quit") || (strDataList[0].toLower() == "q"))
            strResult = cmd_quit();
        else if ((strDataList[0].toLower() == "help") || (strDataList[0].toLower() == "pomoc"))
            strResult = cmd_help();
        else
            strResult = strData;

        if ((strChan != "Status") && (strResult == strData))
        {
            if ((strDataList[0].toLower() == "cycle") || (strDataList[0].toLower() == "hop"))
                strResult = cmd_cycle();
            else if (strDataList[0].toLower() == "me")
                strResult = cmd_me();
            else if (strDataList[0].toLower() == "topic")
                strResult = cmd_topic();
            else if ((strDataList[0].toLower() == "part") || (strDataList[0].toLower() == "p"))
                strResult = cmd_part();
            else if (strDataList[0].toLower() == "invite")
                strResult = cmd_invite();
            else if (strDataList[0].toLower() == "kick")
                strResult = cmd_kick();
            else if (strDataList[0].toLower() == "ban")
                strResult = cmd_ban();
            else if (strDataList[0].toLower() == "sop")
                strResult = cmd_sop();
            else if (strDataList[0].toLower() == "op")
                strResult = cmd_op();
            else if ((strDataList[0].toLower() == "moder") || (strDataList[0].toLower() == "moderator"))
                strResult = cmd_moder();
            else if (strDataList[0].toLower() == "vip")
                strResult = cmd_vip();
            else
                strResult = strData;
        }
    }

    return strResult;
}

QString Commands::cmd_join()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strDataList[1];
    if ((strChannel[0] != '#') && (strChannel[0] != '^'))
        strChannel = "#"+strChannel;

    return QString("JOIN %1").arg(strChannel);
}

QString Commands::cmd_priv()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strNick = strDataList[1];

    return QString("PRIV %1").arg(strNick);
}

QString Commands::cmd_ignore()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("NS IGNORE ADD %1").arg(strNick);
    else if (cPlusMinus == '-')
        return QString("NS IGNORE DEL %1").arg(strNick);
    else
        return QString::null;
}

QString Commands::cmd_friend()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("NS FRIENDS ADD %1").arg(strNick);
    else if (cPlusMinus == '-')
        return QString("NS FRIENDS DEL %1").arg(strNick);
    else
        return QString::null;
}

QString Commands::cmd_whereis()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strNick = strDataList[1];

    return QString("WHOIS %1 :%1").arg(strNick);
}

QString Commands::cmd_busy()
{
    QString strBusy = settings->value("busy").toString();

    if (strBusy == "on")
        return "BUSY 0";
    else if (strBusy == "off")
        return "BUSY 1";
    else
        return QString::null;
}

QString Commands::cmd_away()
{
    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    return QString("AWAY :%1").arg(strMessage);
}

QString Commands::cmd_quit()
{
    QString strReason;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strReason += " "; strReason += strDataList[i]; }

    settings->setValue("reconnect", "false");

    if (strReason.isEmpty() == false)
        return QString("QUIT :%1").arg(strReason);
    else
        return "QUIT";
}

QString Commands::cmd_help()
{
    QString strResult;

    strResult = "* Dostêpne komendy:\n";
    strResult.append("/cycle lub /hop\n");
    strResult.append("/me [tekst]\n");
    strResult.append("/topic [tekst]\n");
    strResult.append("/join [kana³] lub /j [kana³]\n");
    strResult.append("/part lub /p\n");
    strResult.append("/priv [nick]\n");
    strResult.append("/ignore [[+|-]nick]\n");
    strResult.append("/friend [[+|-]nick]\n");
    strResult.append("/whereis [nick] lub /whois [nick] lub /wi [nick] lub /wii [nick]\n");
    strResult.append("/busy\n");
    strResult.append("/away [tekst]\n");
    strResult.append("/invite [nick]\n");
    strResult.append("/logout lub /quit lub /q\n");
    strResult.append("/kick [nick] [powód]\n");
    strResult.append("/ban [[+|-]nick]\n");
    strResult.append("/sop [[+|-]nick]\n");
    strResult.append("/op [[+|-]nick]\n");
    strResult.append("/moder [[+|-]nick] lub /moderator [[+|-]nick]\n");
    strResult.append("/vip [[+|-]nick]\n");
    strResult.append("/help lub /pomoc\n");

    return strResult;
}

QString Commands::cmd_cycle()
{
    QString strChannel = strChan;

    return QString("PART %1\nJOIN %1").arg(strChannel);
}

QString Commands::cmd_me()
{
    QString strChannel = strChan;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    return QString("PRIVMSG %1 :%2ACTION %3%4").arg(strChannel).arg(QString(QByteArray("\x01"))).arg(strMessage).arg(QString(QByteArray("\x01")));
}

QString Commands::cmd_topic()
{
    QString strChannel = strChan;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    return QString("CS SET %1 TOPIC %2").arg(strChannel).arg(strMessage);
}

QString Commands::cmd_part()
{
    QString strChannel;
    if (strDataList.value(1).isEmpty() == true)
        strChannel = strChan;
    else
        strChannel = strDataList[1];
    if ((strChannel[0] != '#') && (strChannel[0] != '^'))
        strChannel = "#"+strChannel;

    return QString("PART %1").arg(strChannel);
}

QString Commands::cmd_invite()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strNick = strDataList[1];
    QString strChannel = strChan;

    return QString("INVITE %1 %2").arg(strNick).arg(strChannel);
}

QString Commands::cmd_kick()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];

    QString strReason;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strReason += " "; strReason += strDataList[i]; }

    if (strReason.isEmpty() == true)
        strReason = "Zachowuj siê! Byle jak ale siê zachowuj!";

    return QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strReason);
}

QString Commands::cmd_ban()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick);
    else if (cPlusMinus == '-')
        return QString("CS BAN %1 DEL %2").arg(strChannel).arg(strNick);
    else
        return QString::null;
}

QString Commands::cmd_sop()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick);
    else if (cPlusMinus == '-')
        return QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick);
    else
        return QString::null;
}

QString Commands::cmd_op()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick);
    else if (cPlusMinus == '-')
        return QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick);
    else
        return QString::null;
}

QString Commands::cmd_moder()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick);
    else if (cPlusMinus == '-')
        return QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick);
    else
        return QString::null;
}

QString Commands::cmd_vip()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick);
    else if (cPlusMinus == '-')
        return QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick);
    else
        return QString::null;
}
