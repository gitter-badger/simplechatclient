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

#ifndef DLG_MY_AVATAR_H
#define DLG_MY_AVATAR_H

class Network;
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include "ui_my_avatar.h"

class DlgMyAvatar : public QDialog
{
    Q_OBJECT
public:
    DlgMyAvatar(QWidget *, Network *, QMap <QString, QByteArray> *);
    ~DlgMyAvatar();

private:
    Ui::uiMyAvatar ui;
    QWidget *myparent;
    Network *pNetwork;
    QMap <QString, QByteArray> *mNickAvatar;
    // network
    QNetworkAccessManager *networkAccessManager;
    QNetworkCookieJar *cookieJar;
    // collections names
    bool bReadedCollectionNames;
    QList<QString> lNameCollections; // list of name collections
    QMap<QString, int> mCollectionId; // name, id
    // collections
    QList<QString> lCollections; // list of collections
    // my avatars
    QList<QString> lMyAvatars; // list of my avatars
    QMap<QString, QString> mMyAvatarsID; // url, id

    void get_cookies();
    void get_avatar(QString);
    void got_avatar(QString, QByteArray);
    // my avatars
    void load_my_avatars();
    void got_my_avatars(QString);
    void draw_my_avatar(QString, QByteArray);
    // collections
    void get_collections();
    void got_collections(QString);
    void draw_collections();
    // avatars from collect
    void get_avatars_from_collect(int);
    void got_avatars_from_collect(QString);
    void draw_avatar_from_collect(QString, QByteArray);
    // add avatar
    void got_add_avatar(QString);
    // remove avatar
    void got_remove_avatar(QString);

private slots:
    void network_finished(QNetworkReply*);
    void refresh_avatar();
    void tab_changed(int);
    void collection_changed(QString);
    void button_add_avatar();
    void button_apply_avatar();
    void button_remove_avatar();
    void button_set_empty_avatar();
    void button_apply_collection_avatar();
    void button_close();
};

#endif // DLG_MY_AVATAR_H
