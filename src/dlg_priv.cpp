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

#include "dlg_priv.h"

DlgPriv::DlgPriv(Network *param1, QSettings *param2, TabContainer *param3, QString param4, QString param5)
{
    ui.setupUi(this);

    pNetwork = param1;
    settings = param2;
    tabc = param3;
    strNick = param4;
    strChannel = param5;

    ui.label->setText(QString("%1 zaprasza do rozmowy prywatnej").arg(strNick));

    QObject::connect(ui.pushButtonWhois, SIGNAL(clicked()), this, SLOT(button_whois()));
    QObject::connect(ui.pushButtonReject, SIGNAL(clicked()), this, SLOT(button_reject()));
    QObject::connect(ui.pushButtonIgnore, SIGNAL(clicked()), this, SLOT(button_ignore()));
    QObject::connect(ui.pushButtonAccept, SIGNAL(clicked()), this, SLOT(button_accept()));
}

void DlgPriv::button_whois()
{
    pNetwork->send(QString("WHOIS %1 %1").arg(strNick));
}

void DlgPriv::button_reject()
{
    pNetwork->send(QString("INVREJECT %1 %2").arg(strNick).arg(strChannel));

    ui.pushButtonWhois->QObject::disconnect();
    ui.pushButtonAccept->QObject::disconnect();
    ui.pushButtonIgnore->QObject::disconnect();
    ui.pushButtonReject->QObject::disconnect();
    this->close();
}

void DlgPriv::button_ignore()
{
    pNetwork->send(QString("INVIGNORE %1 %2").arg(strNick).arg(strChannel));

    ui.pushButtonWhois->QObject::disconnect();
    ui.pushButtonAccept->QObject::disconnect();
    ui.pushButtonIgnore->QObject::disconnect();
    ui.pushButtonReject->QObject::disconnect();
    this->close();
}

void DlgPriv::button_accept()
{
    pNetwork->send(QString("JOIN %1").arg(strChannel));
    strTimerChannel = strChannel;
    strTimerNick = strNick;
    QTimer::singleShot(1000, this, SLOT(timer_timeout()));

    ui.pushButtonWhois->QObject::disconnect();
    ui.pushButtonAccept->QObject::disconnect();
    ui.pushButtonIgnore->QObject::disconnect();
    ui.pushButtonReject->QObject::disconnect();
    this->close();
}

void DlgPriv::timer_timeout()
{
    tabc->rename_tab(strTimerChannel, strTimerNick);
    strTimerChannel = QString::null;
    strTimerNick = QString::null;
}
