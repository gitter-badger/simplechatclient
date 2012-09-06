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

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include "channel_list_model.h"
#include "core.h"
#include "convert.h"
#include "message.h"
#include "replace.h"
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
    if (strDataList.size() == 0)
        return strData;

    QString strCmd = strDataList[0].toLower();

    if (strCmd == "raw")
        return cmdRaw();
    else if (strCmd == "all")
        return cmdAll();
    else if ((strCmd == "join") || (strCmd == "j"))
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

    if ((strChan != DEBUG) && (strChan != STATUS))
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
    QList<OnetChannelList> list = ChannelList::instance()->get();
    foreach (OnetChannelList channel, list)
    {
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

QString Commands::cmdRaw()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strCommand;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strCommand += " "; strCommand += strDataList[i]; }

    return strCommand;
}

QString Commands::cmdAll()
{
    if (strDataList.value(1).isEmpty()) return QString::null;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    return strMessage;
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
        if (Settings::instance()->get("age_check") == "true")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo16x16.png"));
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
    QString strBusy = Settings::instance()->get("busy");

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

    Convert::simpleReverseConvert(strMessage);
    Replace::replaceEmots(strMessage);

    return QString("AWAY :%1").arg(strMessage);
}

QString Commands::cmdOffmsg()
{
    if (strDataList.value(1).isEmpty()) return QString::null;
    if (strDataList.value(2).isEmpty()) return QString::null;

    QString strNick = strDataList[1];

    QString strMessage;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strMessage += " "; strMessage += strDataList[i]; }

    Convert::simpleReverseConvert(strMessage);
    Replace::replaceEmots(strMessage);

    return QString("NS OFFLINE MSG %1 %2").arg(strNick, strMessage);
}

QString Commands::cmdQuit()
{
    QString strReason;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strReason += " "; strReason += strDataList[i]; }

    Core::instance()->settings["reconnect"] = "false";

    if (!strReason.isEmpty())
    {
        Convert::simpleReverseConvert(strReason);
        Replace::replaceEmots(strReason);

        return QString("QUIT :%1").arg(strReason);
    }
    else
        return "QUIT";
}

QString Commands::cmdMp3()
{
    QString strMessage = QString::null;

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

            QString strWinamp = Settings::instance()->get("winamp");

            strWinamp.replace("$version", strVersion);
            strWinamp.replace("$song", strSong);
            strWinamp.replace("$position", strPosition);
            strWinamp.replace("$length", strLength);

            strMessage = strWinamp;
        }
        else if (iState == 3)
            strMessage = tr("Winamp is paused");
        else if (iState == 0)
            strMessage = tr("Winamp is not playing");
        else
            strMessage = tr("Winamp is not running");
    }
    else
        strMessage = tr("Winamp is not running");
#else
    strMessage = QString::null;
#endif

    return strMessage;
}

QString Commands::cmdHelp()
{
    QString strHelp;

    strHelp = (tr("* Available commands:")+";");
    strHelp.append(tr("/raw [text]")+";");
    strHelp.append(tr("/all [text]")+";");
    strHelp.append(tr("/cycle [text] or /hop [text]")+";");
    strHelp.append(tr("/me [text]")+";");
    strHelp.append(tr("/topic [text]")+";");
    strHelp.append(tr("/join [channel] [key] or /j [channel] [key]")+";");
    strHelp.append(tr("/part [text] or /p [text]")+";");
    strHelp.append(tr("/priv [nick]")+";");
    strHelp.append(tr("/ignore [[+|-]nick]")+";");
    strHelp.append(tr("/friend [[+|-]nick]")+";");
    strHelp.append(tr("/whereis [nick] or /whois [nick] or /wi [nick] or /wii [nick]")+";");
    strHelp.append(tr("/busy")+";");
    strHelp.append(tr("/away [text]")+";");
    strHelp.append(tr("/invite [nick]")+";");
    strHelp.append(tr("/offmsg [nick] [text]")+";");
    strHelp.append(tr("/logout [text] or /quit [text] or /q [text]")+";");
    strHelp.append(tr("/kick [nick] [reason] or /k [nick] [reason]")+";");
    strHelp.append(tr("/ban [[+|-]nick]")+";");
    strHelp.append(tr("/banip [[+|-]nick]")+";");
    strHelp.append(tr("/sop [[+|-]nick]")+";");
    strHelp.append(tr("/op [[+|-]nick]")+";");
    strHelp.append(tr("/moder [[+|-]nick] or /moderator [[+|-]nick]")+";");
    strHelp.append(tr("/vip [[+|-]nick]")+";");
    strHelp.append(tr("/mp3 or /winamp")+";");
    strHelp.append(tr("/help"));

    QStringList lHelp = strHelp.split(";");
    foreach (QString strDisplay, lHelp)
        Message::instance()->showMessage(strChan, strDisplay, MessageInfo);

    return QString::null;
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

    Convert::createText(strMessage);
    Convert::simpleReverseConvert(strMessage);
    Replace::replaceEmots(strMessage);

    return QString("PRIVMSG %1 :%2ACTION %3%4").arg(strChannel, QString(QByteArray("\x01")), strMessage, QString(QByteArray("\x01")));
}

QString Commands::cmdTopic()
{
    QString strChannel = strChan;

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    Convert::createText(strMessage);
    Convert::simpleReverseConvert(strMessage);
    Replace::replaceEmots(strMessage);

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
        msgBox.setWindowIcon(QIcon(":/images/logo16x16.png"));
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
