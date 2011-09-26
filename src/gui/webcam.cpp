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
        QObject::connect(pWebcamEngine, SIGNAL(updateImage(QByteArray)), pWebcamMini, SLOT(updateImage(QByteArray)));
        QObject::connect(pWebcamEngine, SIGNAL(updateText(QString)), pWebcamMini, SLOT(updateText(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(userError(QString)), this, SLOT(userError(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(error(QString)), pWebcamMini, SLOT(error(QString)));

        QObject::connect(pWebcamMini, SIGNAL(closeCam()), this, SLOT(closeCam()));
    }
    else
    {
        QObject::connect(pWebcamEngine, SIGNAL(updateImage(QByteArray)), pWebcamStandard, SLOT(updateImage(QByteArray)));
        QObject::connect(pWebcamEngine, SIGNAL(updateText(QString)), pWebcamStandard, SLOT(updateText(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(updateRank(int)), pWebcamStandard, SLOT(updateRank(int)));
        QObject::connect(pWebcamEngine, SIGNAL(updateStatus(QString)), pWebcamStandard, SLOT(updateStatus(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(voteOk()), pWebcamStandard, SLOT(voteOk()));
        QObject::connect(pWebcamEngine, SIGNAL(userError(QString)), pWebcamStandard, SLOT(userError(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(error(QString)), pWebcamStandard, SLOT(error(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(addUser(QString,int,QString)), pWebcamStandard, SLOT(addUser(QString,int,QString)));
        QObject::connect(pWebcamEngine, SIGNAL(updateUser(QString,int,QString)), pWebcamStandard, SLOT(updateUser(QString,int,QString)));
        QObject::connect(pWebcamEngine, SIGNAL(removeUser(QString)), pWebcamStandard, SLOT(removeUser(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(clearUsers()), pWebcamStandard, SLOT(clearUsers()));

        QObject::connect(pWebcamStandard, SIGNAL(closeCam()), this, SLOT(closeCam()));
        QObject::connect(pWebcamStandard, SIGNAL(networkSend(QString)), pWebcamEngine, SLOT(networkSend(QString)));
        QObject::connect(pWebcamStandard, SIGNAL(setUser(QString)), pWebcamEngine, SLOT(setUser(QString)));
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
