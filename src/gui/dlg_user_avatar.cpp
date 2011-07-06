/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include <QDesktopWidget>
#include <QPushButton>
#include "mainwindow.h"
#include "dlg_user_avatar.h"

DlgUserAvatar::DlgUserAvatar(MainWindow *parent, QPixmap param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Avatar"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    avatar = param1;

    createGui();
    setDefaultValues();
    createSignals();
}

void DlgUserAvatar::createGui()
{
    ui.toolButton_zoom_out->setIcon(QIcon(":/images/oxygen/16x16/zoom-out.png"));
    ui.toolButton_zoom_in->setIcon(QIcon(":/images/oxygen/16x16/zoom-in.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));
}

void DlgUserAvatar::setDefaultValues()
{
    // slider value 5
    ui.label_avatar->setPixmap(avatar.scaled(QSize(250,250)));
}

void DlgUserAvatar::createSignals()
{
    QObject::connect(ui.toolButton_zoom_out, SIGNAL(clicked()), this, SLOT(buttonZoomOut()));
    QObject::connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    QObject::connect(ui.toolButton_zoom_in, SIGNAL(clicked()), this, SLOT(buttonZoomIn()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgUserAvatar::buttonZoomOut()
{
    int value = ui.horizontalSlider->value();
    value--;

    if (value >= ui.horizontalSlider->minimum())
        ui.horizontalSlider->setValue(value);
}

void DlgUserAvatar::sliderValueChanged(int iValue)
{
    int iSize = 50;
    iSize *= iValue;

    ui.label_avatar->setPixmap(avatar.scaled(QSize(iSize,iSize)));
}

void DlgUserAvatar::buttonZoomIn()
{
    int value = ui.horizontalSlider->value();
    value++;

    if (value <= ui.horizontalSlider->maximum())
        ui.horizontalSlider->setValue(value);
}
