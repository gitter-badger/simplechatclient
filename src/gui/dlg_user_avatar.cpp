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

#include "dlg_user_avatar.h"

DlgUserAvatar::DlgUserAvatar(QWidget *parent, QPixmap param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Avatar"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    myparent = parent;
    avatar = param1;

    // slider value 5
    ui.label_avatar->setPixmap(avatar.scaled(QSize(250,250)));

    QObject::connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slider_value_changed(int)));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
}

void DlgUserAvatar::slider_value_changed(int iValue)
{
    int iSize = 50;
    iSize *= iValue;

    ui.label_avatar->setPixmap(avatar.scaled(QSize(iSize,iSize)));
}

void DlgUserAvatar::button_ok()
{
    close();
}
