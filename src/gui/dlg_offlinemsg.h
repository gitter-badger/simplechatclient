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

#ifndef DLG_OFFLINEMSG_H
#define DLG_OFFLINEMSG_H

class Network;
#include <QDialog>
#include "ui_offlinemsg.h"

class DlgOfflineMsg : public QDialog
{
    Q_OBJECT
public:
    DlgOfflineMsg(QWidget *, Network *);

private:
    Ui::uiOfflineMsg ui;
    Network *pNetwork;
    QString strCurrentNick;
    QList<QString> messagesQuoted;
    QList<QString> messagesReplied;
    QList<QString> messagesQuotedToSender;

    void create_gui();
    void create_signals();
    void refresh();
    void remove_nick(QString);
    bool exist_nick();

private slots:
    void refresh_msg();
    void button_read();
    void button_reject();
    void button_reply();
    void button_close();
};

#endif // DLG_OFFLINEMSG_H
