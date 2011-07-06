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
    DlgMyAvatar(QWidget *, Network *);
    virtual ~DlgMyAvatar();

private:
    Ui::uiMyAvatar ui;
    Network *pNetwork;
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

    void createGui();
    void setDefaultValues();
    void createSignals();

    void getCookies();
    // avatar
    void getAvatar(QString);
    void gotAvatar(QString, QByteArray);
    // my avatars
    void loadMyAvatars();
    void gotMyAvatars(QString);
    void drawMyAvatar(QString, QByteArray);
    // collections
    void getCollections();
    void gotCollections(QString);
    void drawCollections();
    // avatars from collect
    void getAvatarsFromCollect(int);
    void gotAvatarsFromCollect(QString);
    void drawAvatarFromCollect(QString, QByteArray);
    // add avatar
    void gotAddAvatar(QString);
    // remove avatar
    void gotRemoveAvatar(QString);

private slots:
    void networkFinished(QNetworkReply*);
    void refreshAvatar();
    void tabChanged(int);
    void collectionChanged(QString);
    void buttonAddAvatar();
    void buttonApplyAvatar();
    void buttonRemoveAvatar();
    void buttonSetEmptyAvatar();
    void buttonApplyCollectionAvatar();
};

#endif // DLG_MY_AVATAR_H
