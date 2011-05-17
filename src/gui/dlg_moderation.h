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

#ifndef DLG_MODERATION_H
#define DLG_MODERATION_H

#include <QDialog>
#include "defines.h"
#include "ui_moderation.h"

class DlgModeration : public QDialog
{
    Q_OBJECT
public:
    DlgModeration(QWidget *);
    void set_current_channel(QString);
    void add_msg(QString, QString, QString, QString);

private:
    Ui::uiModeration ui;
    QString strCurrentChannel;
    sModerateMsg mModerateMessages;

    void create_gui();
    void create_signals();
    void refresh();
    void remove_selected();

private slots:
    void combo_changed(QString);
    void button_accept();
    void button_remove();
    void button_close();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

signals:
    void display_msg(QString&, QString&, int);
    void send(QString);
};

#endif // DLG_MODERATION_H
