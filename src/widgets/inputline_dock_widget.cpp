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
#include "dlg_moderation.h"
#include "input_widget.h"
#include "log.h"
#include "tool_widget.h"
#include "inputline_dock_widget.h"

InputLineDockWidget::InputLineDockWidget(QWidget *parent, DlgChannelSettings *_pDlgChannelSettings, DlgModeration *_pDlgModeration) : QWidget(parent), pDlgChannelSettings(_pDlgChannelSettings), pDlgModeration(_pDlgModeration)
{
    // layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    pInputWidget = new InputWidget(this);
    pInputWidget->show();

    pToolWidget = new ToolWidget(this, pInputWidget, pDlgChannelSettings, pDlgModeration);
    pToolWidget->show();

    mainLayout->addWidget(pToolWidget);
    mainLayout->addWidget(pInputWidget);
    setLayout(mainLayout);

    // set minimum size
    int iMinimumSize = sizeHint().height();
    setMinimumHeight(iMinimumSize);
    setMaximumHeight(iMinimumSize);

    // focus
    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(pInputWidget);

    // signals
    QObject::connect(pInputWidget, SIGNAL(displayMessage(QString&,QString&,MessageCategory)), this, SLOT(slotDisplayMessage(QString&,QString&,MessageCategory)));
    QObject::connect(pInputWidget, SIGNAL(showMsg(QString&,QString&,MessageCategory)), this, SLOT(slotShowMsg(QString&,QString&,MessageCategory)));
    QObject::connect(pToolWidget, SIGNAL(clearContent(QString)), this, SLOT(slotClearContent(QString)));
    QObject::connect(pInputWidget, SIGNAL(showHideToolwidget()), this, SLOT(slotShowHideToolwidget()));

    QObject::connect(pInputWidget, SIGNAL(ctrlTabPressed()), this, SLOT(slotCtrlTabPressed()));
    QObject::connect(pInputWidget, SIGNAL(ctrlShiftTabPressed()), this, SLOT(slotCtrlShiftTabPressed()));
}

InputLineDockWidget::~InputLineDockWidget()
{
    delete pToolWidget;
    delete pInputWidget;
}

void InputLineDockWidget::enableModeration()
{
    pToolWidget->setModeration(true);
    pInputWidget->setModeration(true);
}

void InputLineDockWidget::disableModeration()
{
    pToolWidget->setModeration(false);
    pInputWidget->setModeration(false);
}

void InputLineDockWidget::showToolwidget()
{
    // show toolwidget
    pToolWidget->show();
    // set minimum size
    int iMinimumSize = sizeHint().height();
    setMinimumHeight(iMinimumSize);
    setMaximumHeight(iMinimumSize);
    // refresh icon
    pInputWidget->setToolwidgetIcon(true);
}

void InputLineDockWidget::hideToolwidget()
{
    // hide toolwidget
    pToolWidget->hide();
    // set minimum size
    int iMinimumSize = sizeHint().height();
    setMinimumHeight(iMinimumSize);
    setMaximumHeight(iMinimumSize);
    // refresh icon
    pInputWidget->setToolwidgetIcon(false);
}

void InputLineDockWidget::hideChannelSettings()
{
    pToolWidget->setChannelSettings(false);
}

void InputLineDockWidget::showChannelSettings()
{
    pToolWidget->setChannelSettings(true);
}

// input widget

void InputLineDockWidget::slotUpdateNick(QString strNick)
{
    pInputWidget->updateNick(strNick);
}

void InputLineDockWidget::slotDisplayMessage(QString &strChannel, QString &strData, MessageCategory eMessageCategory)
{
    emit displayMessage(strChannel, strData, eMessageCategory);
}

void InputLineDockWidget::slotShowMsg(QString &strChannel, QString &strData, MessageCategory eMessageCategory)
{
    emit showMsg(strChannel, strData, eMessageCategory);
}

void InputLineDockWidget::slotShowHideToolwidget()
{
    if (pToolWidget->isHidden())
        showToolwidget();
    else
        hideToolwidget();
}

void InputLineDockWidget::slotCtrlTabPressed()
{
    emit ctrlTabPressed();
}

void InputLineDockWidget::slotCtrlShiftTabPressed()
{
    emit ctrlShiftTabPressed();
}

// tool widget

void InputLineDockWidget::slotClearContent(QString strChannel)
{
    emit clearContent(strChannel);
}

// from mainwindow/core/options
void InputLineDockWidget::refreshToolWidgetValues()
{
    pToolWidget->setDefaultValues();
}