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

#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QWidget>
#include "commands.h"
#include "inputlinewidget.h"
#include "log.h"
#include "network.h"
#include "replace.h"
#include "replace.h"

class InputWidget : public QWidget
{
    Q_OBJECT
public:
    InputWidget(QWidget *, Network *);
    void set_active(QString);
    inline QString get_active() { return strChannel; }
    void insert_text(QString);
    void set_font(QFont);
    void set_userslist(QTreeWidget *);
    void set_moderation(bool);
    QString convert_emots(QString);
    QString replace_emots(QString);
    void send_message(bool);

public slots:
    void update_nick(QString);

private:
    // params
    QWidget *myparent;
    Network *pNetwork;
    // inputline widget
    QHBoxLayout *mainLayout;
    QLabel *nickLabel;
    InputLineWidget *inputLine;
    QPushButton *sendButton;
    QPushButton *moderSendButton;
    QString strChannel;
    QString strLast_msg;

private slots:
    void inputline_return_pressed();
    void moder_button_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent *);

signals:
    void show_msg(QString, QString, int);
    void display_message(QString, QString, int);

};

#endif // INPUTWIDGET_H
