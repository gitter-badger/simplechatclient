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

#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include "defines.h"
class QHBoxLayout;
class QLabel;
class QPushButton;
class InputLineWidget;
#include <QWidget>

class InputWidget : public QWidget
{
    Q_OBJECT
public:
    InputWidget(QWidget *);
    virtual ~InputWidget();
    void insertText(QString);
    void setFont(QFont);
    void setColor(QString);
    void setModeration(bool);
    void setToolwidgetIcon(bool);
    void pasteMultiLine(QString, bool);
    void sendMessage(QString, bool);
    void updateNick(QString);

private:
    // inputline widget
    QHBoxLayout *mainLayout;
    QPushButton *showHideToolWidget;
    QLabel *nickLabel;
    InputLineWidget *pInputLine;
    QPushButton *sendButton;
    QPushButton *moderSendButton;

private slots:
    void inputlineReturnPressed();
    void moderButtonClicked();
    void showHideToolwidgetClicked();
    void slotCtrlTabPressed();
    void slotCtrlShiftTabPressed();

signals:
    void showMsg(QString&,QString&,MessageCategory);
    void displayMessage(QString&,QString&,MessageCategory);
    void showHideToolwidget();
    void ctrlTabPressed();
    void ctrlShiftTabPressed();
};

#endif // INPUTWIDGET_H
