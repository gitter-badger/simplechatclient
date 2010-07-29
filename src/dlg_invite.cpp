/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "dlg_invite.h"

DlgInvite::DlgInvite(Network *param1, QSettings *param2, TabContainer *param3, QString param4, QString param5)
{
    ui.setupUi(this);
    setWindowTitle(tr("Invitation"));

    pNetwork = param1;
    settings = param2;
    tabc = param3;
    strNick = param4;
    strChannel = param5;

    ui.pushButton_whois->setText(tr("Whois"));
    ui.pushButton_reject->setText(tr("Reject"));
    ui.pushButton_ignore->setText(tr("Ignore"));
    ui.pushButton_accept->setText(tr("Accept"));

    if (strChannel[0] == '^')
        ui.label_msg->setText(QString(tr("%1 invites you to priv")).arg(strNick));
    else
        ui.label_msg->setText(QString(tr("%1 invites you to channel %2")).arg(strNick).arg(strChannel));

    QObject::connect(ui.pushButton_whois, SIGNAL(clicked()), this, SLOT(button_whois()));
    QObject::connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(button_reject()));
    QObject::connect(ui.pushButton_ignore, SIGNAL(clicked()), this, SLOT(button_ignore()));
    QObject::connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(button_accept()));
}

void DlgInvite::button_whois()
{
    pNetwork->send(QString("WHOIS %1 %1").arg(strNick));
}

void DlgInvite::button_reject()
{
    pNetwork->send(QString("INVREJECT %1 %2").arg(strNick).arg(strChannel));

    ui.pushButton_whois->QObject::disconnect();
    ui.pushButton_accept->QObject::disconnect();
    ui.pushButton_ignore->QObject::disconnect();
    ui.pushButton_reject->QObject::disconnect();
    this->close();
}

void DlgInvite::button_ignore()
{
    pNetwork->send(QString("INVIGNORE %1 %2").arg(strNick).arg(strChannel));

    ui.pushButton_whois->QObject::disconnect();
    ui.pushButton_accept->QObject::disconnect();
    ui.pushButton_ignore->QObject::disconnect();
    ui.pushButton_reject->QObject::disconnect();
    this->close();
}

void DlgInvite::button_accept()
{
    pNetwork->send(QString("JOIN %1").arg(strChannel));

// rename if priv
    if (strChannel[0] == '^')
    {
        strTimerChannel = strChannel;
        strTimerNick = strNick;
        QTimer::singleShot(1000*3, this, SLOT(timer_timeout()));
    }

    ui.pushButton_whois->QObject::disconnect();
    ui.pushButton_accept->QObject::disconnect();
    ui.pushButton_ignore->QObject::disconnect();
    ui.pushButton_reject->QObject::disconnect();
    this->close();
}

void DlgInvite::timer_timeout()
{
    tabc->rename_tab(strTimerChannel, strTimerNick);
    strTimerChannel = QString::null;
    strTimerNick = QString::null;
}
