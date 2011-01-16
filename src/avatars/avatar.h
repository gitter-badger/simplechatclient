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

class TabContainer;
#include <QMap>
#include <QObject>
#include <QThread>

/**
 * Thread for downloading avatar
 * @param QString nick or channel name
 * @param QString url
 * @param QString category
 */
class AvatarThread : public QThread
{
    Q_OBJECT
public:
    AvatarThread(QString, QString, QString);
    void run();

private:
    QString strNickChannel;
    QString strUrl;
    QString strCategory;

private slots:
    void thread_work();

signals:
    void set_avatar(QString, QString, QByteArray);
    void stop_thread();
};

/**
 * Class for downloading avatar
 * @param TabContainer* for refresh image
 * @param QString nick or channel name
 * @param QString url
 * @param QString category
 * @param QMap map with nick avatar
 * @param QMap map with channel avatar
 */
class Avatar : public QObject
{
    Q_OBJECT
public:
    Avatar(TabContainer *, QString, QString, QString, QMap <QString, QByteArray> *, QMap <QString, QByteArray> *);
    void kill_thread();

private:
    TabContainer *tabc;
    QString strNickChannel;
    QString strUrl;
    QString strCategory;
    QMap <QString, QByteArray> *mNickAvatar;
    QMap <QString, QByteArray> *mChannelAvatar;
    AvatarThread *avatarThr;

public slots:
    void set_avatar(QString, QString, QByteArray);
    void stop_thread();

signals:
    void set_nick_avatar(QString);
    void set_channel_avatar(QString);
    void sremove_athread(Avatar*);
};

#endif // AVATAR_H
