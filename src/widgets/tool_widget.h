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

#ifndef TOOL_WIDGET_H
#define TOOL_WIDGET_H

#include "defines.h"
class QComboBox;
class QLabel;
class QFrame;
class QMenu;
class QToolButton;
class InputLineWidget;
#include <QWidget>

class ToolWidget : public QWidget
{
    Q_OBJECT
public:
    ToolWidget(QWidget *);
    virtual ~ToolWidget();
    void setChannelSettings(bool);
    void setDefaultValues();
    void clearInputLine();

public slots:
    void setModeration(bool);
    void updateNick(QString);

private:
    bool bShowFontButtons;
    bool bMyBold;
    bool bMyItalic;
    QString strMyFontWeight;
    QString strMyFontStyle;
    QString strMyFontFamily;
    QString strCurrentColor;

    QToolButton *showFontButtons;
    QFrame *separator1;
    QToolButton *bold;
    QToolButton *italic;
    QToolButton *fontfamily;
    QComboBox *color;
    QToolButton *emoticons;
    QFrame *separator2;
    QToolButton *channel_settings;
    QToolButton *moderation;
    QLabel *nickLabel;
    InputLineWidget *pInputLine;
    QToolButton *sendButton;
    QToolButton *moderSendButton;

    QMenu *fontMenu;
    QAction *arialAct;
    QAction *timesAct;
    QAction *verdanaAct;
    QAction *tahomaAct;
    QAction *courierAct;

    QToolButton *size;
    QMenu *sizeMenu;
    QAction *size8Act;
    QAction *size9Act;
    QAction *size10Act;
    QAction *size11Act;
    QAction *size12Act;
    QAction *size14Act;
    QAction *size16Act;
    QAction *size18Act;
    QAction *size20Act;
    QAction *size24Act;

    void pasteMultiLine(QString, bool);
    void sendMessage(QString, bool);

private slots:
    void showFontButtonsClicked();
    void boldClicked();
    void italicClicked();
    void arialTriggered();
    void timesTriggered();
    void verdanaTriggered();
    void tahomaTriggered();
    void courierTriggered();

    void size8Triggered();
    void size9Triggered();
    void size10Triggered();
    void size11Triggered();
    void size12Triggered();
    void size14Triggered();
    void size16Triggered();
    void size18Triggered();
    void size20Triggered();
    void size24Triggered();

    void colorClicked(int);
    void emoticonsClicked();
    void channelSettingsClicked();
    void moderationClicked();

    void inputlineReturnPressed();
    void moderButtonClicked();
    void slotCtrlTabPressed();
    void slotCtrlShiftTabPressed();

signals:
    void ctrlTabPressed();
    void ctrlShiftTabPressed();
    void showMessage(QString,QString,MessageCategory);
    void showMessage(QString,QString,MessageCategory,QString,QString);
};

#endif // TOOL_WIDGET_H
