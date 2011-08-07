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

#include <QDialog>
#include "ui_offlinemsg.h"

class DlgOfflineMsg : public QDialog
{
    Q_OBJECT
public:
    DlgOfflineMsg(QWidget *);

private:
    Ui::uiOfflineMsg ui;
    QString strCurrentNick;
    QList<QString> messagesQuoted;
    QList<QString> messagesReplied;
    QList<QString> messagesQuotedToSender;

    void createGui();
    void createSignals();
    void refresh();
    void removeNick(QString);
    bool existNick();

private slots:
    void refreshMsg();
    void buttonRead();
    void buttonReject();
    void buttonReply();
    void buttonClose();
};

#endif // DLG_OFFLINEMSG_H
