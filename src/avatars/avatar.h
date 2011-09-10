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

#ifndef AVATAR_H
#define AVATAR_H

class QNetworkAccessManager;
class QNetworkReply;
class TabContainer;
#include <QObject>

/**
 * Class for downloading avatar
 * @param TabContainer* for refresh image
 */
class Avatar : public QObject
{
    Q_OBJECT
public:
    Avatar(TabContainer *);
    virtual ~Avatar();
    void getAvatar(QString, QString, QString);

private:
    TabContainer *tabc;
    QNetworkAccessManager *accessManager;

public slots:
    void avatarFinished(QNetworkReply*);
    void setAvatar(QString&, QString&, QByteArray&);

signals:
    void setChannelAvatar(QString);
};

#endif // AVATAR_H
