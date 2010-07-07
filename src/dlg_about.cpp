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

#include "dlg_about.h"

DlgAbout::DlgAbout(QWidget *parent, QSettings *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    settings = param1;

    ui.labelLogo->setPixmap(QPixmap(":/images/logo_128.png"));

    QString strVersion = settings->value("version").toString();
    QString strDescription = "<p style=\"font-size:18px;font-weight:bold\">Simple Chat Client</p>";
    strDescription += "<p>"+tr("Version")+" "+strVersion+"</p>";
    strDescription += "<p>&copy; 2010 Piotr £uczko</p>";
    strDescription += "<p><a style=\"text-decoration:none;\" href=\"http://simplechatclien.sourceforge.net/\">"+tr("Home page")+"</a></p>";
    strDescription += "<p>"+tr("License")+" <a style=\"text-decoration:none;\" href=\"http://www.fsf.org/licensing/licenses/gpl.html\">GPL v3</a></p>";

    ui.labelDescription->setText(strDescription);

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
}

void DlgAbout::button_ok()
{
    ui.buttonBox->QObject::disconnect();
    this->close();
}
