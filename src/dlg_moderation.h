/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#ifndef DLG_MODERATION_H
#define DLG_MODERATION_H

#include <QDialog>
#include <QSettings>
#include "network.h"
#include "ui_moderation.h"

class DlgModeration : public QDialog
{
    Q_OBJECT
public:
    DlgModeration(Network *, QSettings *);
    ~DlgModeration();
    void add_msg(QString, QString, QString, QString);
    void clear();

private:
    Ui::uiModeration ui;
    Network *pNetwork;
    QSettings *settings;
    QMultiHash <QString, QString> channel_id;
    QMultiHash <QString, QString> id_nick;
    QMultiHash <QString, QString> id_message;

    void refresh(QString);
    void del_msg(QString);
    bool combo_exist(QString);

private slots:
    void combo_changed(int);
    void button_accept();
    void button_remove();
    void button_close();

};

#endif // DLG_MODERATION_H
