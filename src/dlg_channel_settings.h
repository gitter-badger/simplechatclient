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

#ifndef DLG_CHANNEL_SETTINGS_H
#define DLG_CHANNEL_SETTINGS_H

#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QDateTime>
#include <QDialog>
#include "dlg_email.h"
#include "dlg_privilege.h"
#include "network.h"
#include "ui_channel_settings.h"

class DlgChannelSettings : public QDialog
{
    Q_OBJECT
public:
    DlgChannelSettings(Network *, QSettings *);
    void set_channel(QString);
    void add_topic(QString);
    void add_pubpriv(int);
    void add_owner(QString);
    void add_email(QString);
    void add_op(QString);
    void add_halfop(QString);
    void add_ban(QString, QString, QString);
    void add_invite(QString, QString, QString);
    void add_cat(int, int);
    void add_guardian(int);
    void add_moderated(int);
    void add_www(QString);
    void add_created(QString);
    void add_password(QString);
    void add_limit(int);
    void add_auditorium(int);
    void add_description(QString);

private:
    Ui::uiChannelSettings ui;
    Network *pNetwork;
    QSettings *settings;
    QString strChannel;

    bool exist_item(QString, QTableWidget *);
    void clear();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

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
    void limit_changed(int);
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
