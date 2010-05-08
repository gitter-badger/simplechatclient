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

#include "dlg_channel_favourites.h"

dlg_channel_favourites::dlg_channel_favourites(QSettings *param1, QTcpSocket *param2, tab_container *param3)
{
    ui.setupUi(this);

    settings = param1;
    socket = param2;
    tabc = param3;

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void dlg_channel_favourites::add_channel(QString strChannel)
{
    ui.listWidget->addItem(new QListWidgetItem(QIcon(":/images/channel_avatar.png"), strChannel));
}

void dlg_channel_favourites::clear()
{
    ui.listWidget->clear();
}

void dlg_channel_favourites::button_add()
{
    ui.listWidget->clear();
    (new dlg_channel_favourites_ad(socket, tabc, settings, "add", ""))->show();
}

void dlg_channel_favourites::button_remove()
{
    QString strSelected;
    if (ui.listWidget->selectedItems().count() != 0)
        strSelected = ui.listWidget->selectedItems().at(0)->text();

    ui.listWidget->clear();
    (new dlg_channel_favourites_ad(socket, tabc, settings, "remove", strSelected))->show();
}

void dlg_channel_favourites::button_ok()
{
    ui.listWidget->clear();
    this->hide();
}

void dlg_channel_favourites::button_cancel()
{
    ui.listWidget->clear();
    this->hide();
}

// copy of network::send
void dlg_channel_favourites::send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
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
        if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->waitForBytesWritten() == false))
           tabc->show_msg("Status", QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
    }
    else
        tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

void dlg_channel_favourites::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget->clear();
    dlg_channel_favourites::send("NS FAVOURITES");
}
