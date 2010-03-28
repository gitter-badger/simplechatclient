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

dlg_channel_homes_ad::dlg_channel_homes_ad(QTcpSocket *param1, tab_container *param2, QSettings *param3, QString param4)
{
    ui.setupUi(this);

    socket = param1;
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
            dlg_channel_homes_ad::send(QString("CS REGISTER %1").arg(strChannel));
        else
            dlg_channel_homes_ad::send(QString("CS DROP %1").arg(strChannel));
    }
    else
        dlg_channel_homes_ad::send("CS HOMES");

    this->hide();
}

void dlg_channel_homes_ad::button_cancel()
{
    dlg_channel_homes_ad::send("CS HOMES");
    this->hide();
}

// copy of network::send
void dlg_channel_homes_ad::send(QString strData)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "-> " << strData;
#endif
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        socket->write(qbaData);
        if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->waitForBytesWritten(30*1000) == false))
           tabc->show_msg("Status", QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
    }
    else
        tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}
