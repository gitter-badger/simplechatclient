/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *   Copyright (C) 2012 Dariusz Markowicz <dmarkowicz@alari.pl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DLG_MY_AVATAR_H
#define DLG_MY_AVATAR_H

#include <QDialog>

#include "avatar_client.h"
#include "ui_my_avatar.h"

class DlgMyAvatar : public QDialog
{
    Q_OBJECT
public:
    DlgMyAvatar(QWidget *parent = 0);
    virtual ~DlgMyAvatar();

private:
    Ui::uiMyAvatar ui;
    AvatarClient *avatarClient;

    void createGui();
    void setDefaultValues();
    void createSignals();

private slots:
    void avatarSelected(const QString &url);
    void getAvatarReady(const QByteArray &content, const QString &avatarUrl, AvatarClient::AvatarType type);
    void refreshAvatar();
};

#endif // DLG_MY_AVATAR_H
