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

dlg_priv::dlg_priv(network *param1, tab_container *param2, QSettings *param3, QString param4, QString param5)
{
    ui.setupUi(this);

    pNetwork = param1;
    tabc = param2;
    settings = param3;
    strNick = param4;
    strChannel = param5;

    ui.label->setText(QString("%1 zaprasza do rozmowy prywatnej").arg(strNick));

    QObject::connect(ui.pushButtonWhois, SIGNAL(clicked()), this, SLOT(button_whois()));
    QObject::connect(ui.pushButtonReject, SIGNAL(clicked()), this, SLOT(button_reject()));
    QObject::connect(ui.pushButtonIgnore, SIGNAL(clicked()), this, SLOT(button_ignore()));
    QObject::connect(ui.pushButtonAccept, SIGNAL(clicked()), this, SLOT(button_accept()));
}

void dlg_priv::button_whois()
{
    pNetwork->send(QString("WHOIS %1 %1").arg(strNick));
}

void dlg_priv::button_reject()
{
    pNetwork->send(QString("INVREJECT %1 %2").arg(strNick).arg(strChannel));

    ui.pushButtonWhois->QObject::disconnect();
    ui.pushButtonAccept->QObject::disconnect();
    ui.pushButtonIgnore->QObject::disconnect();
    ui.pushButtonReject->QObject::disconnect();
    this->close();
}

void dlg_priv::button_ignore()
{
    pNetwork->send(QString("INVIGNORE %1 %2").arg(strNick).arg(strChannel));

    ui.pushButtonWhois->QObject::disconnect();
    ui.pushButtonAccept->QObject::disconnect();
    ui.pushButtonIgnore->QObject::disconnect();
    ui.pushButtonReject->QObject::disconnect();
    this->close();
}

void dlg_priv::button_accept()
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

void dlg_priv::timer_timeout()
{
    tabc->rename_tab(strTimerChannel, strTimerNick);
    strTimerChannel = QString::null;
    strTimerNick = QString::null;
}
