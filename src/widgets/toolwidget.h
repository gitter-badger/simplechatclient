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

#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

class QComboBox;
class QFrame;
class QHBoxLayout;
class QMenu;
class QPushButton;
class DlgChannelSettings;
class DlgModeration;
class InputWidget;
#include <QWidget>

class ToolWidget : public QWidget
{
    Q_OBJECT
public:
    ToolWidget(QWidget *, InputWidget *, DlgChannelSettings *, DlgModeration *);
    void setModeration(bool);
    void setChannelSettings(bool);

private:
    // params
    InputWidget *pInputWidget;
    DlgChannelSettings *pDlgChannelSettings;
    DlgModeration *pDlgModeration;
    // other
    bool bShowFontButtons;
    bool bMyBold;
    bool bMyItalic;
    QString strMyFontWeight;
    QString strMyFontStyle;
    QString strMyFontFamily;
    QString strCurrentColor;

    QHBoxLayout *toolLayout;
    QPushButton *showFontButtons;
    QFrame *separator1;
    QPushButton *bold;
    QPushButton *italic;
    QPushButton *fontfamily;
    QComboBox *color;
    QPushButton *emoticons;
    QFrame *separator2;
    QPushButton *channel_settings;
    QPushButton *moderation;
    QPushButton *clear;

    QMenu *fontMenu;
    QAction *arialAct;
    QAction *timesAct;
    QAction *verdanaAct;
    QAction *tahomaAct;
    QAction *courierAct;

    QPushButton *size;
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

    void setDefault();

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
    void clearClicked();

signals:
    void changeFontSize(QString);
    void clearContent(QString);
};

#endif // TOOLWIDGET_H
