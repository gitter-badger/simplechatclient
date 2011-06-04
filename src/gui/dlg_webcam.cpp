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

#include "dlg_webcam_mini.h"
#include "dlg_webcam_standard.h"
#include "webcam_engine.h"
#include "dlg_webcam.h"

DlgWebcam::DlgWebcam(QString n, bool m)
{
    strNick = n;
    bMini = m;

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

    create_signals();
}

DlgWebcam::~DlgWebcam()
{
    if (bMini)
        delete pWebcamMini;
    else
        delete pWebcamStandard;

    delete pWebcamEngine;
}

void DlgWebcam::create_signals()
{
    if (bMini)
    {
        QObject::connect(pWebcamEngine, SIGNAL(update_image(QByteArray)), pWebcamMini, SLOT(update_image(QByteArray)));
        QObject::connect(pWebcamEngine, SIGNAL(update_text(QString)), pWebcamMini, SLOT(update_text(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(user_error(QString)), this, SLOT(user_error(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(error(QString)), pWebcamMini, SLOT(error(QString)));

        QObject::connect(pWebcamMini, SIGNAL(close_cam()), this, SLOT(close_cam()));
    }
    else
    {
        QObject::connect(pWebcamEngine, SIGNAL(update_image(QByteArray)), pWebcamStandard, SLOT(update_image(QByteArray)));
        QObject::connect(pWebcamEngine, SIGNAL(update_text(QString)), pWebcamStandard, SLOT(update_text(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(update_rank(int)), pWebcamStandard, SLOT(update_rank(int)));
        QObject::connect(pWebcamEngine, SIGNAL(update_status(QString)), pWebcamStandard, SLOT(update_status(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(vote_ok()), pWebcamStandard, SLOT(vote_ok()));
        QObject::connect(pWebcamEngine, SIGNAL(user_error(QString)), pWebcamStandard, SLOT(user_error(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(error(QString)), pWebcamStandard, SLOT(error(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(add_user(QString,int,QString)), pWebcamStandard, SLOT(add_user(QString,int,QString)));
        QObject::connect(pWebcamEngine, SIGNAL(update_user(QString,int,QString)), pWebcamStandard, SLOT(update_user(QString,int,QString)));
        QObject::connect(pWebcamEngine, SIGNAL(remove_user(QString)), pWebcamStandard, SLOT(remove_user(QString)));
        QObject::connect(pWebcamEngine, SIGNAL(clear_users()), pWebcamStandard, SLOT(clear_users()));

        QObject::connect(pWebcamStandard, SIGNAL(close_cam()), this, SLOT(close_cam()));
        QObject::connect(pWebcamStandard, SIGNAL(network_send(QString)), pWebcamEngine, SLOT(network_send(QString)));
        QObject::connect(pWebcamStandard, SIGNAL(set_user(QString)), pWebcamEngine, SLOT(set_user(QString)));
    }
}

void DlgWebcam::close_cam()
{
    pWebcamEngine->close_engine();
    this->deleteLater();
}

void DlgWebcam::user_error(QString s)
{
    /* mini */
    pWebcamMini->update_text(s);
    pWebcamEngine->close_engine();
}
