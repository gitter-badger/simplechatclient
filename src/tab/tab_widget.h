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

#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include "defines.h"
class QLabel;
class QVBoxLayout;
class DlgUserProfile;
class MainTextEdit;
class MainWindow;
class TabManager;
#include <QWidget>

/**
 * Main widget used for displaying all text messages from users
 */
class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QString, DlgUserProfile *);
    virtual ~TabWidget();
    inline QString getName() { return strName; }
    void addslashes(QString &);
    void displayMsg(QString &, QString &, MessageCategory);
    void displayMsg(QString &, MessageCategory);
    void displayMessage(QString &, MessageCategory); // private, exteption: inputlinewidget
    void setTopic(QString &);
    void authorTopic(QString &);
    inline void changeFontSize(QString f) { strFontSize = f; }
    void clearContent();
    void refreshColors();
    void refreshBackgroundImage();

private:
    // params
    QString strName;
    DlgUserProfile *pDlgUserProfile;

    // other
    QString strFontSize;

    QVBoxLayout *mainLayout;
    QWidget *mainWidget;

    QLabel *topic;
    MainTextEdit *pMainTextEdit;

    void convertEmots(QString &);
    void replaceEmots(QString &);
};

#endif // TAB_WIDGET_H
