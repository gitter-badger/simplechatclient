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

dlg_privilege::dlg_privilege(network *param1, QSettings *param2, QString param3, QString param4, QString param5, QTableWidget *param6, QTableWidget *param7, QTableWidget *param8, QTableWidget *param9)
{
    ui.setupUi(this);

    pNetwork = param1;
    settings = param2;
    strStatus = param3;
    strChannel = param4;
    strAddDel = param5;
    opList = param6;
    halfopList = param7;
    banList = param8;
    inviteList = param9;

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
                pNetwork->send(QString("CS TRANSFER %1 %2").arg(strChannel).arg(strNick));
            else if (strStatus == "op")
                pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
            else if (strStatus == "halfop")
                pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
            else if (strStatus == "ban")
                pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
            else if (strStatus == "invite")
                pNetwork->send(QString("CS INVITE %1 ADD %2").arg(strChannel).arg(strNick));
        }
        else
        {
            if (strStatus == "op")
                pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
            else if (strStatus == "halfop")
                pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
            else if (strStatus == "ban")
                pNetwork->send(QString("CS BAN %1 DEL %2").arg(strChannel).arg(strNick));
            else if (strStatus == "invite")
                pNetwork->send(QString("CS INVITE %1 DEL %2").arg(strChannel).arg(strNick));
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setWindowTitle("Error");
        msgBox.setText(QString("Nie podano nicka"));
        msgBox.exec();
    }

    dlg_privilege::clear();
    pNetwork->send(QString("CS INFO %1").arg(strChannel));

    ui.buttonBox->QObject::disconnect();
    this->close();
}

void dlg_privilege::button_cancel()
{
    dlg_privilege::clear();
    pNetwork->send(QString("CS INFO %1").arg(strChannel));

    ui.buttonBox->QObject::disconnect();
    this->close();
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
