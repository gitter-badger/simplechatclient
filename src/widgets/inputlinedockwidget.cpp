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

#include "inputlinedockwidget.h"

InputLineDockWidget::InputLineDockWidget(QWidget *parent, Network *param1, DlgChannelSettings *param2, DlgModeration *param3) : QWidget(parent)
{
    setMinimumHeight(60);
    setMaximumHeight(60);
    myparent = parent;

    pNetwork = param1;
    dlgchannel_settings = param2;
    dlgmoderation = param3;

    QVBoxLayout *mainLayout = new QVBoxLayout();

    pInputWidget = new InputWidget(myparent, pNetwork);
    pInputWidget->show();

    pToolWidget = new ToolWidget(myparent, pNetwork, pInputWidget, dlgchannel_settings, dlgmoderation);
    pToolWidget->show();

    mainLayout->addWidget(pToolWidget);
    mainLayout->addWidget(pInputWidget);
    this->setLayout(mainLayout);

    QObject::connect(this, SIGNAL(update_nick(QString)), pInputWidget, SLOT(update_nick(QString)));
    QObject::connect(pInputWidget, SIGNAL(display_message(QString,QString,int)), this, SLOT(slot_display_message(QString,QString,int)));
    QObject::connect(pInputWidget, SIGNAL(show_msg(QString,QString,int)), this, SLOT(slot_show_msg(QString,QString,int)));
    QObject::connect(pToolWidget, SIGNAL(change_font_size(QString)), this, SLOT(slot_change_font_size(QString)));
    QObject::connect(pToolWidget, SIGNAL(clear_content(QString)), this, SLOT(slot_clear_content(QString)));
    QObject::connect(pToolWidget, SIGNAL(set_scroll(QString, bool)), this, SLOT(slot_set_scroll(QString, bool)));
}

void InputLineDockWidget::enable_moderation()
{
    pToolWidget->set_moderation(true);
    pInputWidget->set_moderation(true);
}

void InputLineDockWidget::disable_moderation()
{
    pToolWidget->set_moderation(false);
    pInputWidget->set_moderation(false);
}

// input widget

void InputLineDockWidget::slot_update_nick(QString strNick)
{
    emit update_nick(strNick);
}

void InputLineDockWidget::slot_display_message(QString strChannel, QString strData, int iLevel)
{
    emit display_message(strChannel, strData, iLevel);
}

void InputLineDockWidget::slot_show_msg(QString strChannel, QString strData, int iLevel)
{
    emit show_msg(strChannel, strData, iLevel);
}

// tool widget

void InputLineDockWidget::slot_change_font_size(QString strSize)
{
    emit change_font_size(strSize);
}

void InputLineDockWidget::slot_clear_content(QString strChannel)
{
    emit clear_content(strChannel);
}

void InputLineDockWidget::slot_set_scroll(QString strChannel, bool bSetScroll)
{
    emit set_scroll(strChannel, bSetScroll);
}
