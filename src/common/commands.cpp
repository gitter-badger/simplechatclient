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

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include "commands.h"

Commands::Commands(sChannelList *param1, QString param2, QString param3)
{
    stlChannelList = param1;
    strChan = param2;
    strData = param3;
    strDataList = strData.split(" ");
}

QString Commands::execute()
{
    QString strResult = QString::null;

    if (strDataList.value(0).isEmpty() == false)
    {
        QString strCmd = strDataList[0].toLower();

        if ((strCmd == "join") || (strCmd == "j"))
            strResult = cmd_join();
        else if (strCmd == "priv")
            strResult = cmd_priv();
        else if (strCmd == "ignore")
            strResult = cmd_ignore();
        else if (strCmd == "friend")
            strResult = cmd_friend();
        else if ((strCmd == "whereis") || (strCmd == "whois")  || (strCmd == "wi")  || (strCmd == "wii"))
            strResult = cmd_whereis();
        else if (strCmd == "busy")
            strResult = cmd_busy();
        else if (strCmd == "away")
            strResult = cmd_away();
        else if ((strCmd == "logout") || (strCmd == "quit") || (strCmd == "q"))
            strResult = cmd_quit();
        else if ((strCmd == "help") || (strCmd == "pomoc"))
            strResult = cmd_help();
        else
            strResult = strData;

        if ((strChan != "Status") && (strResult == strData))
        {
            if ((strCmd == "cycle") || (strCmd == "hop"))
                strResult = cmd_cycle();
            else if (strCmd == "me")
                strResult = cmd_me();
            else if (strCmd == "topic")
                strResult = cmd_topic();
            else if ((strCmd == "part") || (strCmd == "p"))
                strResult = cmd_part();
            else if (strCmd == "invite")
                strResult = cmd_invite();
            else if ((strCmd == "kick") || (strCmd == "k"))
                strResult = cmd_kick();
            else if (strCmd == "ban")
                strResult = cmd_ban();
            else if (strCmd == "banip")
                strResult = cmd_banip();
            else if (strCmd == "sop")
                strResult = cmd_sop();
            else if (strCmd == "op")
                strResult = cmd_op();
            else if ((strCmd == "moder") || (strCmd == "moderator"))
                strResult = cmd_moder();
            else if (strCmd == "vip")
                strResult = cmd_vip();
            else
                strResult = strData;
        }
    }

    return strResult;
}

bool Commands::is_erotic(QString strChannel)
{
    for (int i = 0; i < stlChannelList->size(); i++)
    {
        ChannelList channel = stlChannelList->at(i);
        QString strName = channel.name;
        QString strType = channel.type;

        if (strName.toLower() == strChannel.toLower())
        {
            if (strType == tr("Erotic"))
                return true;
            else
                return false;
        }
    }
    return false;
}

QString Commands::cmd_join()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strDataList[1];
    if ((strChannel[0] != '#') && (strChannel[0] != '^'))
        strChannel = "#"+strChannel;

    QString strKey;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strKey += " "; strKey += strDataList[i]; }

    if (is_erotic(strChannel) == true)
    {
        QSettings settings;
        if (settings.value("age_check").toString() == "on")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults.")).arg(tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
                settings.setValue("age_check", "off");
            else
                return QString::null;
        }
    }

    if (strKey.isEmpty() == false)
        return QString("JOIN %1 %2").arg(strChannel).arg(strKey);
    else
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
    QSettings settings;
    QString strBusy = settings.value("busy").toString();

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

    QSettings settings;
    settings.setValue("reconnect", "false");

    if (strReason.isEmpty() == false)
        return QString("QUIT :%1").arg(strReason);
    else
        return "QUIT";
}

QString Commands::cmd_help()
{
    QString strResult;

    strResult = (tr("* Available commands:")+";");
    strResult.append(tr("/cycle or /hop")+";");
    strResult.append(tr("/me [text]")+";");
    strResult.append(tr("/topic [text]")+";");
    strResult.append(tr("/join [channel] [key] or /j [channel] [key]")+";");
    strResult.append(tr("/part or /p")+";");
    strResult.append(tr("/priv [nick]")+";");
    strResult.append(tr("/ignore [[+|-]nick]")+";");
    strResult.append(tr("/friend [[+|-]nick]")+";");
    strResult.append(tr("/whereis [nick] or /whois [nick] or /wi [nick] or /wii [nick]")+";");
    strResult.append(tr("/busy")+";");
    strResult.append(tr("/away [text]")+";");
    strResult.append(tr("/invite [nick]")+";");
    strResult.append(tr("/logout or /quit or /q")+";");
    strResult.append(tr("/kick [nick] [reason] or /k [nick] [reason]")+";");
    strResult.append(tr("/ban [[+|-]nick]")+";");
    strResult.append(tr("/banip [[+|-]nick]")+";");
    strResult.append(tr("/sop [[+|-]nick]")+";");
    strResult.append(tr("/op [[+|-]nick]")+";");
    strResult.append(tr("/moder [[+|-]nick] or /moderator [[+|-]nick]")+";");
    strResult.append(tr("/vip [[+|-]nick]")+";");
    strResult.append(tr("/help"));

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

    // convert emoticons
    strMessage.replace(QRegExp("//([a-zA-Z0-9_-]+)"), "%I\\1%");

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
        strReason = tr("No reason");

    return QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strReason);
}

QString Commands::cmd_ban()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if ((strNick.contains("*") == true) && (cPlusMinus == '+'))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText(tr("This ban will ban large numbers of people. Are you sure you want to use this ban?"));
        int iResult = msgBox.exec();

        if (iResult == QMessageBox::Ok)
            return QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick);
        else
            return QString::null;
    }

    if (cPlusMinus == '+')
        return QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick);
    else if (cPlusMinus == '-')
        return QString("CS BAN %1 DEL %2").arg(strChannel).arg(strNick);
    else
        return QString::null;
}

QString Commands::cmd_banip()
{
    if (strDataList.value(1).isEmpty() == true) return QString::null;

    QString strChannel = strChan;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  strNick[0];
    strNick = strNick.right(strNick.length()-1);

    if (cPlusMinus == '+')
        return QString("CS BANIP %1 ADD %2").arg(strChannel).arg(strNick);
    else if (cPlusMinus == '-')
        return QString("CS BANIP %1 DEL %2").arg(strChannel).arg(strNick);
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
