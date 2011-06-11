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
class DlgUserProfile;
class MainTextEdit;
class MainWindow;
class Network;
class TabManager;
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

/**
 * Main widget used for displaying all text messages from users
 */
class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(Network *, QString, DlgUserProfile *);
    virtual ~TabWidget();
    inline QString get_name() { return strName; }
    void addslashes(QString &);
    void display_msg(QString &, QString &, MessageCategory);
    void display_msg(QString &, MessageCategory);
    void display_message(QString &, MessageCategory); // private, exteption: inputlinewidget
    void set_topic(QString &);
    void author_topic(QString &);
    inline void change_font_size(QString f) { strFontSize = f; }
    void clear_content();
    void refresh_colors();
    void refresh_background_image();

private:
    // params
    Network *pNetwork;
    QString strName;
    DlgUserProfile *pDlgUserProfile;

    // other
    QString strFontSize;

    QVBoxLayout *mainLayout;
    QWidget *mainWidget;

    QLabel *topic;
    MainTextEdit *pMainTextEdit;

    void convert_emots(QString &);
    void replace_emots(QString &);
};

#endif // TAB_WIDGET_H
