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

#include "dlg_kick.h"

DlgKick::DlgKick(QWidget *parent, Network *param1, QString param2, QString param3) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Kick"));

    pNetwork = param1;
    strNick = param2;
    strChannel = param3;

    ui.label_msg->setText(tr("Reason for kicking:"));
    ui.lineEdit_reason->setText(tr("No reason"));

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgKick::button_ok()
{
    QString strReason = ui.lineEdit_reason->text();
    if (strReason.isEmpty() == true)
        strReason = tr("No reason");

    pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strReason));

    ui.buttonBox->QObject::disconnect();
    this->close();
}

void DlgKick::button_cancel()
{
    ui.buttonBox->QObject::disconnect();
    this->close();
}
