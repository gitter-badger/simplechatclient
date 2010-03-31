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

#include "dlg_ignore.h"

dlg_ignore::dlg_ignore(QSettings *param1, QTcpSocket *param2, tab_container *param3)
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

void dlg_ignore::add_ignore(QString strNick)
{
    QListWidgetItem *nick;
    nick = new QListWidgetItem(QIcon(":/3rdparty/images/people.png"), strNick);
    ui.listWidget->addItem(nick);
}

void dlg_ignore::clear()
{
    ui.listWidget->clear();
}

void dlg_ignore::button_add()
{
    ui.listWidget->clear();
    (new dlg_ignore_ad(socket, tabc, settings, "add"))->show();
}

void dlg_ignore::button_remove()
{
    ui.listWidget->clear();
    (new dlg_ignore_ad(socket, tabc, settings, "remove"))->show();
}

void dlg_ignore::button_ok()
{
    this->hide();
}

void dlg_ignore::button_cancel()
{
    this->hide();
}

// copy of network::send
void dlg_ignore::send(QString strData)
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

void dlg_ignore::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget->clear();
    dlg_ignore::send("NS IGNORE");
}
