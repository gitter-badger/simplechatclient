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

#include <QDateTime>
#include <QMessageBox>
#include <QSettings>
#include <QStringList>
#include <QTimer>
#include "avatar.h"
#include "core.h"
#include "dlg_channel_key.h"
#include "dlg_channel_settings.h"
#include "dlg_invite.h"
#include "dlg_moderation.h"
#include "dlg_user_profile.h"
#include "log.h"
#include "network.h"
#include "notify.h"
#include "onet_auth.h"
#include "tab_container.h"
#include "onet_kernel.h"

OnetKernel::OnetKernel(Network *param1, TabContainer *param2, DlgChannelSettings *param3, DlgModeration *param4,  DlgUserProfile *param5)
{
    pNetwork = param1;
    pTabC = param2;
    pDlgChannelSettings = param3;
    pDlgModeration = param4;
    pDlgUserProfile = param5;

    avatar = new Avatar(pTabC);
}

OnetKernel::~OnetKernel()
{
    delete avatar;
}

void OnetKernel::timer_rename_channel()
{
    QMap <QString, QString>::const_iterator i = mOldNameNewName.begin();
    while (i != mOldNameNewName.constEnd())
    {
        QString strOldName = i.key();
        QString strNewName = i.value();
        if (pTabC->exist_tab(strOldName) == true)
        {
            bool bRenamed = pTabC->rename_tab(strOldName, strNewName);
            if (bRenamed == true)
                mOldNameNewName.remove(strOldName);
            else
                QTimer::singleShot(1000*5, this, SLOT(timer_rename_channel())); // 5 sec
        }
        else
            QTimer::singleShot(1000*5, this, SLOT(timer_rename_channel())); // 5 sec

        ++i;
    }
}

void OnetKernel::kernel(QString param1)
{
    strData = param1;
    strDataList = strData.split(" ");

#ifdef Q_WS_X11
    QSettings settings;
    if (settings.value("debug").toString() == "on")
        qDebug() << "<- " << strData;
#endif

    bool bUnknownRaw1 = false;
    bool bUnknownRaw2 = false;
    bool bUnknownRaw3 = false;

    if (strDataList.value(1).isEmpty() == false)
    {
        if (strDataList[0].toLower() == "ping")
            raw_ping();
        else if (strDataList[0].toLower() == "error")
            raw_error();
        else if (strDataList[1].toLower() == "pong")
            raw_pong();
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
        else if (strDataList[1].toLower() == "kill")
            raw_kill();
        else
            bUnknownRaw1 = true;

        if (strDataList[1].toLower() == "001")
            raw_001();
        else if (strDataList[1].toLower() == "002")
            raw_002();
        else if (strDataList[1].toLower() == "003")
            raw_003();
        else if (strDataList[1].toLower() == "004")
            raw_004();
        else if (strDataList[1].toLower() == "005")
            raw_005();
        else if (strDataList[1].toLower() == "251")
            raw_251();
        else if (strDataList[1].toLower() == "252")
            raw_252();
        else if (strDataList[1].toLower() == "253")
            raw_253();
        else if (strDataList[1].toLower() == "254")
            raw_254();
        else if (strDataList[1].toLower() == "255")
            raw_255();
        else if (strDataList[1].toLower() == "256")
            raw_256();
        else if (strDataList[1].toLower() == "257")
            raw_257();
        else if (strDataList[1].toLower() == "258")
            raw_258();
        else if (strDataList[1].toLower() == "259")
            raw_259();
        else if (strDataList[1].toLower() == "265")
            raw_265();
        else if (strDataList[1].toLower() == "266")
            raw_266();
        else if (strDataList[1].toLower() == "271")
            raw_271();
        else if (strDataList[1].toLower() == "272")
            raw_272();
        else if (strDataList[1].toLower() == "301")
            raw_301();
        else if (strDataList[1].toLower() == "302")
            raw_302();
        else if (strDataList[1].toLower() == "303")
            raw_303();
        else if (strDataList[1].toLower() == "304")
            raw_304();
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
        else if (strDataList[1].toLower() == "314")
            raw_314();
        else if (strDataList[1].toLower() == "315")
            raw_315();
        else if (strDataList[1].toLower() == "316")
            raw_316();
        else if (strDataList[1].toLower() == "317")
            raw_317();
        else if (strDataList[1].toLower() == "318")
            raw_318();
        else if (strDataList[1].toLower() == "319")
            raw_319();
        else if (strDataList[1].toLower() == "332")
            raw_332();
        else if (strDataList[1].toLower() == "333")
            raw_333();
        else if (strDataList[1].toLower() == "341")
            raw_341();
        else if (strDataList[1].toLower() == "352")
            raw_352();
        else if (strDataList[1].toLower() == "353")
            raw_353();
        else if (strDataList[1].toLower() == "355")
            raw_355();
        else if (strDataList[1].toLower() == "366")
            raw_366();
        else if (strDataList[1].toLower() == "369")
            raw_369();
        else if (strDataList[1].toLower() == "371")
            raw_371();
        else if (strDataList[1].toLower() == "372")
            raw_372();
        else if (strDataList[1].toLower() == "374")
            raw_374();
        else if (strDataList[1].toLower() == "375")
            raw_375();
        else if (strDataList[1].toLower() == "376")
            raw_376();
        else if (strDataList[1].toLower() == "378")
            raw_378();
        else if (strDataList[1].toLower() == "391")
            raw_391();
        else if (strDataList[1].toLower() == "396")
            raw_396();
        else if (strDataList[1].toLower() == "401")
            raw_401();
        else if (strDataList[1].toLower() == "402")
            raw_402();
        else if (strDataList[1].toLower() == "403")
            raw_403();
        else if (strDataList[1].toLower() == "404")
            raw_404();
        else if (strDataList[1].toLower() == "406")
            raw_406();
        else if (strDataList[1].toLower() == "421")
            raw_421();
        else if (strDataList[1].toLower() == "432")
            raw_432();
        else if (strDataList[1].toLower() == "433")
            raw_433();
        else if (strDataList[1].toLower() == "443")
            raw_443();
        else if (strDataList[1].toLower() == "445")
            raw_445();
        else if (strDataList[1].toLower() == "446")
            raw_446();
        else if (strDataList[1].toLower() == "451")
            raw_451();
        else if (strDataList[1].toLower() == "461")
            raw_461();
        else if (strDataList[1].toLower() == "462")
            raw_462();
        else if (strDataList[1].toLower() == "470")
            raw_470();
        else if (strDataList[1].toLower() == "471")
            raw_471();
        else if (strDataList[1].toLower() == "473")
            raw_473();
        else if (strDataList[1].toLower() == "474")
            raw_474();
        else if (strDataList[1].toLower() == "475")
            raw_475();
        else if (strDataList[1].toLower() == "481")
            raw_481();
        else if (strDataList[1].toLower() == "482")
            raw_482();
        else if (strDataList[1].toLower() == "484")
            raw_484();
        else if (strDataList[1].toLower() == "491")
            raw_491();
        else if (strDataList[1].toLower() == "530")
            raw_530();
        else if (strDataList[1].toLower() == "531")
            raw_531();
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
        else if (strDataList[1].toLower() == "607")
            raw_607();
        else if (strDataList[1].toLower() == "666")
            raw_666();
        else if (strDataList[1].toLower() == "702")
            raw_702();
        else if (strDataList[1].toLower() == "703")
            raw_703();
        else if (strDataList[1].toLower() == "704")
            raw_704();
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
        else if (strDataList[1].toLower() == "950")
            raw_950();
        else if (strDataList[1].toLower() == "951")
            raw_951();
        else if (strDataList[1].toLower() == "952")
            raw_952();
        else
            bUnknownRaw2 = true;

        if ((strDataList[1].toLower() == "notice") && (strDataList.value(3).isEmpty() == false))
        {
            if (strDataList[3].toLower() == ":100")
                raw_100n();
            else if (strDataList[3].toLower() == ":109")
                raw_109n();
            else if (strDataList[3].toLower() == ":111")
                raw_111n();
            else if (strDataList[3].toLower() == ":112")
                raw_112n();
            else if (strDataList[3].toLower() == ":121")
                raw_121n();
            else if (strDataList[3].toLower() == ":122")
                raw_122n();
            else if (strDataList[3].toLower() == ":123")
                raw_123n();
            else if (strDataList[3].toLower() == ":131")
                raw_131n();
            else if (strDataList[3].toLower() == ":132")
                raw_132n();
            else if (strDataList[3].toLower() == ":133")
                raw_133n();
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
            else if (strDataList[3].toLower() == ":170")
                raw_170n();
            else if (strDataList[3].toLower() == ":171")
                raw_171n();
            else if (strDataList[3].toLower() == ":175")
                raw_175n();
            else if (strDataList[3].toLower() == ":176")
                raw_176n();
            else if (strDataList[3].toLower() == ":210")
                raw_210n();
            else if (strDataList[3].toLower() == ":211")
                raw_211n();
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
            else if (strDataList[3].toLower() == ":262")
                raw_262n();
            else if (strDataList[3].toLower() == ":263")
                raw_263n();
            else if (strDataList[3].toLower() == ":400")
                raw_400n();
            else if (strDataList[3].toLower() == ":401")
                raw_401n();
            else if (strDataList[3].toLower() == ":402")
                raw_402n();
            else if (strDataList[3].toLower() == ":403")
                raw_403n();
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
            else if (strDataList[3].toLower() == ":413")
                raw_413n();
            else if (strDataList[3].toLower() == ":414")
                raw_414n();
            else if (strDataList[3].toLower() == ":415")
                raw_415n();
            else if (strDataList[3].toLower() == ":416")
                raw_416n();
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
            else if (strDataList[3].toLower() == ":456")
                raw_456n();
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
            else if (strDataList[3].toLower() == ":469")
                raw_469n();
            else if (strDataList[3].toLower() == ":472")
                raw_472n();
            else if ((strDataList[3].length() != 4) || (strDataList[3].toLower() == ":***"))
                raw_notice();
            else
                bUnknownRaw3 = true;
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
    if (bUnknownRaw1 == true && bUnknownRaw2 == true && bUnknownRaw3 == true)
    {
        Log *l = new Log();
        l->save("unknown_raw", strData);
        delete l;
    }
}

// PING :cf1f1.onet
void OnetKernel::raw_ping()
{
    if (strDataList.value(1).isEmpty() == true) return;

    QString strServer = strDataList[1];

    if (strServer.isEmpty() == false)
        pNetwork->send(QString("PONG %1").arg(strServer));
}

// :cf1f4.onet PONG cf1f4.onet :1279652441.189
void OnetKernel::raw_pong()
{
    if (strDataList.value(1).isEmpty() == true) return;
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strServerTime = strDataList[3];
    if (strServerTime[0] == ':')
        strServerTime = strServerTime.right(strServerTime.length()-1);

    // check correct pong
    if (strServerTime.contains(QRegExp("(\\d+)\\.(\\d+)")) == false)
        return; // incorrect

    // get time from pong
    QStringList strTimeAll = strServerTime.split(".");
    int iTime1 = strTimeAll.at(0).toInt();
    int iTime2 = strTimeAll.at(1).toInt();

    // get current time
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrent1 = (int)dt.toTime_t(); // seconds that have passed since 1970
    int iCurrent2 = (dt.toString("zzz")).toInt(); // miliseconds

    // calculate
    int iCTime1 = iCurrent1-iTime1;
    int iCTime2 = iCurrent2-iTime2;

    // correct miliseconds
    if (iCTime2 < 0)
        iCTime2 += 1000;

    // if not correct
    if (((iCTime1 < 0) || (iCTime1 > 301)) || ((iCTime2 < 0) || (iCTime2 > 1001)))
        return;

    QString sec = QString::number(iCTime1);
    QString msec = QString::number(iCTime2);

    // display lag
    if (msec.size() > 3)
        msec = msec.left(3);
    else if (msec.size() == 2)
        msec = "0"+msec;
    else if (msec.size() == 1)
        msec = "00"+msec;

    QString strLag = sec+"."+msec;
    strLag = "Lag: "+strLag+"s";

    Core::instance()->lagAct->setText(strLag);
}

// ERROR :Closing link (unknown@95.48.183.154) [Registration timeout]
void OnetKernel::raw_error()
{
    QString strMessage;
    for (int i = 0; i < strDataList.size(); i++) { if (i != 0) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    pTabC->show_msg_all(strMessage, 7);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 JOIN #Quiz :rx,0
void OnetKernel::raw_join()
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

    QString strSuffix;
    if (strDataList.value(3).isEmpty() == false)
    {
        strSuffix= strDataList[3];
        if (strSuffix[0] == ':')
            strSuffix = strSuffix.right(strSuffix.length()-1);
        strSuffix = strSuffix.left(strSuffix.length()-2);
    }

    QSettings settings;
    QString strDisplay;

    if (strChannel[0] != '^')
    {
        if (settings.value("show_zuo").toString() == "on")
            strDisplay = QString(tr("* %1 [%2@%3] has joined %4")).arg(strNick).arg(strZUO).arg(strIP).arg(strChannel);
        else
            strDisplay = QString(tr("* %1 [%2] has joined %3")).arg(strNick).arg(strIP).arg(strChannel);
    }
    else
    {
        if (settings.value("show_zuo").toString() == "on")
            strDisplay = QString(tr("* %1 [%2@%3] has joined priv")).arg(strNick).arg(strZUO).arg(strIP);
        else
            strDisplay = QString(tr("* %1 [%2] has joined priv")).arg(strNick).arg(strIP);
    }

    pTabC->add_tab(strChannel);
    pTabC->show_msg(strChannel, strDisplay, 1);

    QString strMe = settings.value("nick").toString();

    if (strNick == strMe)
    {
        emit update_nick(strNick);

        if (pTabC->exist_tab(strChannel))
            emit clear_nicklist(strChannel);
    }
    if ((strNick == strMe) && (strChannel[0] != '^'))
        pNetwork->send(QString("CS INFO %1 i").arg(strChannel));

    // nick avatar
    if ((strNick[0] != '~') && (Core::instance()->mNickAvatar.contains(strNick) == false))
    {
        if (settings.value("disable_avatars").toString() == "off") // with avatars
            pNetwork->send(QString("NS INFO %1 s").arg(strNick));
    }

    emit add_user(strChannel, strNick, QString::null, strSuffix);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 PART #scc
void OnetKernel::raw_part()
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

    QSettings settings;
    QString strDisplay;

    if (strChannel[0] != '^')
    {
        if (strReason.isEmpty() == false)
        {
            if (settings.value("show_zuo").toString() == "on")
                strDisplay = QString(tr("* %1 [%2@%3] has left %4 [%5]")).arg(strNick).arg(strZUO).arg(strIP).arg(strChannel).arg(strReason);
            else
                strDisplay = QString(tr("* %1 [%2] has left %3 [%4]")).arg(strNick).arg(strIP).arg(strChannel).arg(strReason);
        }
        else
        {
            if (settings.value("show_zuo").toString() == "on")
                strDisplay = QString(tr("* %1 [%2@%3] has left %4")).arg(strNick).arg(strZUO).arg(strIP).arg(strChannel);
            else
                strDisplay = QString(tr("* %1 [%2] has left %3")).arg(strNick).arg(strIP).arg(strChannel);
        }
    }
    else
    {
        if (settings.value("show_zuo").toString() == "on")
            strDisplay = QString(tr("* %1 [%2@%3] has left priv")).arg(strNick).arg(strZUO).arg(strIP);
        else
            strDisplay = QString(tr("* %1 [%2] has left priv")).arg(strNick).arg(strIP);
    }

    pTabC->show_msg(strChannel, strDisplay, 2);

    emit del_user(strChannel, strNick);

    // remove nick avatar if not exist on any channel
    if ((Core::instance()->mNickAvatar.contains(strNick) == true) && (pTabC->get_nick_channels(strNick) == 0))
        Core::instance()->mNickAvatar.remove(strNick);

    // if self part

    QString strMe = settings.value("nick").toString();

    if (strNick == strMe)
    {
        // remove nick avatars
        emit clear_channel_all_nick_avatars(strChannel);

        // close channel
        if (strChannel != "Status")
            pTabC->remove_tab(strChannel);
    }
}

// :Stark!38566204@A5F2F1.68FE5E.DE32AF.62ECB9 QUIT :Client exited
void OnetKernel::raw_quit()
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

    QSettings settings;
    QString strDisplay;
    if (settings.value("show_zuo").toString() == "on")
        strDisplay = QString(tr("* %1 [%2@%3] has quit [%4]")).arg(strNick).arg(strZUO).arg(strIP).arg(strReason);
    else
        strDisplay = QString(tr("* %1 [%2] has quit [%3]")).arg(strNick).arg(strIP).arg(strReason);

    // remove nick from avatars
    if (Core::instance()->mNickAvatar.contains(strNick) == true)
        Core::instance()->mNickAvatar.remove(strNick);

    emit quit_user(strNick, strDisplay);
}

// :scc_test!51976824@3DE379.B7103A.6CF799.6902F4 KICK #scc Moment_w_atmosferze :sio
void OnetKernel::raw_kick()
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
    strDisplay = QString(tr("* %1 has been kicked from channel %2 by %3 Reason: %4")).arg(strNick).arg(strChannel).arg(strWho).arg(strReason);

    pTabC->show_msg(strChannel, strDisplay, 4);

    emit del_user(strChannel, strNick);

    // remove nick from avatars if not exist on open channels
    if ((Core::instance()->mNickAvatar.contains(strNick) == true) && (pTabC->get_nick_channels(strNick) == 0))
        Core::instance()->mNickAvatar.remove(strNick);

    QSettings settings;
    QString strMe = settings.value("nick").toString();

    if (strNick == strMe)
    {
        QMessageBox::information(0, "", QString(tr("You have been kicked from %1 by %2")+"<br>"+tr("Reason: %3")).arg(strWho).arg(strChannel).arg(strReason));

        pTabC->remove_tab(strChannel);
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
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strNickChannel = strDataList[2];

    if (strNickChannel[0] == '#')
    {
        QString strFlag = strDataList[3];
        QString strDisplay;
        int index = 4;
        QString plusminus = QString::null;
        QMultiHash<QString, QString> flag_nick;

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
                if (index <= strDataList.size()) // prevent crash!
                {
                    QString strFlag = plusminus+c;
                    // flag ntipsmuGQVL do not need value
                    if ((c != "n") && (c != "t") && (c != "i") && (c != "p") && (c != "s") && (c != "m") && (c != "u") && (c != "G") && (c != "Q") && (c != "V") && (c != "L"))
                    {
                        if (index >= strDataList.size())
                            flag_nick.insert(strFlag, QString::null);
                        else
                        {
                            flag_nick.insert(strFlag, strDataList[index]);
                            index++;
                        }
                    }
                    else
                        flag_nick.insert(strFlag, QString::null);
                }
            }
        }

        QMultiHash<QString, QString>::const_iterator i = flag_nick.constBegin();
        while (i != flag_nick.constEnd())
        {
            QString strFlag = i.key();
            QString strNick = i.value();

            if (strFlag == "+q") strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-q") strDisplay = QString(tr("* %1 is no longer the owner of the channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+o") strDisplay = QString(tr("* %1 is now super-operator on the channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-o") strDisplay = QString(tr("* %1 is no longer super-operator on the channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+h") strDisplay = QString(tr("* %1 is now the operator of the channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-h") strDisplay = QString(tr("* %1 is no longer the operator of the channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+v") strDisplay = QString(tr("* %1 is now a guest of channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-v") strDisplay = QString(tr("* %1 is no longer a guest of channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+X") strDisplay = QString(tr("* %1 is now moderator of the channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-X") strDisplay = QString(tr("* %1 is no longer moderating channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+Y") strDisplay = QString(tr("* %1 is now screener channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-Y") strDisplay = QString(tr("* %1 is no longer a screener channel %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+b") strDisplay = QString(tr("* %1 is now on the banned list in %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-b") strDisplay = QString(tr("* %1 is no longer on the banned list in %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+I") strDisplay = QString(tr("* %1 is now on the list of invitees in %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-I") strDisplay = QString(tr("* %1 is no longer on the list of invitees in %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+e") strDisplay = QString(tr("* %1 now has ban exception flag in %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-e") strDisplay = QString(tr("* %1 no longer has a ban exception flag in %2 (set by %3)")).arg(strNick).arg(strNickChannel).arg(strWho);

            else if (strFlag == "+k") strDisplay = QString(tr("* Channel %1 now has key set to %2 (set by %3)")).arg(strNickChannel).arg(strNick).arg(strWho);
            else if (strFlag == "-k") strDisplay = QString(tr("* Channel %1 no longer has key set (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+n") strDisplay = QString(tr("* Channel %1 will now have no external messages sent to the channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-n") strDisplay = QString(tr("* Channel %1 will now allow external messages sent to the channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+t") strDisplay = QString(tr("* Only channel operators can now change the topic in %1 channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-t") strDisplay = QString(tr("* Anyone can now change the topic in %1 channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+i") strDisplay = QString(tr("* Channel %1 is now a hidden channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-i") strDisplay = QString(tr("* Channel %1 is no longer hidden channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+p") strDisplay = QString(tr("* Channel %1 is now a private channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-p") strDisplay = QString(tr("* Channel %1 is no longer a private channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+s") strDisplay = QString(tr("* Channel %1 is now a secret channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-s") strDisplay = QString(tr("* Channel %1 is no longer a secret channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+m") strDisplay = QString(tr("* Channel %1 is now moderated channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-m") strDisplay = QString(tr("* Channel %1 is no longer moderated channel (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+u") strDisplay = QString(tr("* Channel %1 now has enabled auditorium mode (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-u") strDisplay = QString(tr("* Channel %1 no longer has enabled auditorium mode (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+l") strDisplay = QString(tr("* Channel %1 now has max number of users set to %2 (set by %3)")).arg(strNickChannel).arg(strNick).arg(strWho);
            else if (strFlag == "-l") strDisplay = QString(tr("* Channel %1 no longer has max number of users set (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+G") strDisplay = QString(tr("* Channel %1 now has word censoring enabled (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-G") strDisplay = QString(tr("* Channel %1 no longer has word censoring enabled (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+Q") strDisplay = QString(tr("* Channel %1 now has blocked kick (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-Q") strDisplay = QString(tr("* Channel %1 no longer has blocked kick (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+V") strDisplay = QString(tr("* Channel %1 now has blocked invites for 1 hour (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "-V") strDisplay = QString(tr("* Channel %1 no longer has blocked invites (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+L") strDisplay = QString(tr("* Channel %1 now has limit redirection to %2 channel (set by %3)")).arg(strNickChannel).arg(strNick).arg(strWho);
            else if (strFlag == "-L") strDisplay = QString(tr("* Channel %1 no longer has limit redirection (set by %2)")).arg(strNickChannel).arg(strWho);
            else if (strFlag == "+J") strDisplay = QString(tr("* Channel %1 now prevents users from automatically rejoining the channel when they are kicked. Limit is set to %2 sec. (set by %3)")).arg(strNickChannel).arg(strNick).arg(strWho);
            else if (strFlag == "-J") strDisplay = QString(tr("* Channel %1 no longer prevents users from automatically rejoining the channel when they are kicked (set by %2)")).arg(strNickChannel).arg(strWho);

            else if (strFlag.at(1) == QChar('F'))
            {
                QString strStatus;
                if (strNick == "1") strStatus = tr("Tame");
                else if (strNick == "2") strStatus = tr("With class");
                else if (strNick == "3") strStatus = tr("Cult");
                else strStatus = "unknown";

                if (strFlag.at(0) == QChar('+'))
                    strDisplay = QString(tr("* Channel %1 now has status %2 (set by %3)")).arg(strNickChannel).arg(strStatus).arg(strWho);
                else if (strFlag.at(0) == QChar('-'))
                    strDisplay = QString(tr("* Channel %1 no longer has status %2 (set by %3)")).arg(strNickChannel).arg(strStatus).arg(strWho);
            }
            else if (strFlag.at(1) == QChar('c'))
            {
                QString strCategory;
                if (strNick == "1") strCategory = tr("Teen");
                else if (strNick == "2") strCategory = tr("Common");
                else if (strNick == "3") strCategory = tr("Erotic");
                else if (strNick == "4") strCategory = tr("Thematic");
                else if (strNick == "5") strCategory = tr("Regional");
                else strCategory = "unknown";

                if (strFlag.at(0) == QChar('+'))
                    strDisplay = QString(tr("* Channel %1 now belongs to a category %2 (set by %3)")).arg(strNickChannel).arg(strCategory).arg(strWho);
                else if (strFlag.at(0) == QChar('-'))
                    strDisplay = QString(tr("* Channel %1 is no longer category %2 (set by %3)")).arg(strNickChannel).arg(strCategory).arg(strWho);
            }

            else
            {
                if (strNick.isEmpty() == true)
                    strDisplay = QString(tr("* Channel %1 now has a flag %2 (set by %3)")).arg(strNickChannel).arg(strFlag).arg(strWho);
                else
                    strDisplay = QString(tr("* %1 now has a flag %2 (set by %3)")).arg(strNick).arg(strFlag).arg(strWho);
            }

            pTabC->show_msg(strNickChannel, strDisplay, 5);
            emit change_flag(strNick, strNickChannel, strFlag);

            ++i;
        }
        flag_nick.clear();
    }
    // nick
    else
    {
        // get args
        QString strFlag = strDataList[3];
        if (strFlag[0] == ':') strFlag = strFlag.right(strFlag.length()-1);

        // get +-
        QString plusminus = strFlag.at(0);
        // fix flag
        strFlag = strFlag.right(strFlag.length()-1);

        // create flags list
        QStringList strlFlags;
        for (int i = 0; i < strFlag.size(); i++)
            strlFlags << strFlag.at(i);

        foreach (strFlag, strlFlags)
        {
            strFlag = plusminus+strFlag;

            QString strDisplay;
            if (strFlag == "+O") strDisplay = QString(tr("* %1 is marked as NetAdmin")).arg(strNickChannel);
            else if (strFlag == "-O") strDisplay = QString(tr("* %1 is no longer marked as NetAdmin")).arg(strNickChannel);
            else if (strFlag == "+b") strDisplay = QString(tr("* %1 is marked as busy")).arg(strNickChannel);
            else if (strFlag == "-b") strDisplay = QString(tr("* %1 is no longer marked as busy")).arg(strNickChannel);
            else if (strFlag == "+W") strDisplay = QString(tr("* %1 has now enabled public webcam")).arg(strNickChannel);
            else if (strFlag == "-W") strDisplay = QString(tr("* %1 has no longer enabled public webcam")).arg(strNickChannel);
            else if (strFlag == "+V") strDisplay = QString(tr("* %1 has now enabled private webcam")).arg(strNickChannel);
            else if (strFlag == "-V") strDisplay = QString(tr("* %1 has no longer enabled private webcam")).arg(strNickChannel);
            else if (strFlag == "+x") strDisplay = QString(tr("* %1 has now encrypted IP")).arg(strNickChannel);
            else if (strFlag == "-x") strDisplay = QString(tr("* %1 has no longer encrypted IP")).arg(strNickChannel);
            else if (strFlag == "+r") strDisplay = QString(tr("* %1 is marked as registered and identified")).arg(strNickChannel);
            else if (strFlag == "-r") strDisplay = QString(tr("* %1 is no longer marked as registered and identified")).arg(strNickChannel);
            else
                strDisplay = QString(tr("* %1 now has a flag %2")).arg(strNickChannel).arg(strFlag);

            if ((strFlag == "+r") || (strFlag == "-r") || (strFlag == "+x") || (strFlag == "-x"))
                pTabC->show_msg("Status", strDisplay, 5);

            emit change_flag(strNickChannel, strFlag);

            // registered nick
            QSettings settings;
            if ((strNickChannel == settings.value("nick").toString()) && (strFlag == "+r"))
            {
                // get my stats
                pNetwork->send(QString("RS INFO %1").arg(settings.value("nick").toString()));

                // channel homes
                pNetwork->send("CS HOMES");
            }
        }
    }
}

// :Merovingian!26269559@2294E8.94913F.2EAEC9.11F26D PRIVMSG #scc :hello
void OnetKernel::raw_privmsg()
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
        pTabC->show_msg(strChannel, strDisplay, 0);
    else
        pTabC->show_msg(strNick, strDisplay, 0);
}

// :cf1f2.onet NOTICE scc_test :Your message has been filtered and opers notified: spam #2480
// :Llanero!43347263@admin.onet NOTICE $* :458852 * * :%Fb%%C008100%Weź udział w Konkursie Mikołajkowym - skompletuj zaprzęg Świetego Mikołaja! Więcej info w Wieściach z Czata ! http://czat.onet.pl/1632594,wiesci.html
void OnetKernel::raw_notice()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strNick = strDataList[2];
    QString strMessage;

    // special notice
    if ((strDataList.value(4).isEmpty() == false) && (strDataList[4] == "*") && (strDataList.value(5).isEmpty() == false) && (strDataList[5] == "*"))
    {
        for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strMessage += " "; strMessage += strDataList[i]; }
        if (strMessage[0] == ':')
            strMessage = strMessage.right(strMessage.length()-1);
    }
    // standard notice
    else
    {
        for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
        if (strMessage[0] == ':')
            strMessage = strMessage.right(strMessage.length()-1);
    }

    // display
    QString strDisplay = QString("-%1- %2").arg(strWho).arg(strMessage);
    if (strNick[0] == '#')
        pTabC->show_msg(strNick, strDisplay, 6);
    else
        pTabC->show_msg_active(strDisplay, 6);
}

// :osa1987!47751777@F4C727.DA810F.7E1789.E71ED5 INVITE scc_test :^cf1f41437962
// :Merovingian!26269559@jest.piekny.i.uroczy.ma.przesliczne.oczy INVITE scc_test :#Komputery
void OnetKernel::raw_invite()
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

    Notify::instance()->play(Query);

    (new DlgInvite(Core::instance()->sccWindow(), pNetwork, strWho, strWhere))->show(); // should be show - prevent hangup!
}

// :cf1f3.onet TOPIC #scc :Simple Chat Client; current version: beta;
void OnetKernel::raw_topic()
{
    if (strDataList.value(2).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList[2];

    QString strTopic;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strTopic += " "; strTopic += strDataList[i]; }
    if (strTopic[0] == ':')
        strTopic = strTopic.right(strTopic.length()-1);

    QString strDisplay = QString(tr("* %1 changed the topic to: %2")).arg(strWho).arg(strTopic);

    // show msg
    pTabC->show_msg(strChannel, strDisplay, 5);

    // set topic in channel settings
    if (pDlgChannelSettings->get_channel() == strChannel)
        pDlgChannelSettings->set_topic(strTopic);

    // set topic in widget
    pTabC->set_topic(strChannel, strTopic);

    // get info
    pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
}

// :~test34534!anonymous@2294E8.94913F.A00186.1A3C28 INVREJECT Merovingian #Scrabble
// :Merovingian!26269559@2294E8.94913F.2E3993.4AF50D INVREJECT scc_test ^cf1f41038619
void OnetKernel::raw_invreject()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList[3];

    QString strDisplay;
    if (strChannel[0] == '^')
        strDisplay = QString(tr("* %1 rejected an invitation to priv")).arg(strWho);
    else
        strDisplay = QString(tr("* %1 rejected an invitation to channel %2")).arg(strWho).arg(strChannel);

    pTabC->show_msg(strChannel, strDisplay, 7);
}

// :~test34534!anonymous@2294E8.94913F.A00186.1A3C28 INVIGNORE Merovingian #Scrabble
// :Merovingian!26269559@2294E8.94913F.A00186.4A2B76 INVIGNORE scc_test ^cf1f31294352
void OnetKernel::raw_invignore()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strWho = strDataList[0];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    strWho = strWho.left(strWho.indexOf('!'));

    QString strChannel = strDataList[3];

    QString strDisplay;
    if (strChannel[0] == '^')
        strDisplay = QString(tr("* %1 ignored your invitation to the priv")).arg(strWho);
    else
        strDisplay = QString(tr("* %1 ignored your invitation to the channel %2")).arg(strWho).arg(strChannel);

    // display
    pTabC->show_msg(strChannel, strDisplay, 7);

    // rename
    pTabC->rename_tab(strChannel, strWho);
}

// :~testa!anonymous@3DE379.B7103A.6CF799.6902F4 MODERMSG test1 - #Scrabble :%F:verdana%%Ihehe%
void OnetKernel::raw_modermsg()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strModerator = strDataList[0];
    if (strModerator[0] == ':')
        strModerator = strModerator.right(strModerator.length()-1);
    strModerator = strModerator.left(strModerator.indexOf('!'));

    QString strNick = strDataList[2];
    QString strChannel = strDataList[4];

    QString strMessage;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = QString("<%1> %2 [%3 %4]").arg(strNick).arg(strMessage).arg(tr("Moderated by")).arg(strModerator);

    // display
    pTabC->show_msg(strChannel, strDisplay, 0);
}

// :~testa!anonymous@3DE379.B7103A.6CF799.6902F4 MODERNOTICE #Scrabble :a
void OnetKernel::raw_modernotice()
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

    // display
    pTabC->show_msg(strChannel, strDisplay, 6);
}

// :cf1f1.onet MODERATE ~testa opnick #channel cf1f44c3b4b870f8a :%F:verdana%ladnie to tak
void OnetKernel::raw_moderate()
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

    pDlgModeration->add_msg(strID, strChannel, strNick, strMessage);
}

// :cf1f4.onet KILL scc_test :cf1f4.onet (Killed (Nickname collision))
// :Darom!12265854@devel.onet KILL Merovingian :cf1f4.onet!devel.onet!Darom (Killed (Darom (bo tak)))
void OnetKernel::raw_kill()
{
    if (strDataList.value(2).isEmpty() == true) return;
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[2];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    QString strWho = strDataList[3];
    if (strWho[0] == ':')
        strWho = strWho.right(strWho.length()-1);
    QRegExp rx("(.*)!(.*)!(.*)");
    if (rx.exactMatch(strWho))
        strWho = strWho.replace(QRegExp("(.*)!(.*)!(.*)"), "\\3");

    QString strReason;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strReason += " "; strReason += strDataList[i]; }

    QString strDisplay = QString(tr("* You were killed by %1 %2")).arg(strWho).arg(strReason);

    // display
    pTabC->show_msg_all(strDisplay, 9);
}

// :cf1f4.onet 001 scc_test :Welcome to the OnetCzat IRC Network scc_test!51976824@83.28.35.219
void OnetKernel::raw_001()
{
    QSettings settings;

    // logged
    settings.setValue("logged", "on");

    // clear
    Core::instance()->mFriends.clear();
    Core::instance()->lIgnore.clear();
    Core::instance()->lChannelFavourites.clear();
    Core::instance()->stlChannelList.clear();
    Core::instance()->mMyStats.clear();
    Core::instance()->mMyProfile.clear();
    Core::instance()->lChannelHomes.clear();

    // protocol
    pNetwork->send("PROTOCTL ONETNAMESX");

    // busy
    settings.setValue("busy", "off");

    // away
    settings.setValue("away", "off");

    // auto busy
    if (settings.value("auto_busy").toString() == "on")
        pNetwork->send("BUSY 1");

    // ignore_raw_141
    settings.setValue("ignore_raw_141", "off");

    // override off
    settings.setValue("override", "off");

    // age check on
    settings.setValue("age_check", "on");

    // auto rejoin
    QStringList strlOpenChannels = pTabC->get_open_channels();
    for (int i = 0; i < strlOpenChannels.size(); i++)
    {
        pTabC->remove_tab(strlOpenChannels[i]);
        pNetwork->send(QString("JOIN %1").arg(strlOpenChannels[i]));
    }

    // channel list
    pNetwork->send("SLIST  R- 0 0 100 null");
}

// :cf1f4.onet 002 Merovingian :Your host is cf1f4.onet, running version InspIRCd-1.1
void OnetKernel::raw_002()
{
// ignore
}

// :cf1f4.onet 003 Merovingian :This server was created 06:35:35 Jan 12 2010
void OnetKernel::raw_003()
{
// ignore
}

// :cf1f4.onet 004 Merovingian cf1f4.onet InspIRCd-1.1 BGQRVWbinoqrswx DFGIJLMPQRVXYabcehiklmnopqrstuv FIJLXYabcehkloqv
void OnetKernel::raw_004()
{
// ignore
}

// :cf1f4.onet 005 Merovingian WALLCHOPS WALLVOICES MODES=19 CHANTYPES=^# PREFIX=(qaohXYv)`&@%!=+ MAP MAXCHANNELS=20 MAXBANS=60 VBANLIST NICKLEN=32 CASEMAPPING=rfc1459 STATUSMSG=@%+ CHARSET=ascii :are supported by this server
// :cf1f4.onet 005 Merovingian TOPICLEN=203 KICKLEN=255 MAXTARGETS=20 AWAYLEN=200 CHANMODES=Ibe,k,FJLcl,DGMPQRVimnprstu FNC NETWORK=OnetCzat MAXPARA=32 ELIST=MU OVERRIDE ONETNAMESX INVEX=I EXCEPTS=e :are supported by this server
// :cf1f4.onet 005 Merovingian WATCH=200 INVIGNORE=100 USERIP ESILENCE SILENCE=100 NAMESX :are supported by this server
void OnetKernel::raw_005()
{
    QMap<QString,QString> mKeyValue;

    for (int i = 0; i < strDataList.size(); i++)
    {
        if (strDataList.at(i).indexOf("=") != -1)
        {
            QString strData = strDataList.at(i);
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
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strTime = strDataList[5];
    QDateTime dt = QDateTime::fromTime_t(strTime.toInt());
    QString strDateTime = dt.toString("hh:mm");

    int iTime = strTime.toInt();
    QDateTime dta = QDateTime::currentDateTime();
    int iCurrentTime = (int)dta.toTime_t();

    QString strMessage;
    for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay;
    if (iTime < iCurrentTime)
        strDisplay = QString(tr("* %1 In progress on channel %2")).arg(strMessage).arg(strChannel);
    else
        strDisplay = QString(tr("* %1 Starting at %2 on channel %3")).arg(strMessage).arg(strDateTime).arg(strChannel);

    pTabC->show_msg_active(strDisplay, 6);
}

// :GuardServ!service@service.onet NOTICE scc_test :109 #scc :rzucanie mięsem nie będzie tolerowane
void OnetKernel::raw_109n()
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

    // display
    pTabC->show_msg(strChannel, strMessage, 6);
}

// NS INFO aleksa7
// :NickServ!service@service.onet NOTICE Merovingian :111 aleksa7 type :2
void OnetKernel::raw_111n()
{
    QString strNick = strDataList[4];
    QString strKey = strDataList[5];

    QString strValue;
    for (int i = 6; i < strDataList.size(); i++) { if (i != 6) strValue += " "; strValue += strDataList[i]; }
    if (strValue[0] == ':')
        strValue = strValue.right(strValue.length()-1);

    QSettings settings;
    QString strMe = settings.value("nick").toString();

    // avatar
    QString strAvatarLink;
    if (strKey == "avatar")
        strAvatarLink = strValue;

    // set user info
    if (pDlgUserProfile->get_nick() == strNick)
        pDlgUserProfile->set_user_info(strKey, strValue);

    // set my profile
    if (strNick == strMe)
    {
        if (Core::instance()->mMyProfile.contains(strKey) == true)
            Core::instance()->mMyProfile[strKey] = strValue;
        else
            Core::instance()->mMyProfile.insert(strKey, strValue);
    }

    // get avatar
    if (strAvatarLink.isEmpty() == false)
        avatar->get_avatar(strNick, "nick", strAvatarLink);
}

// NS INFO aleksa7
// :NickServ!service@service.onet NOTICE Merovingian :112 aleksa7 :end of user info
void OnetKernel::raw_112n()
{
// ignore
}

// NS FRIENDS
// :NickServ!service@service.onet NOTICE scc_test :121 :scc_test Merovingian Succubi Radowsky
void OnetKernel::raw_121n()
{
    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strNick = strDataList[i];
        if (strNick[0] == ':')
            strNick = strNick.right(strNick.length()-1);

        // nothing
    }
}

// NS FRIENDS
// :NickServ!service@service.onet NOTICE scc_test :122 :end of friend list
void OnetKernel::raw_122n()
{
// ignore
}

// :NickServ!service@service.onet NOTICE Merovingian :123 mokka00 Viola_de_luxe :friend nick changed
void OnetKernel::raw_123n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strOldNick = strDataList[4];
    if (strOldNick[0] == ':')
        strOldNick = strOldNick.right(strOldNick.length()-1);

    QString strNewNick = strDataList[5];
    if (strNewNick[0] == ':')
        strNewNick = strNewNick.right(strNewNick.length()-1);

    QString strDisplay = QString(tr("* %1 changed nickname to %2 from your friend list")).arg(strOldNick).arg(strNewNick);
    pTabC->show_msg_active(strDisplay, 7);
}

// NS IGNORE
// :NickServ!service@service.onet NOTICE scc_test :131 :arabeska22 test wilk ~test
void OnetKernel::raw_131n()
{
    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strNick = strDataList[i];
        if (strNick[0] == ':')
            strNick = strNick.right(strNick.length()-1);

        if (Core::instance()->lIgnore.contains(strNick) == false)
            Core::instance()->lIgnore.append(strNick);
    }
}

// NS IGNORE
// :NickServ!service@service.onet NOTICE scc_test :132 :end of ignore list
void OnetKernel::raw_132n()
{
// ignore
}

// :NickServ!service@service.onet NOTICE Merovingian :133 test_nick test_nick_nowy :ignored nick changed
void OnetKernel::raw_133n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strOldNick = strDataList[4];
    if (strOldNick[0] == ':')
        strOldNick = strOldNick.right(strOldNick.length()-1);

    QString strNewNick = strDataList[5];
    if (strNewNick[0] == ':')
        strNewNick = strNewNick.right(strNewNick.length()-1);

    QString strDisplay = QString(tr("* %1 changed nickname to %2 from your ignored list")).arg(strOldNick).arg(strNewNick);
    pTabC->show_msg_active(strDisplay, 7);
}

// NS FAVOURITES
// :NickServ!service@service.onet NOTICE scc_test :141 :#Scrabble #Quiz #scc
void OnetKernel::raw_141n()
{
    QSettings settings;

    for (int i = 4; i < strDataList.size(); i++)
    {
        QString strChannel = strDataList[i];
        if (strChannel[0] == ':')
            strChannel = strChannel.right(strChannel.length()-1);

        if (Core::instance()->lChannelFavourites.contains(strChannel) == false)
            Core::instance()->lChannelFavourites.append(strChannel);

        if ((settings.value("ignore_raw_141").toString() == "off") && (settings.value("disable_autojoin_favourites").toString() == "off"))
            pNetwork->send(QString("JOIN %1").arg(strChannel));
    }

    // turn on ignore_raw_141
    if (settings.value("ignore_raw_141").toString() == "off")
        settings.setValue("ignore_raw_141", "on");
}

// NS FAVOURITES
// :NickServ!service@service.onet NOTICE scc_test :142 :end of favourites list
void OnetKernel::raw_142n()
{
// ignore
}

// NS OFFLINE
// :NickServ!service@service.onet NOTICE Merovingian :151 :jubee_blue
// CS HOMES
// :ChanServ!service@service.onet NOTICE scc_test :151 :h#scc
void OnetKernel::raw_151n()
{
    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        for (int i = 4; i < strDataList.size(); i++)
        {
            QString strChannel = strDataList[i];
            if (strChannel[0] == ':')
                strChannel = strChannel.right(strChannel.length()-1);

            if (Core::instance()->lChannelHomes.contains(strChannel) == false)
                Core::instance()->lChannelHomes.append(strChannel);
        }
    }
    else if (strNick.toLower() == "nickserv")
    {
        // TODO
    }
}

// NS OFFLINE
// :NickServ!service@service.onet NOTICE Merovingian :152 :end of offline senders list
// CS HOMES
// :ChanServ!service@service.onet NOTICE scc_test :152 :end of homes list
void OnetKernel::raw_152n()
{
// ignore
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE scc_test :160 #scc :Simple Chat Client;
void OnetKernel::raw_160n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strTopic;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strTopic += " "; strTopic += strDataList[i]; }
    if (strTopic[0] == ':')
        strTopic = strTopic.right(strTopic.length()-1);

    // set topic in channel settings
    if (pDlgChannelSettings->get_channel() == strChannel)
        pDlgChannelSettings->set_topic(strTopic);

    // set topic in widget
    pTabC->set_topic(strChannel, strTopic);
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE scc_test :161 #scc :topicAuthor=Merovingian rank=0.9095 topicDate=1251579281 private=1 password= limit=0 type=0 createdDate=1247005186 vEmail=0 www=http://simplechatclien.sourceforge.net/ catMajor=4 catMinor=0 official=0 recommended=0 protected=0 moderated=0 avatar=http://foto0.m.onet.pl/_m/e7bd33787bb5cd96031db4034e5f1d54,1,19,0.jpg status=ok guardian=3 auditorium=0
// :ChanServ!service@service.onet NOTICE Merovingian :161 #scc :topicAuthor=Merovingian rank=1.7068 topicDate=1297944969 private=0 type=1 createdDate=1247005186 vEmail=1 www=http://simplechatclien.sourceforge.net/ catMajor=4 moderated=0 avatar=http://foto3.m.onet.pl/_m/97198666362c2c72c6311640f9e791cb,1,19,0-5-53-53-0.jpg guardian=3 email=merovirgian@gmail.com auditorium=0
void OnetKernel::raw_161n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QMap <QString, QString> mKeyValue;

    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList[i];
        if (i == 5) strLine = strLine.right(strLine.length()-1);
        QString strKey = strLine.left(strLine.indexOf("="));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf("=")-1);

        mKeyValue.insert(strKey, strValue);
    }

    // set data
    if (pDlgChannelSettings->get_channel() == strChannel)
        pDlgChannelSettings->set_data(mKeyValue);

    // update link
    QString strLink = mKeyValue.value("www");
    if (strLink.isEmpty() == false)
        pTabC->set_link(strChannel, strLink);

    // update topic author
    QString strTopicAuthor = mKeyValue.value("topicAuthor");
    QString strTopicDate = mKeyValue.value("topicDate");
    if (strTopicAuthor.isEmpty() == false)
    {
        QDateTime dt = QDateTime::fromTime_t(strTopicDate.toInt());
        QString strDT = dt.toString("dd/MM/yy hh:mm:ss");
        QString strTopicDetails = QString("%1 (%2)").arg(strTopicAuthor).arg(strDT);
        pTabC->author_topic(strChannel, strTopicDetails);
    }

    // avatar
    QString strAvatarUrl = mKeyValue.value("avatar");
    if (strAvatarUrl.isEmpty() == false)
    {
        QSettings settings;
        if (settings.value("style").toString() == "modern")
            avatar->get_avatar(strChannel, "channel", strAvatarUrl);
    }
}

// CS INFO #lunar
// :ChanServ!service@service.onet NOTICE scc_test :162 #lunar :q,Merovingian o,Radowsky o,aleksa7 o,chanky o,osa1987 h,scc_test o,MajkeI
void OnetKernel::raw_162n()
{
    QString strChannel = strDataList[4];

    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList[i];
        if (i == 5) strLine = strLine.right(strLine.length()-1);
        QString strKey = strLine.left(strLine.indexOf(","));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf(",")-1);

        if ((strKey.isEmpty() == false) && (strValue.isEmpty() == false))
        {
            if (pDlgChannelSettings->get_channel() == strChannel)
            {
                if (strKey == "q")
                    pDlgChannelSettings->set_owner(strValue);
                else if (strKey == "o")
                    pDlgChannelSettings->add_op(strValue);
                else if (strKey == "h")
                    pDlgChannelSettings->add_halfop(strValue);
            }
        }
    }
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE Merovingian :163 #scc I Olka Merovingian 1289498809 :
// :ChanServ!service@service.onet NOTICE Merovingian :163 #scc b test!*@* Merovingian 1289498776 :
// :ChanServ!service@service.onet NOTICE Merovingian :163 #scc b *!*@haxgu3xx7ptcn4u72yrkbp4daq Merovingian 1289497781 :Tony_Montana
void OnetKernel::raw_163n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;
    if (strDataList.value(7).isEmpty() == true) return;
    if (strDataList.value(8).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strFlag = strDataList[5];
    QString strNick = strDataList[6];
    QString strWho = strDataList[7];
    QString strDT = strDataList[8];
    QString strIPNick = strDataList[9];
    if (strIPNick[0] == ':')
        strIPNick = strIPNick.right(strIPNick.length()-1);

    QDateTime dt = QDateTime::fromTime_t(strDT.toInt());
    strDT = dt.toString("dd/MM/yyyy hh:mm:ss");

    if (pDlgChannelSettings->get_channel() == strChannel)
    {
        if (strFlag == "b")
            pDlgChannelSettings->add_ban(strNick, strWho, strDT, strIPNick);
        else if (strFlag == "I")
            pDlgChannelSettings->add_invite(strNick, strWho, strDT);
    }
}

// CS INFO #scc
// :ChanServ!service@service.onet NOTICE scc_test :164 #scc :end of channel info
void OnetKernel::raw_164n()
{
// ignore
}

// CS INFO #Relax
// :ChanServ!service@service.onet NOTICE ~test :165 #Relax :Nie ważne, czy szukasz dobrej zabawy, ...
void OnetKernel::raw_165n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDescription;
    for (int i = 5; i < strDataList.size(); i++) { if (i != 5) strDescription += " "; strDescription += strDataList[i]; }
    if (strDescription[0] == ':')
        strDescription = strDescription.right(strDescription.length()-1);

    if (pDlgChannelSettings->get_channel() == strChannel)
        pDlgChannelSettings->set_description(strDescription);
}

// RS INFO Merovingian
// :RankServ!service@service.onet NOTICE Merovingian :170 Merovingian :histActive=edgbcebbdccecbdbbccbcdcdccbabb histTotal=ijqkhhlfihiqlnqjlmmllomkohqfji idleTime=14020283 noise=101660 relationsFriend=91 relationsIgnored=0 sessionsTime=19023384 words=361679
void OnetKernel::raw_170n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QSettings settings;
    if (strNick != settings.value("nick").toString()) return; // not my nick

    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList[i];
        if (i == 5) strLine = strLine.right(strLine.length()-1);
        QString strKey = strLine.left(strLine.indexOf("="));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf("=")-1);

        Core::instance()->mMyStats.insert(strKey, strValue);
    }
}

// RS INFO Merovingian
// :RankServ!service@service.onet NOTICE Merovingian :171 Merovingian :end of user stats
void OnetKernel::raw_171n()
{
// ignore
}

// RS INFO #scc
// :RankServ!service@service.onet NOTICE Merovingian :175 #scc :histActiveTime=1d9a,6a4,b1b,12c3,157b,3c5a,981,8e9d,5b14,4ea4,1970,198c,2cbd,3505,5500,dc8a,a263,5635,3ab5,232a,2bad,2f51,359f,3b2a,100f,4a17,1c32,15c1,4290,2b06 histNoise=ba,14,4a,82,71,147,3b,3df,301,1d7,e5,99,109,f2,118,69f,432,21f,16d,c0,f0,110,110,144,51,18a,fb,75,1d9,e9 histRelationsFavourite=28,28,29,29,29,29,29,29,29,29,29,29,29,28,28,28,28,28,28,27,28,2a,2b,2a,2a,2b,2b,2b,2c,2c
// :RankServ!service@service.onet NOTICE Merovingian :175 #scc :histSessionsTime=bf9e4,40439,b7be0,66e21,6273f,6e8ff,8af6e,7fdad,7a6ad,766a3,621d0,728be,718cc,83f03,b5c1f,9ae59,96d4e,82724,7c192,8f166,8fef5,6f35d,9384a,87f97,7e031,a3e97,64f00,c2a84,ad3b4,8999b histWebcamTime=aa4,162a,f4a,0,3afa,2c,edc,44a,fe7,1d9,fb0,3e1,1531,33d5,15dc,b5c,2d6d,5c0d,0,48f2,1f85,2111,7a2,2251,25e,fea,1ecb,1445,143c,2280
// :RankServ!service@service.onet NOTICE Merovingian :175 #scc :histWords=247,4d,aa,1a3,130,3f3,a2,dfd,a21,5e8,245,185,379,2f0,41e,161d,1194,8a4,454,2ec,35f,30d,424,493,bc,426,33d,f7,705,307 noise=80619 relationsFavourite=40 visits=78 words=268782
void OnetKernel::raw_175n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QMap <QString, QString> mKeyValue;
    for (int i = 5; i < strDataList.size(); i++)
    {
        QString strLine = strDataList[i];
        if (i == 5) strLine = strLine.right(strLine.length()-1);
        QString strKey = strLine.left(strLine.indexOf("="));
        QString strValue = strLine.right(strLine.length() - strLine.indexOf("=")-1);

        mKeyValue.insert(strKey, strValue);
    }

    if (pDlgChannelSettings->get_channel() == strChannel)
        pDlgChannelSettings->set_stats_data(mKeyValue);
}

// :RankServ!service@service.onet NOTICE Merovingian :176 #scc :end of channel stats
void OnetKernel::raw_176n()
{
// ignore
}

// NS SET city
// :NickServ!service@service.onet NOTICE Merovingian :210 :nothing changed
void OnetKernel::raw_210n()
{
// ignore
}

// NS SET city
// :NickServ!service@service.onet NOTICE scc_test :211 city :value unset
void OnetKernel::raw_211n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[2];
    QString strKey = strDataList[4];

    QSettings settings;
    QString strMe = settings.value("nick").toString();

    // set my profile
    if (strNick == strMe)
    {
        // my profile
        if (Core::instance()->mMyProfile.contains(strKey) == true)
            Core::instance()->mMyProfile[strKey] = "";
        else
            Core::instance()->mMyProfile.insert(strKey, "");

        // my avatar
        if (strKey == "avatar")
        {
            if (Core::instance()->mNickAvatar.contains(strMe) == true)
                Core::instance()->mNickAvatar.remove(strMe);
        }
    }


}

// NS FRIENDS ADD aaa
// :NickServ!service@service.onet NOTICE scc_test :220 aaa :friend added to list
void OnetKernel::raw_220n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString(tr("* Added the nickname %1 to a friends list")).arg(strNick);
    pTabC->show_msg_active(strDisplay, 7);
}

// NS FRIENDS DEL aaa
// :NickServ!service@service.onet NOTICE scc_test :221 scc_test :friend removed from list
void OnetKernel::raw_221n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString(tr("* Removed the nickname %1 from your friends list")).arg(strNick);
    pTabC->show_msg_active(strDisplay, 7);
}

// NS IGNORE ADD ~test
// :NickServ!service@service.onet NOTICE scc_test :230 ~test :ignore added to list
void OnetKernel::raw_230n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString(tr("* Added %1 to your ignore list")).arg(strNick);
    pTabC->show_msg_active(strDisplay, 7);

    if (Core::instance()->lIgnore.contains(strNick) == false)
        Core::instance()->lIgnore.append(strNick);
}

// NS IGNORE DEL aaa
// :NickServ!service@service.onet NOTICE scc_test :231 ~test :ignore removed from list
void OnetKernel::raw_231n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strDisplay = QString(tr("* Removed %1 from your ignore list")).arg(strNick);
    pTabC->show_msg_active(strDisplay, 7);

    if (Core::instance()->lIgnore.contains(strNick) == true)
        Core::instance()->lIgnore.removeOne(strNick);
}

// NS FAVOURITES ADD scc
// :NickServ!service@service.onet NOTICE scc_test :240 #scc :favourite added to list
void OnetKernel::raw_240n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDisplay = QString(tr("* Added %1 channel to your favorites list")).arg(strChannel);
    pTabC->show_msg_active(strDisplay, 7);

    if (Core::instance()->lChannelFavourites.contains(strChannel) == false)
        Core::instance()->lChannelFavourites.append(strChannel);
}

// NS FAVOURITES DEL scc
// :NickServ!service@service.onet NOTICE scc_test :241 #scc :favourite removed from list
void OnetKernel::raw_241n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDisplay = QString(tr("* Removed channel %1 from your favorites list")).arg(strChannel);
    pTabC->show_msg_active(strDisplay, 7);

    if (Core::instance()->lChannelFavourites.contains(strChannel) == true)
        Core::instance()->lChannelFavourites.removeOne(strChannel);
}

// CS REGISTER czesctoja
// :ChanServ!service@service.onet NOTICE scc_test :250 #czesctoja :channel registered
// NS OFFLINE MSG nick text
// :NickServ!service@service.onet NOTICE Merovingian :250 scc_test :offline message sent
void OnetKernel::raw_250n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        QString strChannel = strDataList[4];

        QMessageBox::information(0, "", QString(tr("Successfully created a channel %1")).arg(strChannel));

        // add to list
        if (Core::instance()->lChannelHomes.contains(strChannel) == false)
            Core::instance()->lChannelHomes.append(strChannel);

        // join
        pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
    else if (strNick.toLower() == "nickserv")
    {
        // TODO
    }
}

// LUSERS
// :cf1f4.onet 251 Merovingian :There are 2300 users and 5 invisible on 10 servers
void OnetKernel::raw_251()
{
// ignore
}

// CS DROP czesctoja
// :ChanServ!service@service.onet NOTICE scc_test :251 #czesctoja :has been dropped
// NS OFFLINE GET scc_test
// :NickServ!service@service.onet NOTICE Merovingian :251 scc_test 1291386193 msg :test message
void OnetKernel::raw_251n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        QString strChannel = strDataList[4];

        QString strDisplay = QString(tr("* Channel %1 has been removed")).arg(strChannel);
        pTabC->show_msg_active(strDisplay, 5);
    }
    else if (strNick.toLower() == "nickserv")
    {
        // TODO
    }
}

// LUSERS
// :cf1f4.onet 252 Merovingian 5 :operator(s) online
void OnetKernel::raw_252()
{
// ignore
}

// CS DROP czesctoja
// :ChanServ!service@service.onet NOTICE #testabc :252 scc_test :has dropped this channel
// NS OFFLINE REJECT scc_test
// :NickServ!service@service.onet NOTICE Merovingian :252 scc_test :offline messages rejected
void OnetKernel::raw_252n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        QString strChannel = strDataList[2];

        QString strDisplay = QString(tr("* Confirmed the removal of the channel %1")).arg(strChannel);
        pTabC->show_msg_active(strDisplay, 5);
    }
    else if (strNick.toLower() == "nickserv")
    {
        // TODO
    }
}

// LUSERS
// :cf1f4.onet 253 Merovingian 1 :unknown connections
void OnetKernel::raw_253()
{
// ignore
}

// :ChanServ!service@service.onet NOTICE scc_test :253 #test_scc_moj Merovingian :channel owner changed
void OnetKernel::raw_253n()
{
    // copy raw 254
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[2];
    QString strNick = strDataList[5];

    QString strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);

    // display
    pTabC->show_msg(strChannel, strDisplay, 5);
}

// LUSERS
// :cf1f4.onet 254 Merovingian 4641 :channels formed
void OnetKernel::raw_254()
{
// ignore
}

// CS TRANSFER #test_scc_moj Merovingian
// :ChanServ!service@service.onet NOTICE #test_scc_moj :254 scc_test Merovingian :changed channel owner
void OnetKernel::raw_254n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[2];
    QString strWho = strDataList[4];
    QString strNick = strDataList[5];

    QString strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);

    // display
    pTabC->show_msg(strChannel, strDisplay, 5);
}

// LUSERS
// :cf1f4.onet 255 Merovingian :I have 568 clients and 1 servers
void OnetKernel::raw_255()
{
// ignore
}

// CS BAN #scc ADD cos
// :ChanServ!service@service.onet NOTICE scc_test :255 #scc +b cos :channel privilege changed
void OnetKernel::raw_255n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strNick = strDataList[6];

    QString strDisplay = QString(tr("* Changing privileges confirmed for %1 at %2")).arg(strNick).arg(strChannel);
    pTabC->show_msg_active(strDisplay, 5);
}

// ADMIN
// :cf1f1.onet 256 ~test :Administrative info for cf1f1.onet
void OnetKernel::raw_256()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg("Status", strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE #scc :256 Merovingian +o scc_test :channel privilege changed
void OnetKernel::raw_256n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[2];
    QString strWho = strDataList[4];
    QString strFlag = strDataList[5];
    QString strNick = strDataList[6];
    QString strDisplay;

    if (strFlag == "+q") strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-q") strDisplay = QString(tr("* %1 is no longer the owner of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+o") strDisplay = QString(tr("* %1 is now super-operator on the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-o") strDisplay = QString(tr("* %1 is no longer super-operator on the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+h") strDisplay = QString(tr("* %1 is now the operator of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-h") strDisplay = QString(tr("* %1 is no longer the operator of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+v") strDisplay = QString(tr("* %1 is now a guest of channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-v") strDisplay = QString(tr("* %1 is no longer a guest of channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+X") strDisplay = QString(tr("* %1 is now moderator of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-X") strDisplay = QString(tr("* %1 is no longer moderating channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+Y") strDisplay = QString(tr("* %1 is now screener channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-Y") strDisplay = QString(tr("* %1 is no longer a screener channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+b") strDisplay = QString(tr("* %1 is now on the banned list in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-b") strDisplay = QString(tr("* %1 is no longer on the banned list in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+I") strDisplay = QString(tr("* %1 is now on the list of invitees in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-I") strDisplay = QString(tr("* %1 is no longer on the list of invitees in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else
        strDisplay = QString(tr("* %1 now has a flag %2 (set by %3)")).arg(strNick).arg(strFlag).arg(strWho);

    // display
    pTabC->show_msg(strChannel, strDisplay, 5);
}

// ADMIN
// :cf1f1.onet 257 ~test :Name     - Czat Admin
void OnetKernel::raw_257()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg("Status", strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :257 #scc * :settings changed
void OnetKernel::raw_257n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
}

// ADMIN
// :cf1f1.onet 258 ~test :Nickname - czat_admin
void OnetKernel::raw_258()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg("Status", strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE #glupia_nazwa :258 ovo_ d :channel settings changed
// :ChanServ!service@service.onet NOTICE #scc :258 Merovingian * :channel settings changed
void OnetKernel::raw_258n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[2];
    QString strNick = strDataList[4];

    QString strDisplay = QString(tr("* %1 changed channel %2 settings")).arg(strNick).arg(strChannel);

    // display
    pTabC->show_msg(strChannel, strDisplay, 7);

    pNetwork->send(QString("CS INFO %1 i").arg(strChannel));
}

// ADMIN
// :cf1f1.onet 259 ~test :E-Mail   - czat_admin@czat.onet.pl
void OnetKernel::raw_259()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg("Status", strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :259 #scc :nothing changed
void OnetKernel::raw_259n()
{
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strDisplay = QString(tr("* Nothing changed in %1")).arg(strChannel);
    pTabC->show_msg_active(strDisplay, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :260 Merovingian #scc +o :channel privilege changed
void OnetKernel::raw_260n()
{
    // copy raw 256
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[5];
    QString strWho = strDataList[4];
    QString strFlag = strDataList[6];
    QString strNick = strDataList[2];
    QString strDisplay;

    if (strFlag == "+q") strDisplay = QString(tr("* %1 is now the owner of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-q") strDisplay = QString(tr("* %1 is no longer the owner of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+o") strDisplay = QString(tr("* %1 is now super-operator on the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-o") strDisplay = QString(tr("* %1 is no longer super-operator on the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+h") strDisplay = QString(tr("* %1 is now the operator of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-h") strDisplay = QString(tr("* %1 is no longer the operator of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+v") strDisplay = QString(tr("* %1 is now a guest of channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-v") strDisplay = QString(tr("* %1 is no longer a guest of channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+X") strDisplay = QString(tr("* %1 is now moderator of the channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-X") strDisplay = QString(tr("* %1 is no longer moderating channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+Y") strDisplay = QString(tr("* %1 is now screener channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-Y") strDisplay = QString(tr("* %1 is no longer a screener channel %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+b") strDisplay = QString(tr("* %1 is now on the banned list in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-b") strDisplay = QString(tr("* %1 is no longer on the banned list in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "+I") strDisplay = QString(tr("* %1 is now on the list of invitees in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else if (strFlag == "-I") strDisplay = QString(tr("* %1 is no longer on the list of invitees in %2 (set by %3)")).arg(strNick).arg(strChannel).arg(strWho);
    else
        strDisplay = QString(tr("* %1 now has a flag %2 (set by %3)")).arg(strNick).arg(strFlag).arg(strWho);

    // display
    pTabC->show_msg(strChannel, strDisplay, 5);
}

// CS DROP #czesctoja
// :ChanServ!service@service.onet NOTICE scc_test :261 scc_test #czesctoja :has dropped this channel
// NS LIST aaa
// :NickServ!service@service.onet NOTICE Merovingian :261 aa_PrezesCiemnosci gaafa7 jaanka9 Naatasza23 zaak_333 Agaaaaaaaa ~Faajny25 kubaaa19 ~Amaadeusz_x misiaa_40
void OnetKernel::raw_261n()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        QString strChannel = strDataList[5];

        QMessageBox::information(0, "", QString(tr("Successfully removed channel %1")).arg(strChannel));

        // remove from list
        if (Core::instance()->lChannelHomes.contains(strChannel) == true)
            Core::instance()->lChannelHomes.removeOne(strChannel);

        // part
        if (pTabC->exist_tab(strChannel) == true)
            pNetwork->send(QString("PART %1").arg(strChannel));
    }
    else if (strNick.toLower() == "nickserv")
    {
        // TODO
    }
}

// :NickServ!service@service.onet NOTICE Merovingian :262 aa :end of list
void OnetKernel::raw_262n()
{
// TODO
}

// NS LIST #scc
// :NickServ!service@service.onet NOTICE Merovingian :263 #scc :no users found
void OnetKernel::raw_263n()
{
// TODO
}

// LUSERS
// :cf1f4.onet 265 Merovingian :Current Local Users: 568  Max: 1633
void OnetKernel::raw_265()
{
// ignore
}

// LUSERS
// :cf1f4.onet 266 Merovingian :Current Global Users: 2305  Max: 6562
void OnetKernel::raw_266()
{
// ignore
}

// SILENCE
// :cf1f2.onet 271 Merovingian Merovingian Aldinach!*@* <privatemessages,channelmessages,invites>
void OnetKernel::raw_271()
{
// TODO
}

// SILENCE
// :cf1f2.onet 272 Merovingian :End of Silence List
void OnetKernel::raw_272()
{
// ignore
}

// WHOIS Merovingian
// :cf1f2.onet 301 scc_test Merovingian :nie ma
void OnetKernel::raw_301()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay = QString(tr("%1 is away: %2")).arg(strNick).arg(strMessage);
    pTabC->show_msg_active(strDisplay, 7);
}

// USERHOST a
// :cf1f2.onet 302 Merovingian :
// USERHOST aleksa7
// :cf1f2.onet 302 Merovingian :aleksa7=+14833406@44DC43.4DB130.368946.600B51
void OnetKernel::raw_302()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg_active(strMessage, 7);
}

// ISON
// :cf1f1.onet 303 ~test :Darom
void OnetKernel::raw_303()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);

    QString strDisplay = QString("* ISON: %1").arg(strNick);
    pTabC->show_msg_active(strDisplay, 7);
}

// OPER
// :cf1f3.onet 304 ~test :SYNTAX OPER <username> <password>
void OnetKernel::raw_304()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg_active(strMessage, 7);
}

// AWAY :
// :cf1f3.onet 305 scc_test :You are no longer marked as being away
void OnetKernel::raw_305()
{
    QSettings settings;
    settings.setValue("away", "off");

    QString strDisplay = tr("You are no longer marked as being away");
    pTabC->show_msg_active(strDisplay, 7);
}

// AWAY :reason
// :cf1f3.onet 306 scc_test :You have been marked as being away
void OnetKernel::raw_306()
{
    QSettings settings;
    settings.setValue("away", "on");

    QString strDisplay = tr("You have been marked as being away");
    pTabC->show_msg_active(strDisplay, 7);
}

// WHOIS
// :cf1f3.onet 307 scc_test Merovingian :is a registered nick
void OnetKernel::raw_307()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    if (strMessage == "is a registered nick")
        strMessage = tr("is a registered nick");

    QString strDisplay = QString("* %1 %2").arg(strNick).arg(strMessage);
    pTabC->show_msg_active(strDisplay, 7);
}

// WHOIS
// :cf1f1.onet 311 scc_test Merovingian 26269559 2294E8.94913F.196694.9BAE58 * :Merovingian
void OnetKernel::raw_311()
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

    QString strDisplayNick = QString(tr("* %1 is %2@%3")).arg(strNick).arg(strZUO).arg(strIP);
    pTabC->show_msg_active(strDisplayNick, 7);

    QString strDisplayIrcname = QString(tr("* %1 ircname: %2")).arg(strNick).arg(strIrcname);
    pTabC->show_msg_active(strDisplayIrcname, 7);
}

// WHOIS
// :cf1f2.onet 312 scc_test Merovingian *.onet :OnetCzat
// WHOWAS
// :cf1f2.onet 312 Merovingian merovingian *.onet :Wed Sep  1 18:37:42 2010
void OnetKernel::raw_312()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strServer;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strServer += " "; strServer += strDataList[i]; }

    QString strDisplay = QString(tr("* %1 is online via %2")).arg(strNick).arg(strServer);
    pTabC->show_msg_active(strDisplay, 7);
}

// WHOIS
// :cf1f1.onet 313 scc_test Llanero :is a GlobalOp on OnetCzat
// :cf1f2.onet 313 Merovingian Darom :is a NetAdmin on OnetCzat
void OnetKernel::raw_313()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    QString strDisplay;
    if (strMessage == "is a GlobalOp on OnetCzat")
        strDisplay = QString(tr("* %1 is a GlobalOp on OnetCzat")).arg(strNick);
    else if (strMessage == "is a NetAdmin on OnetCzat")
        strDisplay = QString(tr("* %1 is a NetAdmin on OnetCzat")).arg(strNick);
    else
        strDisplay = strMessage;

    pTabC->show_msg_active(strDisplay, 7);
}

// WHOWAS
// :cf1f4.onet 314 Merovingian wilk 54995510 44DC43.553BE3.6C367A.FFF110 * :admin on ...
void OnetKernel::raw_314()
{
    // copy of raw 311
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

    QString strDisplayNick = QString(tr("* %1 is %2@%3")).arg(strNick).arg(strZUO).arg(strIP);
    pTabC->show_msg_active(strDisplayNick, 7);

    QString strDisplayIrcname = QString(tr("* %1 ircname: %2")).arg(strNick).arg(strIrcname);
    pTabC->show_msg_active(strDisplayIrcname, 7);
}

// WHO
// :cf1f2.onet 315 Merovingian a :End of /WHO list.
// :cf1f4.onet 315 Merovingian #16_17_18_19_lat :Too many results
void OnetKernel::raw_315()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNickChannel = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    if (strMessage == "End of /WHO list.")
        return;
    else if (strMessage == "Too many results")
        strMessage = tr("Too many results");

    QString strDisplay = QString("* %1 :%2").arg(strNickChannel).arg(strMessage);
    pTabC->show_msg_active(strDisplay, 7);
}

// WHOWAS
// :cf1f2.onet 314 Merovingian merovingian 26269559 2294E8.94913F.75F4D7.D7A8A7 * :Merovingian
void OnetKernel::raw_316()
{
    // copy of raw 311
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

    QString strDisplayNick = QString(tr("* %1 is %2@%3")).arg(strNick).arg(strZUO).arg(strIP);
    pTabC->show_msg_active(strDisplayNick, 7);

    QString strDisplayIrcname = QString(tr("* %1 ircname: %2")).arg(strNick).arg(strIrcname);
    pTabC->show_msg_active(strDisplayIrcname, 7);
}

// WHOIS
// :cf1f3.onet 317 scc_test Merovingian 7 1263650617 :seconds idle, signon time
void OnetKernel::raw_317()
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
    else if ((iDT_d == 0) && (iDT_h == 0) && (iDT_m == 0) && (iDT_s >= 0))
        strDT_idle = QString("%1s").arg(iDT_s);

    QString strDisplayIdle = QString(tr("* %1 is away %2")).arg(strNick).arg(strDT_idle);
    pTabC->show_msg_active(strDisplayIdle, 7);

    QDateTime dt_time = QDateTime::fromTime_t(strTime.toInt());
    QString strDT_time = dt_time.toString("dd/MM/yyyy hh:mm:ss");

    QString strDisplaySignon = QString(tr("* %1 is logged in since %2")).arg(strNick).arg(strDT_time);
    pTabC->show_msg_active(strDisplaySignon, 7);
}

// WHOIS
// :cf1f4.onet 318 scc_test Merovingian :End of /WHOIS list.
void OnetKernel::raw_318()
{
// ignore
}

// WHOIS
// :cf1f4.onet 319 scc_test Merovingian :#testy %#Komputery `#scc `#Quiz `#Scrabble `#hack
void OnetKernel::raw_319()
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

    QString strDisplay = QString(tr("* %1 is on channels: %2")).arg(strNick).arg(strChannels);
    pTabC->show_msg_active(strDisplay, 7);
}

// :cf1f4.onet 332 scc_test #scc :Simple Chat Client; current version: beta;
void OnetKernel::raw_332()
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

    pTabC->set_topic(strChannel, strTopic);
}

// :cf1f1.onet 333 scc_test #scc Merovingian!26269559 1253193639
void OnetKernel::raw_333()
{
// supported by raw 161
}

// :cf1f4.onet 341 Merovingian ~test34534 #Scrabble
// :cf1f1.onet 341 scc_test Merovingian ^cf1f1162848
void OnetKernel::raw_341()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strChannel = strDataList[4];

    if (strChannel[0] == '^')
    {
        mOldNameNewName.insert(strChannel, strNick);
        QTimer::singleShot(1000*3, this, SLOT(timer_rename_channel())); // 3 sec
    }
}

// WHO
// :cf1f2.onet 352 Merovingian #testy 12265854 F3F8AF.464CED.BF6592.28AAB2 *.onet Darom G` :0 Darom
// :cf1f2.onet 352 Merovingian * 26269559 2294E8.94913F.75F4D7.D7A8A7 *.onet Merovingian H :0 Merovingian
// :cf1f2.onet 352 Merovingian #uwaga 43347263 0AD995.BF5FE3.665A1E.9BBABB *.onet Llanero G@ :0 Llanero
// :cf1f2.onet 352 Merovingian #RADIO_PIORUNFM_PL 14833406 44DC43.4DB130.368946.600B51 *.onet aleksa7 H% :0 aleksa7
// :cf1f2.onet 352 Merovingian * 18359115 admin.łona *.onet MAS_PSOTKA G :0 onet-czat
void OnetKernel::raw_352()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    strMessage = "* "+strMessage;

    pTabC->show_msg_active(strMessage, 7);
}

// NAMES
// :cf1f1.onet 353 scc_test = #scc :scc_test|rx,0 `@Merovingian|brx,1 @chanky|rx,1
// :cf1f3.onet 353 Merovingian = #hack :%Hacker %weed %cvf @Majkel SzperaCZ_11 Merovingian `ChanServ %but

// owner      `
// op         @
// halfop     %
// mod        !
// screener   =
// voice      +

// busy       b
// registered r
// encrypted  x
// publiccam  W
// privcam    V
// admin      o
// developer  O

void OnetKernel::raw_353()
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
            if (strSuffix.indexOf("|") != -1)
            {
                strSuffix = strSuffix.right(strSuffix.length() - strSuffix.indexOf("|") -1);
                strSuffix = strSuffix.left(strSuffix.length()-2);
            }
            else
                strSuffix = QString::null;

            QString strCleanNick = strNick;

            QString strPrefix;
            if (strCleanNick.indexOf("`") != -1) { strCleanNick.remove("`"); strPrefix.append("`"); }
            if (strCleanNick.indexOf("@") != -1) { strCleanNick.remove("@"); strPrefix.append("@"); }
            if (strCleanNick.indexOf("%") != -1) { strCleanNick.remove("%"); strPrefix.append("%"); }
            if (strCleanNick.indexOf("!") != -1) { strCleanNick.remove("!"); strPrefix.append("!"); }
            if (strCleanNick.indexOf("=") != -1) { strCleanNick.remove("="); strPrefix.append("="); }
            if (strCleanNick.indexOf("+") != -1) { strCleanNick.remove("+"); strPrefix.append("+"); }

            emit add_user(strChannel, strCleanNick, strPrefix, strSuffix);

            // if ^ rename channel
            if (strChannel[0] == '^')
            {
                QSettings settings;
                QString strMe = settings.value("nick").toString();

                if (strCleanNick != strMe)
                {
                    mOldNameNewName.insert(strChannel, strCleanNick);
                    QTimer::singleShot(1000*3, this, SLOT(timer_rename_channel())); // 3 sec
                }
            }

            // nick avatar
            if ((strCleanNick[0] != '~') && (Core::instance()->mNickAvatar.contains(strCleanNick) == false))
            {
                QSettings settings;
                if (settings.value("disable_avatars").toString() == "off") // with avatars
                    pNetwork->send(QString("NS INFO %1 s").arg(strCleanNick));
            }
        }
    }
}

// :cf1f1.onet 355 Merovingian #uwaga 958 :users
void OnetKernel::raw_355()
{
// ignore
}

// NAMES
// :cf1f2.onet 366 scc_test #scc :End of /NAMES list.
void OnetKernel::raw_366()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    emit nicklist_refresh(strChannel);
}

// WHOWAS
// :cf1f2.onet 369 Merovingian merovingian :End of WHOWAS
void OnetKernel::raw_369()
{
// ignore
}

// INFO
// :cf1f3.onet 371 ~test :Core Developers:
void OnetKernel::raw_371()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg("Status", strMessage, 7);
}

// MOTD
// :cf1f4.onet 372 scc_test :- Onet Czat. Inny Wymiar Czatowania. Witamy
void OnetKernel::raw_372()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    pTabC->show_msg("Status", strMessage, 0);
}

// INFO
// :cf1f3.onet 374 ~test :End of /INFO list
void OnetKernel::raw_374()
{
// ignore
}

// MOTD
// :cf1f4.onet 375 scc_test :cf1f4.onet message of the day
void OnetKernel::raw_375()
{
    QString strDisplay = tr("Message Of The Day:");
    pTabC->show_msg("Status", strDisplay, 0);
}

// MOTD
// :cf1f1.onet 376 scc_test :End of message of the day.
void OnetKernel::raw_376()
{
// ignore
}

// WHOIS
// :cf1f1.onet 378 Merovingian Merovingian :is connecting from 26269559@46.113.153.49 46.113.153.49
void OnetKernel::raw_378()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;
    if (strDataList.value(7).isEmpty() == true) return;
    if (strDataList.value(8).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strZuoIP = strDataList[7];
    QString strIP = strDataList[8];

    QString strMessage = QString(tr("* %1 is connecting from %2 %3")).arg(strNick).arg(strZuoIP).arg(strIP);
    pTabC->show_msg_active(strMessage, 7);
}

// TIME
// :cf1f2.onet 391 ~test cf1f2.onet :Tue Jul 13 18:33:05 2010
void OnetKernel::raw_391()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strServer = strDataList[3];

    QString strDateTime;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strDateTime += " "; strDateTime += strDataList[i]; }
    if (strDateTime[0] == ':')
        strDateTime = strDateTime.right(strDateTime.length()-1);

    QString strMessage = QString(tr("* Date and time of the server %1: %2")).arg(strServer).arg(strDateTime);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f2.onet 396 ~scc_test 3DE379.B7103A.6CF799.6902F4 :is now your displayed host
void OnetKernel::raw_396()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strHost = strDataList[3];

    QString strMessage = QString(tr("* %1 is now your displayed host")).arg(strHost);

    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE ~scc_test :400 :you are not registred
void OnetKernel::raw_400n()
{
    QString strNick = strDataList[2];

    QString strMessage = QString(tr("* %1: Nick is not registered")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 401 ~Merovingian ~Merovingian1 :No such nick
// :cf1f3.onet 401 Merovingian #asdasdasd :No such channel
// :cf1f3.onet 401 scc_test scc :No such nick/channel
// :cf1f4.onet 401 Merovingian ChanServ :is currently unavailable. Please try again later.
void OnetKernel::raw_401()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNickChannel = strDataList[3];

    QString strMessage;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);

    if (strMessage == "No such nick")
        strMessage = QString(tr("* %1 :No such nick")).arg(strNickChannel);
    else if (strMessage == "No such channel")
        strMessage = QString(tr("* %1 :No such channel")).arg(strNickChannel);
    else if (strMessage == "No such nick/channel")
        strMessage = QString(tr("* %1 :No such nick/channel")).arg(strNickChannel);
    else if (strMessage == "is currently unavailable. Please try again later.")
        strMessage = QString(tr("* %1 is currently unavailable. Please try again later.")).arg(strNickChannel);

    // display
    pTabC->show_msg_active(strMessage, 7);

    // close inactive priv
    if (strNickChannel[0] == '^')
    {
        if (pTabC->exist_tab(strNickChannel) == true)
            pTabC->remove_tab(strNickChannel);
    }
}

// :ChanServ!service@service.onet NOTICE scc_test :401 aa :no such nick
// :NickServ!service@service.onet NOTICE Merovingian :401 a :no such nick
void OnetKernel::raw_401n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString(tr("* %1 :Nick does not exist")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 402 Merovingian a :No such server
void OnetKernel::raw_402()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strServer = strDataList[3];

    QString strMessage = QString(tr("* %1 :No such server")).arg(strServer);

    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE Merovingian :402 !*@*aa :invalid mask
void OnetKernel::raw_402n()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strMask = strDataList[4];

    QString strMessage = QString(tr("* %1 :Invalid mask")).arg(strMask);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f1.onet 403 ~testa #^cf1f41568 :Invalid channel name
void OnetKernel::raw_403()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* %1 :Invalid channel name")).arg(strChannel);

    // display
    pTabC->show_msg_active(strMessage, 7);

    // close inactive priv
    if (strChannel[0] == '^')
    {
        if (pTabC->exist_tab(strChannel) == true)
            pTabC->remove_tab(strChannel);
    }
}

// CS BANIP #scc ADD wilk
// :ChanServ!service@service.onet NOTICE Merovingian :403 wilk :user is not on-line
// NS OFFLINE MSG a a
// :NickServ!service@service.onet NOTICE Merovingian :403 msg :user is not on-line
void OnetKernel::raw_403n()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        QString strNick = strDataList[4];
        QString strMessage = QString(tr("* %1 :User is not on-line")).arg(strNick);
        pTabC->show_msg_active(strMessage, 7);
    }
    else if (strNick.toLower() == "nickserv")
    {
        QString strNick = strDataList[4];
        QString strMessage = QString(tr("* %1 :User is not on-line")).arg(strNick);
        pTabC->show_msg_active(strMessage, 7);
    }
}

// :cf1f1.onet 404 scc_test #Quiz :Cannot send to channel (+m)
// :cf1f4.onet 404 ~scc_test #lunar :Cannot send to channel (no external messages)
void OnetKernel::raw_404()
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
        strReason = tr("No moderator on the channel!");

    QString strMessage = QString(tr("* Unable to send a message to %1: %2")).arg(strChannel).arg(strReason);

    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :404 ~zwariowany_zdzich0 :user is not registred
void OnetKernel::raw_404n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString(tr("* %1 :User is not registred")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// WHOWAS a
//:cf1f2.onet 406 Merovingian a :There was no such nickname
void OnetKernel::raw_406()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* %1 :There was no such nickname")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE ~test :406 VHOST :unknown command
void OnetKernel::raw_406n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strCmd = strDataList[4];

    QString strMessage = QString(tr("* %1 :Unknown command")).arg(strCmd);

    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :407 VOICE :not enough parameters
// :NickServ!service@service.onet NOTICE Merovingian :407 OFFLINE GET :not enough parameters
void OnetKernel::raw_407n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strCmd = strDataList[4];

    QString strMessage = QString(tr("* %1 :Not enough parameters")).arg(strCmd);

    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :408 dsfdsf :no such channel
// :RankServ!service@service.onet NOTICE Merovingian :408 #aa :no such channel
void OnetKernel::raw_408n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* %1 :No such channel")).arg(strChannel);

    pTabC->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE ~Merovingian :412 admi :user's data is not ready
void OnetKernel::raw_412n()
{
// ignore
}

// RS INFO istota_bezduszna
// :RankServ!service@service.onet NOTICE istota_bezduszna :413 istota_bezduszna :user has no stats
void OnetKernel::raw_413n()
{
// ignore
}

// RS INFO #testa
// :RankServ!service@service.onet NOTICE Merovingian :414 #testa :channel has no stats
void OnetKernel::raw_414n()
{
// ignore
}

// RS INFO succubi
// :RankServ!service@service.onet NOTICE Merovingian :415 Succubi :permission denied
void OnetKernel::raw_415n()
{
// ignore
}

// RS INFO #a
// :RankServ!service@service.onet NOTICE Merovingian :416 #a :permission denied
void OnetKernel::raw_416n()
{
// TODO
}

// :NickServ!service@service.onet NOTICE scc_test :420 aleksa7 :is already on your friend list
void OnetKernel::raw_420n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString(tr("* Nick %1 is already on your friend list")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f2.onet 421 ~test VERSION :This command has been disabled.
// :cf1f4.onet 421 scc_test MOD :Unknown command
void OnetKernel::raw_421()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strCmd = strDataList[3];

    QString strReason;
    for (int i = 4; i < strDataList.size(); i++) { if (i != 4) strReason += " "; strReason += strDataList[i]; }
    if (strReason[0] == ':')
        strReason = strReason.right(strReason.length()-1);

    if (strReason == "Unknown command")
        strReason = tr("Unknown command");
    else if (strReason == "This command has been disabled.")
        strReason = tr("This command has been disabled.");

    QString strMessage = QString("* %1 :%2").arg(strCmd).arg(strReason);

    pTabC->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :421 aaa :is not on your friend list
void OnetKernel::raw_421n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString(tr("* Nick %1 is not on your friend list")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :430 wilk :is already on your ignore list
void OnetKernel::raw_430n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString(tr("* %1 is already on your ignore list")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :431 a :is not on your ignore list
void OnetKernel::raw_431n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];

    QString strMessage = QString(tr("* %1 is not on your ignore list")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 432 1501-unknown ~?o? :Erroneous Nickname
void OnetKernel::raw_432()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* %1 :Erroneous Nickname")).arg(strNick);

    pTabC->show_msg_all(strMessage, 9);
}

// :cf1f1.onet 433 * scc_test :Nickname is already in use.
void OnetKernel::raw_433()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* Nickname %1 is already in use.")).arg(strNick);

    pTabC->show_msg_all(strMessage, 9);

    QSettings settings;
    settings.setValue("override", "on");

    // reconnect
    if (strNick[0] != '~')
    {
        pNetwork->disconnect();
        pNetwork->connect();
    }
}

// :NickServ!service@service.onet NOTICE scc_test :440 #scc :is already on your favourite list
void OnetKernel::raw_440n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* Channel %1 is already on your favourite list")).arg(strChannel);

    pTabC->show_msg_active(strMessage, 7);
}

// :NickServ!service@service.onet NOTICE scc_test :441 #scc :is not on your favourite list
void OnetKernel::raw_441n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* Channel %1 is not on your favourite list")).arg(strChannel);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 443 Merovingian scc #Scrabble :is already on channel
void OnetKernel::raw_443()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* %1 is already on channel %2")).arg(strNick).arg(strChannel);

    pTabC->show_msg_active(strMessage, 7);
}

// SUMMON
// :cf1f3.onet 445 ~test :SUMMON has been disabled (depreciated command)
void OnetKernel::raw_445()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg_active(strMessage, 7);
}

// USERS
// :cf1f2.onet 446 Merovingian :USERS has been disabled (depreciated command)
void OnetKernel::raw_446()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 451 SLIST :You have not registered
void OnetKernel::raw_451()
{
    QString strCommand = strDataList[2];

    QString strMessage = QString(tr("* You have not registered to perform operation %1")).arg(strCommand);
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :452 #aaa :channel name already in use
void OnetKernel::raw_452n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* %1 :Channel name already in use")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :454 #aaaaaaaaaaaaaaaaaaaaaa :not enough unique channel name
// NS OFFLINE GET
// :NickServ!service@service.onet NOTICE Merovingian :454 a :no messages
void OnetKernel::raw_454n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        QString strChannel = strDataList[4];

        QString strMessage = QString(tr("* %1 :Not enough unique channel name")).arg(strChannel);
        pTabC->show_msg_active(strMessage, 7);
    }
    else if (strNick.toLower() == "nickserv")
    {
        // TODO
    }
}

// :ChanServ!service@service.onet NOTICE Merovingian :456 #test2 Merovingian :is already channel owner
// NS OFFLINE MSG exist_nick test
// :NickServ!service@service.onet NOTICE Merovingian :456 Merovingian :is online
void OnetKernel::raw_456n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strNick = strDataList[0];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);
    strNick = strNick.left(strNick.indexOf('!'));

    if (strNick.toLower() == "chanserv")
    {
        QString strNick = strDataList[5];

        QString strMessage = QString(tr("* %1 is already channel owner")).arg(strNick);

        // display
        pTabC->show_msg_active(strMessage, 7);
    }
    else if (strNick.toLower() == "nickserv")
    {
        // TODO
    }
}

// :ChanServ!service@service.onet NOTICE scc_test :458 #scc v scc :unable to remove non-existent privilege
void OnetKernel::raw_458n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[6];

    QString strMessage = QString(tr("* %1 :Unable to remove non-existent privilege")).arg(strWho);

    // display
    pTabC->show_msg(strChannel, strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :459 #scc b test :channel privilege already given
void OnetKernel::raw_459n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;
    if (strDataList.value(6).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[6];

    QString strMessage = QString(tr("* %1 :Channel privilege already given")).arg(strWho);

    // display
    pTabC->show_msg(strChannel, strMessage, 7);
}

// :cf1f2.onet 461 ~test OPER :Not enough parameters.
void OnetKernel::raw_461()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strCmd = strDataList[3];

    QString strMessage = QString(tr("* %1 :Not enough parameters")).arg(strCmd);

    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :461 #scc scc :channel operators cannot be banned
void OnetKernel::raw_461n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWho = strDataList[5];

    QString strMessage = QString(tr("* %1 :Channel operators cannot be banned")).arg(strWho);

    // display
    pTabC->show_msg(strChannel, strMessage, 7);
}

// PASS
// :cf1f2.onet 462 Merovingian :You may not reregister
void OnetKernel::raw_462()
{
    QString strMessage = QString(tr("* You may not reregister"));
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :463 #lunar AUDITORIUM :permission denied, insufficient privileges
void OnetKernel::raw_463n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];
    QString strWhat = strDataList[5];

    QString strMessage = QString(tr("* %1 :Permission denied, insufficient privileges in %2 channel")).arg(strWhat).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE Merovingian :464 TOPIC :invalid argument
void OnetKernel::raw_464n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strWhat = strDataList[4];

    QString strMessage = QString(tr("* %1 :Invalid argument")).arg(strWhat);
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :467 #scc :permission denied, you are not a channel owner
void OnetKernel::raw_467n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* %1 :Permission denied, you are not a channel owner")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :468 #scc :permission denied, insufficient privileges
void OnetKernel::raw_468n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* Permission denied, insufficient privileges in %1 channel")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE Merovingian :469 #Czat :channel is private
void OnetKernel::raw_469n()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strChannel = strDataList[4];

    QString strMessage = QString(tr("* Channel %1 is private")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 470 ~Merovingian :#testy has become full, so you are automatically being transferred to the linked channel #Awaria
void OnetKernel::raw_470()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];
    if (strChannel[0] == ':')
        strChannel = strChannel.right(strChannel.length()-1);

    QString strLinked = strDataList[strDataList.size()-1];

    QString strMessage = QString(tr("* %1 has become full, so you are automatically being transferred to the linked channel %2")).arg(strChannel).arg(strLinked);
    pTabC->show_msg("Status", strMessage, 7);
}

// :cf1f2.onet 471 ~Merovingian #testy :Cannot join channel (Channel is full)
void OnetKernel::raw_471()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* Cannot join channel %1: channel is full")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// :ChanServ!service@service.onet NOTICE scc_test :472 #aaaaaaaaaaaaaaaaaaaaaaaaaaaaa :wait 60 seconds before next REGISTER
void OnetKernel::raw_472n()
{
    QString strMessage = QString(tr("* Wait 60 seconds before creating next channel"));
    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f3.onet 473 ~scc_test #lunar :Cannot join channel (Invite only)
void OnetKernel::raw_473()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* Cannot join channel %1: Invite only")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);

    // close inactive priv
    if (strChannel[0] == '^')
    {
        if (pTabC->exist_tab(strChannel) == true)
            pTabC->remove_tab(strChannel);
    }
}

// :cf1f3.onet 474 ~scc_test #Quiz :Cannot join channel (You're banned)
void OnetKernel::raw_474()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* Cannot join channel %1: You're banned")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 475 Merovingian #glupia_nazwa :Cannot join channel (Incorrect channel key)
void OnetKernel::raw_475()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* Cannot join channel %1: Incorrect channel key")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);

    (new DlgChannelKey(Core::instance()->sccWindow(), pNetwork, strChannel))->show(); // should be show - prevent hangup!
}

// :cf1f4.onet 481 Merovingian :Permission Denied - You do not have the required operator privileges
void OnetKernel::raw_481()
{
    QString strMessage = QString(tr("* Permission Denied - You do not have the required operator privileges"));
    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f1.onet 482 Merovingian #Scrabble :Only a u-line may kick a u-line from a channel.
void OnetKernel::raw_482()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* Only a u-line may kick a u-line from a channel."));

    pTabC->show_msg(strChannel, strMessage, 7);
}

// :cf1f4.onet 484 Merovingian #testy :Can't kick user advocato000 from channel (+Q set)
// :cf1f4.onet 484 scc_test #scc :Can't kick scc as your spells are not good enough
void OnetKernel::raw_484()
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

    pTabC->show_msg(strChannel, strMessage, 7);
}

// :cf1f3.onet 491 ~test :Invalid oper credentials
void OnetKernel::raw_491()
{
    QString strMessage = QString(tr("* Invalid oper credentials"));

    pTabC->show_msg_active(strMessage, 7);
}

//:cf1f2.onet 530 Merovingian #f :Only IRC operators may create new channels
void OnetKernel::raw_530()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* %1 :Only IRC operators may create new channels")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// PRIVMSG a
// :cf1f2.onet 531 Merovingian chanky :You are not permitted to send private messages to this user
void OnetKernel::raw_531()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* %1 :You are not permitted to send private messages to this user").arg(strNick));
    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 600 scc_test Radowsky 16172032 690A6F.A8219B.7F5EC1.35E57C 1267055769 :arrived online
void OnetKernel::raw_600()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* Your friend %1 arrived online")).arg(strNick);
    pTabC->show_msg_active(strMessage, 7);

    if (Core::instance()->mFriends.contains(strNick))
        Core::instance()->mFriends[strNick] = true;
    else
        Core::instance()->mFriends.insert(strNick, true);
}

// :cf1f4.onet 601 scc_test Radowsky 16172032 690A6F.A8219B.7F5EC1.35E57C 1267055692 :went offline
void OnetKernel::raw_601()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* Your friend %1 went offline")).arg(strNick);
    pTabC->show_msg_active(strMessage, 7);

    if (Core::instance()->mFriends.contains(strNick))
        Core::instance()->mFriends[strNick] = false;
    else
        Core::instance()->mFriends.insert(strNick, false);
}

// NS FRIENDS DEL nick
// :cf1f3.onet 602 scc_test aaa * * 0 :stopped watching
void OnetKernel::raw_602()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    Core::instance()->mFriends.remove(strNick);
}

//:cf1f1.onet 604 scc_test scc_test 51976824 3DE379.B7103A.6CF799.6902F4 1267054441 :is online
void OnetKernel::raw_604()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* Your friend %1 is now on-line")).arg(strNick);
    pTabC->show_msg_active(strMessage, 7);

    if (Core::instance()->mFriends.contains(strNick))
        Core::instance()->mFriends[strNick] = true;
    else
        Core::instance()->mFriends.insert(strNick, true);
}

// :cf1f1.onet 605 scc_test Radowsky * * 0 :is offline
void OnetKernel::raw_605()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* Your friend %1 is now off-line")).arg(strNick);
    pTabC->show_msg_active(strMessage, 7);

    if (Core::instance()->mFriends.contains(strNick))
        Core::instance()->mFriends[strNick] = false;
    else
        Core::instance()->mFriends.insert(strNick, false);
}

// WATCH
// :cf1f2.onet 607 Merovingian :End of WATCH list
void OnetKernel::raw_607()
{
// ignore
}

// SERVER
// :cf1f1.onet 666 ~test :You cannot identify as a server, you are a USER. IRC Operators informed.
void OnetKernel::raw_666()
{
    QString strMessage = QString(tr("* You cannot identify as a server, you are a USER. IRC Operators informed."));

    pTabC->show_msg_active(strMessage, 7);
}

// COMMANDS
// :cf1f2.onet 702 Merovingian :ZLINE <core> 1
// MODULES
// :cf1f2.onet 702 Merovingian :m_onetauditorium.so
void OnetKernel::raw_702()
{
    QString strMessage;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strMessage += " "; strMessage += strDataList[i]; }
    if (strMessage[0] == ':')
        strMessage = strMessage.right(strMessage.length()-1);
    strMessage = "* "+strMessage;

    pTabC->show_msg_active(strMessage, 7);
}

// MODULES
// :cf1f2.onet 703 Merovingian :End of MODULES list
void OnetKernel::raw_703()
{
// ignore
}

// COMMANDS
// :cf1f2.onet 704 Merovingian :End of COMMANDS list
void OnetKernel::raw_704()
{
// ignore
}

//:cf1f3.onet 801 scc_test :q5VMy1wl6hKL5ZUt
void OnetKernel::raw_801()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strKey = strDataList[3];
    if (strKey[0] == ':')
        strKey = strKey.right(strKey.length()-1);

    OnetAuth *pOnet_auth = new OnetAuth(pTabC);
    QString strAuth = pOnet_auth->transform_key(strKey);
    delete pOnet_auth;

    if (strAuth.length() == 16)
    {
        QSettings settings;
        pNetwork->send(QString("AUTHKEY %1").arg(strAuth));
        QString strUOKey = settings.value("uokey").toString();
        QString strNickUo = settings.value("uo_nick").toString();
        if ((strUOKey.isEmpty() == false) && (strNickUo.isEmpty() == false))
            pNetwork->send(QString("USER * %1 czat-app.onet.pl :%2").arg(strUOKey).arg(strNickUo));
    }
    else
        pTabC->show_msg("Status", tr("Error: Invalid auth key"), 9);
}

// :cf1f1.onet 802 * :Corrupted Nickname
void OnetKernel::raw_802()
{
    QString strMessage = QString(tr("* Corrupted Nickname"));
    pTabC->show_msg_active(strMessage, 9);
}

// BUSY 1
// :cf1f2.onet 807 scc_test :You are marked as busy
void OnetKernel::raw_807()
{
    QSettings settings;
    settings.setValue("busy", "on");

    QString strDisplay = tr("* You are marked as busy");
    pTabC->show_msg_active(strDisplay, 7);
}

// BUSY 0
// :cf1f2.onet 808 scc_test :You are no longer marked busy
void OnetKernel::raw_808()
{
    QSettings settings;
    settings.setValue("busy", "off");

    QString strDisplay = tr("You are no longer marked busy");
    pTabC->show_msg_active(strDisplay, 7);
}

// WHOIS
// :cf1f2.onet 809 scc_test Succubi :is busy
void OnetKernel::raw_809()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage = QString(tr("* %1 is busy")).arg(strNick);
    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f2.onet 811 scc_test Merovingian :Ignore invites
void OnetKernel::raw_811()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strMessage;

    if (strNick[0] == '^')
        strMessage = QString(tr("* Ignored priv from %1")).arg(strNick);
    else
        strMessage = QString(tr("* Ignored invite from %1")).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f2.onet 812 scc_test Merovingian ^cf1f41284615 :Invite rejected
// :cf1f3.onet 812 Merovingian Vasquez_ #Kraina_Lagodności :Invite rejected
void OnetKernel::raw_812()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[3];
    QString strChannel = strDataList[4];

    QString strMessage;

    if (strChannel[0] == '^')
        strMessage = QString(tr("* Rejected priv from %1")).arg(strNick);
    else
        strMessage = QString(tr("* Rejected invite to %1 from %2")).arg(strChannel).arg(strNick);

    pTabC->show_msg_active(strMessage, 7);
}

// WHOIS
// :cf1f4.onet 815 ~test testnick :Public webcam
void OnetKernel::raw_815()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strNick = strDataList[3];

    QString strDisplay = QString(tr("* %1 has public webcam")).arg(strNick);
    pTabC->show_msg_active(strDisplay, 7);
}

// :cf1f2.onet 817 scc_test #scc 1253216797 mikefear - :%Fb:arial%%Ce40f0f%re
void OnetKernel::raw_817()
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
        pTabC->show_msg(strTime, strChannel, QString("<%1> %2").arg(strNick).arg(strMessage), 0);
}

// SLIST
// :cf1f3.onet 818 scc_test :Start of simple channels list.
void OnetKernel::raw_818()
{
    Core::instance()->stlChannelList.clear();
}

// SLIST
// :cf1f3.onet 819 scc_test :#tarnów_dziki:g:1,#Żory:g:0,#Mława:O:0,#Lineage_II:_:1,#kakakak:O:0,#apostolat_yfl:_:0,#ITALIA_CLUB:i:23,#Finał_WOŚP:P:0,#sama_słodycz:O:0,#Suwałki:i:14,#Mamuśki:O:0,#Pokój_Radości:O:0,#Antwerpia:g:0,#Kolo_Gospodyn_Wiejskich:O:0,#Samotnia_Kurka:G:0,#Wszystko_o_grach:O:0,#VIPy_NowySącz:h:0,#tymczasowy:G:0,#Zielona_Góra:h:2,#45slonko:P:0,#kawalek_nieba:O:0,#Wirtualna_Przyjazn:a:11,#Magiczny_swiat:O:1,#herbatka_u_cynamonki:P:0,#DEUTSCHLAND:i:111,#informatyka:`:1
void OnetKernel::raw_819()
{
    QString strChannelsString;
    for (int i = 3; i < strDataList.size(); i++) { if (i != 3) strChannelsString += " "; strChannelsString += strDataList[i]; }
    if (strChannelsString[0] == ':')
        strChannelsString = strChannelsString.right(strChannelsString.length()-1);

    QStringList strChannelsList = strChannelsString.split(",");
    for (int i = 0; i < strChannelsList.size(); i++)
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
                strChannelType = tr("Teen");
                break;
            case 2:
                strChannelType = tr("Common");
                break;
            case 3:
                strChannelType = tr("Erotic");
                break;
            case 4:
                strChannelType = tr("Thematic");
                break;
            case 5:
                strChannelType = tr("Regional");
                break;
        }

        if (flag == true)
            strChannelCat += tr("Moderated")+" ";
        if (flag1 == true)
            strChannelCat += tr("Recommended")+" ";

        switch(k)
        {
            case 0:
                strChannelCat = tr("Wild")+" "+ strChannelCat;
                break;
            case 1:
                strChannelCat = tr("Tame")+" "+ strChannelCat;
                break;
            case 2:
                strChannelCat = tr("With class")+" "+ strChannelCat;
                break;
            case 3:
                strChannelCat = tr("Cult")+" "+ strChannelCat;
                break;
        }

        ChannelList add;
        add.name = strChannelName;
        add.people = strChannelPeople;
        add.cat = strChannelCat;
        add.type = strChannelType;

        Core::instance()->stlChannelList.append(add);
    }
}

// SLIST
// :cf1f3.onet 820 scc_test :End of simple channel list.
void OnetKernel::raw_820()
{
    // ignore
}

// :cf1f3.onet 821 scc_test #scc :Channel is not moderated
void OnetKernel::raw_821()
{
    if (strDataList.value(3).isEmpty() == true) return;

    QString strChannel = strDataList[3];

    QString strMessage = QString(tr("* Channel %1 is not moderated")).arg(strChannel);
    pTabC->show_msg_active(strMessage, 7);
}

// NS IGNORE DEL nick
// :cf1f2.onet 950 Merovingian Merovingian :Removed Succubi!*@* <privatemessages,channelmessages,invites> from silence list
void OnetKernel::raw_950()
{
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strNick = strDataList[5];

    QString strDisplay = QString(tr("* Removed %1 from silence list")).arg(strNick);
    pTabC->show_msg_active(strDisplay, 7);
}

// NS IGNORE ADD nick
// :cf1f1.onet 951 scc_test scc_test :Added test!*@* <privatemessages,channelmessages,invites> to silence list
void OnetKernel::raw_951()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;
    if (strDataList.value(5).isEmpty() == true) return;

    QString strNick = strDataList[5];

    QString strMessage = QString(tr("* Added %1 to silence list")).arg(strNick);
    pTabC->show_msg_active(strMessage, 7);
}

// :cf1f4.onet 952 Merovingian Merovingian :Succubi!*@* <privatemessages,channelmessages,invites> is already on your silence list
void OnetKernel::raw_952()
{
    if (strDataList.value(3).isEmpty() == true) return;
    if (strDataList.value(4).isEmpty() == true) return;

    QString strNick = strDataList[4];
    if (strNick[0] == ':')
        strNick = strNick.right(strNick.length()-1);

    QString strMessage = QString(tr("* %1 is already on your silence list")).arg(strNick);
    pTabC->show_msg_active(strMessage, 7);
}
