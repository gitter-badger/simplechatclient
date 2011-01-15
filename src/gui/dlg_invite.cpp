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

#include "dlg_invite.h"

DlgInvite::DlgInvite(QWidget *parent, Network *param1, QString param2, QString param3) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Invitation"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    pNetwork = param1;
    strNick = param2;
    strChannel = param3;

    ui.pushButton_whois->setIcon(QIcon(":/images/oxygen/16x16/user-properties.png"));
    ui.pushButton_reject->setIcon(QIcon(":/images/oxygen/16x16/user-invisible.png"));
    ui.pushButton_ignore->setIcon(QIcon(":/images/oxygen/16x16/user-busy.png"));
    ui.pushButton_accept->setIcon(QIcon(":/images/oxygen/16x16/user-online.png"));

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
    this->close();
}

void DlgInvite::button_ignore()
{
    pNetwork->send(QString("INVIGNORE %1 %2").arg(strNick).arg(strChannel));
    this->close();
}

void DlgInvite::button_accept()
{
    pNetwork->send(QString("JOIN %1").arg(strChannel));
    this->close();
}
