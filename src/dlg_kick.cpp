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

#include "dlg_kick.h"

DlgKick::DlgKick(Network *param1, QSettings *param2, QString param3, QString param4)
{
    ui.setupUi(this);

    pNetwork = param1;
    settings = param2;
    strNick = param3;
    strChannel = param4;

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgKick::button_ok()
{
    QString strReason = ui.lineEdit->text();
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
