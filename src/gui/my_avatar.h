/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>               *
 *   Copyright (C) 2012 Dariusz Markowicz <dmarkowicz@alari.pl>             *
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

#include <QDialog>

#include "avatar_client.h"
#include "ui_my_avatar.h"

class DlgMyAvatar : public QDialog
{
    Q_OBJECT
private:
    Ui::uiMyAvatar ui;
    AvatarClient *pAvatarClient;
    // collections names
    bool bReadedCollectionsNames;
    QList<QString> lCollectionsNames; // list of collections names
    QMap<QString, int> mCollectionId; // name, id
    // collections
    QList<QString> lCollectionAvatars; // list of collection avatars
    // my avatars
    QList<QString> lMyAvatars; // list of my avatars
    QMap<QString, int> mMyAvatarsId; // url, id

    void createGui();
    void setDefaultValues();
    void createSignals();

    void drawMyAvatar(QString, QByteArray);
    void drawCollections();
    void drawAvatarFromCollection(QString, QByteArray);
    void drawCurrentAvatar(QString strUrl, QByteArray bData);

private slots:
    void refreshAvatar();
    void tabChanged(int);
    void collectionChanged(QString);
    void addAvatarClicked();
    void applyAvatarClicked();
    void removeAvatarClicked();
    void setEmptyAvatarClicked();
    void applyCollectionAvatarClicked();

public:
    DlgMyAvatar(QWidget *);
    virtual ~DlgMyAvatar();

public slots:
    void getCollectionsReady(QString strResult);
    void getCollectionAvatarsReady(QString strResult);
    void getMyAvatarsReady(QString strResult);
    void setAvatarReady(QString strResult);
    void uploadImageReady(QString strResult, QString strFileName);
    void updatePhotoReady(QString strResult);
    void addPhotoReady(QString strResult);
    void deletePhotoReady(QString strResult);
    void getAvatarReady(QString strUrl, QByteArray bData, AvatarType type);
};

#endif // DLG_MY_AVATAR_H
