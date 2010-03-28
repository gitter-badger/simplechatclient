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

#include "dlg_moderation.h"

dlg_moderation::dlg_moderation(QSettings *param1, QTcpSocket *param2)
{
    ui.setupUi(this);

    settings = param1;
    socket = param2;

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(button_accept()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
    QObject::connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_changed(int)));
}

dlg_moderation::~dlg_moderation()
{
    channel_id.clear();
    id_nick.clear();
    id_message.clear();
}

void dlg_moderation::add_msg(QString strID, QString strChannel, QString strNick, QString strMessage)
{
    if (dlg_moderation::combo_exist(strChannel) == false)
        ui.comboBox->addItem(strChannel);

    channel_id.insert(strChannel, strID);
    id_nick.insert(strID, strNick);
    id_message.insert(strID, strMessage);

    dlg_moderation::refresh(strChannel);
}

void dlg_moderation::del_msg(QString strData)
{
    QStringList strDataList = strData.split(" ");
    QString strNick = strDataList[0];
    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    QString strID;
    QList <QString> strIDs = id_message.keys(strMessage);
    if (strIDs.count() == 1)
    {
        strID = strIDs.at(0);
        QString strChannel;
        QList <QString> strChannels = channel_id.keys(strID);
        if (strChannels.count() == 1)
        {
            strChannel = strChannels.at(0);
            channel_id.remove(strChannel, strID);
            id_nick.remove(strID, strNick);
            id_message.remove(strID, strMessage);

            dlg_moderation::refresh(strChannel);
        }
    }
}

void dlg_moderation::refresh(QString strChannel)
{
    if (ui.comboBox->currentText() == strChannel)
       ui.listWidget->clear();

    QList <QString> ids = channel_id.values(strChannel);
    for (int i = 0; i < ids.count(); i++)
    {
        QString strID = ids.at(i);
        QString strNick = id_nick.value(strID);
        QString strMessage = id_message.value(strID);
        QString strData = QString("%1 %2").arg(strNick).arg(strMessage);
        if (ui.comboBox->currentText() == strChannel)
            ui.listWidget->insertItem(ui.listWidget->count()-1, strData);
    }
}

bool dlg_moderation::combo_exist(QString strChannel)
{
    for (int i = 0; i < ui.comboBox->count(); i++)
    {
        if (ui.comboBox->itemText(i) == strChannel)
            return true;
    }
    return false;
}

void dlg_moderation::combo_changed(int index)
{
    QString strChannel = ui.comboBox->itemText(index);
    dlg_moderation::refresh(strChannel);
}

void dlg_moderation::button_accept()
{
    if (ui.listWidget->selectedItems().count() == 0)
        return;

    QString strData = ui.listWidget->selectedItems().at(0)->text();
    QStringList strDataList = strData.split(" ");

    QString strChannel = ui.comboBox->currentText();
    QString strNick = strDataList[0];
    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    QString strDisplay = QString("MODERMSG %1 - %2 :%3").arg(strNick).arg(strChannel).arg(strMessage);
    dlg_moderation::send(strDisplay);
    dlg_moderation::del_msg(strData);
}

void dlg_moderation::button_remove()
{
    if (ui.listWidget->selectedItems().count() == 0)
        return;

    QString strData = ui.listWidget->selectedItems().at(0)->text();
    dlg_moderation::del_msg(strData);
}

void dlg_moderation::button_close()
{
    this->hide();
}

// copy of network::send
void dlg_moderation::send(QString strData)
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
        {
            int nop;
            nop = 1;
           //tabc->show_msg("Status", QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
        }
    }
    //else
        //tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}
