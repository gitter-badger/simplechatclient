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
#include <QSettings>
#include <QStringList>
#include "webcam_network.h"
#include "webcam_engine.h"

WebcamEngine::WebcamEngine(QString n, bool m)
{
    strNick = n;
    bMini = m;

    pWebcamNetwork = new WebcamNetwork();

    create_signals();

    emit update_text(tr("Starting the service webcams"));

    pWebcamNetwork->network_connect();
}

WebcamEngine::~WebcamEngine()
{
    delete pWebcamNetwork;
}

void WebcamEngine::create_signals()
{
    QObject::connect(pWebcamNetwork, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(pWebcamNetwork, SIGNAL(data_kernel(QByteArray)), this, SLOT(data_kernel(QByteArray)));
    QObject::connect(pWebcamNetwork, SIGNAL(text_kernel(QString)), this, SLOT(text_kernel(QString)));
    QObject::connect(pWebcamNetwork, SIGNAL(error(QString)), this, SLOT(slot_error(QString)));
}

void WebcamEngine::close_engine()
{
    pWebcamNetwork->network_disconnect();
}

void WebcamEngine::network_send(QString s)
{
    pWebcamNetwork->network_send(s);
}

void WebcamEngine::set_user(QString s)
{
    strNick = s;
}

/* from WebcamNetwork to DlgWebcam */
void WebcamEngine::slot_error(QString s)
{
    emit error(s);
}

void WebcamEngine::connected()
{
    emit update_text(tr("Connected to server webcam.")+"<br>"+tr("Please wait ..."));

    QString strCAUTH = "1234567890123456";
    pWebcamNetwork->network_send(QString("CAUTH %1 3.00.159").arg(strCAUTH));
}

void WebcamEngine::disconnected()
{
    emit update_text(tr("Disconnected from server webcams"));
}

void WebcamEngine::data_kernel(QByteArray bData)
{
#ifdef Q_WS_X11
        QSettings settings;
        if (settings.value("debug").toString() == "on")
            qDebug() << "CAM byte <- " << bData;
#endif

    if (iCamCmd == 202)
        raw_202b(bData);
    else if (iCamCmd == 250)
        raw_250b(bData);
    else if (iCamCmd == 251)
        raw_251b(bData);
    else if (iCamCmd == 252)
        raw_252b(bData);
    else if (iCamCmd == 254)
        raw_254b(bData);
    else if (iCamCmd == 403)
        raw_403b(bData);

    iCamCmd = 0;
}

/*
202 14283 IMAGE_UPDATE_BIG psotnica2603
*/
void WebcamEngine::raw_202b(QByteArray &data)
{
    emit update_image(data);
}

/*
ja31:-
osa1987:1:-2/-2/osa1987/1:0::0
ToWiemTylkoJa:1:2/0/#Relax/0,-2/-2/ToWiemTylkoJa/1:2::13
scc_test:1:2/0/#Quiz/0,2/0/#Relax/0,2/0/#Scrabble/0,4/0/#scc/0:0::0
*/
/* multiline */
void WebcamEngine::raw_250b(QByteArray &data)
{
    // init data
    QString strData(data);
    QStringList strDataList = strData.split("\n");

    /* clear users */
    emit clear_users();

    foreach (QString strLine, strDataList)
    {
        QStringList strLineList = strLine.split(":");
        if (strLineList.size() == 6)
        {
            QString strUser = strLineList[0];
            QString strCamOnOff = strLineList[1]; // 1 = on; 2 = off
            QString strChannelsParams = strLineList[2];
            QString strSpectators = strLineList[3];
            QString strUdget = strLineList[4]; // udget (012345)
            int iRank = strLineList[5].toInt(); // -500 to 500

            if (!strChannelsParams.isEmpty())
            {
                QString strAllChannels;
                QStringList strlChannelsParams = strChannelsParams.split(",");
                foreach (QString strChannelParams, strlChannelsParams)
                {
                    QStringList strlChannelParams = strChannelParams.split("/");
                    if (strlChannelParams.size() == 4)
                    {
                        QString strChannelCategory = strlChannelParams[0];
                        QString strUnknown = strlChannelParams[1]; // always 0 (?)
                        QString strChannelName = strlChannelParams[2];
                        QString strCamPubPriv = strlChannelParams[3]; // 0 = public; 1 = private

                        if ((strChannelName[0] == '#') || (strChannelName[0] == '^'))
                            strAllChannels += strChannelName+" ";
                    }
                }

                // insert into map
                //mNickChannels[strUser] = strAllChannels;
            }

            // add user
            emit add_user(strUser, iRank, strSpectators);
        }
    }
    emit update_text(tr("Select user"));
}

/*
ja31:-
osa1987:1:-2/-2/osa1987/1:0::0
ToWiemTylkoJa:1:2/0/#Relax/0,-2/-2/ToWiemTylkoJa/1:2::13
scc_test:1:2/0/#Quiz/0,2/0/#Relax/0,2/0/#Scrabble/0,4/0/#scc/0:0::0
*/
void WebcamEngine::raw_251b(QByteArray &data)
{
    QString strLine(data);
    if (!strLine.isEmpty())
    {
        QStringList strLineList = strLine.split(":");
        if (strLineList.size() == 6)
        {
            QString strUser = strLineList[0];
            int iCamOnOff = strLineList[1].toInt(); // 1 = on; 2 = off
            QString strChannelsParams = strLineList[2];
            QString strSpectators = strLineList[3];
            QString strUdget = strLineList[4]; // udget (012345)
            int iRank = strLineList[5].toInt(); // -500 to 500

            if (!strChannelsParams.isEmpty())
            {
                QString strAllChannels;

                QStringList strlChannelsParams = strChannelsParams.split(",");
                foreach (QString strChannelParams, strlChannelsParams)
                {
                    QStringList strlChannelParams = strChannelParams.split("/");
                    if (strlChannelParams.size() == 4)
                    {
                        QString strChannelCategory = strlChannelParams[0];
                        QString strUnknown = strlChannelParams[1]; // always 0 (?)
                        QString strChannelName = strlChannelParams[2];
                        QString strCamPubPriv = strlChannelParams[3]; // 0 = public; 1 = private

                        if ((strChannelName[0] == '#') || (strChannelName[0] == '^'))
                            strAllChannels += strChannelName+" ";
                    }
                }

                // update map
                //mNickChannels[strUser] = strAllChannels;
            }

            // if current nick
            if (strUser == strNick)
            {
                // update rank
                emit update_rank(iRank);

                // update channels
                //ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:")).arg(mNickChannels[strUser]));
            }

            // cam off
            if (iCamOnOff == 0)
            {
                //mNickChannels.remove(strUser);
                emit remove_user(strUser);
            }
            // cam on
            else
            {
                // add user or update user
                emit update_user(strUser, iRank, strSpectators);
            }
        }
    }
}

/*
SETSTATUS //panda
*/
void WebcamEngine::raw_252b(QByteArray &data)
{
    QString strStatus = data;
    if (strStatus.left(9) == "SETSTATUS")
    {
#ifdef Q_WS_X11
        QSettings settings;
        if (settings.value("debug").toString() == "on")
            qDebug() << "CAM <- " << strStatus;
#endif
        strStatus.remove("SETSTATUS ");
        emit update_status(strStatus);
    }
}

/*
scc_test 2 0
*/
/* multi-line */
void WebcamEngine::raw_254b(QByteArray &data)
{
    QString strData(data);
    QStringList strDataList = strData.split("\n");

    /* clear users */
    emit clear_users();

    foreach (QString strLine, strDataList)
    {
        if (!strLine.isEmpty())
        {
            QStringList strLineList = strLine.split(" ");
            if (strLineList.size() == 3) // is correct ?
            {
                QString strUser = strLineList[0];
                QString strSpectators = strLineList[1];
                int iRank = strLineList[2].toInt();

                // add to table
                emit add_user(strUser, iRank, strSpectators);

                // if current nick
                if (strUser == strNick)
                    emit update_rank(iRank);
            }
        }
    }
}

/*
Invalid key
*/
void WebcamEngine::raw_403b(QByteArray &data)
{
    QString strError = data;
#ifdef Q_WS_X11
    QSettings settings;
    if (settings.value("debug").toString() == "on")
        qDebug() << "CAM <- " << strError;
#endif
    emit error(strError);
}

void WebcamEngine::text_kernel(QString strData)
{
#ifdef Q_WS_X11
    QSettings settings;
    if (settings.value("debug").toString() == "on")
        qDebug() << "CAM <- " << strData;
#endif

    QStringList strDataList = strData.split(" ");
    QString strCmd = strDataList[0];

    if (strCmd == "200")
        raw_200();
    else if (strCmd == "202")
        raw_202(strDataList);
    else if (strCmd == "211")
        raw_211(strDataList);
    else if (strCmd == "221")
        raw_221();
    else if (strCmd == "231")
        raw_231();
    else if (strCmd == "232")
        raw_232();
    else if (strCmd == "233")
        raw_233();
    else if (strCmd == "250")
        raw_250(strDataList);
    else if (strCmd == "251")
        raw_251(strDataList);
    else if (strCmd == "252")
        raw_252(strDataList);
    else if (strCmd == "253")
        raw_253(strDataList);
    else if (strCmd == "254")
        raw_254(strDataList);
    else if (strCmd == "264")
        raw_264();
    else if (strCmd == "261")
        raw_261();
    else if (strCmd == "262")
        raw_262();
    else if (strCmd == "263")
        raw_263();
    else if (strCmd == "267")
        raw_267();
    else if (strCmd == "268")
        raw_268();
    else if (strCmd == "403")
        raw_403(strDataList);
    else if (strCmd == "405")
        raw_405(strDataList);
    else if (strCmd == "406")
        raw_406();
    else if (strCmd == "408")
        raw_408(strDataList);
    else if (strCmd == "410")
        raw_410();
    else if (strCmd == "412")
        raw_411();
    else if (strCmd == "412")
        raw_412(strDataList);
    else if (strCmd == "413")
        raw_413(strDataList);
    else if (strCmd == "418")
        raw_418();
    else if (strCmd == "501")
        raw_501();
    else if (strCmd == "502")
        raw_502();
    else if (strCmd == "504")
        raw_504();
    else if (strCmd == "508")
        raw_508();
    else if (strCmd == "515")
        raw_515();
    else if (strCmd == "520")
        raw_520();
    else
        qDebug() << "Unknown CAM RAW:" << strData;
}

/*
200 0 OK
*/
void WebcamEngine::raw_200()
{
    pWebcamNetwork->network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
}

/*
202 17244 IMAGE_UPDATE_BIG Ekscentryk
*/
void WebcamEngine::raw_202(QStringList &strDataList)
{
    if (strDataList.size() < 4) return;

    QString strUser = strDataList[3];

    pWebcamNetwork->set_bytes_need(strDataList[1].toInt());
    if (strDataList[1].toInt() != 0)
    {
        // re-send
        if (strUser == strNick)
        {
            QDateTime dt = QDateTime::currentDateTime();
            int iCurrentTime = (int)dt.toTime_t(); // seconds that have passed since 1970
            pWebcamNetwork->set_last_keep_alive(iCurrentTime);

            pWebcamNetwork->network_send(QString("KEEPALIVE_BIG %1").arg(strNick));
        }

        pWebcamNetwork->set_btext(false);
        if (strUser == strNick)
            iCamCmd = 202;
        else
            iCamCmd = 0;
    }
    else
    {
        if (strUser == strNick)
            emit user_error(tr("This user does not send data"));
    }
}

/*
211 19995 Noemi_01@0
211 13584 Noemi_01@1
211 13584 Noemi_01@2
211 13584 Noemi_01@3
211 29 my_dwoje@4
211 17 myszka29brunetka@5
*/
void WebcamEngine::raw_211(QStringList &strDataList)
{
    if (strDataList.size() < 3) return;

    if (strDataList[1].toInt() > 0)
    {
        pWebcamNetwork->set_btext(false);
        pWebcamNetwork->set_bytes_need(strDataList[1].toInt());
        iCamCmd = 211;
    }
}

/*
221 0 UDPUT_OK
*/
void WebcamEngine::raw_221()
{
}

/*
231 0 OK scc_test
*/
void WebcamEngine::raw_231()
{
}

/*
232 0 CMODE 0
*/
void WebcamEngine::raw_232()
{
}

/*
233 0 QUALITY_FACTOR 1
*/
void WebcamEngine::raw_233()
{
}

/*
250 12519 OK
*/
void WebcamEngine::raw_250(QStringList &strDataList)
{
    if (strDataList.size() < 3) return;

    // initial read users status
    pWebcamNetwork->set_btext(false);
    pWebcamNetwork->set_bytes_need(strDataList[1].toInt());
    iCamCmd = 250;
}

/*
251 52 UPDATE
*/
void WebcamEngine::raw_251(QStringList &strDataList)
{
    if (strDataList.size() < 3) return;

    pWebcamNetwork->set_btext(false);
    pWebcamNetwork->set_bytes_need(strDataList[1].toInt());
    iCamCmd = 251;
}

/*
252 41 USER_STATUS pati28ash
*/
void WebcamEngine::raw_252(QStringList &strDataList)
{
    if (strDataList.size() < 4) return;

    if (strDataList[1].toInt() > 0)
    {
        pWebcamNetwork->set_bytes_need(strDataList[1].toInt());
        pWebcamNetwork->set_btext(false);

        QString strUser = strDataList[3];
        if (strUser == strNick)
            iCamCmd = 252;
        else
            iCamCmd = 0;
    }
}

/*
253 0 USER_VOTES Delikatna 38
*/
void WebcamEngine::raw_253(QStringList &strDataList)
{
    if (strDataList.size() < 5) return;

    QString strUser = strDataList[3];
    int iRank = strDataList[4].toInt();

    if (strUser == strNick) // is current nick
        emit update_rank(iRank);
}

/*
254 1489 USER_COUNT_UPDATE
*/
void WebcamEngine::raw_254(QStringList &strDataList)
{
    if (strDataList.size() < 3) return;

    pWebcamNetwork->set_btext(false);
    pWebcamNetwork->set_bytes_need(strDataList[1].toInt());
    iCamCmd = 254;

    // check keepalive
    QDateTime dt = QDateTime::currentDateTime();
    int iCurrentTime = (int)dt.toTime_t(); // seconds that have passed since 1970

    if (iCurrentTime - pWebcamNetwork->get_last_keep_alive() > 30) // 30 sec
    {
        if (!strNick.isEmpty())
            pWebcamNetwork->network_send(QString("KEEPALIVE_BIG %1").arg(strNick));
    }
}

/*
261 0 OK
*/
void WebcamEngine::raw_261()
{
    /* stopped */
}

/*
264 0 CODE_ACCEPTED ffffffff 2147483647
*/
/* successfully logged in */
void WebcamEngine::raw_264()
{
    emit update_text(tr("Setting mode for viewing only"));
    pWebcamNetwork->network_send("SENDMODE 0");

    // if mini get user image
    if (bMini)
        pWebcamNetwork->network_send(QString("SUBSCRIBE_BIG * %1").arg(strNick));
}

/*
262 0 NEW_FAN ~matka_wariatka
*/
void WebcamEngine::raw_262()
{
}

/*
263 0 VOTE_OK alicja17 3
*/
void WebcamEngine::raw_263()
{
    emit vote_ok();
}

/*
267 0 SENDMODE=0
267 0 SENDMODE=1
*/
void WebcamEngine::raw_267()
{
}

/*
268 0 OK
*/
void WebcamEngine::raw_268()
{
    // CAUTH ok
    QSettings settings;
    QString strUOKey = settings.value("uokey").toString();
    pWebcamNetwork->network_send(QString("AUTH %1 3.00.159").arg(strUOKey));
}

/*
403 11 ACCESS_DENIED
Invalid key
*/
void WebcamEngine::raw_403(QStringList &strDataList)
{
    if (strDataList.size() < 3) return;

    pWebcamNetwork->set_btext(false);
    pWebcamNetwork->set_bytes_need(strDataList[1].toInt());
    iCamCmd = 403;
}

/*
405 0 USER_GONE Restonka
*/
void WebcamEngine::raw_405(QStringList &strDataList)
{
    if (strDataList.size() < 4) return;

    QString strUser = strDataList[3];
    if (strUser == strNick)
        emit user_error(tr("The specified user has left the chat"));
}

/*
406 0 NO_SUCH_USER_UDGET 0@4
*/
void WebcamEngine::raw_406()
{
}

/*
408 0 NO_SUCH_USER_SUBSCRIBE LOLexx
*/
void WebcamEngine::raw_408(QStringList &strDataList)
{
    if (strDataList.size() < 4) return;

    QString strUser = strDataList[3];
    if (strUser == strNick)
        emit user_error(tr("The specified user does not have a webcam enabled"));
}

/*
410 0 FAN_GONE Merovingian
*/
void WebcamEngine::raw_410()
{
}

/*
411 0 VOTE_DENIED soneja
*/
void WebcamEngine::raw_411()
{
}

/*
412 0 SUBSCRIBE_FAILED olgusia32
*/
void WebcamEngine::raw_412(QStringList &strDataList)
{
    if (strDataList.size() < 4) return;

    QString strUser = strDataList[3];
    if (strUser == strNick)
        emit user_error(tr("Failed to retrieve the image from the webcam"));
}

/*
413 0 SUBSCRIBE_DENIED aliina
*/
void WebcamEngine::raw_413(QStringList &strDataList)
{
    if (strDataList.size() < 4) return;

    QString strUser = strDataList[3];
    if (strUser == strNick)
        emit user_error(tr("Failed to retrieve the image from the webcam"));
}

/*
418 0 QUIT_CZAT
*/
void WebcamEngine::raw_418()
{
    pWebcamNetwork->network_disconnect();
}

/*
501 0 INVALID_USER_DATA_NUM
*/
/* udget with -1 */
void WebcamEngine::raw_501()
{
}

/*
502 0 SESSION_TIMED_OUT
*/
void WebcamEngine::raw_502()
{
    pWebcamNetwork->network_disconnect();
}

/*
504 0 UNKNOWN_COMMAND PUT2
*/
void WebcamEngine::raw_504()
{
}

/*
508 0 SESSION_OVERRIDEN
*/
void WebcamEngine::raw_508()
{
}

/*
515 0 USER_DATA_TOO_LARGE
*/
void WebcamEngine::raw_515()
{
}

/*
520 0 INVALID_UOKEY 1q3j0llVg40cu2784j9EVoz8sRdfNl3w
*/
void WebcamEngine::raw_520()
{
    emit error(tr("Invalid authorization key"));
    pWebcamNetwork->network_disconnect();
}
