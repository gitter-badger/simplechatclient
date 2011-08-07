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

#ifndef INPUTLINEDOCKWIDGET_H
#define INPUTLINEDOCKWIDGET_H

#include "defines.h"
class DlgChannelSettings;
class DlgModeration;
class InputWidget;
class ToolWidget;
#include <QWidget>

class InputLineDockWidget : public QWidget
{
    Q_OBJECT
public:
    InputLineDockWidget(QWidget *, DlgChannelSettings *, DlgModeration *);
    virtual ~InputLineDockWidget();
    void enableModeration();
    void disableModeration();
    void hideToolwidget();
    void showToolwidget();
    void hideChannelSettings();
    void showChannelSettings();

public slots:
    void slotUpdateNick(QString);
    void slotDisplayMessage(QString&, QString&, MessageCategory);
    void slotShowMsg(QString&, QString&, MessageCategory);
    void slotShowHideToolwidget();
    void slotChangeFontSize(QString);
    void slotClearContent(QString);
    void slotCtrlTabPressed();
    void slotCtrlShiftTabPressed();

private:
    // params
    DlgChannelSettings *pDlgChannelSettings;
    DlgModeration *pDlgModeration;
    // other
    InputWidget *pInputWidget;
    ToolWidget *pToolWidget;

signals:
    void displayMessage(QString&,QString&,MessageCategory);
    void showMsg(QString&,QString&,MessageCategory);
    void changeFontSize(QString);
    void clearContent(QString);
    void ctrlTabPressed();
    void ctrlShiftTabPressed();
};

#endif // INPUTLINEDOCKWIDGET_H
