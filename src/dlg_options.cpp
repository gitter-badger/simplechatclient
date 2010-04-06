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

#include "dlg_options.h"

dlg_options::dlg_options(QSettings *param1)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    settings = param1;

    config *pConfig = new config();
    QString strNick = pConfig->get_value("login-nick");
    QString strPass = pConfig->get_value("login-pass");
    QString strDebugAll = pConfig->get_value("debug_all");
    QString strShowZuo = pConfig->get_value("show_zuo");
    QString strHideFormating = pConfig->get_value("hide_formating");
    QString strHideJoinPart = pConfig->get_value("hide_join_part");
    delete pConfig;

    if (strPass.isEmpty() == false)
    {
        qcrypt *pCrypt = new qcrypt();
        strPass = pCrypt->decrypt(strNick, strPass);
        delete pCrypt;
    }

    ui.lineEditNick->setText(strNick);
    ui.lineEditPass->setText(strPass);

    if (strDebugAll == "on")
        ui.checkBox->setChecked(true);
    else
        ui.checkBox->setChecked(false);

    if (strShowZuo == "on")
        ui.checkBox_2->setChecked(true);
    else
        ui.checkBox_2->setChecked(false);

    if (strHideFormating == "on")
        ui.checkBox_3->setChecked(true);
    else
        ui.checkBox_3->setChecked(false);

    if (strHideJoinPart == "on")
        ui.checkBox_4->setChecked(true);
    else
        ui.checkBox_4->setChecked(false);

    QObject::connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(debug_all()));
    QObject::connect(ui.checkBox_2, SIGNAL(clicked()), this, SLOT(show_zuo()));
    QObject::connect(ui.checkBox_3, SIGNAL(clicked()), this, SLOT(hide_formating()));
    QObject::connect(ui.checkBox_4, SIGNAL(clicked()), this, SLOT(hide_join_part()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void dlg_options::debug_all()
{
    config *pConfig = new config();
    if (ui.checkBox->isChecked() == true)
    {
        pConfig->set_value("debug_all", "on");
        settings->setValue("debug_all", "on");
    }
    else
    {
        pConfig->set_value("debug_all", "off");
        settings->setValue("debug_all", "off");
    }
    delete pConfig;
}

void dlg_options::show_zuo()
{
    config *pConfig = new config();
    if (ui.checkBox_2->isChecked() == true)
    {
        pConfig->set_value("show_zuo", "on");
        settings->setValue("show_zuo", "on");
    }
    else
    {
        pConfig->set_value("show_zuo", "off");
        settings->setValue("show_zuo", "off");
    }
    delete pConfig;
}

void dlg_options::hide_formating()
{
    config *pConfig = new config();
    if (ui.checkBox_3->isChecked() == true)
    {
        pConfig->set_value("hide_formating", "on");
        settings->setValue("hide_formating", "on");
    }
    else
    {
        pConfig->set_value("hide_formating", "off");
        settings->setValue("hide_formating", "off");
    }
    delete pConfig;
}

void dlg_options::hide_join_part()
{
    config *pConfig = new config();
    if (ui.checkBox_4->isChecked() == true)
    {
        pConfig->set_value("hide_join_part", "on");
        settings->setValue("hide_join_part", "on");
    }
    else
    {
        pConfig->set_value("hide_join_part", "off");
        settings->setValue("hide_join_part", "off");
    }
    delete pConfig;
}

void dlg_options::button_cancel()
{
    this->close();
}

void dlg_options::button_ok()
{
    QString strNick = ui.lineEditNick->text();
    QString strPass = ui.lineEditPass->text();

    if (strPass.isEmpty() == false)
    {
        qcrypt *pCrypt = new qcrypt();
        strPass = pCrypt->encrypt(strNick, strPass);
        delete pCrypt;
    }

    config *pConfig = new config();
    pConfig->set_value("login-nick", strNick);
    pConfig->set_value("login-pass", strPass);
    delete pConfig;

    this->close();
}
