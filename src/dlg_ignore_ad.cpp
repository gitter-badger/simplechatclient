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

#include "dlg_ignore_ad.h"

DlgIgnoreAd::DlgIgnoreAd(Network *param1, QSettings *param2, TabContainer *param3, QString param4, QString param5)
{
    ui.setupUi(this);
    setWindowTitle(tr("Change your ignore list"));

    pNetwork = param1;
    settings = param2;
    tabc = param3;
    strStatus = param4;
    ui.lineEdit_nick->setText(param5);

    if (strStatus == "add")
        ui.label_msg->setText(tr("Enter a nickname to be added:"));
    else
        ui.label_msg->setText(tr("Enter a nickname for removal:"));

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgIgnoreAd::button_ok()
{
    QString strNick = ui.lineEdit_nick->text();

    if (strNick.isEmpty() == false)
    {
        if (strStatus == "add")
            pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
        else
            pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
    }
    else
        pNetwork->send("NS IGNORE");

    ui.buttonBox->QObject::disconnect();
    this->close();
}

void DlgIgnoreAd::button_cancel()
{
    pNetwork->send("NS IGNORE");
    ui.buttonBox->QObject::disconnect();
    this->close();
}
