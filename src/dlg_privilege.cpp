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

#include "dlg_privilege.h"

dlg_privilege::dlg_privilege(QString param1, QString param2, QString param3, QTcpSocket *param4, QTableWidget *param5, QTableWidget *param6, QTableWidget *param7, QTableWidget *param8, QSettings *param9)
{
    ui.setupUi(this);

    strStatus = param1;
    strChannel = param2;
    strAddDel = param3;
    socket = param4;
    opList = param5;
    halfopList = param6;
    banList = param7;
    inviteList = param8;
    settings = param9;

    if (strAddDel == "add")
    {
        if (strStatus == "owner")
            ui.label->setText("<p style=\"font-weight:bold;\">W³a¶ciciel kana³u mo¿e byæ tylko jeden!</p><p>Podaj nick nowego w³a¶ciciela:</p>");
        else if (strStatus == "op")
            ui.label->setText("Dodaj super operatora:");
        else if (strStatus == "halfop")
            ui.label->setText("Dodaj operatora:");
        else if (strStatus == "ban")
            ui.label->setText("Dodaj ban:");
        else if (strStatus == "invite")
            ui.label->setText("Dodaj zaproszenie:");
    }
    else
    {
        if (strStatus == "op")
            ui.label->setText("Usuñ super operatora:");
        else if (strStatus == "halfop")
            ui.label->setText("Usuñ operatora:");
        else if (strStatus == "ban")
            ui.label->setText("Usuñ ban:");
        else if (strStatus == "invite")
            ui.label->setText("Usuñ zaproszenie:");
    }

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void dlg_privilege::button_ok()
{
    QString strNick = ui.lineEdit->text();

    if (strNick.isEmpty() == false)
    {
        if (strAddDel == "add")
        {
            if (strStatus == "owner")
                dlg_privilege::send(QString("CS TRANSFER %1 %2").arg(strChannel).arg(strNick));
            else if (strStatus == "op")
                dlg_privilege::send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
            else if (strStatus == "halfop")
                dlg_privilege::send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
            else if (strStatus == "ban")
                dlg_privilege::send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
            else if (strStatus == "invite")
                dlg_privilege::send(QString("CS INVITE %1 ADD %2").arg(strChannel).arg(strNick));
        }
        else
        {
            if (strStatus == "op")
                dlg_privilege::send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
            else if (strStatus == "halfop")
                dlg_privilege::send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
            else if (strStatus == "ban")
                dlg_privilege::send(QString("CS BAN %1 DEL %2").arg(strChannel).arg(strNick));
            else if (strStatus == "invite")
                dlg_privilege::send(QString("CS INVITE %1 DEL %2").arg(strChannel).arg(strNick));
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QString("Nie podano nicka"));
        msgBox.exec();
    }

    dlg_privilege::clear();
    dlg_privilege::send(QString("CS INFO %1").arg(strChannel));
    this->hide();
}

void dlg_privilege::button_cancel()
{
    dlg_privilege::clear();
    dlg_privilege::send(QString("CS INFO %1").arg(strChannel));
    this->hide();
}

void dlg_privilege::clear()
{
    for (int i = opList->rowCount()-1; i >= 0; --i)
        opList->removeRow(i);

    for (int i = halfopList->rowCount()-1; i >= 0; --i)
        halfopList->removeRow(i);

    for (int i = banList->rowCount()-1; i >= 0; --i)
        banList->removeRow(i);

    for (int i = inviteList->rowCount()-1; i >= 0; --i)
        inviteList->removeRow(i);
}

// copy of network::send
void dlg_privilege::send(QString strData)
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
        {
            int nop;
            nop = 1;
           //tabc->show_msg("Status", QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
        }
    }
    //else
        //tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}
