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

#include "webcam_mini.h"
#include "webcam_standard.h"
#include "webcam_engine.h"
#include "webcam.h"

DlgWebcam::DlgWebcam(QString _strNick, bool _bMini) : strNick(_strNick), bMini(_bMini)
{
    if (bMini)
    {
        pWebcamMini = new DlgWebcamMini(strNick);
        pWebcamMini->show();
    }
    else
    {
        pWebcamStandard = new DlgWebcamStandard();
        pWebcamStandard->show();
    }

    pWebcamEngine = new WebcamEngine(strNick, bMini);

    createSignals();
}

DlgWebcam::~DlgWebcam()
{
    delete pWebcamEngine;

    if (bMini)
        delete pWebcamMini;
    else
        delete pWebcamStandard;
}

void DlgWebcam::createSignals()
{
    if (bMini)
    {
        connect(pWebcamEngine, SIGNAL(updateImage(QByteArray)), pWebcamMini, SLOT(updateImage(QByteArray)));
        connect(pWebcamEngine, SIGNAL(updateText(QString)), pWebcamMini, SLOT(updateText(QString)));
        connect(pWebcamEngine, SIGNAL(userError(QString)), this, SLOT(userError(QString)));
        connect(pWebcamEngine, SIGNAL(error(QString)), pWebcamMini, SLOT(error(QString)));

        connect(pWebcamMini, SIGNAL(closeCam()), this, SLOT(closeCam()));
    }
    else
    {
        connect(pWebcamEngine, SIGNAL(updateImage(QByteArray)), pWebcamStandard, SLOT(updateImage(QByteArray)));
        connect(pWebcamEngine, SIGNAL(updateText(QString)), pWebcamStandard, SLOT(updateText(QString)));
        connect(pWebcamEngine, SIGNAL(updateRank(int)), pWebcamStandard, SLOT(updateRank(int)));
        connect(pWebcamEngine, SIGNAL(updateStatus(QString)), pWebcamStandard, SLOT(updateStatus(QString)));
        connect(pWebcamEngine, SIGNAL(voteOk()), pWebcamStandard, SLOT(voteOk()));
        connect(pWebcamEngine, SIGNAL(userError(QString)), pWebcamStandard, SLOT(userError(QString)));
        connect(pWebcamEngine, SIGNAL(error(QString)), pWebcamStandard, SLOT(error(QString)));
        connect(pWebcamEngine, SIGNAL(addUser(QString,int,QString)), pWebcamStandard, SLOT(addUser(QString,int,QString)));
        connect(pWebcamEngine, SIGNAL(updateUser(QString,int,QString)), pWebcamStandard, SLOT(updateUser(QString,int,QString)));
        connect(pWebcamEngine, SIGNAL(removeUser(QString)), pWebcamStandard, SLOT(removeUser(QString)));
        connect(pWebcamEngine, SIGNAL(clearUsers()), pWebcamStandard, SLOT(clearUsers()));

        connect(pWebcamStandard, SIGNAL(closeCam()), this, SLOT(closeCam()));
        connect(pWebcamStandard, SIGNAL(networkSend(QString)), pWebcamEngine, SLOT(networkSend(QString)));
        connect(pWebcamStandard, SIGNAL(setUser(QString)), pWebcamEngine, SLOT(setUser(QString)));
    }
}

void DlgWebcam::closeCam()
{
    pWebcamEngine->closeEngine();
    this->deleteLater();
}

void DlgWebcam::userError(QString s)
{
    /* mini */
    pWebcamMini->updateText(s);
    pWebcamEngine->closeEngine();
}
