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
class InputLineWidget;
class Network;
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class InputWidget : public QWidget
{
    Q_OBJECT
public:
    InputWidget(QWidget *, Network *);
    void set_active(QString);
    inline QString get_active() { return strChannel; }
    void update_nicklist();
    void insert_text(QString);
    void set_font(QFont);
    void set_color(QString);
    void set_moderation(bool);
    void set_toolwidget_icon(bool);
    void convert_emots(QString &);
    void replace_emots(QString &);
    void paste_multi_line(QString, bool);
    void send_message(QString, bool);
    void update_nick(QString);

private:
    // params
    Network *pNetwork;
    // inputline widget
    QHBoxLayout *mainLayout;
    QPushButton *showHideToolWidget;
    QLabel *nickLabel;
    InputLineWidget *pInputLine;
    QPushButton *sendButton;
    QPushButton *moderSendButton;
    QString strChannel;

private slots:
    void inputline_return_pressed();
    void moder_button_clicked();
    void show_hide_toolwidget_clicked();

signals:
    void show_msg(QString&,QString&,MessageCategory);
    void display_message(QString&,QString&,MessageCategory);
    void show_hide_toolwidget();
};

#endif // INPUTWIDGET_H
