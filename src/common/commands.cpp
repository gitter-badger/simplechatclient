/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
#include "core.h"
#include "commands.h"

#ifdef Q_WS_WIN
    #include "winamp.h"
#endif

Commands::Commands(const QString &_strChan, const QString &_strData) : strChan(_strChan), strData(_strData)
{
    strDataList = strData.split(" ");
}

QString Commands::execute()
{
    if (strDataList.value(0).isEmpty())
        return strData;

    QString strCmd = strDataList[0].toLower();

    if ((strCmd == "join") || (strCmd == "j"))
        return cmdJoin();
    else if (strCmd == "priv")
        return cmdPriv();
    else if (strCmd == "ignore")
        return cmdIgnore();
    else if (strCmd == "friend")
        return cmdFriend();
    else if ((strCmd == "whereis") || (strCmd == "whois")  || (strCmd == "wi")  || (strCmd == "wii"))
        return cmdWhereis();
    else if (strCmd == "busy")
        return cmdBusy();
    else if (strCmd == "away")
        return cmdAway();
    else if (strCmd == "offmsg")
        return cmdOffmsg();
    else if ((strCmd == "logout") || (strCmd == "quit") || (strCmd == "q"))
        return cmdQuit();
    else if ((strCmd == "mp3") || (strCmd == "winamp"))
        return cmdMp3();
    else if ((strCmd == "help") || (strCmd == "pomoc"))
        return cmdHelp();

    if (strChan != STATUS)
    {
        if ((strCmd == "cycle") || (strCmd == "hop"))
            return cmdCycle();
        else if (strCmd == "me")
            return cmdMe();
        else if (strCmd == "topic")
            return cmdTopic();
        else if ((strCmd == "part") || (strCmd == "p"))
            return cmdPart();
        else if (strCmd == "invite")
            return cmdInvite();
        else if ((strCmd == "kick") || (strCmd == "k"))
            return cmdKick();
        else if (strCmd == "ban")
            return cmdBan();
        else if (strCmd == "banip")
            return cmdBanip();
        else if (strCmd == "sop")
            return cmdSop();
        else if (strCmd == "op")
            return cmdOp();
        else if ((strCmd == "moder") || (strCmd == "moderator"))
            return cmdModer();
        else if (strCmd == "vip")
            return cmdVip();
    }

    return strData;
}

bool Commands::isErotic(const QString &strChannel)
{
    for (int i = 0; i < Core::instance()->lChannelList.size(); i++)
    {
        ChannelList channel = Core::instance()->lChannelList.at(i);
        QString strName = channel.name;
        int iType = channel.type;

        if (strName.toLower() == strChannel.toLower())
        {
            if (iType == 3)
                return true;
            else
                return false;
        }
    }
    return false;
}

QString Commands::cmdJoin()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strDataList[1];
    if ((strChannel[0] != '#') && (strChannel[0] != '^'))
        strChannel = "#"+strChannel;

    QString strKey;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strKey += " "; strKey += strDataList[i]; }

    if (isErotic(strChannel))
    {
        if (Core::instance()->settings.value("age_check") == "true")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults."), tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
                Core::instance()->settings["age_check"] =  "false";
            else
                return QString::null;
        }
    }

    if (!strKey.isEmpty())
        return QString("JOIN %1 %2").arg(strChannel, strKey);
    else
        return QString("JOIN %1").arg(strChannel);
}

QString Commands::cmdPriv()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strNick = strDataList[1];

    return QString("PRIV %1").arg(strNick);
}

QString Commands::cmdIgnore()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if (cPlusMinus == '+')
        return QString("NS IGNORE ADD %1").arg(strNick);
    else if (cPlusMinus == '-')
        return QString("NS IGNORE DEL %1").arg(strNick);
    else
        return QString::null;
}

QString Commands::cmdFriend()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if (cPlusMinus == '+')
        return QString("NS FRIENDS ADD %1").arg(strNick);
    else if (cPlusMinus == '-')
        return QString("NS FRIENDS DEL %1").arg(strNick);
    else
        return QString::null;
}

QString Commands::cmdWhereis()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strNick = strDataList[1];

    return QString("WHOIS %1 :%1").arg(strNick);
}

QString Commands::cmdBusy()
{
    QString strBusy = Core::instance()->settings.value("busy");

    if (strBusy == "true")
        return "BUSY 0";
    else if (strBusy == "false")
        return "BUSY 1";
    else
        return QString::null;
}

QString Commands::cmdAway()
{
    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    return QString("AWAY :%1").arg(strMessage);
}

QString Commands::cmdOffmsg()
{
    if (strDataList.value(1).isEmpty()) return QString::null;
    if (strDataList.value(2).isEmpty()) return QString::null;

    QString strNick = strDataList[1];

    QString strMessage;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strMessage += " "; strMessage += strDataList[i]; }

    return QString("NS OFFLINE MSG %1 %2").arg(strNick, strMessage);
}

QString Commands::cmdQuit()
{
    QString strReason;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strReason += " "; strReason += strDataList[i]; }

    Core::instance()->settings["reconnect"] = "false";

    if (!strReason.isEmpty())
        return QString("QUIT :%1").arg(strReason);
    else
        return "QUIT";
}

QString Commands::cmdMp3()
{
#ifdef Q_WS_WIN
    Winamp *pWinamp = new Winamp();
    bool bIsRunning = pWinamp->isRunning();
    int iState = pWinamp->state();
    delete pWinamp;

    if (bIsRunning)
    {
        if (iState == 1)
        {
            Winamp *pWinamp = new Winamp();
            QString strVersion = pWinamp->version();
            QString strSong = pWinamp->song();
            QString strPosition = pWinamp->position();
            QString strLength = pWinamp->length();
            delete pWinamp;

            QString strWinamp = Core::instance()->settings.value("winamp");

            strWinamp.replace("$version", strVersion);
            strWinamp.replace("$song", strSong);
            strWinamp.replace("$position", strPosition);
            strWinamp.replace("$length", strLength);

            // emots
            strWinamp.replace(QRegExp("(ftp:|http:|https:)//"), "\\1\\\\"); // fix ftp http https
            strWinamp.replace(QRegExp("//([a-zA-Z0-9_-]+)\\b"), "%I\\1%");
            strWinamp.replace(QRegExp("(ftp:|http:|https:)\\\\\\\\"), "\\1//"); // fix ftp http https

            return strWinamp;
        }
        else if (iState == 3)
            return tr("Winamp is paused");
        else if (iState == 0)
            return tr("Winamp is not playing");
        else
            return tr("Winamp is not running");
    }
    else
        return tr("Winamp is not running");
#else
    return tr("This command is only for Windows");
#endif
}

QString Commands::cmdHelp()
{
    QString strResult;

    strResult = (tr("* Available commands:")+";");
    strResult.append(tr("/cycle [text] or /hop [text]")+";");
    strResult.append(tr("/me [text]")+";");
    strResult.append(tr("/topic [text]")+";");
    strResult.append(tr("/join [channel] [key] or /j [channel] [key]")+";");
    strResult.append(tr("/part [text] or /p [text]")+";");
    strResult.append(tr("/priv [nick]")+";");
    strResult.append(tr("/ignore [[+|-]nick]")+";");
    strResult.append(tr("/friend [[+|-]nick]")+";");
    strResult.append(tr("/whereis [nick] or /whois [nick] or /wi [nick] or /wii [nick]")+";");
    strResult.append(tr("/busy")+";");
    strResult.append(tr("/away [text]")+";");
    strResult.append(tr("/invite [nick]")+";");
    strResult.append(tr("/offmsg [nick] [text]")+";");
    strResult.append(tr("/logout [text] or /quit [text] or /q [text]")+";");
    strResult.append(tr("/kick [nick] [reason] or /k [nick] [reason]")+";");
    strResult.append(tr("/ban [[+|-]nick]")+";");
    strResult.append(tr("/banip [[+|-]nick]")+";");
    strResult.append(tr("/sop [[+|-]nick]")+";");
    strResult.append(tr("/op [[+|-]nick]")+";");
    strResult.append(tr("/moder [[+|-]nick] or /moderator [[+|-]nick]")+";");
    strResult.append(tr("/vip [[+|-]nick]")+";");
    strResult.append(tr("/mp3 or /winamp")+";");
    strResult.append(tr("/help"));

    return strResult;
}

QString Commands::cmdCycle()
{
    QString strChannel = strChan;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    if (!strMessage.isEmpty())
        return QString("PART %1 :%2\nJOIN %1").arg(strChannel, strMessage);
    else
        return QString("PART %1\nJOIN %1").arg(strChannel);
}

QString Commands::cmdMe()
{
    QString strChannel = strChan;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    return QString("PRIVMSG %1 :%2ACTION %3%4").arg(strChannel, QString(QByteArray("\x01")), strMessage, QString(QByteArray("\x01")));
}

QString Commands::cmdTopic()
{
    QString strChannel = strChan;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    // convert emoticons
    strMessage.replace(QRegExp("//([a-zA-Z0-9_-]+)"), "%I\\1%");

    return QString("CS SET %1 TOPIC %2").arg(strChannel, strMessage);
}

QString Commands::cmdPart()
{
    QString strChannel = strChan;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    if (!strMessage.isEmpty())
        return QString("PART %1 :%2").arg(strChannel, strMessage);
    else
        return QString("PART %1").arg(strChannel);
}

QString Commands::cmdInvite()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strNick = strDataList[1];
    QString strChannel = strChan;

    return QString("INVITE %1 %2").arg(strNick, strChannel);
}

QString Commands::cmdKick()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];

    QString strReason;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strReason += " "; strReason += strDataList[i]; }

    if (strReason.isEmpty())
        strReason = tr("No reason");

    return QString("KICK %1 %2 :%3").arg(strChannel, strNick, strReason);
}

QString Commands::cmdBan()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strChan;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if ((strNick.contains("*")) && (cPlusMinus == '+'))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText(tr("This ban will ban large numbers of people. Are you sure you want to use this ban?"));
        int iResult = msgBox.exec();

        if (iResult == QMessageBox::Ok)
            return QString("CS BAN %1 ADD %2").arg(strChannel, strNick);
        else
            return QString::null;
    }

    if (cPlusMinus == '+')
        return QString("CS BAN %1 ADD %2").arg(strChannel, strNick);
    else if (cPlusMinus == '-')
        return QString("CS BAN %1 DEL %2").arg(strChannel, strNick);
    else
        return QString::null;
}

QString Commands::cmdBanip()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strChan;

    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if (cPlusMinus == '+')
        return QString("CS BANIP %1 ADD %2").arg(strChannel, strNick);
    else if (cPlusMinus == '-')
        return QString("CS BANIP %1 DEL %2").arg(strChannel, strNick);
    else
        return QString::null;
}

QString Commands::cmdSop()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if (cPlusMinus == '+')
        return QString("CS OP %1 ADD %2").arg(strChannel, strNick);
    else if (cPlusMinus == '-')
        return QString("CS OP %1 DEL %2").arg(strChannel, strNick);
    else
        return QString::null;
}

QString Commands::cmdOp()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if (cPlusMinus == '+')
        return QString("CS HALFOP %1 ADD %2").arg(strChannel, strNick);
    else if (cPlusMinus == '-')
        return QString("CS HALFOP %1 DEL %2").arg(strChannel, strNick);
    else
        return QString::null;
}

QString Commands::cmdModer()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if (cPlusMinus == '+')
        return QString("CS MODERATOR %1 ADD %2").arg(strChannel, strNick);
    else if (cPlusMinus == '-')
        return QString("CS MODERATOR %1 DEL %2").arg(strChannel, strNick);
    else
        return QString::null;
}

QString Commands::cmdVip()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strChannel = strChan;
    QString strNick = strDataList[1];
    QChar cPlusMinus =  '+';

    if ((strNick[0] == '+') || (strNick[0] == '-'))
    {
        cPlusMinus = strNick[0];
        strNick.remove(0,1);
    }

    if (cPlusMinus == '+')
        return QString("CS VOICE %1 ADD %2").arg(strChannel, strNick);
    else if (cPlusMinus == '-')
        return QString("CS VOICE %1 DEL %2").arg(strChannel, strNick);
    else
        return QString::null;
}
