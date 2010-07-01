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

#include "irc_kernel.h"

IrcKernel::IrcKernel(Network *param1, QSettings *param2, TabContainer *param3, QMap <QString, QByteArray> *param4, DlgChannelSettings *param5, DlgChannelHomes *param6, DlgChannelList *param7, DlgChannelFavourites *param8, DlgFriends *param9, DlgIgnore *param10, DlgModeration *param11)
{
    pNetwork = param1;
    settings = param2;
    tabc = param3;
    mNickAvatar = param4;
    dlgchannel_settings = param5;
    dlgchannel_homes = param6;
    dlgchannel_list = param7;
    dlgchannel_favourites = param8;
    dlgfriends = param9;
    dlgignore = param10;
    dlgmoderation = param11;
}

void IrcKernel::remove_cathread(ChannelAvatar *cathr)
{
    cathr->QObject::disconnect();
    caThreadList.removeOne(cathr);

#ifdef Q_WS_X11
    if (settings->value("debug").toString() == "on")
        qDebug() << "Channel avatar thread -1 (size: " << caThreadList.size() << ")";
#endif
}

void IrcKernel::remove_nathread(NickAvatar *nathr)
{
    nathr->QObject::disconnect();
    naThreadList.removeOne(nathr);

#ifdef Q_WS_X11
    if (settings->value("debug").toString() == "on")
        qDebug() << "Nick avatar thread -1 (size: " << naThreadList.size() << ")";
#endif
}

void IrcKernel::kernel(QString param1)
{
    strData = param1;
    strDataList = strData.split(" ");

    if ((settings->value("debug_all").toString() == "on") && (strDataList.value(1).isEmpty() == false) && (strDataList[1].toLower() != "privmsg"))
       tabc->show_msg("Status", strData, 7);

    if (strDataList.value(1).isEmpty() == false)
    {
        if (strDataList[0].toLower() == "ping")
            raw_ping();
        else if (strDataList[0].toLower() == "error")
            raw_error();
        else if (strDataList[1].toLower() == "join")
            raw_join();
        else if (strDataList[1].toLower() == "part")
            raw_part();
        else if (strDataList[1].toLower() == "quit")
            raw_quit();
        else if (strDataList[1].toLower() == "kick")
            raw_kick();
        else if (strDataList[1].toLower() == "mode")
            raw_mode();
        else if (strDataList[1].toLower() == "privmsg")
            raw_privmsg();
        else if (strDataList[1].toLower() == "invite")
            raw_invite();
        else if (strDataList[1].toLower() == "topic")
            raw_topic();
        else if (strDataList[1].toLower() == "invreject")
            raw_invreject();
        else if (strDataList[1].toLower() == "invignore")
            raw_invignore();
        else if (strDataList[1].toLower() == "modermsg")
            raw_modermsg();
        else if (strDataList[1].toLower() == "modernotice")
            raw_modernotice();
        else if (strDataList[1].toLower() == "moderate")
            raw_moderate();
        else if (strDataList[1].toLower() == "001")
            raw_001();
        else if (strDataList[1].toLower() == "301")
            raw_301();
        else if (strDataList[1].toLower() == "305")
            raw_305();
        else if (strDataList[1].toLower() == "306")
            raw_306();
        else if (strDataList[1].toLower() == "307")
            raw_307();
        else if (strDataList[1].toLower() == "311")
            raw_311();
        else if (strDataList[1].toLower() == "312")
            raw_312();
        else if (strDataList[1].toLower() == "313")
            raw_313();
        else if (strDataList[1].toLower() == "317")
            raw_317();
        else if (strDataList[1].toLower() == "318")
            raw_318();
        else if (strDataList[1].toLower() == "319")
            raw_319();
        else if (strDataList[1].toLower() == "332")
            raw_332();
        else if (strDataList[1].toLower() == "341")
            raw_341();
        else if (strDataList[1].toLower() == "333")
            raw_333();
        else if (strDataList[1].toLower() == "353")
            raw_353();
        else if (strDataList[1].toLower() == "366")
            raw_366();
        else if (strDataList[1].toLower() == "372")
            raw_372();
        else if (strDataList[1].toLower() == "375")
            raw_375();
        else if (strDataList[1].toLower() == "376")
            raw_376();
        else if (strDataList[1].toLower() == "396")
            raw_396();
        else if (strDataList[1].toLower() == "401")
            raw_401();
        else if (strDataList[1].toLower() == "403")
            raw_403();
        else if (strDataList[1].toLower() == "404")
            raw_404();
        else if (strDataList[1].toLower() == "421")
            raw_421();
        else if (strDataList[1].toLower() == "432")
            raw_432();
        else if (strDataList[1].toLower() == "433")
            raw_433();
        else if (strDataList[1].toLower() == "443")
            raw_443();
        else if (strDataList[1].toLower() == "451")
            raw_451();
        else if (strDataList[1].toLower() == "470")
            raw_470();
        else if (strDataList[1].toLower() == "471")
            raw_471();
        else if (strDataList[1].toLower() == "473")
            raw_473();
        else if (strDataList[1].toLower() == "474")
            raw_474();
        else if (strDataList[1].toLower() == "482")
            raw_482();
        else if (strDataList[1].toLower() == "484")
            raw_484();
        else if (strDataList[1].toLower() == "530")
            raw_530();
        else if (strDataList[1].toLower() == "600")
            raw_600();
        else if (strDataList[1].toLower() == "601")
            raw_601();
        else if (strDataList[1].toLower() == "602")
            raw_602();
        else if (strDataList[1].toLower() == "604")
            raw_604();
        else if (strDataList[1].toLower() == "605")
            raw_605();
        else if (strDataList[1].toLower() == "801")
            raw_801();
        else if (strDataList[1].toLower() == "802")
            raw_802();
        else if (strDataList[1].toLower() == "807")
            raw_807();
        else if (strDataList[1].toLower() == "808")
            raw_808();
        else if (strDataList[1].toLower() == "809")
            raw_809();
        else if (strDataList[1].toLower() == "811")
            raw_811();
        else if (strDataList[1].toLower() == "812")
            raw_812();
        else if (strDataList[1].toLower() == "815")
            raw_815();
        else if (strDataList[1].toLower() == "817")
            raw_817();
        else if (strDataList[1].toLower() == "818")
            raw_818();
        else if (strDataList[1].toLower() == "819")
            raw_819();
        else if (strDataList[1].toLower() == "820")
            raw_820();
        else if (strDataList[1].toLower() == "821")
            raw_821();
        else if (strDataList[1].toLower() == "951")
            raw_951();
        else if ((strDataList[1].toLower() == "notice") && (strDataList.value(3).isEmpty() == false))
        {
            if (strDataList[3].toLower() == ":109")
                raw_109n();
            else if (strDataList[3].toLower() == ":111")
                raw_111n();
            else if (strDataList[3].toLower() == ":112")
                raw_112n();
            else if (strDataList[3].toLower() == ":121")
                raw_121n();
            else if (strDataList[3].toLower() == ":122")
                raw_122n();
            else if (strDataList[3].toLower() == ":131")
                raw_131n();
            else if (strDataList[3].toLower() == ":132")
                raw_132n();
            else if (strDataList[3].toLower() == ":141")
                raw_141n();
            else if (strDataList[3].toLower() == ":142")
                raw_142n();
            else if (strDataList[3].toLower() == ":151")
                raw_151n();
            else if (strDataList[3].toLower() == ":152")
                raw_152n();
            else if (strDataList[3].toLower() == ":160")
                raw_160n();
            else if (strDataList[3].toLower() == ":161")
                raw_161n();
            else if (strDataList[3].toLower() == ":162")
                raw_162n();
            else if (strDataList[3].toLower() == ":163")
                raw_163n();
            else if (strDataList[3].toLower() == ":164")
                raw_164n();
            else if (strDataList[3].toLower() == ":165")
                raw_165n();
            else if (strDataList[3].toLower() == ":220")
                raw_220n();
            else if (strDataList[3].toLower() == ":221")
                raw_221n();
            else if (strDataList[3].toLower() == ":230")
                raw_230n();
            else if (strDataList[3].toLower() == ":231")
                raw_231n();
            else if (strDataList[3].toLower() == ":240")
                raw_240n();
            else if (strDataList[3].toLower() == ":241")
                raw_241n();
            else if (strDataList[3].toLower() == ":250")
                raw_250n();
            else if (strDataList[3].toLower() == ":251")
                raw_251n();
            else if (strDataList[3].toLower() == ":252")
                raw_252n();
            else if (strDataList[3].toLower() == ":253")
                raw_253n();
            else if (strDataList[3].toLower() == ":254")
                raw_254n();
            else if (strDataList[3].toLower() == ":255")
                raw_255n();
            else if (strDataList[3].toLower() == ":256")
                raw_256n();
            else if (strDataList[3].toLower() == ":257")
                raw_257n();
            else if (strDataList[3].toLower() == ":258")
                raw_258n();
            else if (strDataList[3].toLower() == ":259")
                raw_259n();
            else if (strDataList[3].toLower() == ":260")
                raw_260n();
            else if (strDataList[3].toLower() == ":261")
                raw_261n();
            else if (strDataList[3].toLower() == ":400")
                raw_400n();
            else if (strDataList[3].toLower() == ":401")
                raw_401n();
            else if (strDataList[3].toLower() == ":404")
                raw_404n();
            else if (strDataList[3].toLower() == ":406")
                raw_406n();
            else if (strDataList[3].toLower() == ":407")
                raw_407n();
            else if (strDataList[3].toLower() == ":408")
                raw_408n();
            else if (strDataList[3].toLower() == ":412")
                raw_412n();
            else if (strDataList[3].toLower() == ":420")
                raw_420n();
            else if (strDataList[3].toLower() == ":421")
                raw_421n();
            else if (strDataList[3].toLower() == ":430")
                raw_430n();
            else if (strDataList[3].toLower() == ":431")
                raw_431n();
            else if (strDataList[3].toLower() == ":440")
                raw_440n();
            else if (strDataList[3].toLower() == ":441")
                raw_441n();
            else if (strDataList[3].toLower() == ":452")
                raw_452n();
            else if (strDataList[3].toLower() == ":454")
                raw_454n();
            else if (strDataList[3].toLower() == ":458")
                raw_458n();
            else if (strDataList[3].toLower() == ":459")
                raw_459n();
            else if (strDataList[3].toLower() == ":461")
                raw_461n();
            else if (strDataList[3].toLower() == ":463")
                raw_463n();
            else if (strDataList[3].toLower() == ":464")
                raw_464n();
            else if (strDataList[3].toLower() == ":467")
                raw_467n();
            else if (strDataList[3].toLower() == ":468")
                raw_468n();
            else if (strDataList[3].toLower() == ":472")
                raw_472n();
            else if (strDataList[1].toLower() == "notice")
                raw_notice();
            else
            {
#ifndef Q_WS_X11
                if (settings->value("debug").toString() == "on")
                    tabc->show_msg("Status", strData, 7);
#endif
            }
        }
        else
        {
#ifndef Q_WS_X11
            if (settings->value("debug").toString() == "on")
                tabc->show_msg("Status", strData, 7);
#endif
        }
    }

#ifdef Q_WS_X11
    if (settings->value("debug").toString() == "on")
        qDebug() << "<- " << strData;
#endif
}

// PING :cf1f1.onet
void IrcKernel::raw_ping()
{
    if (strDataList.value(1).isEmpty() == true) return;

    QString strServer = strDataList[1];

    if (strServer.isEmpty() == false)
        pNetwork->send(QString("PONG %1").arg(strServer));
}

// ERROR :Closing link (unknown@95.48.183.154) [Registration timeout]
void IrcKernel::raw_error()
{
    QString strMessage;
    for (int i = 0; i < strDataList.size(); i++) { if (i != 0) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    tabc->show_msg_all(strMessage, 7);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 JOIN #Quiz :rx,0
void IrcKernel::raw_join()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strZUO = strDataList[0];
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList[0];
    if (strIP[0] == ':')
        strIP = strIP.right(strIP.length()-1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strChannel = strDataList[2];
    if (strChannel[0] == ':')
        strChannel = strChannel.right(strChannel.length()-1);

    QString strSuffix = strDataList[3];
    if (strSuffix[0] == ':')
        strSuffix = strSuffix.right(strSuffix.length()-1);

    QString strDisplay;
    if (settings->value("show_zuo").toString() == "on")
        strDisplay = QString("* %1 [%2@%3] wszed³ do kana³u %4").arg(strNick).arg(strZUO).arg(strIP).arg(strChannel);
    else
        strDisplay = QString("* %1 [%2] wszed³ do kana³u %3").arg(strNick).arg(strIP).arg(strChannel);

    tabc->show_msg(strChannel, strDisplay, 1);

    Config *pConfig = new Config();
    QString strMe = pConfig->get_value("login-nick");
    delete pConfig;

    if (strNick == strMe)
    {
        tabc->update_nick(strNick);
        if (tabc->exist_tab(strChannel))
            tabc->clear_nicklist(strChannel);
    }
    if ((strNick == strMe) && (strChannel[0] != '^'))
        pNetwork->send(QString("CS INFO %1 i").arg(strChannel));

    tabc->add_user(strChannel, strNick, strSuffix, 1);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 PART #scc
void IrcKernel::raw_part()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strZUO = strDataList[0];
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList[0];
    if (strIP[0] == ':')
        strIP = strIP.right(strIP.length()-1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strChannel = strDataList[2];
    if (strChannel[0] == ':')
        strChannel = strChannel.right(strChannel.length()-1);

    QString strReason;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strReason += " "; strReason += strDataList[i]; }
    if (strReason[0] == ':')
        strReason = strReason.right(strReason.length()-1);

    QString strDisplay;
    if (strReason.isEmpty() == false)
    {
        if (settings->value("show_zuo").toString() == "on")
            strDisplay = QString("* %1 [%2@%3] wyszed³ z kana³u %4. Powód: %5").arg(strNick).arg(strZUO).arg(strIP).arg(strChannel).arg(strReason);
        else
            strDisplay = QString("* %1 [%2] wyszed³ z kana³u %3. Powód: %4").arg(strNick).arg(strIP).arg(strChannel).arg(strReason);
    }
    else
    {
        if (settings->value("show_zuo").toString() == "on")
            strDisplay = QString("* %1 [%2@%3] wyszed³ z kana³u %4").arg(strNick).arg(strZUO).arg(strIP).arg(strChannel);
        else
            strDisplay = QString("* %1 [%2] wyszed³ z kana³u %3").arg(strNick).arg(strIP).arg(strChannel);
    }

    if (tabc->exist_tab(strChannel) == true) // fix for self part
        tabc->show_msg(strChannel, strDisplay, 2);

    tabc->del_user(strChannel, strNick);

    // if self part

    Config *pConfig = new Config();
    QString strMe = pConfig->get_value("login-nick");
    delete pConfig;

    if (strNick == strMe)
    {
        if (strChannel != "Status")
        {
            if (strChannel[0] == '#')
                tabc->remove_tab(strChannel);
            else
            {
                QString strNewChannel = get_settings_key(strChannel);
                if (strNewChannel.isEmpty() == false)
                {
                    tabc->remove_tab(strChannel);
                    QString strRemoveSettingsKey = "priv"+strNewChannel;
                    if (settings->contains(strRemoveSettingsKey) == true)
                        settings->remove(strRemoveSettingsKey);
                }
                else
                {
                    tabc->remove_tab(strChannel);
                    QString strRemoveSettingsKey = "priv"+strChannel;
                    if (settings->contains(strRemoveSettingsKey) == true)
                        settings->remove(strRemoveSettingsKey);
                }
            }
        }
    }
}

// :Stark!38566204@A5F2F1.68FE5E.DE32AF.62ECB9 QUIT :Client exited
void IrcKernel::raw_quit()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strZUO = strDataList[0];
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList[0];
    if (strIP[0] == ':')
        strIP = strIP.right(strIP.length()-1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strReason;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strReason += " "; strReason += strDataList[i]; }
    if (strReason[0] == ':')
        strReason = strReason.right(strReason.length()-1);

    QString strDisplay;
    if (settings->value("show_zuo").toString() == "on")
        strDisplay = QString("* %1 [%2@%3] opu¶ci³ irc [%4]").arg(strNick).arg(strZUO).arg(strIP).arg(strReason);
    else
        strDisplay = QString("* %1 [%2] opu¶ci³ irc [%3]").arg(strNick).arg(strIP).arg(strReason);

    if (mNickAvatar->contains(strNick) == true)
        mNickAvatar->remove(strNick);

    tabc->quit_user(strNick, strDisplay);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 KICK #scc Moment_w_atmosferze :sio
void IrcKernel::raw_kick()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strZUO = strDataList[0];
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList[0];
    if (strIP[0] == ':')
        strIP = strIP.right(strIP.length()-1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strChannel = strDataList[2];
    if (strChannel[0] == ':')
        strChannel = strChannel.right(strChannel.length()-1);

    QString strNick = strDataList[3];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strReason;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strReason += " "; strReason += strDataList[i]; }
    if (strReason[0] == ':')
        strReason = strReason.right(strReason.length()-1);

    QString strDisplay;
    strDisplay = QString("* %1 zosta³ wyrzucony z kana³u %2 przez %3. Powód: %4").arg(strNick).arg(strChannel).arg(strWho).arg(strReason);

    if (tabc->exist_tab(strChannel) == true)
        tabc->show_msg(strChannel, strDisplay, 4);

    tabc->del_user(strChannel, strNick);

    Config *pConfig = new Config();
    QString strMe = pConfig->get_value("login-nick");
    delete pConfig;

    if (strNick == strMe)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText(QString("%1 wyrzuci³ Cie z kana³u %2. Powód: <br>%3").arg(strWho).arg(strChannel).arg(strReason));
        msgBox.exec();

        tabc->remove_tab(strChannel);
    }
}

// :Merovingian!26269559@jest.piekny.i.uroczy.ma.przesliczne.oczy MODE Merovingian :+b
// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D MODE Merovingian :+b
// :ankaszo!51613093@F4C727.446F67.966AC9.BAAE26 MODE ankaszo -W
// :NickServ!service@service.onet MODE scc_test +r
// :ChanServ!service@service.onet MODE #scc +ips
// :ChanServ!service@service.onet MODE #scc +o scc_test
// :ChanServ!service@service.onet MODE #scc +eo *!51976824@* scc_test
// :ChanServ!service@service.onet MODE #abc123 +il-e 1 *!51976824@*
void IrcKernel::raw_mode()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strNickChannel = strDataList[2];

    if (strNickChannel[0] == '#')
    {
        if (strDataList.value(4).isEmpty() == true)
        {
            QString strFlag = strDataList[3];
            QString strDisplay;
            if (strFlag == "+ips") strDisplay = QString("* Kana³ %1 jest teraz kana³em prywatnym (Ustawi³ %2)").arg(strNickChannel).arg(strWho);
            else if (strFlag == "-ips") strDisplay = QString("* Kana³ %1 nie jest ju¿ kana³em prywatnym (Ustawi³ %2)").arg(strNickChannel).arg(strWho);
            else if (strFlag == "+m") strDisplay = QString("* Kana³ %1 jest teraz kana³em moderowanym (Ustawi³ %2)").arg(strNickChannel).arg(strWho);
            else if (strFlag == "-m") strDisplay = QString("* Kana³ %1 nie jest ju¿ kana³em moderowanym (Ustawi³ %2)").arg(strNickChannel).arg(strWho);
            else if (strFlag == "+u") strDisplay = QString("* Kana³ %1 jest teraz kana³em audytywnym (Ustawi³ %2)").arg(strNickChannel).arg(strWho);
            else if (strFlag == "-u") strDisplay = QString("* Kana³ %1 nie jest ju¿ kana³em audytywnym (Ustawi³ %2)").arg(strNickChannel).arg(strWho);
            else strDisplay = QString("* Kana³ %1 ma teraz flage %2 (Ustawi³ %3)").arg(strNickChannel).arg(strFlag).arg(strWho);
            tabc->show_msg(strNickChannel, strDisplay, 5);
        }
        else
        {
            QString strFlag = strDataList[3];
            QString strDisplay;
            int index = 4;
            QString plusminus = QString::null;
            QMap<QString, QString> flag_nick;

            for (int i = 0; i < strFlag.length(); i++)
            {
                QString c = strFlag.at(i);
                if ((c == "+") || (c == "-"))
                {
                    if (c == "+") plusminus = "+";
                    else if (c == "-") plusminus = "-";
                }
                else
                {
                    if (index < strDataList.size())
                        flag_nick.insert(plusminus+c, strDataList[index]);
                    index++;
                }
            }

            QMap<QString, QString>::const_iterator i = flag_nick.constBegin();
            while (i != flag_nick.constEnd())
            {
                QString strFlag = i.key();
                QString strNick = i.value();

                if ((strFlag != "+e") && (strFlag != "-e"))
                {
                    if (strFlag == "+q") strDisplay = QString("* %1 jest teraz w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-q") strDisplay = QString("* %1 nie jest ju¿ w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "+o") strDisplay = QString("* %1 jest teraz super operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-o") strDisplay = QString("* %1 nie jest ju¿ super operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "+h") strDisplay = QString("* %1 jest teraz operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-h") strDisplay = QString("* %1 nie jest ju¿ operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "+v") strDisplay = QString("* %1 jest teraz go¶ciem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-v") strDisplay = QString("* %1 nie jest ju¿ go¶ciem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "+X") strDisplay = QString("* %1 jest teraz moderatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-X") strDisplay = QString("* %1 nie jest ju¿ moderatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "+Y") strDisplay = QString("* %1 jest teraz screenerem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-Y") strDisplay = QString("* %1 nie jest ju¿ screenerem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "+b") strDisplay = QString("* %1 jest teraz na li¶cie wyproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-b") strDisplay = QString("* %1 nie jest ju¿ na li¶cie wyproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "+I") strDisplay = QString("* %1 jest teraz na li¶cie zaproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else if (strFlag == "-I") strDisplay = QString("* %1 nie jest ju¿ na li¶cie zaproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strNickChannel).arg(strWho);
                    else
                        strDisplay = QString("* %1 ma teraz flage %2 (Ustawi³ %3)").arg(strNick).arg(strFlag).arg(strWho);

                    tabc->show_msg(strNickChannel, strDisplay, 5);
                    tabc->change_flag(strNick, strNickChannel, strFlag);
                }
                ++i;
            }
            flag_nick.clear();
        }
    }
    else
    {
        if (strWho == strNickChannel)
        {
            QString strFlag = strDataList[3];
            if (strFlag[0] == ':') strFlag = strFlag.right(strFlag.length()-1);
            QString strDisplay;

            if (strFlag == "+b") strDisplay = QString("* %1 jest oznaczony jako zajêty").arg(strNickChannel);
            else if (strFlag == "-b") strDisplay = QString("* %1 nie jest ju¿ oznaczony jako zajêty").arg(strNickChannel);
            else if (strFlag == "+W") strDisplay = QString("* %1 ma teraz w³±czon± kamerke publiczn±").arg(strNickChannel);
            else if (strFlag == "-W") strDisplay = QString("* %1 ma teraz wy³±czon± kamerke publiczn±").arg(strNickChannel);
            else if (strFlag == "+V") strDisplay = QString("* %1 ma teraz w³±czon± kamerke prywatn±").arg(strNickChannel);
            else if (strFlag == "-V") strDisplay = QString("* %1 ma teraz wy³±czon± kamerke prywatn±").arg(strNickChannel);
            else if (strFlag == "+x") strDisplay = QString("* %1 ma teraz szyfrowane IP").arg(strNickChannel);
            else if (strFlag == "-x") strDisplay = QString("* %1 nie ma ju¿ szyfrowanego IP").arg(strNickChannel);
            else
                strDisplay = QString("* %1 ma teraz flage %2").arg(strNickChannel).arg(strFlag);

            if ((strFlag != "+W") && (strFlag != "-W") && (strFlag != "+V") && (strFlag != "-V"))
                tabc->show_msg("Status", strDisplay, 5);

            tabc->change_flag(strNickChannel, strFlag);
        }
        else
        {
            QString strFlag = strDataList[3];
            QString strDisplay;

            if (strFlag == "+r") strDisplay = QString("* %1 jest oznaczony jako ograniczony").arg(strNickChannel);
            else if (strFlag == "-r") strDisplay = QString("* %1 nie jest ju¿ oznaczony jako ograniczony").arg(strNickChannel);
            else if (strFlag == "+b") strDisplay = QString("* %1 jest oznaczony jako zajêty").arg(strNickChannel);
            else if (strFlag == "-b") strDisplay = QString("* %1 nie jest ju¿ oznaczony jako zajêty").arg(strNickChannel);
            else
                strDisplay = QString("* %1 ma teraz flage %2").arg(strNickChannel).arg(strFlag);

            tabc->show_msg("Status", strDisplay, 5);
            tabc->change_flag(strNickChannel, strFlag);
        }
    }
}

// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D PRIVMSG #scc :hello
void IrcKernel::raw_privmsg()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strChannel = strDataList[2];
    if (strChannel[0] == ':')
        strChannel = strChannel.right(strChannel.length()-1);

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = QString("<%1> %2").arg(strNick).arg(strMessage);
    if ((strChannel[0] == '#') || (strChannel[0] == '^'))
        tabc->show_msg(strChannel, strDisplay, 0);
    else
        tabc->show_msg(strNick, strDisplay, 0);
}

// :cf1f2.onet NOTICE scc_test :Your message has been filtered and opers notified: spam #2480
void IrcKernel::raw_notice()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strNick = strDataList[2];

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = QString("-%1- %2").arg(strWho).arg(strMessage);
    if (strNick[0] == '#')
        tabc->show_msg(strNick, strDisplay, 6);
    else
        tabc->show_msg_active(strDisplay, 6);
}

// :osa1987!47751777@F4C727.DA810F.7E1789.E71ED5 INVITE scc_test :^cf1f41437962
// :Merovingian!26269559@jest.piekny.i.uroczy.ma.przesliczne.oczy INVITE scc_test :#Komputery
void IrcKernel::raw_invite()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strIP = strDataList[0];
    if (strIP[0] == ':')
        strIP = strIP.right(strIP.length()-1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strWhere = strDataList[3];
    if (strWhere[0] == ':')
        strWhere = strWhere.right(strWhere.length()-1);

    // priv
    if (strWhere[0] == '^')
    {
        (new DlgPriv(pNetwork, settings, tabc, strWho, strWhere))->show();
    }
    // channel
    else
    {
        QString strDisplay = QString("* %1[%2] zaprasza Cie do kana³u %3. Napisz /j %4 aby tam wej¶æ").arg(strWho).arg(strIP).arg(strWhere).arg(strWhere);
        tabc->show_msg_active(strDisplay, 7);
    }
}

// :cf1f3.onet TOPIC #scc :Simple Chat Client; current version: beta;
void IrcKernel::raw_topic()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList[2];

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = QString("* %1 ustawi³ nowy temat: %2").arg(strWho).arg(strMessage);
    tabc->show_msg(strChannel, strDisplay, 5);

    tabc->set_topic(strChannel, strMessage);
}

// :Merovingian!26269559@2294E8.94913F.2E3993.4AF50D INVREJECT scc_test ^cf1f41038619
void IrcKernel::raw_invreject()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList[3];

    QString strDisplay = QString("* %1 odrzuci³/a zaproszenie do rozmowy prywatnej").arg(strWho);
    tabc->show_msg(strChannel, strDisplay, 7);
}

// :Merovingian!26269559@2294E8.94913F.A00186.4A2B76 INVIGNORE scc_test ^cf1f31294352
void IrcKernel::raw_invignore()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList[3];

    QString strDisplay = QString("* %1 zignorowa³/a Twoje zaproszenie do rozmowy prywatnej").arg(strWho);
    tabc->show_msg(strChannel, strDisplay, 7);

    tabc->rename_tab(strChannel, strWho);
}

// :~testa!anonymous@3DE379.B7103A.6CF799.6902F4 MODERMSG test1 - #Scrabble :%F:verdana%%Ihehe%
void IrcKernel::raw_modermsg()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[2];

    QString strChannel = strDataList[4];

    QString strMessage;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = "<"+strNick+"> "+strMessage;
    tabc->show_msg(strChannel, strDisplay, 0);
}

// :~testa!anonymous@3DE379.B7103A.6CF799.6902F4 MODERNOTICE #Scrabble :a
void IrcKernel::raw_modernotice()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strChannel = strDataList[2];

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = "* <"+strNick+"> "+strMessage;
    tabc->show_msg(strChannel, strDisplay, 6);
}

// :cf1f1.onet MODERATE ~testa opnick #channel cf1f44c3b4b870f8a :%F:verdana%ladnie to tak
void IrcKernel::raw_moderate()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strChannel = strDataList[4];
    QString strID = strDataList[5];

    QString strMessage;
    for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    dlgmoderation->add_msg(strID, strChannel, strNick, strMessage);
}

// :cf1f4.onet 001 scc_test :Welcome to the OnetCzat IRC Network scc_test!51976824@83.28.35.219
void IrcKernel::raw_001()
{
// busy
    settings->setValue("logged", "on");

// protocol
    pNetwork->send("PROTOCTL ONETNAMESX");

// auto rejoin
    QStringList strlOpenChannels = tabc->get_open_channels();
    for (int i = 0; i < strlOpenChannels.size(); i++)
        pNetwork->send(QString("JOIN %1").arg(strlOpenChannels[i]));

// busy
    settings->setValue("busy", "off");

// clear friends
    dlgfriends->clear();

// auto busy
    if (settings->value("auto_busy").toString() == "on")
        pNetwork->send("BUSY 1");

// autojoin favourites
    settings->setValue("autojoin_favourites", "on");

}

// :GuardServ!service@service.onet NOTICE scc_test :109 #scc :rzucanie miêsem nie bêdzie tolerowane
void IrcKernel::raw_109n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage[0] = strMessage[0].toUpper();

    strMessage = QString("* %1").arg(strMessage);
    tabc->show_msg(strChannel, strMessage, 6);
}

// :NickServ!service@service.onet NOTICE Merovingian :111 aleksa7 type :2
void IrcKernel::raw_111n()
{
    QString strNick = strDataList[4];
    QString strInfo = strDataList[5];

    QString strValue;
    for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strValue += " "; strValue += strDataList[i]; }
    if (strValue[0] == ':')
        strValue = strValue.right(strValue.length()-1);

    if (strValue.isEmpty() == false)
    {
        if (strInfo == "avatar")
        {
            naThreadList.append(new NickAvatar(tabc, strNick, strValue, mNickAvatar));
            QObject::connect(naThreadList.at(naThreadList.size()-1), SIGNAL(do_remove_nathread(NickAvatar*)), this, SLOT(remove_nathread(NickAvatar*)));

#ifdef Q_WS_X11
            if (settings->value("debug").toString() == "on")
                qDebug() << "Nick avatar thread +1 (size: " << naThreadList.size() << ")";
#endif
        }
    }
}

// :NickServ!service@service.onet NOTICE Merovingian :112 aleksa7 :end of user info
void IrcKernel::raw_112n()
{
// ignore
}

// :NickServ!service@service.onet NOTICE scc_test :121 :scc_test Merovingian Succubi Radowsky
void IrcKernel::raw_121n()
{
    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strNick = strDataList[i];
        if (strNick[0] == ':')
            strNick = strNick.right(strNick.length()-1);

        dlgfriends->set_friend(strNick, false);
    }
}

// :NickServ!service@service.onet NOTICE scc_test :122 :end of friend list
void IrcKernel::raw_122n()
{
// ignore
}

// :NickServ!service@service.onet NOTICE scc_test :131 :arabeska22 test wilk ~test
void IrcKernel::raw_131n()
{
    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strNick = strDataList[i];
        if (strNick[0] == ':')
            strNick = strNick.right(strNick.length()-1);

        dlgignore->add_ignore(strNick);
    }
}

// :NickServ!service@service.onet NOTICE scc_test :132 :end of ignore list
void IrcKernel::raw_132n()
{
// ignore
}

// :NickServ!service@service.onet NOTICE scc_test :141 :#Scrabble #Quiz #scc
void IrcKernel::raw_141n()
{
    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strChannel = strDataList[i];
        if (strChannel[0] == ':')
            strChannel = strChannel.right(strChannel.length()-1);

        dlgchannel_favourites->add_channel(strChannel);

        if (settings->value("autojoin_favourites").toString() == "on")
            pNetwork->send(QString("JOIN %1").arg(strChannel));
    }

    // turn off autojoin
    if (settings->value("autojoin_favourites").toString() == "on")
        settings->setValue("autojoin_favourites", "off");
}

// :NickServ!service@service.onet NOTICE scc_test :142 :end of favourites list
void IrcKernel::raw_142n()
{
// ignore
}

// :ChanServ!service@service.onet NOTICE scc_test :151 :h#scc
void IrcKernel::raw_151n()
{
    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strChannel = strDataList[i];
        if (strChannel[0] == ':')
            strChannel = strChannel.right(strChannel.length()-1);

        dlgchannel_homes->add_channel(strChannel);
    }
}

// :ChanServ!service@service.onet NOTICE scc_test :152 :end of homes list
void IrcKernel::raw_152n()
{
// ignore
}

// :ChanServ!service@service.onet NOTICE scc_test :160 #scc :Simple Chat Client;
void IrcKernel::raw_160n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strTopic;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strTopic += " "; strTopic += strDataList[i]; }
    if (strTopic[0] == ':')
        strTopic = strTopic.right(strTopic.length()-1);

    dlgchannel_settings->add_topic(strTopic);
    tabc->set_topic(strChannel, strTopic);
}

// :ChanServ!service@service.onet NOTICE scc_test :161 #scc :topicAuthor=Merovingian rank=0.9095 topicDate=1251579281 private=1 password= limit=0 type=0 createdDate=1247005186 vEmail=0 www=http://simplechatclien.sourceforge.net/ catMajor=4 catMinor=0 official=0 recommended=0 protected=0 moderated=0 avatar=http://foto0.m.onet.pl/_m/e7bd33787bb5cd96031db4034e5f1d54,1,19,0.jpg status=ok guardian=3 auditorium=0
void IrcKernel::raw_161n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strTopicAuthor;
    int iCatMajor = 0;

    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList[i];
        if (i == 5) strLine = strLine.right(strLine.length()-1);
        QString strKey = strLine.left(strLine.indexOf("="));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf("=")-1);

        if ((strKey.isEmpty() == false) && (strValue.isEmpty() == false))
        {
            if (strKey == "www")
            {
                tabc->set_link(strChannel, strValue);
                dlgchannel_settings->add_www(strValue);
            }
            else if (strKey == "topicAuthor")
                strTopicAuthor = strValue;
            else if (strKey == "topicDate")
            {
                if (strTopicAuthor.isEmpty() == false)
                {
                    QString strTime = strValue;
                    QDateTime dt = QDateTime::fromTime_t(strTime.toInt());
                    QString strDT = dt.toString("dd/MM/yy hh:mm:ss");
                    QString strTopicDetails = QString("%1 (%2)").arg(strTopicAuthor).arg(strDT);
                    tabc->author_topic(strChannel, strTopicDetails);
                }
            }
            else if (strKey == "private")
            {
                if (strValue == "0")
                    dlgchannel_settings->add_pubpriv(0);
                else if (strValue == "1")
                    dlgchannel_settings->add_pubpriv(1);
            }
            else if (strKey == "catMajor")
                iCatMajor = strValue.toInt();
            else if (strKey == "catMinor")
                dlgchannel_settings->add_cat(iCatMajor, strValue.toInt());
            else if (strKey == "guardian")
                dlgchannel_settings->add_guardian(strValue.toInt());
            else if (strKey == "moderated")
                dlgchannel_settings->add_moderated(strValue.toInt());
            else if (strKey == "createdDate")
                dlgchannel_settings->add_created(strValue);
            else if (strKey == "password")
                dlgchannel_settings->add_password(strValue);
            else if (strKey == "limit")
                dlgchannel_settings->add_limit(strValue.toInt());
            else if (strKey == "auditorium")
                dlgchannel_settings->add_auditorium(strValue.toInt());
            else if (strKey == "email")
                dlgchannel_settings->add_email(strValue);
            else if (strKey == "avatar")
            {
                QString strUrl = strValue;

                caThreadList.append(new ChannelAvatar(tabc, strChannel, strUrl));
                QObject::connect(caThreadList.at(caThreadList.size()-1), SIGNAL(do_remove_cathread(ChannelAvatar*)), this, SLOT(remove_cathread(ChannelAvatar*)));

#ifdef Q_WS_X11
                if (settings->value("debug").toString() == "on")
                    qDebug() << "Channel avatar thread +1 (size: " << caThreadList.size() << ")";
#endif
            }
        }
    }
}

// :ChanServ!service@service.onet NOTICE scc_test :162 #lunar :q,Merovingian o,Radowsky o,aleksa7 o,chanky o,osa1987 h,scc_test o,MajkeI
void IrcKernel::raw_162n()
{
    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList[i];
        if (i == 5) strLine = strLine.right(strLine.length()-1);
        QString strKey = strLine.left(strLine.indexOf(","));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf(",")-1);

        if ((strKey.isEmpty() == false) && (strValue.isEmpty() == false))
        {
            if (strKey == "q")
                dlgchannel_settings->add_owner(strValue);
            else if (strKey == "o")
                dlgchannel_settings->add_op(strValue);
            else if (strKey == "h")
                dlgchannel_settings->add_halfop(strValue);
        }
    }
}

// :ChanServ!service@service.onet NOTICE scc_test :163 #scc b test2!*@* Merovingian :1253230938
// :ChanServ!service@service.onet NOTICE scc_test :163 #lunar I Olka Merovingian :1252595321
void IrcKernel::raw_163n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;
    if (strDataList.value(7).isEmpty() == true) return;
    if (strDataList.value(8).isEmpty() == true) return;

    QString strFlag = strDataList[5];
    QString strNick = strDataList[6];
    QString strWho = strDataList[7];
    QString strDT = strDataList[8];
    if (strDT[0] == ':')
        strDT = strDT.right(strDT.length()-1);

    QDateTime dt = QDateTime::fromTime_t(strDT.toInt());
    strDT = dt.toString("dd/MM/yyyy hh:mm:ss");

    if (strFlag == "b")
        dlgchannel_settings->add_ban(strNick, strWho, strDT);
    else if (strFlag == "I")
        dlgchannel_settings->add_invite(strNick, strWho, strDT);
}

// :ChanServ!service@service.onet NOTICE scc_test :164 #scc :end of channel info
void IrcKernel::raw_164n()
{
// ignore
}

// :ChanServ!service@service.onet NOTICE ~test :165 #Relax :Nie wa¿ne, czy szukasz dobrej zabawy, ...
void IrcKernel::raw_165n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strDescription;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strDescription += " "; strDescription += strDataList[i]; }
    if (strDescription[0] == ':')
        strDescription = strDescription.right(strDescription.length()-1);

    dlgchannel_settings->add_description(strDescription);
}

// :NickServ!service@service.onet NOTICE scc_test :220 aaa :friend added to list
void IrcKernel::raw_220n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString("* Dodano nick %1 do listy przyjació³").arg(strNick);
    tabc->show_msg_active(strDisplay, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :221 scc_test :friend removed from list
void IrcKernel::raw_221n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString("* Usuniêto nick %1 z listy przyjació³").arg(strNick);
    tabc->show_msg_active(strDisplay, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :230 ~test :ignore added to list
void IrcKernel::raw_230n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString("* Dodano %1 do listy ignorowanych").arg(strNick);
    tabc->show_msg_active(strDisplay, 7);

    dlgignore->clear();
    pNetwork->send("NS IGNORE");
}

// :NickServ!service@service.onet NOTICE scc_test :231 ~test :ignore removed from list
void IrcKernel::raw_231n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString("* Usuniêto %1 z listy ignorowanych").arg(strNick);
    tabc->show_msg_active(strDisplay, 7);

    dlgignore->clear();
    pNetwork->send("NS IGNORE");
}

// :NickServ!service@service.onet NOTICE scc_test :240 #scc :favourite added to list
void IrcKernel::raw_240n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDisplay = QString("* Dodano kana³ %1 do listy ulubionych").arg(strChannel);
    tabc->show_msg_active(strDisplay, 7);

    dlgchannel_favourites->clear();
    pNetwork->send("NS FAVOURITES");
}

// :NickServ!service@service.onet NOTICE scc_test :241 #scc :favourite removed from list
void IrcKernel::raw_241n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDisplay = QString("* Usuniêto kana³ %1 z listy ulubionych").arg(strChannel);
    tabc->show_msg_active(strDisplay, 7);

    dlgchannel_favourites->clear();
    pNetwork->send("NS FAVOURITES");
}

// :ChanServ!service@service.onet NOTICE scc_test :250 #czesctoja :channel registered
void IrcKernel::raw_250n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString("Pomy¶lnie utworzono kana³ %1").arg(strChannel));
    msgBox.exec();

    dlgchannel_homes->clear();
    pNetwork->send("CS HOMES");
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

// :ChanServ!service@service.onet NOTICE scc_test :251 #czesctoja :has been dropped
void IrcKernel::raw_251n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDisplay = QString("* Usuniêto kana³ %1").arg(strChannel);
    tabc->show_msg_active(strDisplay, 5);
}

// :ChanServ!service@service.onet NOTICE #testabc :252 scc_test :has dropped this channel
void IrcKernel::raw_252n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[2];

    QString strDisplay = QString("* Potwierdzam usuniêcie kana³u %1").arg(strChannel);
    tabc->show_msg(strChannel, strDisplay, 5);
}

// :ChanServ!service@service.onet NOTICE scc_test :253 #test_scc_moj Merovingian :channel owner changed
void IrcKernel::raw_253n()
{
    // kopia raw 254
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[2];
    QString strNick = strDataList[5];

    QString strDisplay = QString("* %1 jest teraz w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    tabc->show_msg(strChannel, strDisplay, 5);
}

// :ChanServ!service@service.onet NOTICE #test_scc_moj :254 scc_test Merovingian :changed channel owner
void IrcKernel::raw_254n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[2];
    QString strWho = strDataList[4];
    QString strNick = strDataList[5];

    QString strDisplay = QString("* %1 jest teraz w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    tabc->show_msg(strChannel, strDisplay, 5);
}

// :ChanServ!service@service.onet NOTICE scc_test :255 #scc +b cos :channel privilege changed
void IrcKernel::raw_255n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strNick = strDataList[6];

    QString strDisplay = QString("* Potwierdzam zmianê uprawnieñ dla %1 w %2").arg(strNick).arg(strChannel);
    tabc->show_msg_active(strDisplay, 5);
}

// :ChanServ!service@service.onet NOTICE #scc :256 Merovingian +o scc_test :channel privilege changed
void IrcKernel::raw_256n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[2];
    QString strWho = strDataList[4];
    QString strFlag = strDataList[5];
    QString strNick = strDataList[6];
    QString strDisplay;

    if (strFlag == "+q") strDisplay = QString("* %1 jest teraz w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-q") strDisplay = QString("* %1 nie jest ju¿ w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+o") strDisplay = QString("* %1 jest teraz super operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-o") strDisplay = QString("* %1 nie jest ju¿ super operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+h") strDisplay = QString("* %1 jest teraz operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-h") strDisplay = QString("* %1 nie jest ju¿ operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+v") strDisplay = QString("* %1 jest teraz go¶ciem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-v") strDisplay = QString("* %1 nie jest ju¿ go¶ciem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+X") strDisplay = QString("* %1 jest teraz moderatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-X") strDisplay = QString("* %1 nie jest ju¿ moderatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+Y") strDisplay = QString("* %1 jest teraz screenerem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-Y") strDisplay = QString("* %1 nie jest ju¿ screenerem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+b") strDisplay = QString("* %1 jest teraz na li¶cie wyproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-b") strDisplay = QString("* %1 nie jest ju¿ na li¶cie wyproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+I") strDisplay = QString("* %1 jest teraz na li¶cie zaproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-I") strDisplay = QString("* %1 nie jest ju¿ na li¶cie zaproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else
        strDisplay = QString("* %1 ma teraz flage %2 (Ustawi³ %3)").arg(strNick).arg(strFlag).arg(strWho);

    tabc->show_msg(strChannel, strDisplay, 5);
}

// :ChanServ!service@service.onet NOTICE scc_test :257 #scc * :settings changed
void IrcKernel::raw_257n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
}

// :ChanServ!service@service.onet NOTICE #scc :258 Merovingian * :channel settings changed
void IrcKernel::raw_258n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[2];
    QString strNick = strDataList[4];

    QString strDisplay = QString("* %1 zmieni³/a ustawienia kana³u %2").arg(strNick).arg(strChannel);
    tabc->show_msg(strChannel, strDisplay, 7);

    pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
}

// :ChanServ!service@service.onet NOTICE scc_test :259 #scc :nothing changed
void IrcKernel::raw_259n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDisplay = QString("* Nic nie zmieniono w %1").arg(strChannel);
    tabc->show_msg_active(strDisplay, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :260 Merovingian #scc +o :channel privilege changed
void IrcKernel::raw_260n()
{
    // kopia raw 256
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[5];
    QString strWho = strDataList[4];
    QString strFlag = strDataList[6];
    QString strNick = strDataList[2];
    QString strDisplay;

    if (strFlag == "+q") strDisplay = QString("* %1 jest teraz w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-q") strDisplay = QString("* %1 nie jest ju¿ w³a¶cicielem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+o") strDisplay = QString("* %1 jest teraz super operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-o") strDisplay = QString("* %1 nie jest ju¿ super operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+h") strDisplay = QString("* %1 jest teraz operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-h") strDisplay = QString("* %1 nie jest ju¿ operatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+v") strDisplay = QString("* %1 jest teraz go¶ciem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-v") strDisplay = QString("* %1 nie jest ju¿ go¶ciem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+X") strDisplay = QString("* %1 jest teraz moderatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-X") strDisplay = QString("* %1 nie jest ju¿ moderatorem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+Y") strDisplay = QString("* %1 jest teraz screenerem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-Y") strDisplay = QString("* %1 nie jest ju¿ screenerem kana³u %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+b") strDisplay = QString("* %1 jest teraz na li¶cie wyproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-b") strDisplay = QString("* %1 nie jest ju¿ na li¶cie wyproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+I") strDisplay = QString("* %1 jest teraz na li¶cie zaproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-I") strDisplay = QString("* %1 nie jest ju¿ na li¶cie zaproszonych w %2 (Ustawi³ %3)").arg(strNick).arg(strChannel).arg(strWho);
    else
        strDisplay = QString("* %1 ma teraz flage %2 (Ustawi³ %3)").arg(strNick).arg(strFlag).arg(strWho);

    tabc->show_msg(strChannel, strDisplay, 5);
}

// :ChanServ!service@service.onet NOTICE scc_test :261 scc_test #czesctoja :has dropped this channel
void IrcKernel::raw_261n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[5];

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString("Pomy¶lnie usuniêto kana³ %1").arg(strChannel));
    msgBox.exec();

    dlgchannel_homes->clear();
    pNetwork->send("CS HOMES");
    pNetwork->send(QString("PART %1").arg(strChannel));
}

// :cf1f2.onet 301 scc_test Merovingian :nie ma
void IrcKernel::raw_301()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = QString("* %1 jest nieobecny z powodu: %2").arg(strNick).arg(strMessage);
    tabc->show_msg_active(strDisplay, 7);
}

// :cf1f3.onet 305 scc_test :You are no longer marked as being away
void IrcKernel::raw_305()
{
    QString strDisplay = "* Nie jeste¶ ju¿ oznaczony \"zaraz wracam\".";
    tabc->show_msg_all(strDisplay, 7);
}

// :cf1f3.onet 306 scc_test :You have been marked as being away
void IrcKernel::raw_306()
{
    QString strDisplay = "* Jeste¶ teraz oznaczony \"zaraz wracam\".";
    tabc->show_msg_all(strDisplay, 7);
}

// :cf1f3.onet 307 scc_test Merovingian :is a registered nick
void IrcKernel::raw_307()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    if (strMessage == "is a registered nick")
        strMessage = "jest zarejestrowanym u¿ytkownikiem";

    QString strDisplay = QString("* %1 %2").arg(strNick).arg(strMessage);
    tabc->show_msg_active(strDisplay, 7);
}

// :cf1f1.onet 311 scc_test Merovingian 26269559 2294E8.94913F.196694.9BAE58 * :Merovingian
void IrcKernel::raw_311()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(7).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strZUO = strDataList[4];
    QString strIP = strDataList[5];
    QString strIrcname;

    for (int i = 7; i < strDataList.size(); i++) { if (i != 7) strIrcname += " "; strIrcname += strDataList[i]; }
    if (strIrcname[0] == ':')
        strIrcname = strIrcname.right(strIrcname.length()-1);

    QString strDisplayNick = QString("* %1 to %2@%3").arg(strNick).arg(strZUO).arg(strIP);
    tabc->show_msg_active(strDisplayNick, 7);

    QString strDisplayIrcname = QString("* %1 ircname: %2").arg(strNick).arg(strIrcname);
    tabc->show_msg_active(strDisplayIrcname, 7);
}

// :cf1f2.onet 312 scc_test Merovingian *.onet :OnetCzat
void IrcKernel::raw_312()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strServer;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strServer += " "; strServer += strDataList[i]; }

    QString strDisplay = QString("* %1 u¿ywa serwera: %2").arg(strNick).arg(strServer);
    tabc->show_msg_active(strDisplay, 7);
}

// :cf1f1.onet 313 scc_test Llanero :is a GlobalOp on OnetCzat
void IrcKernel::raw_313()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strDisplay = QString("* %1 jest administratorem czata").arg(strNick);
    tabc->show_msg_active(strDisplay, 7);
}

// :cf1f3.onet 317 scc_test Merovingian 7 1263650617 :seconds idle, signon time
void IrcKernel::raw_317()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strIdle = strDataList[4];
    QString strTime = strDataList[5];

    QDateTime dt_idle = QDateTime::fromTime_t(strIdle.toInt());
    int iDT_d = dt_idle.toString("dd").toInt();;
    int iDT_h = dt_idle.toString("hh").toInt();
    int iDT_m = dt_idle.toString("mm").toInt();
    int iDT_s = dt_idle.toString("ss").toInt();

    if (iDT_d > 1) iDT_d--;
    else iDT_d = 0;
    if (iDT_h > 1) iDT_h--;
    else iDT_h = 0;

    QString strDT_idle;
    if (iDT_d > 0)
        strDT_idle = QString("%1d %2h %3m %4s").arg(iDT_d).arg(iDT_h).arg(iDT_m).arg(iDT_s);
    else if ((iDT_d == 0) && (iDT_h > 0))
        strDT_idle = QString("%1h %2m %3s").arg(iDT_h).arg(iDT_m).arg(iDT_s);
    else if ((iDT_d == 0) && (iDT_h == 0) && (iDT_m > 0))
        strDT_idle = QString("%1m %2s").arg(iDT_m).arg(iDT_s);
    else if ((iDT_d == 0) && (iDT_h == 0) && (iDT_m == 0) && (iDT_s > 0))
        strDT_idle = QString("%1s").arg(iDT_s);

    QDateTime dt_time = QDateTime::fromTime_t(strTime.toInt());
    QString strDT_time = dt_time.toString("dd/MM/yyyy hh:mm:ss");

    QString strDisplayIdle = QString("* %1 jest nieobecny %2").arg(strNick).arg(strDT_idle);
    tabc->show_msg_active(strDisplayIdle, 7);

    QString strDisplaySignon = QString("* %1 jest zalogowany od %2").arg(strNick).arg(strDT_time);
    tabc->show_msg_active(strDisplaySignon, 7);
}

// :cf1f4.onet 318 scc_test Merovingian :End of /WHOIS list.
void IrcKernel::raw_318()
{
// ignore
}

// :cf1f4.onet 319 scc_test Merovingian :#testy %#Komputery `#scc `#Quiz `#Scrabble `#hack
void IrcKernel::raw_319()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strChannels;
    for (int i = 4; i < strDataList.size(); i++)
    {
        if (i != 4) strChannels += " ";
        QString strChannel = strDataList[i];
        if (strChannel[0] == ':')
            strChannel = strChannel.right(strChannel.length()-1);
        if ((strChannel[0] != '#') && (strChannel[0] != '^'))
            strChannel = strChannel.right(strChannel.length()-1);
        strChannels += strChannel;
    }

    QString strDisplay = QString("* %1 jest na kana³ach: %2").arg(strNick).arg(strChannels);
    tabc->show_msg_active(strDisplay, 7);
}

// :cf1f4.onet 332 scc_test #scc :Simple Chat Client; current version: beta;
void IrcKernel::raw_332()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[3];
    if (strChannel[0] == ':')
        strChannel = strChannel.right(strChannel.length()-1);

    QString strTopic;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strTopic += " "; strTopic += strDataList[i]; }
    if (strTopic[0] == ':')
        strTopic = strTopic.right(strTopic.length()-1);

    tabc->set_topic(strChannel, strTopic);
}

// :cf1f1.onet 333 scc_test #scc Merovingian!26269559 1253193639
void IrcKernel::raw_333()
{
// supported by raw 161
}

// :cf1f1.onet 341 scc_test Merovingian ^cf1f1162848
void IrcKernel::raw_341()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strChannel = strDataList[4];

    if (strChannel[0] == '^')
        tabc->rename_tab(strChannel, strNick);
}

// :cf1f1.onet 353 scc_test = #scc :scc_test|rx,0 `@Merovingian|brx,1 @chanky|rx,1
void IrcKernel::raw_353()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    for (int i = 5; i < strDataList.size(); i++)
    {
        if (strDataList[i].isEmpty() == false)
        {
            QString strNick = strDataList[i];
            if (i == 5) strNick = strNick.right(strNick.length()-1); // remove :
            strNick = strNick.left(strNick.indexOf("|"));

            QString strSuffix = strDataList[i];
            strSuffix = strSuffix.right(strSuffix.length() - strSuffix.indexOf("|") -1);

            tabc->add_user(strChannel, strNick, strSuffix, 0);
        }
    }
}

// :cf1f2.onet 366 scc_test #scc :End of /NAMES list.
void IrcKernel::raw_366()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    tabc->refresh_nicklist(strChannel);
}

// :cf1f4.onet 372 scc_test :- Onet Czat. Inny Wymiar Czatowania. Witamy
void IrcKernel::raw_372()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    tabc->show_msg("Status", strMessage, 0);
}

// :cf1f4.onet 375 scc_test :cf1f4.onet message of the day
void IrcKernel::raw_375()
{
    QString strDisplay = "Wiadomo¶æ dla u¿ytkowników:";
    tabc->show_msg("Status", strDisplay, 0);
}

// :cf1f1.onet 376 scc_test :End of message of the day.
void IrcKernel::raw_376()
{
// ignore
}

// :cf1f2.onet 396 ~scc_test 3DE379.B7103A.6CF799.6902F4 :is now your displayed host
void IrcKernel::raw_396()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strHost = strDataList[3];

    QString strMessage = QString("* %1 jest teraz Twoim wy¶wietlanym hostem").arg(strHost);

    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE ~scc_test :400 :you are not registred
void IrcKernel::raw_400n()
{
    QString strNick = strDataList[2];

    QString strMessage = QString("* %1 :Nick nie jest zarejestrowany").arg(strNick);

    tabc->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 401 scc_test scc :No such nick/channel
// :cf1f4.onet 401 Merovingian ChanServ :is currently unavailable. Please try again later.
void IrcKernel::raw_401()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    if (strMessage == "No such nick/channel")
        strMessage = QString("* %1 :Nick lub kana³ nie istnieje").arg(strNick);
    else if (strMessage == "is currently unavailable. Please try again later.")
        strMessage = QString("* %1 jest aktualnie niedostêpny. Spróbuj ponownie pó¼niej.").arg(strNick);

    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :401 aa :no such nick
void IrcKernel::raw_401n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString("* %1 :Nick nie istnieje").arg(strNick);

    tabc->show_msg_active(strMessage, 7);
}

// :cf1f1.onet 403 ~testa #^cf1f41568 :Invalid channel name
void IrcKernel::raw_403()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString("* %1 :Nieprawid³owa nazwa kana³u").arg(strChannel);

    tabc->show_msg_active(strMessage, 7);
}

// :cf1f1.onet 404 scc_test #Quiz :Cannot send to channel (+m)
// :cf1f4.onet 404 ~scc_test #lunar :Cannot send to channel (no external messages)
void IrcKernel::raw_404()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strReason;
    for (int i = 8; i < strDataList.size(); i++) { if (i != 8) strReason += " "; strReason += strDataList[i]; }
    if (strReason[0] == ':')
        strReason = strReason.right(strReason.length()-1);
    if (strReason[0] == '(')
        strReason = strReason.right(strReason.length()-1);
    if (strReason[strReason.length()-1] == ')')
        strReason = strReason.left(strReason.length()-1);

    if (strReason == "+m")
        strReason = "Brak moderatora na kanale!";

    QString strMessage = QString("* Nie mozna wys³aæ wiadomo¶ci do %1: %2").arg(strChannel).arg(strReason);

    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :404 ~zwariowany_zdzich0 :user is not registred
void IrcKernel::raw_404n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString("* %1 :Nick nie jest zarejestrowany").arg(strNick);

    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE ~test :406 VHOST :unknown command
void IrcKernel::raw_406n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strCmd = strDataList[4];

    QString strMessage = QString("* %1 :Nieznane polecenie").arg(strCmd);

    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :407 VOICE :not enough parameters
void IrcKernel::raw_407n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strCmd = strDataList[4];

    QString strMessage = QString("* %1 :Nie wystarczaj±ca ilo¶æ parametrów").arg(strCmd);

    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :408 dsfdsf :no such channel
void IrcKernel::raw_408n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString("* %1 :Nie ma takiego kana³u").arg(strChannel);

    tabc->show_msg_active(strMessage, 7);

    dlgchannel_homes->clear();
    pNetwork->send("CS HOMES");
}

// :NickServ!service@service.onet NOTICE ~Merovingian :412 admi :user's data is not ready
void IrcKernel::raw_412n()
{
    // ignore
}

// :cf1f4.onet 421 scc_test MOD :Unknown command
void IrcKernel::raw_421()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strCmd = strDataList[3];

    QString strMessage = QString("* %1 :Nieznane polecenie").arg(strCmd);

    tabc->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :420 aleksa7 :is already on your friend list
void IrcKernel::raw_420n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString("* Nick %1 jest ju¿ na li¶cie przyjació³").arg(strNick);

    tabc->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :421 aaa :is not on your friend list
void IrcKernel::raw_421n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString("* Nick %1 nie jest na li¶cie przyjació³").arg(strNick);

    tabc->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :430 wilk :is already on your ignore list
void IrcKernel::raw_430n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString("* %1 jest ju¿ na li¶cie ignorowanych").arg(strNick);

    tabc->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :431 a :is not on your ignore list
void IrcKernel::raw_431n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString("* %1 nie jest na li¶cie ignorowanych").arg(strNick);

    tabc->show_msg_active(strMessage, 7);

    dlgignore->clear();
    pNetwork->send("NS IGNORE");
}

// :cf1f4.onet 432 1501-unknown ~?o? :Erroneous Nickname
void IrcKernel::raw_432()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Nick %1 jest nieprawid³owy").arg(strNick);

    tabc->show_msg_all(strMessage, 9);
}

// :cf1f1.onet 433 * scc_test :Nickname is already in use.
void IrcKernel::raw_433()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Nick %1 jest ju¿ w u¿yciu").arg(strNick);

    tabc->show_msg_all(strMessage, 9);

    settings->setValue("override", "on");
}

// :NickServ!service@service.onet NOTICE scc_test :440 #scc :is already on your favourite list
void IrcKernel::raw_440n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString("* Kana³ %1 jest ju¿ na li¶cie ulubionych").arg(strChannel);

    tabc->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :441 #scc :is not on your favourite list
void IrcKernel::raw_441n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString("* Kana³ %1 nie jest na li¶cie ulubionych").arg(strChannel);

    tabc->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 443 Merovingian scc #Scrabble :is already on channel
void IrcKernel::raw_443()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strChannel = strDataList[4];

    QString strMessage = QString("* %1 jest ju¿ na kanale %2").arg(strNick).arg(strChannel);

    tabc->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 451 SLIST :You have not registered
void IrcKernel::raw_451()
{
    QString strCommand = strDataList[2];

    QString strMessage = QString("* Nie jeste¶ zarejestrowany by wywo³aæ %1").arg(strCommand);
    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :452 #aaa :channel name already in use
void IrcKernel::raw_452n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString("* %1 :Nazwa kana³u ju¿ istnieje").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);

    dlgchannel_homes->clear();
    pNetwork->send("CS HOMES");
}

// :ChanServ!service@service.onet NOTICE scc_test :454 #aaaaaaaaaaaaaaaaaaaaaa :not enough unique channel name
void IrcKernel::raw_454n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString("* %1 :Nazwa kana³u nie jest unikalna").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);

    dlgchannel_homes->clear();
    pNetwork->send("CS HOMES");
}

// :ChanServ!service@service.onet NOTICE scc_test :458 #scc v scc :unable to remove non-existent privilege
void IrcKernel::raw_458n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[6];

    QString strMessage = QString("* %1 :Nie mo¿na zdj±æ nie nadanych uprawnieñ").arg(strWho);
    tabc->show_msg(strChannel, strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :459 #scc b test :channel privilege already given
void IrcKernel::raw_459n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[6];

    QString strMessage = QString("* %1 :Uprawnienia zosta³y ju¿ nadane").arg(strWho);
    tabc->show_msg(strChannel, strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :461 #scc scc :channel operators cannot be banned
void IrcKernel::raw_461n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[5];

    QString strMessage = QString("* %1 :Nie mo¿na zbanowaæ super operatora/operatora").arg(strWho);
    tabc->show_msg(strChannel, strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :463 #lunar AUDITORIUM :permission denied, insufficient privileges
void IrcKernel::raw_463n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWhat = strDataList[5];

    QString strMessage = QString("* %1 :Dostêp zabroniony, nie posiadasz odpowiednich uprawnieñ w %2").arg(strWhat).arg(strChannel);
    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE Merovingian :464 TOPIC :invalid argument
void IrcKernel::raw_464n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strWhat = strDataList[4];

    QString strMessage = QString("* %1 :Nieprawid³owy argument").arg(strWhat);
    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :467 #scc :permission denied, you are not a channel owner
void IrcKernel::raw_467n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString("* %1 :Dostêp zabroniony, nie jeste¶ w³a¶cicielem kana³u").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);

    dlgchannel_homes->clear();
    pNetwork->send("CS HOMES");
}

// :ChanServ!service@service.onet NOTICE scc_test :468 #scc :permission denied, insufficient privileges
void IrcKernel::raw_468n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString("* Dostêp zabroniony, nie posiadasz odpowiednich uprawnieñ w %1").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 470 ~Merovingian :#testy has become full, so you are automatically being transferred to the linked channel #Awaria
void IrcKernel::raw_470()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];
    if (strChannel[0] == ':')
        strChannel = strChannel.right(strChannel.length()-1);
    QString strLinked = strDataList[strDataList.size()-1];

    QString strMessage = QString("* Kana³ %1 jest pe³ny, zostajesz automatycznie przekieroway do kana³u %2").arg(strChannel).arg(strLinked);
    tabc->show_msg("Status", strMessage, 7);
}

// :cf1f2.onet 471 ~Merovingian #testy :Cannot join channel (Channel is full)
void IrcKernel::raw_471()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString("* Nie mo¿esz wej¶æ do %1: Kana³ jest pe³ny").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :472 #aaaaaaaaaaaaaaaaaaaaaaaaaaaaa :wait 60 seconds before next REGISTER
void IrcKernel::raw_472n()
{
    QString strMessage = QString("* Utworzenie kolejnego kana³u bêdzie mo¿liwe po up³yniêciu 60 sekund");
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 473 ~scc_test #lunar :Cannot join channel (Invite only)
void IrcKernel::raw_473()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString("* Nie mo¿esz wej¶æ do %1: Nie jeste¶ na li¶cie zaproszonych").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 474 ~scc_test #Quiz :Cannot join channel (You're banned)
void IrcKernel::raw_474()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString("* Nie mo¿esz wej¶æ do %1: Jeste¶ zbanowany").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f1.onet 482 Merovingian #Scrabble :Only a u-line may kick a u-line from a channel.
void IrcKernel::raw_482()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString("* Tylko u-line mo¿e wyrzuciæ u-line z kana³u.");
    tabc->show_msg(strChannel, strMessage, 7);
}

// :cf1f4.onet 484 scc_test #scc :Can't kick scc as your spells are not good enough
void IrcKernel::raw_484()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    tabc->show_msg(strChannel, strMessage, 7);
}

//:cf1f2.onet 530 Merovingian #f :Only IRC operators may create new channels
void IrcKernel::raw_530()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString("* %1 :Tylko Administratorzy mog± tworzyæ nowe kana³y").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 600 scc_test Radowsky 16172032 690A6F.A8219B.7F5EC1.35E57C 1267055769 :arrived online
void IrcKernel::raw_600()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Twój przyjaciel %1 jest teraz on-line").arg(strNick);
    tabc->show_msg_active(strMessage, 7);

    dlgfriends->set_friend(strNick, true);
}

// :cf1f4.onet 601 scc_test Radowsky 16172032 690A6F.A8219B.7F5EC1.35E57C 1267055692 :went offline
void IrcKernel::raw_601()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Twój przyjaciel %1 jest teraz off-line").arg(strNick);
    tabc->show_msg_active(strMessage, 7);

    dlgfriends->set_friend(strNick, false);
}

// :cf1f3.onet 602 scc_test aaa * * 0 :stopped watching
void IrcKernel::raw_602()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    dlgfriends->remove_friend(strNick);
}

//:cf1f1.onet 604 scc_test scc_test 51976824 3DE379.B7103A.6CF799.6902F4 1267054441 :is online
void IrcKernel::raw_604()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Twój przyjaciel %1 jest teraz on-line").arg(strNick);
    tabc->show_msg_active(strMessage, 7);

    dlgfriends->set_friend(strNick, true);
}

// :cf1f1.onet 605 scc_test Radowsky * * 0 :is offline
void IrcKernel::raw_605()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Twój przyjaciel %1 jest teraz off-line").arg(strNick);
    tabc->show_msg_active(strMessage, 7);

    dlgfriends->set_friend(strNick, false);
}

//:cf1f3.onet 801 scc_test :q5VMy1wl6hKL5ZUt
void IrcKernel::raw_801()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strKey = strDataList[3];
    if (strKey[0] == ':')
        strKey = strKey.right(strKey.length()-1);

    IrcAuth *pIrc_auth = new IrcAuth(pNetwork, settings, tabc);
    QString strAuth = pIrc_auth->transform_key(strKey);
    delete pIrc_auth;

    if (strAuth.length() == 16)
        pNetwork->send(QString("AUTHKEY %1").arg(strAuth));
    else
        tabc->show_msg("Status", "Error: B³êdny klucz auth", 9);
}

// :cf1f1.onet 802 * :Corrupted Nickname
void IrcKernel::raw_802()
{
    QString strMessage = QString("* Nieprawid³owy nick");
    tabc->show_msg_active(strMessage, 9);
}

// :cf1f2.onet 807 scc_test :You are marked as busy
void IrcKernel::raw_807()
{
    settings->setValue("busy", "on");

    QString strDisplay = "* Jeste¶ teraz w stanie \"zajêty\", nie bêdziesz otrzymywaæ ¿adnych prywatnych komunikatów.";
    tabc->show_msg_all(strDisplay, 7);
}

// :cf1f2.onet 808 scc_test :You are no longer marked busy
void IrcKernel::raw_808()
{
    settings->setValue("busy", "off");

    QString strDisplay = "* Nie jeste¶ ju¿ \"zajêty\" - mo¿esz otrzymywaæ prywatne komunikaty.";
    tabc->show_msg_all(strDisplay, 7);
}

// :cf1f2.onet 809 scc_test Succubi :is busy
void IrcKernel::raw_809()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* %1 jest oznaczony jako zajêty").arg(strNick);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f2.onet 811 scc_test Merovingian :Ignore invites
void IrcKernel::raw_811()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Zignorowano rozmowê prywatn± od %1").arg(strNick);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f2.onet 812 scc_test Merovingian ^cf1f41284615 :Invite rejected
void IrcKernel::raw_812()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString("* Odrzucono rozmowê prywatn± od %1").arg(strNick);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 815 ~test testnick :Public webcam
void IrcKernel::raw_815()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strDisplay = QString("* %1 ma w³±czon± kamerê").arg(strNick);
    tabc->show_msg_active(strDisplay, 7);
}

// :cf1f2.onet 817 scc_test #scc 1253216797 mikefear - :%Fb:arial%%Ce40f0f%re
void IrcKernel::raw_817()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[3];
    QString strTime = strDataList[4];
    QString strNick = strDataList[5];

    QString strMessage;
    for (int i = 7; i < strDataList.size(); i++) { if (i != 7) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    if (strMessage.isEmpty() == false)
        tabc->show_msg(strTime, strChannel, QString("<%1> %2").arg(strNick).arg(strMessage), 0);
}

// :cf1f3.onet 818 scc_test :Start of simple channels list.
void IrcKernel::raw_818()
{
    dlgchannel_list->clear();
}

// :cf1f3.onet 819 scc_test :#tarnów_dziki:g:1,#¯ory:g:0,#M³awa:O:0,#Lineage_II:_:1,#kakakak:O:0,#apostolat_yfl:_:0,#ITALIA_CLUB:i:23,#Fina³_WO¦P:P:0,#sama_s³odycz:O:0,#Suwa³ki:i:14,#Mamu¶ki:O:0,#Pokój_Rado¶ci:O:0,#Antwerpia:g:0,#Kolo_Gospodyn_Wiejskich:O:0,#Samotnia_Kurka:G:0,#Wszystko_o_grach:O:0,#VIPy_NowyS±cz:h:0,#tymczasowy:G:0,#Zielona_Góra:h:2,#45slonko:P:0,#kawalek_nieba:O:0,#Wirtualna_Przyjazn:a:11,#Magiczny_swiat:O:1,#herbatka_u_cynamonki:P:0,#DEUTSCHLAND:i:111,#informatyka:`:1
void IrcKernel::raw_819()
{
    QString strChannelsString;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strChannelsString += " "; strChannelsString += strDataList[i]; }
    if (strChannelsString[0] == ':')
        strChannelsString = strChannelsString.right(strChannelsString.length()-1);

    QStringList strChannelsList = strChannelsString.split(",");
    for (int i = 0; i < strChannelsList.count(); i++)
    {
        QStringList strChannelParameters = strChannelsList[i].split(":");
        QString strChannelName = strChannelParameters[0];
        QString strChannelPeople = strChannelParameters[2];
        QString strChannelCat = QString::null;
        QString strChannelType = QString::null;

        bool flag = false;
        bool flag1 = false;
        //bool flag2 = false;
        //bool flag3 = false;
        bool flag4 = false;

        QString s1 = strChannelParameters[1];
        int c;
        if (s1.length() > 1)
        {
            c = s1[0].toAscii();
            flag = (c & 0x6d) == 109;
            flag1 = (c & 0x70) == 112;
            c = s1[1].toAscii();
        }
        else
            c = s1[0].toAscii();
        c++;
        int k = c & 7;
        int l = (c & 0x38) >> 3;
        flag4 = l == 3;

        switch(l)
        {
            case 1:
                strChannelType = "Teen";
                break;
            case 2:
                strChannelType = "Towarzyskie";
                break;
            case 3:
                strChannelType = "Erotyczne";
                break;
            case 4:
                strChannelType = "Tematyczne";
                break;
            case 5:
                strChannelType = "Regionalne";
                break;
        }

        if (flag == true)
            strChannelCat += "Moderowany ";
        if (flag1 == true)
            strChannelCat += "Polecany ";

        switch(k)
        {
            case 0:
                strChannelCat = "Dziki " + strChannelCat;
                break;
            case 1:
                strChannelCat = "Oswojony " + strChannelCat;
                break;
            case 2:
                strChannelCat = "Z klas± " + strChannelCat;
                break;
            case 3:
                strChannelCat = "Kultowy " + strChannelCat;
                break;
        }

        dlgchannel_list->add_channel(strChannelName, strChannelPeople, strChannelCat, strChannelType);
    }
}

// :cf1f3.onet 820 scc_test :End of simple channel list.
void IrcKernel::raw_820()
{
    dlgchannel_list->sort();
}

// :cf1f3.onet 821 scc_test #scc :Channel is not moderated
void IrcKernel::raw_821()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString("* Kana³ %1 nie jest moderowany").arg(strChannel);
    tabc->show_msg_active(strMessage, 7);
}

// :cf1f1.onet 951 scc_test scc_test :Added test!*@* <privatemessages,channelmessages,invites> to silence list
void IrcKernel::raw_951()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strNick = strDataList[5];

    QString strMessage = QString("* Dodano %1 do listy ignorowanych").arg(strNick);
    tabc->show_msg_active(strMessage, 7);
}

QString IrcKernel::get_settings_key(QString strFind)
{
    QStringList keys = settings->allKeys();
    for (int i = 0; i < keys.count(); i++)
    {
        if ((keys.at(i).length() > 4) && (settings->value(keys.at(i)).toString() == strFind))
            return keys.at(i).right(keys.at(i).length()-4);
    }
    return QString::null;
}
