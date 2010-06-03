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

#include "dlg_channel_homes_ad.h"

dlg_channel_homes_ad::dlg_channel_homes_ad(network *param1, tab_container *param2, QSettings *param3, QString param4)
{
    ui.setupUi(this);

    pNetwork = param1;
    tabc = param2;
    settings = param3;
    strStatus = param4;

    if (strStatus == "create")
        ui.label->setText("Podaj nazwê nowego kana³u:");
    else
        ui.label->setText("<p style=\"font-weight:bold;\">Usuniêcie kana³u jest operacj± nieodwracaln±!</p><p>Podaj nazwê kana³u do usuniêcia:</p>");

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void dlg_channel_homes_ad::button_ok()
{
    QString strChannel = ui.lineEdit->text();

    if (strChannel.isEmpty() == false)
    {
        if (strStatus == "create")
            pNetwork->send(QString("CS REGISTER %1").arg(strChannel));
        else
            pNetwork->send(QString("CS DROP %1").arg(strChannel));
    }
    else
        pNetwork->send("CS HOMES");

    ui.buttonBox->QObject::disconnect();
    this->close();
}

void dlg_channel_homes_ad::button_cancel()
{
    pNetwork->send("CS HOMES");
    ui.buttonBox->QObject::disconnect();
    this->close();
}
