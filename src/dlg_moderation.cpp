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

#include "dlg_moderation.h"

DlgModeration::DlgModeration(QWidget *parent, QSettings *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Moderation"));

    settings = param1;

    ui.label_channel->setText(tr(" Channel: "));
    ui.pushButton_accept->setText(tr("Accept"));
    ui.pushButton_remove->setText(tr("Remove"));

    QObject::connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(button_accept()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
    QObject::connect(ui.comboBox_channels, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_changed(int)));
}

DlgModeration::~DlgModeration()
{
    channel_id.clear();
    id_nick.clear();
    id_message.clear();
}

void DlgModeration::set_active_channel(QString param1)
{
    strActiveChannel = param1;
}

void DlgModeration::add_msg(QString strID, QString strChannel, QString strNick, QString strMessage)
{
    if (combo_exist(strChannel) == false)
        ui.comboBox_channels->addItem(strChannel);

    channel_id.insert(strChannel, strID);
    id_nick.insert(strID, strNick);
    id_message.insert(strID, strMessage);

    // refresh
    QString strData = QString("<%2> %3").arg(strNick).arg(strMessage);
    if (ui.comboBox_channels->currentText() == strChannel)
    {
        if (exist_in_widget(strData) == false)
        {
            QDateTime dt = QDateTime::currentDateTime();
            QString strDT = dt.toString("[hh:mm:ss] ");
            strData = strDT+strData;

            ui.listWidget_msg->insertItem(ui.listWidget_msg->count()-1, strData);
        }
    }
}

void DlgModeration::del_msg(QString strChannel, QString strData)
{
    QStringList strDataList = strData.split(" ");
    QString strCheckNick = strDataList[0];
    strCheckNick.replace(QRegExp("<(.*)>"), "\\1");
    QString strCheckMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strCheckMessage += " "; strCheckMessage += strDataList[i]; }

    QList <QString> strIDs = channel_id.values(strChannel);
    for (int i = 0; i < strIDs.count(); i++)
    {
        QString strID = strIDs.at(i);
        QString strNick = id_nick.value(strID);
        QString strMessage = id_message.value(strID);

        if ((strCheckNick == strNick) && (strCheckMessage == strMessage))
        {
            channel_id.remove(strChannel, strID);
            id_nick.remove(strID, strNick);
            id_message.remove(strID, strMessage);
        }
    }
    strIDs.clear();

    // refresh
    for (int i = 0; i < ui.listWidget_msg->count(); i++)
    {
        if (exist_in_list(strChannel, ui.listWidget_msg->item(i)->text()) == false)
            ui.listWidget_msg->takeItem(i);
    }
}

bool DlgModeration::exist_in_widget(QString strMsg)
{
    for (int i = 0; i < ui.listWidget_msg->count(); i++)
    {
        QString strCompare = ui.listWidget_msg->item(i)->text();
        strCompare = strCompare.right(strCompare.length()-11); // minus data

        if (strCompare == strMsg)
            return true;
    }
    return false;
}

bool DlgModeration::exist_in_list(QString strChannel, QString strMsg)
{
    strMsg = strMsg.right(strMsg.length()-11); // minus date

    QList <QString> strIDs = channel_id.values(strChannel);
    for (int i = 0; i < strIDs.count(); i++)
    {
        QString strID = strIDs.at(i);
        QString strNick = id_nick.value(strID);
        QString strMessage = id_message.value(strID);
        QString strData = QString("<%1> %2").arg(strNick).arg(strMessage);

        if (strMsg == strData)
            return true;
    }
    return false;
}

int DlgModeration::combo_id(QString strChannel)
{
    for (int i = 0; i < ui.comboBox_channels->count(); i++)
    {
        if (ui.comboBox_channels->itemText(i) == strChannel)
            return i;
    }
    return 0;
}

bool DlgModeration::combo_exist(QString strChannel)
{
    for (int i = 0; i < ui.comboBox_channels->count(); i++)
    {
        if (ui.comboBox_channels->itemText(i) == strChannel)
            return true;
    }
    return false;
}

void DlgModeration::combo_changed(int index)
{
    ui.listWidget_msg->clear();

    QString strChannel = ui.comboBox_channels->itemText(index);

    QList <QString> strIDs = channel_id.values(strChannel);
    for (int i = 0; i < strIDs.count(); i++)
    {
        QString strID = strIDs.at(i);
        QString strNick = id_nick.value(strID);
        QString strMessage = id_message.value(strID);
        QString strData = QString("<%2> %3").arg(strNick).arg(strMessage);
        if (exist_in_widget(strData) == false)
        {
            QDateTime dt = QDateTime::currentDateTime();
            QString strDT = dt.toString("[hh:mm:ss] ");
            strData = strDT+strData;

            ui.listWidget_msg->insertItem(ui.listWidget_msg->count()-1, strData);
        }
    }
    strIDs.clear();
}

void DlgModeration::button_accept()
{
    if (ui.listWidget_msg->selectedItems().count() == 0)
        return;

    QString strChannel = ui.comboBox_channels->currentText();
    QString strData = ui.listWidget_msg->selectedItems().at(0)->text();
    strData = strData.right(strData.length()-11); // minus date
    QStringList strDataList = strData.split(" ");

    QString strNick = strDataList[0];
    strNick.replace(QRegExp("<(.*)>"), "\\1");

    QString strMessage;
    for (int i = 1; i < strDataList.size(); i++) { if (i != 1) strMessage += " "; strMessage += strDataList[i]; }

    QString strSend = QString("MODERMSG %1 - %2 :%3").arg(strNick).arg(strChannel).arg(strMessage);
    emit send(strSend);

    QString strDisplay = QString("<%1> %2").arg(strNick).arg(strMessage);
    emit display_msg(strChannel, strDisplay, 0);

    del_msg(strChannel, strData);
}

void DlgModeration::button_remove()
{
    if (ui.listWidget_msg->selectedItems().count() == 0)
        return;

    QString strChannel = ui.comboBox_channels->currentText();
    QString strData = ui.listWidget_msg->selectedItems().at(0)->text();
    strData = strData.right(strData.length()-11); // minus date

    del_msg(strChannel, strData);
}

void DlgModeration::button_close()
{
    this->hide();
}

void DlgModeration::showEvent(QShowEvent *event)
{
    event->accept();

    int iId = combo_id(strActiveChannel);
    ui.comboBox_channels->setCurrentIndex(iId);
    combo_changed(iId);
}
