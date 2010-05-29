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

#include "dlg_channel_homes.h"

dlg_channel_homes::dlg_channel_homes(QSettings *param1, QTcpSocket *param2, tab_container *param3, dlg_channel_settings *param4)
{
    ui.setupUi(this);

    settings = param1;
    socket = param2;
    tabc = param3;
    dlgchannel_settings = param4;

    QObject::connect(ui.listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(list_clicked(QModelIndex)));
    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(button_create()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void dlg_channel_homes::add_channel(QString strChannel)
{
    strChannel = strChannel.right(strChannel.length()-1); // remove status
    ui.listWidget->addItem(new QListWidgetItem(QIcon(":/images/channel_avatar.png"), strChannel));
}

void dlg_channel_homes::clear()
{
    ui.listWidget->clear();
}

void dlg_channel_homes::list_clicked(QModelIndex index)
{
    int i = index.row();
    QString strChannel = ui.listWidget->item(i)->text();
    dlgchannel_settings->set_channel(strChannel);
    dlgchannel_settings->show();
    ui.listWidget->clear();
    this->hide();
}

void dlg_channel_homes::button_create()
{
    ui.listWidget->clear();
    (new dlg_channel_homes_ad(socket, tabc, settings, "create"))->show();
}

void dlg_channel_homes::button_remove()
{
    ui.listWidget->clear();
    (new dlg_channel_homes_ad(socket, tabc, settings, "remove"))->show();
}

void dlg_channel_homes::button_ok()
{
    ui.listWidget->clear();
    this->hide();
}

void dlg_channel_homes::button_cancel()
{
    ui.listWidget->clear();
    this->hide();
}

// copy of network::send
void dlg_channel_homes::send(QString strData)
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

        if (socket->write(qbaData) == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
                tabc->show_msg_active(QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
            else if (socket->state() == QAbstractSocket::UnconnectedState)
                tabc->show_msg_active("Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
        }
    }
    else
        tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

void dlg_channel_homes::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget->clear();

    dlg_channel_homes::send("CS HOMES");
}
