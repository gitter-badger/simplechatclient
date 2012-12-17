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

#include <QDateTime>
#include <QMessageBox> // raw 433
#include "autoaway.h"
#include "avatar.h"
#include "away.h"
#include "busy.h"
#include "channel.h"
#include "channel_favourites_model.h"
#include "channel_homes_model.h"
#include "channel_key.h"
#include "channel_list_model.h"
#include "channel_settings_model.h"
#include "convert.h"
#include "core.h"
#include "find_nick_model.h"
#include "friends_model.h"
#include "ignore_model.h"
#include "invite.h"
#include "invite_model.h"
#include "lag.h"
#include "log.h"
#include "mainwindow.h"
#include "message.h"
#include "my_profile_model.h"
#include "my_stats_model.h"
#include "nick.h"
#include "offline.h"
#include "onet_utils.h"
#include "profile_manager_model.h"
#include "replace.h"
#include "settings.h"
#include "sound_notify.h"
#include "tab_container.h"
#include "themes_model.h"
#include "tray.h"
#include "user_profile_model.h"
#include "onet_kernel.h"

OnetKernel::OnetKernel(TabContainer *_pTabC) : pTabC(_pTabC)
{
}

OnetKernel::~OnetKernel()
{
}

void OnetKernel::kernel(const QString &_strData)
{
    strData = _strData;
    strDataList = strData.split(" ");

    if (Settings::instance()->get("debug") == "true")
        Message::instance()->showMessage(DEBUG_WINDOW, "<- "+strData, MessageDefault);

    bool bUnknownRaw1 = false;
    bool bUnknownRaw2 = false;
    bool bUnknownRaw3 = false;

    if (!strDataList.value(1).isEmpty())
    {
        QString strCmd0 = strDataList.at(0);
        QString strCmd1 = strDataList.at(1);

        if (strCmd0 == "PING") raw_ping();
        else if (strCmd0 == "ERROR") raw_error();
        else if (strCmd1 == "PONG") raw_pong();
        else if (strCmd1 == "JOIN") raw_join();
        else if (strCmd1 == "PART") raw_part();
        else if (strCmd1 == "QUIT") raw_quit();
        else if (strCmd1 == "KICK") raw_kick();
        else if (strCmd1 == "MODE") raw_mode();
        else if (strCmd1 == "PRIVMSG") raw_privmsg();
        else if (strCmd1 == "INVITE") raw_invite();
        else if (strCmd1 == "TOPIC") raw_topic();
        else if (strCmd1 == "INVREJECT") raw_invreject();
        else if (strCmd1 == "INVIGNORE") raw_invignore();
        else if (strCmd1 == "MODERMSG") raw_modermsg();
        else if (strCmd1 == "MODERNOTICE") raw_modernotice();
        else if (strCmd1 == "MODERATE") raw_moderate();
        else if (strCmd1 == "KILL") raw_kill();
        else if (strCmd1 == "NICK") raw_nick();
        else
            bUnknownRaw1 = true;

        int iCmd1 = strDataList.at(1).toInt();
        switch (iCmd1)
        {
            case 001: raw_001(); break;
            case 002: raw_002(); break;
            case 003: raw_003(); break;
            case 004: raw_004(); break;
            case 005: raw_005(); break;
            case 251: raw_251(); break;
            case 252: raw_252(); break;
            case 253: raw_253(); break;
            case 254: raw_254(); break;
            case 255: raw_255(); break;
            case 256: raw_256(); break;
            case 257: raw_257(); break;
            case 258: raw_258(); break;
            case 259: raw_259(); break;
            case 265: raw_265(); break;
            case 266: raw_266(); break;
            case 271: raw_271(); break;
            case 272: raw_272(); break;
            case 301: raw_301(); break;
            case 302: raw_302(); break;
            case 303: raw_303(); break;
            case 304: raw_304(); break;
            case 305: raw_305(); break;
            case 306: raw_306(); break;
            case 307: raw_307(); break;
            case 311: raw_311(); break;
            case 312: raw_312(); break;
            case 313: raw_313(); break;
            case 314: raw_314(); break;
            case 315: raw_315(); break;
            case 316: raw_316(); break;
            case 317: raw_317(); break;
            case 318: raw_318(); break;
            case 319: raw_319(); break;
            case 332: raw_332(); break;
            case 333: raw_333(); break;
            case 335: raw_335(); break;
            case 340: raw_340(); break;
            case 341: raw_341(); break;
            case 352: raw_352(); break;
            case 353: raw_353(); break;
            case 355: raw_355(); break;
            case 366: raw_366(); break;
            case 369: raw_369(); break;
            case 371: raw_371(); break;
            case 372: raw_372(); break;
            case 374: raw_374(); break;
            case 375: raw_375(); break;
            case 376: raw_376(); break;
            case 378: raw_378(); break;
            case 381: raw_381(); break;
            case 391: raw_391(); break;
            case 396: raw_396(); break;
            case 401: raw_401(); break;
            case 402: raw_402(); break;
            case 403: raw_403(); break;
            case 404: raw_404(); break;
            case 405: raw_405(); break;
            case 406: raw_406(); break;
            case 421: raw_421(); break;
            case 432: raw_432(); break;
            case 433: raw_433(); break;
            case 441: raw_441(); break;
            case 442: raw_442(); break;
            case 443: raw_443(); break;
            case 445: raw_445(); break;
            case 446: raw_446(); break;
            case 451: raw_451(); break;
            case 461: raw_461(); break;
            case 462: raw_462(); break;
            case 470: raw_470(); break;
            case 471: raw_471(); break;
            case 473: raw_473(); break;
            case 474: raw_474(); break;
            case 475: raw_475(); break;
            case 481: raw_481(); break;
            case 482: raw_482(); break;
            case 484: raw_484(); break;
            case 491: raw_491(); break;
            case 492: raw_492(); break;
            case 530: raw_530(); break;
            case 531: raw_531(); break;
            case 600: raw_600(); break;
            case 601: raw_601(); break;
            case 602: raw_602(); break;
            case 604: raw_604(); break;
            case 605: raw_605(); break;
            case 607: raw_607(); break;
            case 666: raw_666(); break;
            case 702: raw_702(); break;
            case 703: raw_703(); break;
            case 704: raw_704(); break;
            case 801: raw_801(); break;
            case 802: raw_802(); break;
            case 807: raw_807(); break;
            case 808: raw_808(); break;
            case 809: raw_809(); break;
            case 811: raw_811(); break;
            case 812: raw_812(); break;
            case 815: raw_815(); break;
            case 816: raw_816(); break;
            case 817: raw_817(); break;
            case 818: raw_818(); break;
            case 819: raw_819(); break;
            case 820: raw_820(); break;
            case 821: raw_821(); break;
            case 942: raw_942(); break;
            case 950: raw_950(); break;
            case 951: raw_951(); break;
            case 952: raw_952(); break;
            default:
                bUnknownRaw2 = true;
                break;
        }

        if ((strDataList.at(1) == "NOTICE") && (!strDataList.value(3).isEmpty()))
        {
            int iCmd3 = strDataList.at(3).mid(1).toInt();
            switch (iCmd3)
            {
                case 100: raw_100n(); break;
                case 109: raw_109n(); break;
                case 111: raw_111n(); break;
                case 112: raw_112n(); break;
                case 121: raw_121n(); break;
                case 122: raw_122n(); break;
                case 123: raw_123n(); break;
                case 131: raw_131n(); break;
                case 132: raw_132n(); break;
                case 133: raw_133n(); break;
                case 141: raw_141n(); break;
                case 142: raw_142n(); break;
                case 151: raw_151n(); break;
                case 152: raw_152n(); break;
                case 160: raw_160n(); break;
                case 161: raw_161n(); break;
                case 162: raw_162n(); break;
                case 163: raw_163n(); break;
                case 164: raw_164n(); break;
                case 165: raw_165n(); break;
                case 170: raw_170n(); break;
                case 171: raw_171n(); break;
                case 175: raw_175n(); break;
                case 176: raw_176n(); break;
                case 210: raw_210n(); break;
                case 211: raw_211n(); break;
                case 220: raw_220n(); break;
                case 221: raw_221n(); break;
                case 230: raw_230n(); break;
                case 231: raw_231n(); break;
                case 240: raw_240n(); break;
                case 241: raw_241n(); break;
                case 250: raw_250n(); break;
                case 251: raw_251n(); break;
                case 252: raw_252n(); break;
                case 253: raw_253n(); break;
                case 254: raw_254n(); break;
                case 255: raw_255n(); break;
                case 256: raw_256n(); break;
                case 257: raw_257n(); break;
                case 258: raw_258n(); break;
                case 259: raw_259n(); break;
                case 260: raw_260n(); break;
                case 261: raw_261n(); break;
                case 262: raw_262n(); break;
                case 263: raw_263n(); break;
                case 400: raw_400n(); break;
                case 401: raw_401n(); break;
                case 402: raw_402n(); break;
                case 403: raw_403n(); break;
                case 404: raw_404n(); break;
                case 406: raw_406n(); break;
                case 407: raw_407n(); break;
                case 408: raw_408n(); break;
                case 409: raw_409n(); break;
                case 412: raw_412n(); break;
                case 413: raw_413n(); break;
                case 414: raw_414n(); break;
                case 415: raw_415n(); break;
                case 416: raw_416n(); break;
                case 420: raw_420n(); break;
                case 421: raw_421n(); break;
                case 430: raw_430n(); break;
                case 431: raw_431n(); break;
                case 440: raw_440n(); break;
                case 441: raw_441n(); break;
                case 452: raw_452n(); break;
                case 454: raw_454n(); break;
                case 455: raw_455n(); break;
                case 456: raw_456n(); break;
                case 458: raw_458n(); break;
                case 459: raw_459n(); break;
                case 460: raw_460n(); break;
                case 461: raw_461n(); break;
                case 463: raw_463n(); break;
                case 464: raw_464n(); break;
                case 466: raw_466n(); break;
                case 467: raw_467n(); break;
                case 468: raw_468n(); break;
                case 469: raw_469n(); break;
                case 472: raw_472n(); break;
                default:
                    bUnknownRaw3 = true;
            }

            if ((strDataList.at(3).length() != 4) || (strDataList.at(3).toLower() == ":***"))
            {
                raw_notice();
                bUnknownRaw3 = false;
            }
        }
        else
            bUnknownRaw3 = true;
    }
    else
    {
        bUnknownRaw1 = true;
        bUnknownRaw2 = true;
        bUnknownRaw3 = true;
    }

    // detect unknown raw
    if (bUnknownRaw1 && bUnknownRaw2 && bUnknownRaw3)
    {
        Log::save("unknown_raw", strData);
    }
}

// PING :cf1f1.onet
void OnetKernel::raw_ping()
{
    if (strDataList.size() < 2) return;

    QString strServer = strDataList.at(1);

    if (!strServer.isEmpty())
        Core::instance()->network->send(QString("PONG %1").arg(strServer));
}

// :cf1f4.onet PONG cf1f4.onet :1340185644095
void OnetKernel::raw_pong()
{
    if (strDataList.size() < 4) return;

    QString strServerMSecs = strDataList.at(3);
    if (strServerMSecs[0] == ':') strServerMSecs.remove(0,1);

    // check correct pong
    qint64 iServerMSecs = strServerMSecs.toLongLong();
    if (iServerMSecs != 0)
        Lag::instance()->calculate(iServerMSecs);
}

// ERROR :Closing link (unknown@95.48.183.154) [Registration timeout]
void OnetKernel::raw_error()
{
    QString strMessage;
    for (int i = 0; i < strDataList.size(); i++) { if (i != 0) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    Message::instance()->showMessageAll(strMessage, MessageInfo);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 JOIN #Quiz :rx,0
void OnetKernel::raw_join()
{
    if (strDataList.size() < 3) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strZUO = strDataList.at(0);
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList.at(0);
    if (strIP[0] == ':') strIP.remove(0,1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strChannel = strDataList.at(2);
    if (strChannel[0] == ':') strChannel.remove(0,1);

    QString strSuffix;
    if (!strDataList.value(3).isEmpty())
    {
        strSuffix = strDataList.at(3);
        if (strSuffix[0] == ':') strSuffix.remove(0,1);
        strSuffix = strSuffix.left(strSuffix.length()-2);
    }

    QString strDisplay;

    if (strChannel[0] != '^')
    {
        if (Settings::instance()->get("show_zuo_and_ip") == "true")
            strDisplay = QString(tr("* %1 [%2@%3] has joined %4")).arg(strNick, strZUO, strIP, strChannel);
        else
            strDisplay = QString(tr("* %1 has joined %2")).arg(strNick, strChannel);
    }
    else
    {
        if (Settings::instance()->get("show_zuo_and_ip") == "true")
            strDisplay = QString(tr("* %1 [%2@%3] has joined priv")).arg(strNick, strZUO, strIP);
        else
            strDisplay = QString(tr("* %1 has joined priv")).arg(strNick);
    }

    // add tab
    pTabC->addTab(strChannel);

    // show message
    if (strChannel[0] == '^')
    {
        if (Channel::instance()->containsAlternativeName(strChannel))
            Message::instance()->showMessage(strChannel, strDisplay, MessageJoin);
    }
    else
        Message::instance()->showMessage(strChannel, strDisplay, MessageJoin);

    // nick
    Nick::instance()->add(strNick, strChannel, strSuffix);

    // nick avatar
    if ((!strNick.startsWith('~')) && (ThemesModel::instance()->isCurrentWithAvatar()) && (Nick::instance()->getAvatar(strNick).isEmpty()))
    {
        Core::instance()->network->send(QString("NS INFO %1 s").arg(strNick));
    }

    QString strMe = Settings::instance()->get("nick");
    if ((strNick == strMe) && (strChannel[0] != '^'))
        Core::instance()->network->send(QString("CS INFO %1 i").arg(strChannel));
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 PART #scc
void OnetKernel::raw_part()
{
    if (strDataList.size() < 3) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strZUO = strDataList.at(0);
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList.at(0);
    if (strIP[0] == ':') strIP.remove(0,1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strChannel = strDataList.at(2);
    if (strChannel[0] == ':') strChannel.remove(0,1);

    QString strReason;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strReason += " "; strReason += strDataList.at(i); }
    if (strReason[0] == ':') strReason.remove(0,1);

    QString strDisplay;

    if (strChannel[0] != '^')
    {
        if (!strReason.isEmpty())
        {
            if (Settings::instance()->get("show_zuo_and_ip") == "true")
                strDisplay = QString(tr("* %1 [%2@%3] has left %4 [%5]")).arg(strNick, strZUO, strIP, strChannel, strReason);
            else
                strDisplay = QString(tr("* %1 has left %2 [%3]")).arg(strNick, strChannel, strReason);
        }
        else
        {
            if (Settings::instance()->get("show_zuo_and_ip") == "true")
                strDisplay = QString(tr("* %1 [%2@%3] has left %4")).arg(strNick, strZUO, strIP, strChannel);
            else
                strDisplay = QString(tr("* %1 has left %2")).arg(strNick, strChannel);
        }
    }
    else
    {
        if (Settings::instance()->get("show_zuo_and_ip") == "true")
            strDisplay = QString(tr("* %1 [%2@%3] has left priv")).arg(strNick, strZUO, strIP);
        else
            strDisplay = QString(tr("* %1 has left priv")).arg(strNick);
    }

    Message::instance()->showMessage(strChannel, strDisplay, MessagePart);
    Nick::instance()->remove(strNick, strChannel);

    // if self part
    QString strMe = Settings::instance()->get("nick");

    if (strNick == strMe)
    {
        // close channel
        if ((strChannel != DEBUG_WINDOW) && (strChannel != STATUS_WINDOW))
            pTabC->removeTab(strChannel);
    }
}

// :Stark!38566204@A5F2F1.68FE5E.DE32AF.62ECB9 QUIT :Client exited
void OnetKernel::raw_quit()
{
    if (strDataList.size() < 3) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strZUO = strDataList.at(0);
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList.at(0);
    if (strIP[0] == ':') strIP.remove(0,1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strReason;
    for (int i = 2; i < strDataList.size(); i++) { if (i != 2) strReason += " "; strReason += strDataList.at(i); }
    if (strReason[0] == ':') strReason.remove(0,1);

    QString strDisplay;
    if (Settings::instance()->get("show_zuo_and_ip") == "true")
        strDisplay = QString(tr("* %1 [%2@%3] has quit [%4]")).arg(strNick, strZUO, strIP, strReason);
    else
        strDisplay = QString(tr("* %1 has quit [%2]")).arg(strNick, strReason);

    Nick::instance()->quit(strNick, strDisplay);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 KICK #scc Moment_w_atmosferze :sio
void OnetKernel::raw_kick()
{
    if (strDataList.size() < 3) return;

    QString strWho = strDataList.at(0);
    if (strWho[0] == ':') strWho.remove(0,1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strZUO = strDataList.at(0);
    strZUO = strZUO.mid(strZUO.indexOf('!')+1, strZUO.indexOf('@')-strZUO.indexOf('!')-1);

    QString strIP = strDataList.at(0);
    if (strIP[0] == ':') strIP.remove(0,1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strChannel = strDataList.at(2);
    if (strChannel[0] == ':') strChannel.remove(0,1);

    QString strNick = strDataList.at(3);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strReason;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strReason += " "; strReason += strDataList.at(i); }
    if (strReason[0] == ':') strReason.remove(0,1);

    QString strMe = Settings::instance()->get("nick");

    Nick::instance()->remove(strNick, strChannel);

    if (strNick != strMe)
    {
        QString strDisplay = QString(tr("* %1 has been kicked from channel %2 by %3 Reason: %4")).arg(strNick, strChannel, strWho, strReason);
        Message::instance()->showMessage(strChannel, strDisplay, MessageKick);
    }
    else
    {
        QString strDisplay = QString(tr("* You have been kicked from channel %1 by %2 Reason: %3")).arg(strChannel, strWho, strReason);

        Message::instance()->showMessage(strChannel, strDisplay, MessageKick);
        Message::instance()->showMessage(STATUS_WINDOW, strDisplay, MessageKick);

        // remove tab
        pTabC->removeTab(strChannel);
    }
}

// :Merovingian!26269559@jest.piekny.i.uroczy.ma.przesliczne.oczy MODE Merovingian :+b
// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D MODE Merovingian :+b
// :ankaszo!51613093@F4C727.446F67.966AC9.BAAE26 MODE ankaszo -W
// :ChanServ!service@service.onet MODE #glupia_nazwa +k bum
// :ChanServ!service@service.onet MODE #bzzzz -l
// :NickServ!service@service.onet MODE scc_test +r
// :ChanServ!service@service.onet MODE #scc +ips
// :ChanServ!service@service.onet MODE #scc +o scc_test
// :ChanServ!service@service.onet MODE #scc +eo *!51976824@* scc_test
// :ChanServ!service@service.onet MODE #abc123 +il-e 1 *!51976824@*
void OnetKernel::raw_mode()
{
    if (strDataList.size() < 4) return;

    QString strWho = strDataList.at(0);
    if (strWho[0] == ':') strWho.remove(0,1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strNickOrChannel = strDataList.at(2);

    if ((strNickOrChannel[0] == '#') || (strNickOrChannel[0] == '^'))
    {
        QString strChannel = strNickOrChannel;
        QString strFlags = strDataList.at(3);
        QMultiHash<QString, QString> flag_value;
        QString strMode3 = "GQLDMRVimnprstu";
        QString plusminus;

        int index = 4;
        for (int i = 0; i < strFlags.length(); i++)
        {
            QString f = strFlags.at(i);
            if ((f == "+") || (f == "-"))
            {
                if (f == "+") plusminus = f;
                else if (f == "-") plusminus = f;
            }
            else
            {
                QString strFlag = plusminus+f;

                if (strMode3.contains(f))
                    flag_value.insert(strFlag, QString::null);
                else
                {
                    if (index < strDataList.size())
                    {
                        flag_value.insert(strFlag, strDataList.at(index));
                        index++;
                    }
                    else
                        flag_value.insert(strFlag, QString::null);
                }
            }
        }

        QHashIterator <QString, QString> i(flag_value);
        i.toBack();
        while (i.hasPrevious())
        {
            i.previous();
            QString strFlag = i.key();
            QString strValue = i.value();
            QString strDisplay;
            bool bNickFlag = false;

            if (strFlag == "+q") { strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-q") { strDisplay = QString(tr("* %1 is no longer the owner of the channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+o") { strDisplay = QString(tr("* %1 is now super-operator on the channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-o") { strDisplay = QString(tr("* %1 is no longer super-operator on the channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+h") { strDisplay = QString(tr("* %1 is now the operator of the channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-h") { strDisplay = QString(tr("* %1 is no longer the operator of the channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+v") { strDisplay = QString(tr("* %1 is now a guest of channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-v") { strDisplay = QString(tr("* %1 is no longer a guest of channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+X") { strDisplay = QString(tr("* %1 is now moderator of the channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-X") { strDisplay = QString(tr("* %1 is no longer moderating channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+Y") { strDisplay = QString(tr("* %1 is now screener channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-Y") { strDisplay = QString(tr("* %1 is no longer a screener channel %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+b") { strDisplay = QString(tr("* %1 is now on the banned list in %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-b") { strDisplay = QString(tr("* %1 is no longer on the banned list in %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+I") { strDisplay = QString(tr("* %1 is now on the list of invitees in %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-I") { strDisplay = QString(tr("* %1 is no longer on the list of invitees in %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "+e") { strDisplay = QString(tr("* %1 now has ban exception flag in %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }
            else if (strFlag == "-e") { strDisplay = QString(tr("* %1 no longer has a ban exception flag in %2 (set by %3)")).arg(strValue, strChannel, strWho); bNickFlag = true; }

            else if (strFlag == "+k") strDisplay = QString(tr("* Channel %1 now has key set to %2 (set by %3)")).arg(strChannel, strValue, strWho);
            else if (strFlag == "-k") strDisplay = QString(tr("* Channel %1 no longer has key set (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+n") strDisplay = QString(tr("* Channel %1 will now have no external messages sent to the channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-n") strDisplay = QString(tr("* Channel %1 will now allow external messages sent to the channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+t") strDisplay = QString(tr("* Only channel operators can now change the topic in %1 channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-t") strDisplay = QString(tr("* Anyone can now change the topic in %1 channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+i") strDisplay = QString(tr("* Channel %1 is now a hidden channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-i") strDisplay = QString(tr("* Channel %1 is no longer hidden channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+p") strDisplay = QString(tr("* Channel %1 is now a private channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-p") strDisplay = QString(tr("* Channel %1 is no longer a private channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+s") strDisplay = QString(tr("* Channel %1 is now a secret channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-s") strDisplay = QString(tr("* Channel %1 is no longer a secret channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+m") strDisplay = QString(tr("* Channel %1 is now moderated channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-m") strDisplay = QString(tr("* Channel %1 is no longer moderated channel (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+u") strDisplay = QString(tr("* Channel %1 now has enabled auditorium mode (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-u") strDisplay = QString(tr("* Channel %1 no longer has enabled auditorium mode (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+l") strDisplay = QString(tr("* Channel %1 now has max number of users set to %2 (set by %3)")).arg(strChannel, strValue, strWho);
            else if (strFlag == "-l") strDisplay = QString(tr("* Channel %1 no longer has max number of users set (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+G") strDisplay = QString(tr("* Channel %1 now has word censoring enabled (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-G") strDisplay = QString(tr("* Channel %1 no longer has word censoring enabled (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+Q") strDisplay = QString(tr("* Channel %1 now has blocked kick (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-Q") strDisplay = QString(tr("* Channel %1 no longer has blocked kick (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+V") strDisplay = QString(tr("* Channel %1 now has blocked invites for 1 hour (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "-V") strDisplay = QString(tr("* Channel %1 no longer has blocked invites (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+L") strDisplay = QString(tr("* Channel %1 now has limit redirection to %2 channel (set by %3)")).arg(strChannel, strValue, strWho);
            else if (strFlag == "-L") strDisplay = QString(tr("* Channel %1 no longer has limit redirection (set by %2)")).arg(strChannel, strWho);
            else if (strFlag == "+J") strDisplay = QString(tr("* Channel %1 now prevents users from automatically rejoining the channel when they are kicked. Limit is set to %2 sec. (set by %3)")).arg(strChannel, strValue, strWho);
            else if (strFlag == "-J") strDisplay = QString(tr("* Channel %1 no longer prevents users from automatically rejoining the channel when they are kicked (set by %2)")).arg(strChannel, strWho);

            else if (strFlag.at(1) == QChar('F'))
            {
                QString strStatus;
                if (strValue.toInt() == 0) strStatus = tr("Wild");
                else if (strValue.toInt() == 1) strStatus = tr("Tame");
                else if (strValue.toInt() == 2) strStatus = tr("With class");
                else if (strValue.toInt() == 3) strStatus = tr("Cult");
                else strStatus = tr("unknown");

                if (strFlag.at(0) == QChar('+'))
                    strDisplay = QString(tr("* Channel %1 now has status %2 (set by %3)")).arg(strChannel, strStatus, strWho);
                else if (strFlag.at(0) == QChar('-'))
                    strDisplay = QString(tr("* %1 changed channel %2 status")).arg(strWho, strChannel);
            }
            else if (strFlag.at(1) == QChar('c'))
            {
                QString strCategory;
                if (strValue.toInt() == 1) strCategory = tr("Teen");
                else if (strValue.toInt() == 2) strCategory = tr("Common");
                else if (strValue.toInt() == 3) strCategory = tr("Erotic");
                else if (strValue.toInt() == 4) strCategory = tr("Thematic");
                else if (strValue.toInt() == 5) strCategory = tr("Regional");
                else strCategory = tr("unknown");

                if (strFlag.at(0) == QChar('+'))
                    strDisplay = QString(tr("* Channel %1 now belongs to a category %2 (set by %3)")).arg(strChannel, strCategory, strWho);
                else if (strFlag.at(0) == QChar('-'))
                    strDisplay = QString(tr("* %1 changed channel %2 category")).arg(strWho, strChannel);
            }
            else
            {
                if (strValue.isEmpty())
                    strDisplay = QString(tr("* Channel %1 now has a flag %2 (set by %3)")).arg(strChannel, strFlag, strWho);
                else
                {
                    strDisplay = QString(tr("* %1 now has a flag %2 (set by %3)")).arg(strValue, strFlag, strWho);
                    bNickFlag = true;
                }
            }

            Message::instance()->showMessage(strChannel, strDisplay, MessageMode);

            if ((bNickFlag) && (!strFlag.isEmpty()))
                Nick::instance()->changeFlag(strValue, strChannel, strFlag);
        }
        flag_value.clear();
    }
    // nick
    else
    {
        // get args
        QString strNick = strNickOrChannel;
        QString strFlag = strDataList.at(3);
        if (strFlag[0] == ':') strFlag.remove(0,1);

        // get +-
        QString plusminus = strFlag.at(0);
        // fix flag
        strFlag.remove(0,1);

        // create flags list
        QStringList lFlags;
        for (int i = 0; i < strFlag.size(); i++)
            lFlags << strFlag.at(i);

        foreach (strFlag, lFlags)
        {
            strFlag = plusminus+strFlag;

            QString strDisplay;
            if (strFlag == "+O") strDisplay = QString(tr("* %1 is marked as NetAdmin")).arg(strNick);
            else if (strFlag == "-O") strDisplay = QString(tr("* %1 is no longer marked as NetAdmin")).arg(strNick);
            else if (strFlag == "+b") strDisplay = QString(tr("* %1 is marked as busy")).arg(strNick);
            else if (strFlag == "-b") strDisplay = QString(tr("* %1 is no longer marked as busy")).arg(strNick);
            else if (strFlag == "+W") strDisplay = QString(tr("* %1 has now enabled public webcam")).arg(strNick);
            else if (strFlag == "-W") strDisplay = QString(tr("* %1 has no longer enabled public webcam")).arg(strNick);
            else if (strFlag == "+V") strDisplay = QString(tr("* %1 has now enabled private webcam")).arg(strNick);
            else if (strFlag == "-V") strDisplay = QString(tr("* %1 has no longer enabled private webcam")).arg(strNick);
            else if (strFlag == "+x") strDisplay = QString(tr("* %1 has now encrypted IP")).arg(strNick);
            else if (strFlag == "-x") strDisplay = QString(tr("* %1 has no longer encrypted IP")).arg(strNick);
            else if (strFlag == "+r") strDisplay = QString(tr("* %1 is marked as registered and identified")).arg(strNick);
            else if (strFlag == "-r") strDisplay = QString(tr("* %1 is no longer marked as registered and identified")).arg(strNick);
            else
                strDisplay = QString(tr("* %1 now has a flag %2")).arg(strNick, strFlag);

            if ((strFlag.contains("r")) || (strFlag.contains("x")))
                Message::instance()->showMessage(STATUS_WINDOW, strDisplay, MessageMode);

            if (!strFlag.isEmpty())
                Nick::instance()->changeFlag(strNick, strFlag);

            // registered nick
            if ((strNick == Settings::instance()->get("nick")) && (strFlag == "+r"))
            {
                // get my stats
                Core::instance()->network->send(QString("RS INFO %1").arg(Settings::instance()->get("nick")));

                // channel homes
                Core::instance()->network->send("CS HOMES");
            }
        }
    }
}

// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D PRIVMSG @#scc :hello
// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D PRIVMSG %#scc :hello
// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D PRIVMSG +#scc :hello
// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D PRIVMSG #scc :hello
// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D PRIVMSG ^scc_test :hello
void OnetKernel::raw_privmsg()
{
    if (strDataList.size() < 3) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    // fix emots
    Convert::simpleReverseConvert(strMessage);
    Replace::replaceEmots(strMessage);

    QString strNickOrChannel = strDataList.at(2);

    //channel
    if (strNickOrChannel.contains('#'))
    {
        QString strFullChannel = strNickOrChannel;
        QString strGroup = strFullChannel.left(strFullChannel.indexOf('#'));
        Q_UNUSED (strGroup);

        QString strChannel = strFullChannel.right(strFullChannel.length()-strFullChannel.indexOf('#'));
        Message::instance()->showMessage(strChannel, strMessage, MessageDefault, strNick);
    }
    // priv
    else if (strNickOrChannel.contains('^'))
    {
        QString strChannel = strNickOrChannel;
        Message::instance()->showMessage(strChannel, strMessage, MessageDefault, strNick);
    }
    // nick
    else
    {
        QString strDisplay = QString("-%1- %2").arg(strNick, strMessage);
        Message::instance()->showMessageActive(strDisplay, MessageNotice);
    }
}

// :Llanero!43347263@admin.onet NOTICE #channel :test
// :cf1f2.onet NOTICE scc_test :Your message has been filtered and opers notified: spam #2480
// :Llanero!43347263@admin.onet NOTICE $* :458852 * * :%Fb%%C008100%Weź udział w Konkursie Mikołajkowym - skompletuj zaprzęg Świetego Mikołaja! Więcej info w Wieściach z Czata ! http://czat.onet.pl/1632594,wiesci.html
// :Panie_kierowniku!57643619@devel.onet NOTICE Darom :458852 * * :bum
// :Panie_kierowniku!57643619@devel.onet NOTICE Darom :458853 * * :bum
// :Panie_kierowniku!57643619@devel.onet NOTICE Darom :458854 * * :bum
// :Panie_kierowniku!57643619@devel.onet NOTICE Darom :458855 * * :bum
void OnetKernel::raw_notice()
{
    if (strDataList.size() < 3) return;

    QString strWho = strDataList.at(0);
    if (strWho[0] == ':') strWho.remove(0,1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strNickOrChannel = strDataList.at(2);
    QString strMessage;

    QString strCategory = strDataList.at(3);
    if (strCategory[0] == ':') strCategory.remove(0,1);
    int iCategory = strCategory.toInt();
    QString strCategoryString;

    switch (iCategory)
    {
        case NOTICE_INFO:
            if (strCategoryString.isEmpty()) strCategoryString = tr("Information")+": ";
        case NOTICE_WARNING:
            if (strCategoryString.isEmpty()) strCategoryString = tr("Warning")+": ";
        case NOTICE_ERROR:
            if (strCategoryString.isEmpty()) strCategoryString = tr("Error")+": ";
        case NOTICE_QUESTION:
            if (strCategoryString.isEmpty()) strCategoryString = tr("Question")+": ";

            for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strMessage += " "; strMessage += strDataList.at(i); }
            if (strMessage[0] == ':') strMessage.remove(0,1);
            break;
        default:
            for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
            if (strMessage[0] == ':') strMessage.remove(0,1);
            break;
    }

    QString strDisplay = QString("-%1- %2%3").arg(strWho, strCategoryString, strMessage);

    // channel
    if (strNickOrChannel.contains('#'))
    {
        QString strFullChannel = strNickOrChannel;
        QString strGroup = strFullChannel.left(strFullChannel.indexOf('#'));
        Q_UNUSED (strGroup);

        QString strChannel = strFullChannel.right(strFullChannel.length()-strFullChannel.indexOf('#'));
        Message::instance()->showMessage(strChannel, strDisplay, MessageNotice);
    }
    // priv
    else if (strNickOrChannel.contains('^'))
    {
        QString strChannel = strNickOrChannel;
        Message::instance()->showMessage(strChannel, strDisplay, MessageNotice);
    }
    // nick
    else
    {
        Message::instance()->showMessageActive(strDisplay, MessageNotice);
    }
}

// :osa1987!47751777@F4C727.DA810F.7E1789.E71ED5 INVITE scc_test :^cf1f41437962
// :Merovingian!26269559@jest.piekny.i.uroczy.ma.przesliczne.oczy INVITE scc_test :#Komputery
void OnetKernel::raw_invite()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strIP = strDataList.at(0);
    if (strIP[0] == ':') strIP.remove(0,1);
    strIP = strIP.right(strIP.length()-strIP.indexOf('@')-1);

    QString strChannel = strDataList.at(3);
    if (strChannel[0] == ':') strChannel.remove(0,1);

    // priv name
    if (strChannel[0] == '^')
        Channel::instance()->setAlternativeName(strChannel, strNick);

    // add invite notification
    Invite::instance()->add(strNick, strChannel);

    // is active window
    if (Core::instance()->mainWindow()->isActiveWindow())
    {
        (new DlgInvite(strNick, strChannel))->show(); // should be show - prevent hangup!
    }

    // sound
    if (Settings::instance()->get("disable_sounds") == "false")
        SoundNotify::instance()->play(Query);
}

// :cf1f3.onet TOPIC #scc :Simple Chat Client; current version: beta;
void OnetKernel::raw_topic()
{
    if (strDataList.size() < 3) return;

    QString strWho = strDataList.at(0);
    if (strWho[0] == ':') strWho.remove(0,1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList.at(2);

    QString strTopic;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strTopic += " "; strTopic += strDataList.at(i); }
    if (strTopic[0] == ':') strTopic.remove(0,1);

    QString strDisplay = QString(tr("* %1 changed the topic to: %2")).arg(strWho, strTopic);

    // exist channel
    if (Channel::instance()->contains(strChannel))
    {
        // show msg
        Message::instance()->showMessage(strChannel, strDisplay, MessageMode);

        // set topic in widget
        Channel::instance()->setTopic(strChannel, strTopic);
    }

    // get info
    Core::instance()->network->send(QString("CS INFO %1 i").arg(strChannel));
}

// :~test34534!anonymous@2294E8.94913F.A00186.1A3C28 INVREJECT Merovingian #Scrabble
// :Merovingian!26269559@2294E8.94913F.2E3993.4AF50D INVREJECT scc_test ^cf1f41038619
void OnetKernel::raw_invreject()
{
    if (strDataList.size() < 4) return;

    QString strWho = strDataList.at(0);
    if (strWho[0] == ':') strWho.remove(0,1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList.at(3);

    QString strDisplay;
    if (strChannel[0] == '^')
        strDisplay = QString(tr("* %1 rejected an invitation to priv")).arg(strWho);
    else
        strDisplay = QString(tr("* %1 rejected an invitation to channel %2")).arg(strWho, strChannel);

    Message::instance()->showMessage(strChannel, strDisplay, MessageInfo);
}

// :~test34534!anonymous@2294E8.94913F.A00186.1A3C28 INVIGNORE Merovingian #Scrabble
// :Merovingian!26269559@2294E8.94913F.A00186.4A2B76 INVIGNORE scc_test ^cf1f31294352
void OnetKernel::raw_invignore()
{
    if (strDataList.size() < 4) return;

    QString strWho = strDataList.at(0);
    if (strWho[0] == ':') strWho.remove(0,1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList.at(3);

    QString strDisplay;
    if (strChannel[0] == '^')
        strDisplay = QString(tr("* %1 ignored your invitation to the priv")).arg(strWho);
    else
        strDisplay = QString(tr("* %1 ignored your invitation to the channel %2")).arg(strWho, strChannel);

    // display
    Message::instance()->showMessage(strChannel, strDisplay, MessageInfo);
}

// :~testa!anonymous@3DE379.B7103A.6CF799.6902F4 MODERMSG test1 - #Scrabble :%F:verdana%%Ihehe%
void OnetKernel::raw_modermsg()
{
    if (strDataList.size() < 5) return;

    QString strModerator = strDataList.at(0);
    if (strModerator[0] == ':') strModerator.remove(0,1);
    strModerator = strModerator.left(strModerator.indexOf('!'));

    QString strNick = strDataList.at(2);
    QString strChannel = strDataList.at(4);

    QString strMessage;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    QString strDisplay = QString("%1 [%2 %3]").arg(strMessage, tr("Moderated by"), strModerator);

    // display
    Message::instance()->showMessage(strChannel, strDisplay, MessageDefault, strNick);
}

// :~testa!anonymous@3DE379.B7103A.6CF799.6902F4 MODERNOTICE #Scrabble :a
void OnetKernel::raw_modernotice()
{
    if (strDataList.size() < 3) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strChannel = strDataList.at(2);

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    // display
    Message::instance()->showMessage(strChannel, strMessage, MessageModerNotice, strNick);
}

// :cf1f1.onet MODERATE ~testa opnick #channel cf1f44c3b4b870f8a :%F:verdana%ladnie to tak
void OnetKernel::raw_moderate()
{
    if (strDataList.size() < 6) return;

    QString strNick = strDataList.at(3);
    QString strChannel = strDataList.at(4);

    QString strMessage;
    for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    Convert::simpleConvert(strMessage);

    qint64 iTime = QDateTime::currentMSecsSinceEpoch();

    Channel::instance()->addModerateMessage(strChannel, iTime, strNick, strMessage);
}

// :cf1f4.onet KILL scc_test :cf1f4.onet (Killed (Nickname collision))
// :Darom!12265854@devel.onet KILL Merovingian :cf1f4.onet!devel.onet!Darom (Killed (Darom (bo tak)))
void OnetKernel::raw_kill()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(2);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strWho = strDataList.at(3);
    if (strWho[0] == ':') strWho.remove(0,1);
    QRegExp rx("(.*)!(.*)!(.*)");
    if (rx.exactMatch(strWho))
        strWho = strWho.replace(QRegExp("(.*)!(.*)!(.*)"), "\\3");

    QString strReason;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strReason += " "; strReason += strDataList.at(i); }

    QString strDisplay = QString(tr("* You were killed by %1 %2")).arg(strWho, strReason);

    // display
    Message::instance()->showMessageAll(strDisplay, MessageError);
}

// :Darom!12265854@devel.onet NICK dm
void OnetKernel::raw_nick()
{
    if (strDataList.size() < 3) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strNewNick = strDataList.at(2);

    QString strDisplay = QString(tr("* %1 changed nick to %2")).arg(strNick, strNewNick);

    // update nick
    Nick::instance()->rename(strNick, strNewNick, strDisplay);

    // self
    QString strMe = Settings::instance()->get("nick");
    if (strNick == strMe)
    {
        ProfileManagerModel::instance()->renameProfile(strNick, strNewNick);

        // update nick
        Core::instance()->mainWindow()->updateNick(strNewNick);
    }
}

// :cf1f4.onet 001 scc_test :Welcome to the OnetCzat IRC Network scc_test!51976824@83.28.35.219
void OnetKernel::raw_001()
{
    // logged
    Settings::instance()->set("logged", "true");

    // clear
    FriendsModel::instance()->clear();
    IgnoreModel::instance()->clear();
    ChannelFavouritesModel::instance()->clear();
    ChannelList::instance()->clear();
    MyStatsModel::instance()->clear();
    MyProfileModel::instance()->clear();
    ChannelHomes::instance()->clear();
    // user profile
    UserProfileModel::instance()->clear();
    // channel settings
    ChannelSettingsModel::instance()->clear();

    // protocol
    Core::instance()->network->send("PROTOCTL ONETNAMESX");

    // busy
    Settings::instance()->set("busy", "false");

    // away
    Settings::instance()->set("away", "false");

    // auto busy
    if (Settings::instance()->get("auto_busy") == "true")
        Core::instance()->network->send("BUSY 1");

    // ignore favourites
    if (Settings::instance()->get("disable_autojoin_favourites") == "true")
        Settings::instance()->set("ignore_favourites", "true");
    else
        Settings::instance()->set("ignore_favourites", "false");

    // override off
    Settings::instance()->set("override", "false");

    // age check on
    Settings::instance()->set("age_check", "true");

    // auto rejoin
    QList<CaseIgnoreString> lChannelsCaseIgnore = Channel::instance()->getListClearedSorted();

    //foreach (QString strChannel, lChannelsCaseIgnore)
        //pTabC->removeTab(strChannel);

    foreach (QString strChannel, lChannelsCaseIgnore)
        Core::instance()->network->sendQueue(QString("JOIN %1").arg(strChannel));

    // channel list
    Core::instance()->network->send("SLIST  R- 0 0 100 null");

    // update last active
    Settings::instance()->set("last_active", QString::number(QDateTime::currentMSecsSinceEpoch()));

    // auto-away
    Autoaway::instance()->start();
}

// :cf1f4.onet 002 Merovingian :Your host is cf1f4.onet, running version InspIRCd-1.1
void OnetKernel::raw_002()
{
}

// :cf1f4.onet 003 Merovingian :This server was created 06:35:35 Jan 12 2010
void OnetKernel::raw_003()
{
}

// :cf1f4.onet 004 Merovingian cf1f4.onet InspIRCd-1.1 BGQRVWbinoqrswx DFGIJLMPQRVXYabcehiklmnopqrstuv FIJLXYabcehkloqv
void OnetKernel::raw_004()
{
}

// :cf1f4.onet 005 Merovingian WALLCHOPS WALLVOICES MODES=19 CHANTYPES=^# PREFIX=(qaohXYv)`&@%!=+ MAP MAXCHANNELS=20 MAXBANS=60 VBANLIST NICKLEN=32 CASEMAPPING=rfc1459 STATUSMSG=@%+ CHARSET=ascii :are supported by this server
// :cf1f4.onet 005 Merovingian TOPICLEN=203 KICKLEN=255 MAXTARGETS=20 AWAYLEN=200 CHANMODES=Ibe,k,FJLcl,DGMPQRVimnprstu FNC NETWORK=OnetCzat MAXPARA=32 ELIST=MU OVERRIDE ONETNAMESX INVEX=I EXCEPTS=e :are supported by this server
// :cf1f4.onet 005 Merovingian WATCH=200 INVIGNORE=100 USERIP ESILENCE SILENCE=100 NAMESX :are supported by this server
void OnetKernel::raw_005()
{
    QHash<QString,QString> mKeyValue;

    foreach (QString strData, strDataList)
    {
        if (strData.contains("="))
        {
            QString strKey = strData.mid(0, strData.indexOf('='));
            QString strValue = strData.mid(strData.indexOf('=')+1, strData.length() - strData.indexOf('='));

            mKeyValue.insert(strKey, strValue);
        }
    }
}

// :Onet-Informuje!bot@service.onet NOTICE Merovingian :100 #gorący_pokój 1279807200 :Zapraszamy na spotkanie z Rafałem Głogowskim, ratownikiem krakowskiego WOPRU. Jak zachowywać się nad wodą? Na co zwracać uwagę?
// :Onet-Informuje!bot@service.onet NOTICE $* :100 #Podróże 1291377600 :Wolontariat w Afryce - czy warto spróbować?
void OnetKernel::raw_100n()
{
    if (strDataList.size() < 6) return;

    QString strChannel = strDataList.at(4);
    QString strTime = strDataList.at(5);

    qint64 iTime = QDateTime::fromTime_t(strTime.toInt()).toMSecsSinceEpoch();
    qint64 iCurrentTime = QDateTime::currentMSecsSinceEpoch();
    int iTimeResult = (iTime - iCurrentTime)/1000;
    QString strTimeResult;

    int iSeconds = iTimeResult % SECONDS_IN_MINUTE ;
    int iInMinutes = iTimeResult / SECONDS_IN_MINUTE ;
    int iMinutes = iInMinutes % MINUTES_IN_HOUR ;
    int iInHours = iInMinutes / MINUTES_IN_HOUR ;
    int iHours = iInHours % HOURS_IN_DAY ;
    int iDays = iInHours / HOURS_IN_DAY ;

    if (iDays > 0)
        strTimeResult += QString("%1d ").arg(iDays);
    if (iHours > 0)
        strTimeResult += QString("%1h ").arg(iHours);
    if (iMinutes > 0)
        strTimeResult += QString("%1m ").arg(iMinutes);
    if (iSeconds >= 0)
        strTimeResult += QString("%1s ").arg(iSeconds);

    QString strMessage;
    for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    strMessage.replace("&#8211;", "-");
    strMessage.replace("&#8212;", "-");
    strMessage.replace("&#8216;", "`");
    strMessage.replace("&#8217;", "`");
    strMessage.replace("&#8218;", ",");
    strMessage.replace("&#8220;", "\"");
    strMessage.replace("&#8221;", "\"");
    strMessage.replace("&#8222;", "\"");
    strMessage.replace("&#8230;", "...");

    QString strDisplay;
    if (iTime < iCurrentTime)
        strDisplay = QString(tr("* %1 In progress on channel %2")).arg(strMessage, strChannel);
    else
        strDisplay = QString(tr("* %1 Starting in %2 on channel %3")).arg(strMessage, strTimeResult, strChannel);

    Message::instance()->showMessage(STATUS_WINDOW, strDisplay, MessageNotice);
}

// :GuardServ!service@service.onet NOTICE scc_test :109 #scc :rzucanie mięsem nie będzie tolerowane
void OnetKernel::raw_109n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage[0] = strMessage[0].toUpper();

    strMessage = QString("* %1").arg(strMessage);

    // display
    Message::instance()->showMessage(strChannel, strMessage, MessageNotice);
}

// NS INFO aleksa7
// :NickServ!service@service.onet NOTICE Merovingian :111 aleksa7 type :2
void OnetKernel::raw_111n()
{
    if (strDataList.size() < 7) return;

    QString strNick = strDataList.at(4);
    QString strKey = strDataList.at(5);

    QString strValue;
    for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strValue += " "; strValue += strDataList.at(i); }
    if (strValue[0] == ':') strValue.remove(0,1);

    QString strMe = Settings::instance()->get("nick");

    // set user info
    if ((UserProfileModel::instance()->getNick() == strNick) && (UserProfileModel::instance()->getStatus() != StatusCompleted))
        UserProfileModel::instance()->set(strKey, strValue);

    // set my profile
    if (strNick == strMe)
        MyProfileModel::instance()->set(strKey, strValue);

    // get avatar
    if ((strKey == "avatar") && (!strValue.isEmpty()) && (ThemesModel::instance()->isCurrentWithAvatar()))
    {
        QString strAvatar = Nick::instance()->getAvatar(strNick);
        if (strAvatar.isEmpty())
            Avatar::instance()->get(strNick, "nick", strValue);
        else
            Nick::instance()->setAvatar(strNick, strAvatar);
    }
}

// NS INFO aleksa7
// :NickServ!service@service.onet NOTICE Merovingian :112 aleksa7 :end of user info
void OnetKernel::raw_112n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    if (UserProfileModel::instance()->getNick() == strNick)
        UserProfileModel::instance()->setStatus(StatusCompleted);
}

// NS FRIENDS
// :NickServ!service@service.onet NOTICE scc_test :121 :scc_test Merovingian Succubi Radowsky
void OnetKernel::raw_121n()
{
    if (strDataList.size() < 5) return;

    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strNick = strDataList.at(i);
        if (strNick[0] == ':') strNick.remove(0,1);

        // nothing
    }
}

// NS FRIENDS
// :NickServ!service@service.onet NOTICE scc_test :122 :end of friend list
void OnetKernel::raw_122n()
{
}

// :NickServ!service@service.onet NOTICE Merovingian :123 mokka00 Viola_de_luxe :friend nick changed
void OnetKernel::raw_123n()
{
    if (strDataList.size() < 6) return;

    QString strOldNick = strDataList.at(4);
    if (strOldNick[0] == ':') strOldNick.remove(0,1);

    QString strNewNick = strDataList.at(5);
    if (strNewNick[0] == ':') strNewNick.remove(0,1);

    QString strDisplay = QString(tr("* %1 changed nickname to %2 from your friend list")).arg(strOldNick, strNewNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// NS IGNORE
// :NickServ!service@service.onet NOTICE scc_test :131 :arabeska22 test wilk ~test
void OnetKernel::raw_131n()
{
    if (strDataList.size() < 5) return;

    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strNick = strDataList.at(i);
        if (strNick[0] == ':') strNick.remove(0,1);

        IgnoreModel::instance()->add(strNick);
    }
}

// NS IGNORE
// :NickServ!service@service.onet NOTICE scc_test :132 :end of ignore list
void OnetKernel::raw_132n()
{
}

// :NickServ!service@service.onet NOTICE Merovingian :133 test_nick test_nick_nowy :ignored nick changed
void OnetKernel::raw_133n()
{
    if (strDataList.size() < 6) return;

    QString strOldNick = strDataList.at(4);
    if (strOldNick[0] == ':') strOldNick.remove(0,1);

    QString strNewNick = strDataList.at(5);
    if (strNewNick[0] == ':') strNewNick.remove(0,1);

    QString strDisplay = QString(tr("* %1 changed nickname to %2 from your ignored list")).arg(strOldNick, strNewNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// NS FAVOURITES
// :NickServ!service@service.onet NOTICE scc_test :141 :#Scrabble #Quiz #scc
void OnetKernel::raw_141n()
{
    if (strDataList.size() < 5) return;

    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strChannel = strDataList.at(i);
        if (strChannel[0] == ':') strChannel.remove(0,1);

        ChannelFavouritesModel::instance()->add(strChannel);
    }
}

// NS FAVOURITES
// :NickServ!service@service.onet NOTICE scc_test :142 :end of favourites list
void OnetKernel::raw_142n()
{
    // join favourites
    if (Settings::instance()->get("ignore_favourites") == "false")
    {
        Settings::instance()->set("ignore_favourites", "true");

        QList<CaseIgnoreString> lChannelsCaseIgnore = ChannelFavouritesModel::instance()->getAllCaseIgnore();
        foreach (QString strChannel, lChannelsCaseIgnore)
        {
            if (!Channel::instance()->contains(strChannel))
                Core::instance()->network->sendQueue(QString("JOIN %1").arg(strChannel));
        }
    }
}

// CS HOMES
// :ChanServ!service@service.onet NOTICE scc_test :151 :h#scc
// NS OFFLINE
// :NickServ!service@service.onet NOTICE Merovingian :151 :jubee_blue
void OnetKernel::raw_151n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 4) return;
        if (ChannelHomes::instance()->getStatus() == StatusCompleted) return;

        for (int i = 4; i < strDataList.size(); i++)
        {
            QString strChannel = strDataList.at(i);
            if (strChannel[0] == ':') strChannel.remove(0,1);

            ChannelHomes::instance()->add(strChannel);
        }
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 4) return;

        for (int i = 4; i < strDataList.size(); i++)
        {
            QString strNick = strDataList.at(i);
            if (strNick[0] == ':') strNick.remove(0,1);

            Offline::instance()->addNick(strNick);
        }
    }
}

// CS HOMES
// :ChanServ!service@service.onet NOTICE scc_test :152 :end of homes list
// NS OFFLINE
// :NickServ!service@service.onet NOTICE Merovingian :152 :end of offline senders list
void OnetKernel::raw_152n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 4) return;

        ChannelHomes::instance()->setStatus(StatusCompleted);
    }
    else if (strNick.toLower() == "nickserv")
    {
    }
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE scc_test :160 #scc :Simple Chat Client;
void OnetKernel::raw_160n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strTopic;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strTopic += " "; strTopic += strDataList.at(i); }
    if (strTopic[0] == ':') strTopic.remove(0,1);

    // set topic in channel settings
    if (ChannelSettingsModel::instance()->getChannel() == strChannel)
        ChannelSettingsModel::instance()->setInfo("topic", strTopic);

    // set topic in widget
    if (Channel::instance()->contains(strChannel))
        Channel::instance()->setTopic(strChannel, strTopic);
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE scc_test :161 #scc :topicAuthor=Merovingian rank=0.9095 topicDate=1251579281 private=1 password= limit=0 type=0 createdDate=1247005186 vEmail=0 www=http://simplechatclien.sourceforge.net/ catMajor=4 catMinor=0 official=0 recommended=0 protected=0 moderated=0 avatar=http://foto0.m.onet.pl/_m/e7bd33787bb5cd96031db4034e5f1d54,1,19,0.jpg status=ok guardian=3 auditorium=0
// :ChanServ!service@service.onet NOTICE Merovingian :161 #scc :topicAuthor=Merovingian rank=1.7068 topicDate=1297944969 private=0 type=1 createdDate=1247005186 vEmail=1 www=http://simplechatclien.sourceforge.net/ catMajor=4 moderated=0 avatar=http://foto3.m.onet.pl/_m/97198666362c2c72c6311640f9e791cb,1,19,0-5-53-53-0.jpg guardian=3 email=merovirgian@gmail.com auditorium=0
void OnetKernel::raw_161n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);
    QHash<QString,QString> mKeyValue;

    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList.at(i);
        if (i == 5) strLine.remove(0,1);
        QString strKey = strLine.left(strLine.indexOf("="));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf("=")-1);

        mKeyValue.insert(strKey, strValue);
    }

    // channel settins - data
    if (ChannelSettingsModel::instance()->getChannel() == strChannel)
    {
        QHashIterator <QString, QString> i(mKeyValue);
        while (i.hasNext())
        {
            i.next();

            ChannelSettingsModel::instance()->setInfo(i.key(), i.value());
        }
    }

    // opened channel
    if (Channel::instance()->contains(strChannel))
    {
        // channel info
        if (!Channel::instance()->getDisplayedOptions(strChannel))
        {
            if (mKeyValue.value("moderated") == "1")
            {
                QString strDisplay = QString(tr("* Channel %1 is moderated").arg(strChannel));
                Message::instance()->showMessage(strChannel, strDisplay, MessageInfo);
            }
/*
            if (mKeyValue.value("private") == "1")
            {
                QString strDisplay = QString(tr("* Channel %1 is private").arg(strChannel));
                Message::instance()->showMessage(strChannel, strDisplay, MessageInfo);
            }
*/
            Channel::instance()->setDisplayedOptions(strChannel, true);
        }

        // update topic author
        QString strTopicAuthor = mKeyValue.value("topicAuthor");
        QString strTopicDate = mKeyValue.value("topicDate");
        if ((!strTopicAuthor.isEmpty()) && (!strTopicDate.isEmpty()))
        {
            QString strDT = QDateTime::fromTime_t(strTopicDate.toInt()).toString("dd MMM yyyy hh:mm:ss");
            QString strTopicDetails = QString("%1 (%2)").arg(strTopicAuthor, strDT);
            Channel::instance()->setAuthorTopic(strChannel, strTopicDetails);
        }
    }

    // avatar
    QString strAvatarUrl = mKeyValue.value("avatar");
    if (!strAvatarUrl.isEmpty())
    {
        QString strAvatar = Channel::instance()->getAvatar(strChannel);
        if (strAvatar.isEmpty())
            Avatar::instance()->get(strChannel, "channel", strAvatarUrl);
        else
            Channel::instance()->setAvatar(strChannel, strAvatar);
    }
}

// CS INFO #lunar
// :ChanServ!service@service.onet NOTICE scc_test :162 #lunar :q,Merovingian o,Radowsky o,aleksa7 o,chanky o,osa1987 h,scc_test o,MajkeI
void OnetKernel::raw_162n()
{
    if (strDataList.size() < 6) return;

    QString strChannel = strDataList.at(4);

    if (ChannelSettingsModel::instance()->getChannel() == strChannel)
    {
        for (int i = 5; i < strDataList.size(); i++)
        {
            QString strLine = strDataList.at(i);
            if (i == 5) strLine.remove(0,1);
            QString strKey = strLine.left(strLine.indexOf(","));
            QString strValue = strLine.right(strLine.length() - strLine.indexOf(",")-1);

            if ((!strKey.isEmpty()) && (!strValue.isEmpty()))
                ChannelSettingsModel::instance()->setPermission(strKey, strValue);
        }
    }
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE Merovingian :163 #scc I Olka Merovingian 1289498809 :
// :ChanServ!service@service.onet NOTICE Merovingian :163 #scc b test!*@* Merovingian 1289498776 :
// :ChanServ!service@service.onet NOTICE Merovingian :163 #scc b *!*@haxgu3xx7ptcn4u72yrkbp4daq Merovingian 1289497781 :Tony_Montana
void OnetKernel::raw_163n()
{
    if (strDataList.size() < 10) return;

    QString strChannel = strDataList.at(4);
    QString strFlag = strDataList.at(5);
    QString strNick = strDataList.at(6);
    QString strWho = strDataList.at(7);
    QString strDT = strDataList.at(8);
    QString strIPNick = strDataList.at(9);
    if (strIPNick[0] == ':') strIPNick.remove(0,1);

    strDT = QDateTime::fromTime_t(strDT.toInt()).toString("dd MMM yyyy hh:mm:ss");

    if ((ChannelSettingsModel::instance()->getChannel() == strChannel) && (ChannelSettingsModel::instance()->getStatusInfo() != StatusCompleted))
        ChannelSettingsModel::instance()->setPermission(strFlag, QString("%1;%2;%3;%4").arg(strNick, strWho, strDT, strIPNick));
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE scc_test :164 #scc :end of channel info
void OnetKernel::raw_164n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    if (ChannelSettingsModel::instance()->getChannel() == strChannel)
        ChannelSettingsModel::instance()->setStatusInfo(StatusCompleted);
}

// CS INFO #Relax
// :ChanServ!service@service.onet NOTICE ~test :165 #Relax :Nie ważne, czy szukasz dobrej zabawy, ...
void OnetKernel::raw_165n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strDescription;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strDescription += " "; strDescription += strDataList.at(i); }
    if (strDescription[0] == ':') strDescription.remove(0,1);

    if (ChannelSettingsModel::instance()->getChannel() == strChannel)
        ChannelSettingsModel::instance()->setInfo("desc", strDescription);
}

// RS INFO Merovingian
// :RankServ!service@service.onet NOTICE Merovingian :170 Merovingian :histActive=edgbcebbdccecbdbbccbcdcdccbabb histTotal=ijqkhhlfihiqlnqjlmmllomkohqfji idleTime=14020283 noise=101660 relationsFriend=91 relationsIgnored=0 sessionsTime=19023384 words=361679
void OnetKernel::raw_170n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    if (strNick != Settings::instance()->get("nick")) return; // not my nick

    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList.at(i);
        if (i == 5) strLine.remove(0,1);
        QString strKey = strLine.left(strLine.indexOf("="));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf("=")-1);

        MyStatsModel::instance()->set(strKey, strValue);
    }
}

// RS INFO Merovingian
// :RankServ!service@service.onet NOTICE Merovingian :171 Merovingian :end of user stats
void OnetKernel::raw_171n()
{
}

// RS INFO #scc
// :RankServ!service@service.onet NOTICE Merovingian :175 #scc :histActiveTime=1d9a,6a4,b1b,12c3,157b,3c5a,981,8e9d,5b14,4ea4,1970,198c,2cbd,3505,5500,dc8a,a263,5635,3ab5,232a,2bad,2f51,359f,3b2a,100f,4a17,1c32,15c1,4290,2b06 histNoise=ba,14,4a,82,71,147,3b,3df,301,1d7,e5,99,109,f2,118,69f,432,21f,16d,c0,f0,110,110,144,51,18a,fb,75,1d9,e9 histRelationsFavourite=28,28,29,29,29,29,29,29,29,29,29,29,29,28,28,28,28,28,28,27,28,2a,2b,2a,2a,2b,2b,2b,2c,2c
// :RankServ!service@service.onet NOTICE Merovingian :175 #scc :histSessionsTime=bf9e4,40439,b7be0,66e21,6273f,6e8ff,8af6e,7fdad,7a6ad,766a3,621d0,728be,718cc,83f03,b5c1f,9ae59,96d4e,82724,7c192,8f166,8fef5,6f35d,9384a,87f97,7e031,a3e97,64f00,c2a84,ad3b4,8999b histWebcamTime=aa4,162a,f4a,0,3afa,2c,edc,44a,fe7,1d9,fb0,3e1,1531,33d5,15dc,b5c,2d6d,5c0d,0,48f2,1f85,2111,7a2,2251,25e,fea,1ecb,1445,143c,2280
// :RankServ!service@service.onet NOTICE Merovingian :175 #scc :histWords=247,4d,aa,1a3,130,3f3,a2,dfd,a21,5e8,245,185,379,2f0,41e,161d,1194,8a4,454,2ec,35f,30d,424,493,bc,426,33d,f7,705,307 noise=80619 relationsFavourite=40 visits=78 words=268782
void OnetKernel::raw_175n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QHash<QString,QString> mKeyValue;
    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList.at(i);
        if (i == 5) strLine.remove(0,1);
        QString strKey = strLine.left(strLine.indexOf("="));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf("=")-1);

        mKeyValue.insert(strKey, strValue);
    }

    if ((ChannelSettingsModel::instance()->getChannel() == strChannel) && (ChannelSettingsModel::instance()->getStatusStats() != StatusCompleted))
    {
        QHashIterator <QString, QString> i(mKeyValue);
        while (i.hasNext())
        {
            i.next();

            ChannelSettingsModel::instance()->setStats(i.key(), i.value());
        }
    }
}

// :RankServ!service@service.onet NOTICE Merovingian :176 #scc :end of channel stats
void OnetKernel::raw_176n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    if (ChannelSettingsModel::instance()->getChannel() == strChannel)
        ChannelSettingsModel::instance()->setStatusStats(StatusCompleted);
}

// NS SET city
// :NickServ!service@service.onet NOTICE Merovingian :210 :nothing changed
void OnetKernel::raw_210n()
{
}

// NS SET city
// :NickServ!service@service.onet NOTICE scc_test :211 city :value unset
void OnetKernel::raw_211n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(2);
    QString strKey = strDataList.at(4);

    QString strMe = Settings::instance()->get("nick");

    // set my profile
    if (strNick == strMe)
        MyProfileModel::instance()->set(strKey, QString::null);
}

// NS FRIENDS ADD aaa
// :NickServ!service@service.onet NOTICE scc_test :220 aaa :friend added to list
void OnetKernel::raw_220n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strDisplay = QString(tr("* Added the nickname %1 to a friends list")).arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// NS FRIENDS DEL aaa
// :NickServ!service@service.onet NOTICE scc_test :221 scc_test :friend removed from list
void OnetKernel::raw_221n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strDisplay = QString(tr("* Removed the nickname %1 from your friends list")).arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// NS IGNORE ADD ~test
// :NickServ!service@service.onet NOTICE scc_test :230 ~test :ignore added to list
void OnetKernel::raw_230n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strDisplay = QString(tr("* Added %1 to your ignore list")).arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);

    IgnoreModel::instance()->add(strNick);
}

// NS IGNORE DEL aaa
// :NickServ!service@service.onet NOTICE scc_test :231 ~test :ignore removed from list
void OnetKernel::raw_231n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strDisplay = QString(tr("* Removed %1 from your ignore list")).arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);

    IgnoreModel::instance()->remove(strNick);
}

// NS FAVOURITES ADD scc
// :NickServ!service@service.onet NOTICE scc_test :240 #scc :favourite added to list
void OnetKernel::raw_240n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strDisplay = QString(tr("* Added %1 channel to your favorites list")).arg(strChannel);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);

    ChannelFavouritesModel::instance()->add(strChannel);
}

// NS FAVOURITES DEL scc
// :NickServ!service@service.onet NOTICE scc_test :241 #scc :favourite removed from list
void OnetKernel::raw_241n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strDisplay = QString(tr("* Removed channel %1 from your favorites list")).arg(strChannel);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);

    ChannelFavouritesModel::instance()->remove(strChannel);
}

// CS REGISTER czesctoja
// :ChanServ!service@service.onet NOTICE scc_test :250 #czesctoja :channel registered
// NS OFFLINE MSG nick text
// :NickServ!service@service.onet NOTICE Merovingian :250 scc_test :offline message sent
void OnetKernel::raw_250n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 5) return;

        QString strChannel = strDataList.at(4);

        QString strDisplay = QString(tr("* Successfully created a channel %1")).arg(strChannel);
        Message::instance()->showMessageActive(strDisplay, MessageInfo);

        // homes
        Core::instance()->network->send(QString("CS HOMES"));

        // join
        Core::instance()->network->send(QString("JOIN %1").arg(strChannel));
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 5) return;

        QString strNick = strDataList.at(4);

        QString strDisplay = QString(tr("* Offline message sent to %1")).arg(strNick);
        Message::instance()->showMessageActive(strDisplay, MessageInfo);
    }
}

// LUSERS
// :cf1f4.onet 251 Merovingian :There are 2300 users and 5 invisible on 10 servers
void OnetKernel::raw_251()
{
}

// CS DROP czesctoja
// :ChanServ!service@service.onet NOTICE scc_test :251 #czesctoja :has been dropped
// NS OFFLINE GET scc_test
// :NickServ!service@service.onet NOTICE Merovingian :251 scc_test 1291386193 msg :test message
// :NickServ!service@service.onet NOTICE scc_test :251 Merovingian 1308924405 quote :zostawiam ci wiadomosc
// :NickServ!service@service.onet NOTICE scc_test :251 Merovingian 1308924406 reply :spoko
void OnetKernel::raw_251n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 5) return;

        QString strChannel = strDataList.at(4);

        QString strDisplay = QString(tr("* Channel %1 has been removed")).arg(strChannel);
        Message::instance()->showMessageActive(strDisplay, MessageMode);
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 5) return;

        QString strNick = strDataList.at(4);
        qint64 iTime = QDateTime::fromTime_t(strDataList.at(5).toInt()).toMSecsSinceEpoch();
        QString strType = strDataList.at(6);

        QString strMessage;
        for (int i = 7; i < strDataList.size(); i++) { if (i != 7) strMessage += " "; strMessage += strDataList.at(i); }
        if (strMessage[0] == ':') strMessage.remove(0,1);

        Offline::instance()->addMessage(iTime, strType, strNick, strMessage);
    }
}

// LUSERS
// :cf1f4.onet 252 Merovingian 5 :operator(s) online
void OnetKernel::raw_252()
{
}

// CS DROP czesctoja
// :ChanServ!service@service.onet NOTICE #testabc :252 scc_test :has dropped this channel
// NS OFFLINE REJECT scc_test
// :NickServ!service@service.onet NOTICE Merovingian :252 scc_test :offline messages rejected
void OnetKernel::raw_252n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 5) return;

        QString strChannel = strDataList.at(2);

        QString strDisplay = QString(tr("* Confirmed the removal of the channel %1")).arg(strChannel);
        Message::instance()->showMessageActive(strDisplay, MessageMode);
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 5) return;

        QString strNick = strDataList.at(4);

        QString strDisplay = QString(tr("* Offline messages rejected from %1")).arg(strNick);
        Message::instance()->showMessageActive(strDisplay, MessageInfo);
    }
}

// LUSERS
// :cf1f4.onet 253 Merovingian 1 :unknown connections
void OnetKernel::raw_253()
{
}

// CS TRANSFER %1 %2
// :ChanServ!service@service.onet NOTICE scc_test :253 #test_scc_moj Merovingian :channel owner changed
// NS OFFLINE QUOTE merovingian
// :NickServ!service@service.onet NOTICE scc_test :253 merovingian :offline messages quoted to sender
void OnetKernel::raw_253n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 6) return;

        QString strChannel = strDataList.at(4);
        QString strWho = strDataList.at(2);
        QString strNick = strDataList.at(5);

        QString strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);

        // display
        Message::instance()->showMessage(strChannel, strDisplay, MessageMode);
    }
    else if (strNick.toLower() == "nickserv")
    {
    }
}

// LUSERS
// :cf1f4.onet 254 Merovingian 4641 :channels formed
void OnetKernel::raw_254()
{
}

// CS TRANSFER #test_scc_moj Merovingian
// :ChanServ!service@service.onet NOTICE #test_scc_moj :254 scc_test Merovingian :changed channel owner
void OnetKernel::raw_254n()
{
    if (strDataList.size() < 6) return;

    QString strChannel = strDataList.at(2);
    QString strWho = strDataList.at(4);
    QString strNick = strDataList.at(5);

    QString strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);

    // display
    Message::instance()->showMessage(strChannel, strDisplay, MessageMode);
}

// LUSERS
// :cf1f4.onet 255 Merovingian :I have 568 clients and 1 servers
void OnetKernel::raw_255()
{
}

// CS BAN #scc ADD cos
// :ChanServ!service@service.onet NOTICE scc_test :255 #scc +b cos :channel privilege changed
void OnetKernel::raw_255n()
{
    if (strDataList.size() < 7) return;

    QString strChannel = strDataList.at(4);
    QString strNick = strDataList.at(6);

    QString strDisplay = QString(tr("* Changing privileges confirmed for %1 at %2")).arg(strNick, strChannel);
    Message::instance()->showMessageActive(strDisplay, MessageMode);
}

// ADMIN
// :cf1f1.onet 256 ~test :Administrative info for cf1f1.onet
void OnetKernel::raw_256()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessage(STATUS_WINDOW, strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE #scc :256 Merovingian +o scc_test :channel privilege changed
void OnetKernel::raw_256n()
{
    if (strDataList.size() < 7) return;

    QString strChannel = strDataList.at(2);
    QString strWho = strDataList.at(4);
    QString strFlag = strDataList.at(5);
    QString strNick = strDataList.at(6);
    QString strDisplay;

    if (strFlag == "+q") strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-q") strDisplay = QString(tr("* %1 is no longer the owner of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+o") strDisplay = QString(tr("* %1 is now super-operator on the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-o") strDisplay = QString(tr("* %1 is no longer super-operator on the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+h") strDisplay = QString(tr("* %1 is now the operator of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-h") strDisplay = QString(tr("* %1 is no longer the operator of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+v") strDisplay = QString(tr("* %1 is now a guest of channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-v") strDisplay = QString(tr("* %1 is no longer a guest of channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+X") strDisplay = QString(tr("* %1 is now moderator of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-X") strDisplay = QString(tr("* %1 is no longer moderating channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+Y") strDisplay = QString(tr("* %1 is now screener channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-Y") strDisplay = QString(tr("* %1 is no longer a screener channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+b") strDisplay = QString(tr("* %1 is now on the banned list in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-b") strDisplay = QString(tr("* %1 is no longer on the banned list in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+I") strDisplay = QString(tr("* %1 is now on the list of invitees in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-I") strDisplay = QString(tr("* %1 is no longer on the list of invitees in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else
        strDisplay = QString(tr("* %1 now has a flag %2 (set by %3)")).arg(strNick, strFlag, strWho);

    // display
    Message::instance()->showMessage(strChannel, strDisplay, MessageMode);
}

// ADMIN
// :cf1f1.onet 257 ~test :Name     - Czat Admin
void OnetKernel::raw_257()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessage(STATUS_WINDOW, strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :257 #scc * :settings changed
void OnetKernel::raw_257n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    Core::instance()->network->send(QString("CS INFO %1 i").arg(strChannel));
}

// ADMIN
// :cf1f1.onet 258 ~test :Nickname - czat_admin
void OnetKernel::raw_258()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessage(STATUS_WINDOW, strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE #glupia_nazwa :258 ovo_ d :channel settings changed
// :ChanServ!service@service.onet NOTICE #scc :258 Merovingian * :channel settings changed
// *tdisa
void OnetKernel::raw_258n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(2);
    QString strNick = strDataList.at(4);
    //QString strGroup = strDataList.at(5);

    QString strDisplay = QString(tr("* %1 changed channel %2 settings")).arg(strNick, strChannel);

    // display
    Message::instance()->showMessage(strChannel, strDisplay, MessageInfo);

    Core::instance()->network->send(QString("CS INFO %1 i").arg(strChannel));
}

// ADMIN
// :cf1f1.onet 259 ~test :E-Mail   - czat_admin@czat.onet.pl
void OnetKernel::raw_259()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessage(STATUS_WINDOW, strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :259 #scc :nothing changed
void OnetKernel::raw_259n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strDisplay = QString(tr("* Nothing changed in %1")).arg(strChannel);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :260 Merovingian #scc +o :channel privilege changed
void OnetKernel::raw_260n()
{
    // copy raw 256
    if (strDataList.size() < 7) return;

    QString strChannel = strDataList.at(5);
    QString strWho = strDataList.at(4);
    QString strFlag = strDataList.at(6);
    QString strNick = strDataList.at(2);
    QString strDisplay;

    if (strFlag == "+q") strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-q") strDisplay = QString(tr("* %1 is no longer the owner of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+o") strDisplay = QString(tr("* %1 is now super-operator on the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-o") strDisplay = QString(tr("* %1 is no longer super-operator on the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+h") strDisplay = QString(tr("* %1 is now the operator of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-h") strDisplay = QString(tr("* %1 is no longer the operator of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+v") strDisplay = QString(tr("* %1 is now a guest of channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-v") strDisplay = QString(tr("* %1 is no longer a guest of channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+X") strDisplay = QString(tr("* %1 is now moderator of the channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-X") strDisplay = QString(tr("* %1 is no longer moderating channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+Y") strDisplay = QString(tr("* %1 is now screener channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-Y") strDisplay = QString(tr("* %1 is no longer a screener channel %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+b") strDisplay = QString(tr("* %1 is now on the banned list in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-b") strDisplay = QString(tr("* %1 is no longer on the banned list in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "+I") strDisplay = QString(tr("* %1 is now on the list of invitees in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else if (strFlag == "-I") strDisplay = QString(tr("* %1 is no longer on the list of invitees in %2 (set by %3)")).arg(strNick, strChannel, strWho);
    else
        strDisplay = QString(tr("* %1 now has a flag %2 (set by %3)")).arg(strNick, strFlag, strWho);

    // display
    Message::instance()->showMessage(strChannel, strDisplay, MessageMode);
}

// CS DROP #czesctoja
// :ChanServ!service@service.onet NOTICE scc_test :261 scc_test #czesctoja :has dropped this channel
// NS LIST aaa
// :NickServ!service@service.onet NOTICE Merovingian :261 aa_PrezesCiemnosci gaafa7 jaanka9 Naatasza23 zaak_333 Agaaaaaaaa ~Faajny25 kubaaa19 ~Amaadeusz_x misiaa_40
void OnetKernel::raw_261n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 6) return;

        QString strChannel = strDataList.at(5);

        QString strDisplay = QString(tr("* Successfully removed channel %1")).arg(strChannel);
        Message::instance()->showMessageActive(strDisplay, MessageInfo);

        // homes
        Core::instance()->network->send(QString("CS HOMES"));

        // part
        if (Channel::instance()->contains(strChannel))
            Core::instance()->network->send(QString("PART %1").arg(strChannel));
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 4) return;
        if (FindNick::instance()->getStatus() == StatusCompleted) return;

        for (int i = 4; i < strDataList.size(); i++)
            FindNick::instance()->add(strDataList.at(i));
    }
}

// :NickServ!service@service.onet NOTICE Merovingian :262 aa :end of list
void OnetKernel::raw_262n()
{
    FindNick::instance()->setStatus(StatusCompleted);
}

// NS LIST #scc
// :NickServ!service@service.onet NOTICE Merovingian :263 #scc :no users found
void OnetKernel::raw_263n()
{
    FindNick::instance()->setStatus(StatusCompleted);
}

// LUSERS
// :cf1f4.onet 265 Merovingian :Current Local Users: 568  Max: 1633
void OnetKernel::raw_265()
{
}

// LUSERS
// :cf1f4.onet 266 Merovingian :Current Global Users: 2305  Max: 6562
void OnetKernel::raw_266()
{
}

// SILENCE
// :cf1f2.onet 271 Merovingian Merovingian Aldinach!*@* <privatemessages,channelmessages,invites>
void OnetKernel::raw_271()
{
// FEATURE
}

// SILENCE
// :cf1f2.onet 272 Merovingian :End of Silence List
void OnetKernel::raw_272()
{
}

// WHOIS Merovingian
// :cf1f2.onet 301 scc_test Merovingian :nie ma
void OnetKernel::raw_301()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(3);

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    QString strDisplay = QString(tr("* %1 is away: %2")).arg(strNick, strMessage);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// USERHOST a
// :cf1f2.onet 302 Merovingian :
// USERHOST aleksa7
// :cf1f2.onet 302 Merovingian :aleksa7=+14833406@44DC43.4DB130.368946.600B51
void OnetKernel::raw_302()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// ISON
// :cf1f1.onet 303 ~test :Darom
void OnetKernel::raw_303()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);
    if (strNick[0] == ':') strNick.remove(0,1);

    QString strDisplay = QString("* ISON: %1").arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// OPER
// :cf1f3.onet 304 ~test :SYNTAX OPER <username> <password>
void OnetKernel::raw_304()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// AWAY :
// :cf1f3.onet 305 scc_test :You are no longer marked as being away
void OnetKernel::raw_305()
{
    Away::instance()->stop();

    QString strDisplay = tr("* You are no longer marked as being away");
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// AWAY :reason
// :cf1f3.onet 306 scc_test :You have been marked as being away
void OnetKernel::raw_306()
{
    Away::instance()->start();

    QString strDisplay = tr("* You have been marked as being away");
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHOIS
// :cf1f3.onet 307 scc_test Merovingian :is a registered nick
void OnetKernel::raw_307()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    if (strMessage == "is a registered nick")
        strMessage = tr("is a registered nick");

    QString strDisplay = QString("* %1 %2").arg(strNick, strMessage);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHOIS
// :cf1f1.onet 311 scc_test Merovingian 26269559 2294E8.94913F.196694.9BAE58 * :Merovingian
void OnetKernel::raw_311()
{
    if (strDataList.size() < 8) return;

    QString strNick = strDataList.at(3);
    QString strZUO = strDataList.at(4);
    QString strIP = strDataList.at(5);
    QString strIrcname;

    for (int i = 7; i < strDataList.size(); i++) { if (i != 7) strIrcname += " "; strIrcname += strDataList.at(i); }
    if (strIrcname[0] == ':') strIrcname.remove(0,1);

    QString strDisplayNick = QString(tr("* %1 is %2@%3")).arg(strNick, strZUO, strIP);
    Message::instance()->showMessageActive(strDisplayNick, MessageInfo);

    QString strDisplayIrcname = QString(tr("* %1 ircname: %2")).arg(strNick, strIrcname);
    Message::instance()->showMessageActive(strDisplayIrcname, MessageInfo);
}

// WHOIS
// :cf1f2.onet 312 scc_test Merovingian *.onet :OnetCzat
// WHOWAS
// :cf1f2.onet 312 Merovingian merovingian *.onet :Wed Sep  1 18:37:42 2010
void OnetKernel::raw_312()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strServer;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strServer += " "; strServer += strDataList.at(i); }

    QString strDisplay = QString(tr("* %1 is online via %2")).arg(strNick, strServer);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHOIS
// :cf1f1.onet 313 scc_test Llanero :is a GlobalOp on OnetCzat
// :cf1f2.onet 313 Merovingian Darom :is a NetAdmin on OnetCzat
void OnetKernel::raw_313()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    QString strDisplay;
    if (strMessage == "is a GlobalOp on OnetCzat")
        strDisplay = QString(tr("* %1 is a GlobalOp on OnetCzat")).arg(strNick);
    else if (strMessage == "is a NetAdmin on OnetCzat")
        strDisplay = QString(tr("* %1 is a NetAdmin on OnetCzat")).arg(strNick);
    else if (strMessage == "is a Service on OnetCzat")
        strDisplay = QString(tr("* %1 is a Service on OnetCzat")).arg(strNick);
    else
        strDisplay = strMessage;

    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHOWAS
// :cf1f4.onet 314 Merovingian wilk 54995510 44DC43.553BE3.6C367A.FFF110 * :admin on ...
void OnetKernel::raw_314()
{
    // copy of raw 311
    if (strDataList.size() < 8) return;

    QString strNick = strDataList.at(3);
    QString strZUO = strDataList.at(4);
    QString strIP = strDataList.at(5);
    QString strIrcname;

    for (int i = 7; i < strDataList.size(); i++) { if (i != 7) strIrcname += " "; strIrcname += strDataList.at(i); }
    if (strIrcname[0] == ':') strIrcname.remove(0,1);

    QString strDisplayNick = QString(tr("* %1 is %2@%3")).arg(strNick, strZUO, strIP);
    Message::instance()->showMessageActive(strDisplayNick, MessageInfo);

    QString strDisplayIrcname = QString(tr("* %1 ircname: %2")).arg(strNick, strIrcname);
    Message::instance()->showMessageActive(strDisplayIrcname, MessageInfo);
}

// WHO
// :cf1f2.onet 315 Merovingian a :End of /WHO list.
// :cf1f4.onet 315 Merovingian #16_17_18_19_lat :Too many results
void OnetKernel::raw_315()
{
    if (strDataList.size() < 4) return;

    QString strNickChannel = strDataList.at(3);

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    if (strMessage == "End of /WHO list.")
        return;
    else if (strMessage == "Too many results")
        strMessage = tr("Too many results");

    QString strDisplay = QString("* %1 :%2").arg(strNickChannel, strMessage);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHOWAS
// :cf1f2.onet 314 Merovingian merovingian 26269559 2294E8.94913F.75F4D7.D7A8A7 * :Merovingian
void OnetKernel::raw_316()
{
    // copy of raw 311
    if (strDataList.size() < 8) return;

    QString strNick = strDataList.at(3);
    QString strZUO = strDataList.at(4);
    QString strIP = strDataList.at(5);
    QString strIrcname;

    for (int i = 7; i < strDataList.size(); i++) { if (i != 7) strIrcname += " "; strIrcname += strDataList.at(i); }
    if (strIrcname[0] == ':') strIrcname.remove(0,1);

    QString strDisplayNick = QString(tr("* %1 is %2@%3")).arg(strNick, strZUO, strIP);
    Message::instance()->showMessageActive(strDisplayNick, MessageInfo);

    QString strDisplayIrcname = QString(tr("* %1 ircname: %2")).arg(strNick, strIrcname);
    Message::instance()->showMessageActive(strDisplayIrcname, MessageInfo);
}

// WHOIS
// :cf1f3.onet 317 scc_test Merovingian 7 1263650617 :seconds idle, signon time
void OnetKernel::raw_317()
{
    if (strDataList.size() < 6) return;

    QString strNick = strDataList.at(3);
    int iIdle = strDataList.at(4).toInt();
    QString strTime = strDataList.at(5);
    QString strIdle;

    int iSeconds = iIdle % SECONDS_IN_MINUTE ;
    int iInMinutes = iIdle / SECONDS_IN_MINUTE ;
    int iMinutes = iInMinutes % MINUTES_IN_HOUR ;
    int iInHours = iInMinutes / MINUTES_IN_HOUR ;
    int iHours = iInHours % HOURS_IN_DAY ;
    int iDays = iInHours / HOURS_IN_DAY ;

    if (iDays > 0)
        strIdle += QString("%1d ").arg(iDays);
    if (iHours > 0)
        strIdle += QString("%1h ").arg(iHours);
    if (iMinutes > 0)
        strIdle += QString("%1m ").arg(iMinutes);
    if (iSeconds >= 0)
        strIdle += QString("%1s ").arg(iSeconds);

    if (!((iDays == 0) && (iHours == 0) && (iMinutes == 0) && (iSeconds == 0)))
    {
        QString strDisplayIdle = QString(tr("* %1 is away %2")).arg(strNick, strIdle);
        Message::instance()->showMessageActive(strDisplayIdle, MessageInfo);
    }

    QString strDateTime = QDateTime::fromTime_t(strTime.toInt()).toString("dd MMM yyyy hh:mm:ss");

    QString strDisplaySignon = QString(tr("* %1 is logged in since %2")).arg(strNick, strDateTime);
    Message::instance()->showMessageActive(strDisplaySignon, MessageInfo);
}

// WHOIS
// :cf1f4.onet 318 scc_test Merovingian :End of /WHOIS list.
void OnetKernel::raw_318()
{
}

// WHOIS
// :cf1f4.onet 319 scc_test Merovingian :#testy %#Komputery `#scc `#Quiz `#Scrabble `#hack
void OnetKernel::raw_319()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strChannels;
    for (int i = 4; i < strDataList.size(); i++)
    {
        if (i != 4) strChannels += " ";
        QString strChannel = strDataList.at(i);
        if (strChannel[0] == ':') strChannel.remove(0,1);
        if ((strChannel[0] != '#') && (strChannel[0] != '^')) strChannel.remove(0,1);
        strChannels += strChannel;
    }

    QString strDisplay = QString(tr("* %1 is on channels: %2")).arg(strNick, strChannels);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// :cf1f4.onet 332 scc_test #scc :Simple Chat Client; current version: beta;
void OnetKernel::raw_332()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(3);
    if (strChannel[0] == ':') strChannel.remove(0,1);

    QString strTopic;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strTopic += " "; strTopic += strDataList.at(i); }
    if (strTopic[0] == ':') strTopic.remove(0,1);

    if (Channel::instance()->contains(strChannel))
        Channel::instance()->setTopic(strChannel, strTopic);
}

// :cf1f1.onet 333 scc_test #scc Merovingian!26269559 1253193639
void OnetKernel::raw_333()
{
// supported by raw 161
}

// USERIP
// :cf1f1.onet 340 Darom :Darom*=+12265854@89.111.111.11
void OnetKernel::raw_340()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 341 Merovingian ~test34534 #Scrabble
// :cf1f1.onet 341 scc_test Merovingian ^cf1f1162848
void OnetKernel::raw_341()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(3);
    QString strChannel = strDataList.at(4);

    if (strChannel[0] == '^')
    {
        Channel::instance()->setAlternativeName(strChannel, strNick);

        pTabC->renameTab(strChannel, strNick);
    }
}

// :cf1f4.onet 335 Merovingian Onet-KaOwiec :is a bot on OnetCzat
void OnetKernel::raw_335()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    QString strDisplay;
    if (strMessage == "is a bot on OnetCzat")
        strDisplay = QString(tr("* %1 is a bot on OnetCzat")).arg(strNick);
    else
        strDisplay = strMessage;

    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHO
// :cf1f2.onet 352 Merovingian #testy 12265854 F3F8AF.464CED.BF6592.28AAB2 *.onet Darom G` :0 Darom
// :cf1f2.onet 352 Merovingian * 26269559 2294E8.94913F.75F4D7.D7A8A7 *.onet Merovingian H :0 Merovingian
// :cf1f2.onet 352 Merovingian #uwaga 43347263 0AD995.BF5FE3.665A1E.9BBABB *.onet Llanero G@ :0 Llanero
// :cf1f2.onet 352 Merovingian #RADIO_PIORUNFM_PL 14833406 44DC43.4DB130.368946.600B51 *.onet aleksa7 H% :0 aleksa7
// :cf1f2.onet 352 Merovingian * 18359115 admin.łona *.onet MAS_PSOTKA G :0 onet-czat
void OnetKernel::raw_352()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    strMessage = "* "+strMessage;

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// NAMES
// :cf1f1.onet 353 scc_test = #scc :scc_test|rx,0 `@Merovingian|brx,1 @chanky|rx,1
// :cf1f3.onet 353 Merovingian = #hack :%Hacker %weed %cvf @Majkel SzperaCZ_11 Merovingian `ChanServ %but
void OnetKernel::raw_353()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    for (int i = 5; i < strDataList.size(); i++)
    {
        if (!strDataList.at(i).isEmpty())
        {
            QString strNick = strDataList.at(i);
            if (i == 5) strNick.remove(0,1); // remove :
            strNick = strNick.left(strNick.indexOf("|"));

            QString strSuffix = strDataList.at(i);
            if (strSuffix.contains("|"))
            {
                strSuffix = strSuffix.right(strSuffix.length() - strSuffix.indexOf("|") -1);
                strSuffix = strSuffix.left(strSuffix.length()-2);
            }
            else
                strSuffix = QString::null;

            QString strCleanNick = strNick;

            QString strPrefix;
            if (strCleanNick.contains(FLAG_OWNER)) { strCleanNick.remove(FLAG_OWNER); strPrefix.append(FLAG_OWNER); }
            if (strCleanNick.contains(FLAG_OP)) { strCleanNick.remove(FLAG_OP); strPrefix.append(FLAG_OP); }
            if (strCleanNick.contains(FLAG_HALFOP)) { strCleanNick.remove(FLAG_HALFOP); strPrefix.append(FLAG_HALFOP); }
            if (strCleanNick.contains(FLAG_MOD)) { strCleanNick.remove(FLAG_MOD); strPrefix.append(FLAG_MOD); }
            if (strCleanNick.contains(FLAG_SCREENER)) { strCleanNick.remove(FLAG_SCREENER); strPrefix.append(FLAG_SCREENER); }
            if (strCleanNick.contains(FLAG_VOICE)) { strCleanNick.remove(FLAG_VOICE); strPrefix.append(FLAG_VOICE); }

            QString strModes = strPrefix+strSuffix;
            Nick::instance()->add(strCleanNick, strChannel, strModes);

            // nick avatar
            if ((!strCleanNick.startsWith('~')) && (ThemesModel::instance()->isCurrentWithAvatar()) && (Nick::instance()->getAvatar(strCleanNick).isEmpty()))
            {
                Core::instance()->network->send(QString("NS INFO %1 s").arg(strCleanNick));
            }
        }
    }
}

// :cf1f1.onet 355 Merovingian #uwaga 958 :users
void OnetKernel::raw_355()
{
}

// NAMES
// :cf1f2.onet 366 scc_test #scc :End of /NAMES list.
void OnetKernel::raw_366()
{
}

// WHOWAS
// :cf1f2.onet 369 Merovingian merovingian :End of WHOWAS
void OnetKernel::raw_369()
{
}

// INFO
// :cf1f3.onet 371 ~test :Core Developers:
void OnetKernel::raw_371()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessage(STATUS_WINDOW, strMessage, MessageInfo);
}

// MOTD
// :cf1f4.onet 372 scc_test :- Onet Czat. Inny Wymiar Czatowania. Witamy
void OnetKernel::raw_372()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    Message::instance()->showMessage(STATUS_WINDOW, strMessage, MessageDefault);
}

// INFO
// :cf1f3.onet 374 ~test :End of /INFO list
void OnetKernel::raw_374()
{
}

// MOTD
// :cf1f4.onet 375 scc_test :cf1f4.onet message of the day
void OnetKernel::raw_375()
{
    QString strDisplay = tr("Message Of The Day:");

    Message::instance()->showMessage(STATUS_WINDOW, strDisplay, MessageDefault);
}

// MOTD
// :cf1f1.onet 376 scc_test :End of message of the day.
void OnetKernel::raw_376()
{
}

// WHOIS
// :cf1f1.onet 378 Merovingian Merovingian :is connecting from 26269559@46.113.153.49 46.113.153.49
void OnetKernel::raw_378()
{
    if (strDataList.size() < 9) return;

    QString strNick = strDataList.at(3);
    QString strZuoIP = strDataList.at(7);
    QString strIP = strDataList.at(8);

    QString strMessage = QString(tr("* %1 is connecting from %2 %3")).arg(strNick, strZuoIP, strIP);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// OPER
// :cf1f1.onet 381 Darom :You are now an IRC operator of type NetAdmin
void OnetKernel::raw_381()
{
    QString strMessage = QString(tr("* You are now an IRC operator of type NetAdmin"));
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// TIME
// :cf1f2.onet 391 ~test cf1f2.onet :Tue Jul 13 18:33:05 2010
void OnetKernel::raw_391()
{
    if (strDataList.size() < 4) return;

    QString strServer = strDataList.at(3);

    QString strDateTime;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strDateTime += " "; strDateTime += strDataList.at(i); }
    if (strDateTime[0] == ':') strDateTime.remove(0,1);

    QString strMessage = QString(tr("* Date and time of the server %1: %2")).arg(strServer, strDateTime);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f2.onet 396 ~scc_test 3DE379.B7103A.6CF799.6902F4 :is now your displayed host
void OnetKernel::raw_396()
{
    if (strDataList.size() < 4) return;

    QString strHost = strDataList.at(3);

    QString strMessage = QString(tr("* %1 is now your displayed host")).arg(strHost);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE ~scc_test :400 :you are not registred
void OnetKernel::raw_400n()
{
    if (strDataList.size() < 3) return;

    QString strNick = strDataList.at(2);

    QString strMessage = QString(tr("* %1 :Nick is not registered")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 401 ~Merovingian ~Merovingian1 :No such nick
// :cf1f3.onet 401 Merovingian #asdasdasd :No such channel
// :cf1f3.onet 401 scc_test scc :No such nick/channel
// :cf1f4.onet 401 Merovingian ChanServ :is currently unavailable. Please try again later.
void OnetKernel::raw_401()
{
    if (strDataList.size() < 4) return;

    QString strNickChannel = strDataList.at(3);

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    if (strMessage == "No such nick")
        strMessage = QString(tr("* %1 :No such nick")).arg(strNickChannel);
    else if (strMessage == "No such channel")
        strMessage = QString(tr("* %1 :No such channel")).arg(strNickChannel);
    else if (strMessage == "No such nick/channel")
        strMessage = QString(tr("* %1 :No such nick/channel")).arg(strNickChannel);
    else if (strMessage == "is currently unavailable. Please try again later.")
        strMessage = QString(tr("* %1 is currently unavailable. Please try again later.")).arg(strNickChannel);

    // display
    Message::instance()->showMessageActive(strMessage, MessageInfo);

    // close inactive priv
    if (strNickChannel[0] == '^')
    {
        if (Channel::instance()->contains(strNickChannel))
            pTabC->removeTab(strNickChannel);
    }
}

// :ChanServ!service@service.onet NOTICE scc_test :401 aa :no such nick
// :NickServ!service@service.onet NOTICE Merovingian :401 a :no such nick
void OnetKernel::raw_401n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Nick does not exist")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 402 Merovingian a :No such server
void OnetKernel::raw_402()
{
    if (strDataList.size() < 4) return;

    QString strServer = strDataList.at(3);

    QString strMessage = QString(tr("* %1 :No such server")).arg(strServer);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE Merovingian :402 !*@*aa :invalid mask
void OnetKernel::raw_402n()
{
    if (strDataList.size() < 5) return;

    QString strMask = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Invalid mask")).arg(strMask);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f1.onet 403 ~testa #^cf1f41568 :Invalid channel name
void OnetKernel::raw_403()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage;
    if ((strChannel[0] == '^') && (Channel::instance()->containsAlternativeName(strChannel)))
        strMessage = QString(tr("* Invalid priv with %1").arg(Channel::instance()->getAlternativeName(strChannel)));
    else
        strMessage = QString(tr("* %1 :Invalid channel name")).arg(strChannel);

    // display
    Message::instance()->showMessageActive(strMessage, MessageInfo);

    // close inactive priv
    if (strChannel[0] == '^')
    {
        if (Channel::instance()->contains(strChannel))
            pTabC->removeTab(strChannel);
    }
}

// CS BANIP #scc ADD wilk
// :ChanServ!service@service.onet NOTICE Merovingian :403 wilk :user is not on-line
// NS OFFLINE MSG a a
// :NickServ!service@service.onet NOTICE Merovingian :403 msg :user is not on-line
void OnetKernel::raw_403n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 5) return;

        QString strNick = strDataList.at(4);
        QString strMessage = QString(tr("* %1 :User is not on-line")).arg(strNick);
        Message::instance()->showMessageActive(strMessage, MessageInfo);
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 5) return;

        QString strNick = strDataList.at(4);
        QString strMessage = QString(tr("* %1 :User is not on-line")).arg(strNick);
        Message::instance()->showMessageActive(strMessage, MessageInfo);
    }
}

// :cf1f1.onet 404 scc_test #Quiz :Cannot send to channel (+m)
// :cf1f4.onet 404 ~scc_test #lunar :Cannot send to channel (no external messages)
void OnetKernel::raw_404()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strReason;
    for (int i = 8; i < strDataList.size(); i++) { if (i != 8) strReason += " "; strReason += strDataList.at(i); }
    if (strReason[0] == ':') strReason.remove(0,1);
    if (strReason[0] == '(') strReason.remove(0,1);
    if (strReason[strReason.length()-1] == ')')
        strReason = strReason.left(strReason.length()-1);

    if (strReason == "+m")
        strReason = tr("No moderator on the channel!");

    QString strMessage = QString(tr("* Unable to send a message to %1: %2")).arg(strChannel, strReason);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :404 ~zwariowany_zdzich0 :user is not registred
void OnetKernel::raw_404n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :User is not registred")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f2.onet 405 Merovingian #Warszawa :You are on too many channels
void OnetKernel::raw_405()
{
    if (strDataList.size() < 4) return;

    QString strMessage = QString(tr("* You are on too many channels"));

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// WHOWAS a
//:cf1f2.onet 406 Merovingian a :There was no such nickname
void OnetKernel::raw_406()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage = QString(tr("* %1 :There was no such nickname")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE ~test :406 VHOST :unknown command
void OnetKernel::raw_406n()
{
    if (strDataList.size() < 5) return;

    QString strCmd = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Unknown command")).arg(strCmd);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :407 VOICE :not enough parameters
// :NickServ!service@service.onet NOTICE Merovingian :407 OFFLINE GET :not enough parameters
void OnetKernel::raw_407n()
{
    if (strDataList.size() < 5) return;

    QString strCmd = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Not enough parameters")).arg(strCmd);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :408 dsfdsf :no such channel
// :RankServ!service@service.onet NOTICE Merovingian :408 #aa :no such channel
void OnetKernel::raw_408n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :No such channel")).arg(strChannel);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :NickServ!service@service.onet NOTICE Merovingian :409 WWW :invalid argument
void OnetKernel::raw_409n()
{
    if (strDataList.size() < 5) return;

    QString strCommand = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Invalid argument")).arg(strCommand);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :NickServ!service@service.onet NOTICE ~Merovingian :412 admi :user's data is not ready
void OnetKernel::raw_412n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);
    QString strMe = Settings::instance()->get("nick");

    if (strNick == strMe)
        Core::instance()->network->send(QString("RS INFO %1").arg(strNick));
    else
        Core::instance()->network->send(QString("NS INFO %1 s").arg(strNick));
}

// RS INFO istota_bezduszna
// :RankServ!service@service.onet NOTICE istota_bezduszna :413 istota_bezduszna :user has no stats
void OnetKernel::raw_413n()
{
}

// RS INFO #testa
// :RankServ!service@service.onet NOTICE Merovingian :414 #testa :channel has no stats
void OnetKernel::raw_414n()
{
}

// RS INFO succubi
// :RankServ!service@service.onet NOTICE Merovingian :415 Succubi :permission denied
void OnetKernel::raw_415n()
{
}

// RS INFO #a
// :RankServ!service@service.onet NOTICE Merovingian :416 #a :permission denied
void OnetKernel::raw_416n()
{
}

// :NickServ!service@service.onet NOTICE scc_test :420 aleksa7 :is already on your friend list
void OnetKernel::raw_420n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strMessage = QString(tr("* Nick %1 is already on your friend list")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f2.onet 421 ~test VERSION :This command has been disabled.
// :cf1f4.onet 421 scc_test MOD :Unknown command
void OnetKernel::raw_421()
{
    if (strDataList.size() < 4) return;

    QString strCmd = strDataList.at(3);

    QString strReason;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strReason += " "; strReason += strDataList.at(i); }
    if (strReason[0] == ':') strReason.remove(0,1);

    if (strReason == "Unknown command")
        strReason = tr("Unknown command");
    else if (strReason == "This command has been disabled.")
        strReason = tr("This command has been disabled.");

    QString strMessage = QString("* %1 :%2").arg(strCmd, strReason);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :NickServ!service@service.onet NOTICE scc_test :421 aaa :is not on your friend list
void OnetKernel::raw_421n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strMessage = QString(tr("* Nick %1 is not on your friend list")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :NickServ!service@service.onet NOTICE scc_test :430 wilk :is already on your ignore list
void OnetKernel::raw_430n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strMessage = QString(tr("* %1 is already on your ignore list")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :NickServ!service@service.onet NOTICE scc_test :431 a :is not on your ignore list
void OnetKernel::raw_431n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strMessage = QString(tr("* %1 is not on your ignore list")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 432 1501-unknown ~?o? :Erroneous Nickname
void OnetKernel::raw_432()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage = QString(tr("* %1 :Erroneous Nickname")).arg(strNick);

    Message::instance()->showMessageAll(strMessage, MessageError);
}

// :cf1f1.onet 433 * scc_test :Nickname is already in use.
void OnetKernel::raw_433()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    // disconnect
    Settings::instance()->set("reconnect", "false");
    Core::instance()->network->disconnect();

    if (!strNick.startsWith('~'))
    {
        QString strMessage =
            QString("%1\r\n%2").arg(
                    tr("Nickname %1 is already in use.").arg(strNick),
                    tr("Do you want to take over session?")
            );

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/images/logo16x16.png"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setWindowTitle(tr("Warning"));
        msgBox.setText(strMessage);
        int iSelectedButton = msgBox.exec();

        if (iSelectedButton == QMessageBox::Yes)
        {
            // override
            Settings::instance()->set("override", "true");
            Settings::instance()->set("reconnect", "true");

            Core::instance()->network->connect();
        }
    }
    else
    {
        QString strMessage = QString(tr("* Nickname %1 is already in use.")).arg(strNick);
        Message::instance()->showMessageAll(strMessage, MessageError);
    }
}

// :NickServ!service@service.onet NOTICE scc_test :440 #scc :is already on your favourite list
void OnetKernel::raw_440n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* Channel %1 is already on your favourite list")).arg(strChannel);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// KICK #kusicielki ~prawdziwa
// :cf1f2.onet 441 Merovingian ~prawdziwa #kusicielki :They are not on that channel
void OnetKernel::raw_441()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(3);
    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* %1 is not on %2 channel")).arg(strNick, strChannel);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :NickServ!service@service.onet NOTICE scc_test :441 #scc :is not on your favourite list
void OnetKernel::raw_441n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* Channel %1 is not on your favourite list")).arg(strChannel);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// KICK #rozrywka ~test
// :cf1f3.onet 442 Merovingian #Rozrywka :You're not on that channel!
void OnetKernel::raw_442()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* You're not on %1 channel!")).arg(strChannel);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 443 Merovingian scc #Scrabble :is already on channel
void OnetKernel::raw_443()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(3);
    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* %1 is already on channel %2")).arg(strNick, strChannel);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// SUMMON
// :cf1f3.onet 445 ~test :SUMMON has been disabled (depreciated command)
void OnetKernel::raw_445()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// USERS
// :cf1f2.onet 446 Merovingian :USERS has been disabled (depreciated command)
void OnetKernel::raw_446()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f3.onet 451 SLIST :You have not registered
void OnetKernel::raw_451()
{
    if (strDataList.size() < 3) return;

    QString strCommand = strDataList.at(2);

    QString strMessage = QString(tr("* You have not registered to perform operation %1")).arg(strCommand);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :452 #aaa :channel name already in use
void OnetKernel::raw_452n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Channel name already in use")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :454 #aaaaaaaaaaaaaaaaaaaaaa :not enough unique channel name
// NS OFFLINE GET
// :NickServ!service@service.onet NOTICE Merovingian :454 a :no messages
void OnetKernel::raw_454n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 5) return;

        QString strChannel = strDataList.at(4);

        QString strMessage = QString(tr("* %1 :Not enough unique channel name")).arg(strChannel);
        Message::instance()->showMessageActive(strMessage, MessageInfo);
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 5) return;

        QString strNick = strDataList.at(4);

        QString strDisplay = QString(tr("* No offline messages from %1")).arg(strNick);
        Message::instance()->showMessageActive(strDisplay, MessageInfo);
    }
}

// :NickServ!service@service.onet NOTICE Merovingian :455 scc_test5 :ignores offline messages from you
void OnetKernel::raw_455n()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);

    QString strDisplay = QString(tr("* Offline message cannot be sent. %1 ignores offline messages from you")).arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE Merovingian :456 #test2 Merovingian :is already channel owner
// NS OFFLINE MSG exist_nick test
// :NickServ!service@service.onet NOTICE Merovingian :456 Merovingian :is online
void OnetKernel::raw_456n()
{
    if (strDataList.size() < 1) return;

    QString strNick = strDataList.at(0);
    if (strNick[0] == ':') strNick.remove(0,1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        if (strDataList.size() < 6) return;

        QString strNick = strDataList.at(5);

        QString strMessage = QString(tr("* %1 is already channel owner")).arg(strNick);

        // display
        Message::instance()->showMessageActive(strMessage, MessageInfo);
    }
    else if (strNick.toLower() == "nickserv")
    {
        if (strDataList.size() < 5) return;

        QString strNick = strDataList.at(4);

        QString strDisplay = QString(tr("* Offline message cannot be sent. %1 is online")).arg(strNick);
        Message::instance()->showMessageActive(strDisplay, MessageInfo);
    }
}

// :ChanServ!service@service.onet NOTICE scc_test :458 #scc v scc :unable to remove non-existent privilege
void OnetKernel::raw_458n()
{
    if (strDataList.size() < 7) return;

    QString strChannel = strDataList.at(4);
    QString strWho = strDataList.at(6);

    QString strMessage = QString(tr("* %1 :Unable to remove non-existent privilege")).arg(strWho);

    // display
    Message::instance()->showMessage(strChannel, strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :459 #scc b test :channel privilege already given
void OnetKernel::raw_459n()
{
    if (strDataList.size() < 7) return;

    QString strChannel = strDataList.at(4);
    QString strWho = strDataList.at(6);

    QString strMessage = QString(tr("* %1 :Channel privilege already given")).arg(strWho);

    // display
    Message::instance()->showMessage(strChannel, strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE Merovingian :460 #scc b abc193!*@* :channel list is full
void OnetKernel::raw_460n()
{
    if (strDataList.size() < 7) return;

    QString strChannel = strDataList.at(4);
    QString strFlag = strDataList.at(5);
    QString strNick = strDataList.at(6);

    if (strFlag == "b")
        strFlag = tr("ban");
    else if (strFlag == "I")
        strFlag = tr("invite");

    QString strMessage = QString(tr("* Cannot %1 %2. Channel list is full")).arg(strFlag, strNick);

    // display
    Message::instance()->showMessage(strChannel, strMessage, MessageInfo);
}

// :cf1f2.onet 461 ~test OPER :Not enough parameters.
void OnetKernel::raw_461()
{
    if (strDataList.size() < 4) return;

    QString strCmd = strDataList.at(3);

    QString strMessage = QString(tr("* %1 :Not enough parameters")).arg(strCmd);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :461 #scc scc :channel operators cannot be banned
void OnetKernel::raw_461n()
{
    if (strDataList.size() < 6) return;

    QString strChannel = strDataList.at(4);
    QString strWho = strDataList.at(5);

    QString strMessage = QString(tr("* %1 :Channel operators cannot be banned")).arg(strWho);

    // display
    Message::instance()->showMessage(strChannel, strMessage, MessageInfo);
}

// PASS
// :cf1f2.onet 462 Merovingian :You may not reregister
void OnetKernel::raw_462()
{
    QString strMessage = QString(tr("* You may not reregister"));
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :463 #lunar AUDITORIUM :permission denied, insufficient privileges
void OnetKernel::raw_463n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);
    QString strCommand = strDataList.at(5);

    QString strMessage = QString(tr("* %1 :Permission denied, insufficient privileges in %2 channel")).arg(strCommand, strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE Merovingian :464 TOPIC :invalid argument
void OnetKernel::raw_464n()
{
    if (strDataList.size() < 5) return;

    QString strCommand = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Invalid argument")).arg(strCommand);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE Merovingian :466 #Sex_Randki :channel is blocked
void OnetKernel::raw_466n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :channel is blocked")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :467 #scc :permission denied, you are not a channel owner
void OnetKernel::raw_467n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* %1 :Permission denied, you are not a channel owner")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :468 #scc :permission denied, insufficient privileges
void OnetKernel::raw_468n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* Permission denied, insufficient privileges in %1 channel")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE Merovingian :469 #Czat :channel is private
void OnetKernel::raw_469n()
{
    if (strDataList.size() < 5) return;

    QString strChannel = strDataList.at(4);

    QString strMessage = QString(tr("* Channel %1 is private")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f3.onet 470 ~Merovingian :#testy has become full, so you are automatically being transferred to the linked channel #Awaria
void OnetKernel::raw_470()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);
    if (strChannel[0] == ':') strChannel.remove(0,1);

    QString strLinked = strDataList.at(strDataList.size()-1);

    QString strMessage = QString(tr("* %1 has become full, so you are automatically being transferred to the linked channel %2")).arg(strChannel, strLinked);

    Message::instance()->showMessage(STATUS_WINDOW, strMessage, MessageInfo);
}

// :cf1f2.onet 471 ~Merovingian #testy :Cannot join channel (Channel is full)
void OnetKernel::raw_471()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* Cannot join channel %1 - channel is full")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :ChanServ!service@service.onet NOTICE scc_test :472 #aaaaaaaaaaaaaaaaaaaaaaaaaaaaa :wait 60 seconds before next REGISTER
void OnetKernel::raw_472n()
{
    QString strMessage = QString(tr("* Wait 60 seconds before creating next channel"));
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f3.onet 473 ~scc_test #lunar :Cannot join channel (Invite only)
void OnetKernel::raw_473()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* Cannot join channel %1 - Invite only")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);

    // close inactive priv
    if (strChannel[0] == '^')
    {
        if (Channel::instance()->contains(strChannel))
            pTabC->removeTab(strChannel);
    }
}

// :cf1f3.onet 474 ~scc_test #Quiz :Cannot join channel (You're banned)
void OnetKernel::raw_474()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* Cannot join channel %1 - You're banned")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 475 Merovingian #glupia_nazwa :Cannot join channel (Incorrect channel key)
void OnetKernel::raw_475()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* Cannot join channel %1 - Incorrect channel key")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);

    (new DlgChannelKey(strChannel))->show(); // should be show - prevent hangup!
}

// :cf1f4.onet 481 Merovingian :Permission Denied - You do not have the required operator privileges
void OnetKernel::raw_481()
{
    QString strMessage = QString(tr("* Permission Denied - You do not have the required operator privileges"));
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f1.onet 482 Merovingian #Scrabble :Only a u-line may kick a u-line from a channel.
void OnetKernel::raw_482()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* Only a u-line may kick a u-line from a channel."));

    Message::instance()->showMessage(strChannel, strMessage, MessageInfo);
}

// :cf1f4.onet 484 Merovingian #scc :Can't kick Merovingian as they're a channel founder
// :cf1f4.onet 484 Merovingian #testy :Can't kick user advocato000 from channel (+Q set)
// :cf1f4.onet 484 scc_test #scc :Can't kick scc as your spells are not good enough
void OnetKernel::raw_484()
{
    if (strDataList.size() < 6) return;

    QString strChannel = strDataList.at(3);

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessage(strChannel, strMessage, MessageInfo);
}

// :cf1f3.onet 491 ~test :Invalid oper credentials
void OnetKernel::raw_491()
{
    QString strMessage = QString(tr("* Invalid oper credentials"));

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f3.onet 492 Merovingian ^cf1f3954674 :Can't invite bezduszna_istota to channel (+V set)
void OnetKernel::raw_492()
{
    if (strDataList.size() < 10) return;

    QString strChannel = strDataList.at(3);

    QString strNick = strDataList.at(6);

    QString strReason;
    for (int i = 9; i < strDataList.size(); i++) { if (i != 9) strReason += " "; strReason += strDataList.at(i); }

    strReason.remove("(");
    strReason.remove(")");

    if (strReason == "+V set")
        strReason = tr("+V set");

    QString strMessage = QString(tr("* Can't invite %1 to channel (%2)").arg(strNick, strReason));

    Message::instance()->showMessage(strChannel, strMessage, MessageInfo);
}

//:cf1f2.onet 530 Merovingian #f :Only IRC operators may create new channels
void OnetKernel::raw_530()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* %1 :Only IRC operators may create new channels")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// PRIVMSG a
// :cf1f2.onet 531 Merovingian chanky :You are not permitted to send private messages to this user
void OnetKernel::raw_531()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage = QString(tr("* %1 :You are not permitted to send private messages to this user")).arg(strNick);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 600 scc_test Radowsky 16172032 690A6F.A8219B.7F5EC1.35E57C 1267055769 :arrived online
void OnetKernel::raw_600()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage = QString(tr("* Your friend %1 arrived online")).arg(strNick);
    Message::instance()->showMessageActive(strMessage, MessageInfo);

    FriendsModel::instance()->set(strNick, true);
}

// :cf1f4.onet 601 scc_test Radowsky 16172032 690A6F.A8219B.7F5EC1.35E57C 1267055692 :went offline
void OnetKernel::raw_601()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage = QString(tr("* Your friend %1 went offline")).arg(strNick);
    Message::instance()->showMessageActive(strMessage, MessageInfo);

    FriendsModel::instance()->set(strNick, false);
}

// NS FRIENDS DEL nick
// :cf1f3.onet 602 scc_test aaa * * 0 :stopped watching
void OnetKernel::raw_602()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    FriendsModel::instance()->remove(strNick);
}

//:cf1f1.onet 604 scc_test scc_test 51976824 3DE379.B7103A.6CF799.6902F4 1267054441 :is online
void OnetKernel::raw_604()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

//    hidden
//    QString strMessage = QString(tr("* Your friend %1 is now on-line")).arg(strNick);
//    Message::instance()->showMessageActive(strMessage, InfoMessage);

    FriendsModel::instance()->set(strNick, true);
}

// :cf1f1.onet 605 scc_test Radowsky * * 0 :is offline
void OnetKernel::raw_605()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

//    hidden
//    QString strMessage = QString(tr("* Your friend %1 is now off-line")).arg(strNick);
//    Message::instance()->showMessageActive(strMessage, InfoMessage);

    FriendsModel::instance()->set(strNick, false);
}

// WATCH
// :cf1f2.onet 607 Merovingian :End of WATCH list
void OnetKernel::raw_607()
{
}

// SERVER
// :cf1f1.onet 666 ~test :You cannot identify as a server, you are a USER. IRC Operators informed.
void OnetKernel::raw_666()
{
    QString strMessage = QString(tr("* You cannot identify as a server, you are a USER. IRC Operators informed."));

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// COMMANDS
// :cf1f2.onet 702 Merovingian :ZLINE <core> 1
// MODULES
// :cf1f2.onet 702 Merovingian :m_onetauditorium.so
void OnetKernel::raw_702()
{
    if (strDataList.size() < 4) return;

    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);
    strMessage = "* "+strMessage;

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// MODULES
// :cf1f2.onet 703 Merovingian :End of MODULES list
void OnetKernel::raw_703()
{
}

// COMMANDS
// :cf1f2.onet 704 Merovingian :End of COMMANDS list
void OnetKernel::raw_704()
{
}

// :cf1f3.onet 801 scc_test :q5VMy1wl6hKL5ZUt
// :cf1f2.onet 801 384-unknown :mIGlbZP0R056xedZ
void OnetKernel::raw_801()
{
    if (strDataList.size() < 4) return;

    if (!Core::instance()->network->isConnected())
        return;

    QString strKey = strDataList.at(3);
    if (strKey[0] == ':') strKey.remove(0,1);

    QString strAuthKey = OnetUtils::transformKey(strKey);
    if (strAuthKey.length() == 16)
    {
        Core::instance()->network->send(QString("AUTHKEY %1").arg(strAuthKey));
        QString strUOKey = Settings::instance()->get("uokey");
        QString strNickUo = Settings::instance()->get("uo_nick");
        if ((!strUOKey.isEmpty()) && (!strNickUo.isEmpty()))
            Core::instance()->network->send(QString("USER * %1 czat-app.onet.pl :%2").arg(strUOKey, strNickUo));
    }
    else
    {
        QString strError = tr("Error: Invalid auth key");
        Message::instance()->showMessage(STATUS_WINDOW, strError, MessageError);
    }
}

// :cf1f1.onet 802 * :Corrupted Nickname
void OnetKernel::raw_802()
{
    QString strMessage = QString(tr("* Corrupted Nickname"));
    Message::instance()->showMessageActive(strMessage, MessageError);
}

// BUSY 1
// :cf1f2.onet 807 scc_test :You are marked as busy
void OnetKernel::raw_807()
{
    Busy::instance()->start();

    QString strDisplay = tr("* You are marked as busy");
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// BUSY 0
// :cf1f2.onet 808 scc_test :You are no longer marked busy
void OnetKernel::raw_808()
{
    Busy::instance()->stop();

    QString strDisplay = tr("* You are no longer marked busy");
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHOIS
// :cf1f2.onet 809 scc_test Succubi :is busy
void OnetKernel::raw_809()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage = QString(tr("* %1 is busy")).arg(strNick);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f2.onet 811 scc_test Merovingian :Ignore invites
void OnetKernel::raw_811()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage;

    if (strNick[0] == '^')
        strMessage = QString(tr("* Ignored priv from %1")).arg(strNick);
    else
        strMessage = QString(tr("* Ignored invite from %1")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f2.onet 812 scc_test Merovingian ^cf1f41284615 :Invite rejected
// :cf1f3.onet 812 Merovingian Vasquez_ #Kraina_Lagodności :Invite rejected
void OnetKernel::raw_812()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(3);
    QString strChannel = strDataList.at(4);

    QString strMessage;

    if (strChannel[0] == '^')
        strMessage = QString(tr("* Rejected priv from %1")).arg(strNick);
    else
        strMessage = QString(tr("* Rejected invite to %1 from %2")).arg(strChannel, strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// WHOIS
// :cf1f4.onet 815 ~test testnick :Public webcam
void OnetKernel::raw_815()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strDisplay = QString(tr("* %1 has public webcam")).arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// WHOIS
// :cf1f4.onet 816 ~test testnick :Private webcam
void OnetKernel::raw_816()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strDisplay = QString(tr("* %1 has private webcam")).arg(strNick);
    Message::instance()->showMessageActive(strDisplay, MessageInfo);
}

// :cf1f2.onet 817 scc_test #scc 1253216797 mikefear - :%Fb:arial%%Ce40f0f%re
void OnetKernel::raw_817()
{
    if (strDataList.size() < 6) return;

    QString strChannel = strDataList.at(3);
    qint64 iTime = QDateTime::fromTime_t(strDataList.at(4).toInt()).toMSecsSinceEpoch();
    QString strNick = strDataList.at(5);

    QString strMessage;
    for (int i = 7; i < strDataList.size(); i++) { if (i != 7) strMessage += " "; strMessage += strDataList.at(i); }
    if (strMessage[0] == ':') strMessage.remove(0,1);

    if (!strMessage.isEmpty())
        Message::instance()->showMessage(strChannel, strMessage, MessageDefault, strNick, iTime);
}

// SLIST
// :cf1f3.onet 818 scc_test :Start of simple channels list.
void OnetKernel::raw_818()
{
}

// SLIST
// :cf1f3.onet 819 scc_test :#tarnów_dziki:g:1,#Żory:g:0,#Mława:O:0,#Lineage_II:_:1,#kakakak:O:0,#apostolat_yfl:_:0,#ITALIA_CLUB:i:23,#Finał_WOŚP:P:0,#sama_słodycz:O:0,#Suwałki:i:14,#Mamuśki:O:0,#Pokój_Radości:O:0,#Antwerpia:g:0,#Kolo_Gospodyn_Wiejskich:O:0,#Samotnia_Kurka:G:0,#Wszystko_o_grach:O:0,#VIPy_NowySącz:h:0,#tymczasowy:G:0,#Zielona_Góra:h:2,#45slonko:P:0,#kawalek_nieba:O:0,#Wirtualna_Przyjazn:a:11,#Magiczny_swiat:O:1,#herbatka_u_cynamonki:P:0,#DEUTSCHLAND:i:111,#informatyka:`:1
void OnetKernel::raw_819()
{
    if (strDataList.size() < 4) return;

    QString strChannelsString;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strChannelsString += " "; strChannelsString += strDataList.at(i); }
    if (strChannelsString[0] == ':') strChannelsString.remove(0,1);

    if (ChannelList::instance()->getStatus() == StatusCompleted)
        return;

    QStringList strChannelsList = strChannelsString.split(",");
    for (int i = 0; i < strChannelsList.size(); i++)
    {
        QStringList strChannelParameters = strChannelsList.at(i).split(":");
        if (strChannelParameters.size() == 3)
        {
            QString strChannelName = strChannelParameters.at(0);
            int iChannelPeople = QString(strChannelParameters.at(2)).toInt();
            int iChannelCat = 0;
            int iChannelType = 0;
            bool bChannelModerated = false;
            bool bChannelRecommended = false;

            bool flag = false;
            bool flag1 = false;
            //bool flag2 = false;
            //bool flag3 = false;
            //bool flag4 = false;

            QString s1 = strChannelParameters.at(1);
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
            //flag4 = l == 3;

            iChannelType = l;
            iChannelCat = k;

            if (flag)
                bChannelModerated = true;
            if (flag1)
                bChannelRecommended = true;

            OnetChannelList add;
            add.name = strChannelName;
            add.people = iChannelPeople;
            add.cat = iChannelCat;
            add.type = iChannelType;
            add.moderated = bChannelModerated;
            add.recommended = bChannelRecommended;

            ChannelList::instance()->add(add);
        }
    }
}

// SLIST
// :cf1f3.onet 820 scc_test :End of simple channel list.
void OnetKernel::raw_820()
{
    ChannelList::instance()->setStatus(StatusCompleted);
    ChannelList::instance()->setTime(QDateTime::currentMSecsSinceEpoch());
}

// :cf1f3.onet 821 scc_test #scc :Channel is not moderated
void OnetKernel::raw_821()
{
    if (strDataList.size() < 4) return;

    QString strChannel = strDataList.at(3);

    QString strMessage = QString(tr("* Channel %1 is not moderated")).arg(strChannel);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// :cf1f4.onet 942 Merovingian nick_porzucony.64131723 :Invalid nickname
void OnetKernel::raw_942()
{
    if (strDataList.size() < 4) return;

    QString strNick = strDataList.at(3);

    QString strMessage = QString(tr("* %1 :Invalid nickname")).arg(strNick);

    Message::instance()->showMessageActive(strMessage, MessageInfo);
}

// NS IGNORE DEL nick
// :cf1f2.onet 950 Merovingian Merovingian :Removed Succubi!*@* <privatemessages,channelmessages,invites> from silence list
void OnetKernel::raw_950()
{
    if (strDataList.size() < 6) return;

    QString strNick = strDataList.at(5);

    Q_UNUSED (strNick);
//    hidden
//    QString strDisplay = QString(tr("* Removed %1 from silence list")).arg(strNick);
//    Message::instance()->showMessageActive(strDisplay, InfoMessage);
}

// NS IGNORE ADD nick
// :cf1f1.onet 951 scc_test scc_test :Added test!*@* <privatemessages,channelmessages,invites> to silence list
void OnetKernel::raw_951()
{
    if (strDataList.size() < 6) return;

    QString strNick = strDataList.at(5);

    Q_UNUSED (strNick);
//    hidden
//    QString strMessage = QString(tr("* Added %1 to silence list")).arg(strNick);
//    Message::instance()->showMessageActive(strMessage, InfoMessage);
}

// :cf1f4.onet 952 Merovingian Merovingian :Succubi!*@* <privatemessages,channelmessages,invites> is already on your silence list
void OnetKernel::raw_952()
{
    if (strDataList.size() < 5) return;

    QString strNick = strDataList.at(4);
    if (strNick[0] == ':') strNick.remove(0,1);

    QString strMessage = QString(tr("* %1 is already on your silence list")).arg(strNick);
    Message::instance()->showMessageActive(strMessage, MessageInfo);
}
