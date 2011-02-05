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

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "inputwidget.h"
#include "log.h"
#include "network.h"
#include "toolwidget.h"
#include "inputlinedockwidget.h"

InputLineDockWidget::InputLineDockWidget(QWidget *parent, Network *param1, DlgChannelSettings *param2, DlgModeration *param3) : QWidget(parent)
{
    myparent = parent;

    // params
    pNetwork = param1;
    dlgchannel_settings = param2;
    dlgmoderation = param3;

    // layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    pInputWidget = new InputWidget(myparent, pNetwork);
    pInputWidget->show();

    pToolWidget = new ToolWidget(myparent, pNetwork, pInputWidget, dlgchannel_settings, dlgmoderation);
    pToolWidget->show();

    mainLayout->addWidget(pToolWidget);
    mainLayout->addWidget(pInputWidget);
    setLayout(mainLayout);

    // set minimum size
    int iMinimumSize = sizeHint().height();
    setMinimumHeight(iMinimumSize);
    setMaximumHeight(iMinimumSize);

    // signals
    QObject::connect(this, SIGNAL(update_nick(QString)), pInputWidget, SLOT(update_nick(QString)));
    QObject::connect(pInputWidget, SIGNAL(display_message(QString,QString,int)), this, SLOT(slot_display_message(QString,QString,int)));
    QObject::connect(pInputWidget, SIGNAL(show_msg(QString,QString,int)), this, SLOT(slot_show_msg(QString,QString,int)));
    QObject::connect(pToolWidget, SIGNAL(change_font_size(QString)), this, SLOT(slot_change_font_size(QString)));
    QObject::connect(pToolWidget, SIGNAL(clear_content(QString)), this, SLOT(slot_clear_content(QString)));
    QObject::connect(pInputWidget, SIGNAL(show_hide_toolwidget()), this, SLOT(slot_show_hide_toolwidget()));
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

void InputLineDockWidget::show_toolwidget()
{
    // show toolwidget
    pToolWidget->show();
    // set minimum size
    int iMinimumSize = sizeHint().height();
    setMinimumHeight(iMinimumSize);
    setMaximumHeight(iMinimumSize);
    // refresh icon
    pInputWidget->set_toolwidget_icon(true);
}

void InputLineDockWidget::hide_toolwidget()
{
    // hide toolwidget
    pToolWidget->hide();
    // set minimum size
    int iMinimumSize = sizeHint().height();
    setMinimumHeight(iMinimumSize);
    setMaximumHeight(iMinimumSize);
    // refresh icon
    pInputWidget->set_toolwidget_icon(false);
}

void InputLineDockWidget::hide_channel_settings()
{
    pToolWidget->set_channel_settings(false);
}

void InputLineDockWidget::show_channel_settings()
{
    pToolWidget->set_channel_settings(true);
}

void InputLineDockWidget::set_active(QString strChannel)
{
    pInputWidget->set_active(strChannel);
}

QString InputLineDockWidget::get_active()
{
    return pInputWidget->get_active();
}

void InputLineDockWidget::set_userslist(QTreeWidget *treeWidget)
{
    pInputWidget->set_userslist(treeWidget);
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

void InputLineDockWidget::slot_show_hide_toolwidget()
{
    if (pToolWidget->isHidden() == true)
        show_toolwidget();
    else
        hide_toolwidget();
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
