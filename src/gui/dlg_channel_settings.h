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

#ifndef DLG_CHANNEL_SETTINGS_H
#define DLG_CHANNEL_SETTINGS_H

class Network;
#include <QDialog>
#include "ui_channel_settings.h"

class DlgChannelSettings : public QDialog
{
    Q_OBJECT
public:
    DlgChannelSettings(QWidget *, Network *);
    void set_channel(QString);
    void set_data(QString, QMap<QString, QString>);

    void set_topic(QString, QString);
    void set_owner(QString, QString);
    void set_description(QString, QString);
    void add_op(QString, QString);
    void add_halfop(QString, QString);
    void add_ban(QString, QString, QString, QString, QString);
    void add_invite(QString, QString, QString, QString);

private:
    Ui::uiChannelSettings ui;
    QWidget *myparent;
    Network *pNetwork;
    QString strChannel;

    bool exist_item(QString, QTableWidget *);
    void clear();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

private slots:
    void owner_changed();
    void remove_channel_clicked();
    void email_changed();
    void www_changed();
    void topic_changed();
    void desc_changed();
    void status_pub();
    void status_priv();
    void category_changed(int);
    void guardian_inactive();
    void guardian_active();
    void guardian_clicked(int);
    void password_changed();
    void limit_changed();
    void moderated_inactive();
    void moderated_active();
    void auditorium_inactive();
    void auditorium_active();
    void button_op_add();
    void button_op_del();
    void button_halfop_add();
    void button_halfop_del();
    void button_ban_add();
    void button_ban_del();
    void button_invite_add();
    void button_invite_del();
    void button_ok();
    void button_cancel();
};

#endif // DLG_CHANNEL_SETTINGS_H
