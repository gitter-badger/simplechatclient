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

#include "dlg_channel_homes_ad.h"

DlgChannelHomesAd::DlgChannelHomesAd(QWidget *parent, Network *param1, QSettings *param2, TabContainer *param3, QString param4) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Changing channels"));

    pNetwork = param1;
    settings = param2;
    tabc = param3;
    strStatus = param4;

    if (strStatus == "create")
        ui.label_msg->setText(tr("Enter the name of the new channel:"));
    else
        ui.label_msg->setText("<p style=\"font-weight:bold;\">"+tr("The removal of the channel operation is irreversible!")+"</p><p>"+tr("Enter the name of the channel to remove:")+"</p>");

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgChannelHomesAd::button_ok()
{
    QString strChannel = ui.lineEdit_channel->text();

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

void DlgChannelHomesAd::button_cancel()
{
    pNetwork->send("CS HOMES");
    ui.buttonBox->QObject::disconnect();
    this->close();
}
