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

#ifndef NICK_AVATAR_H
#define NICK_AVATAR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QThread>
#include "tab_container.h"

class NickAvatarThread : public QThread
{
    Q_OBJECT
public:
    NickAvatarThread(QString, QString, QMap <QString, QByteArray> *);
    void run();

private:
    QString strNick;
    QString strUrl;
    QMap <QString, QByteArray> *mNickAvatar;

private slots:
    void thread_work();

signals:
    void set_avatar(QString, QByteArray);
    void stop_thread();

};

class NickAvatar : public QObject
{
    Q_OBJECT
public:
    NickAvatar(TabContainer *, QString, QString, QMap <QString, QByteArray> *);

private:
    TabContainer *tabc;
    QString strNick;
    QString strUrl;
    QMap <QString, QByteArray> *mNickAvatar;
    NickAvatarThread *nickAvatarThr;

public slots:
    void set_nick_avatar(QString, QByteArray);
    void stop_thread();

signals:
    void do_remove_nathread(NickAvatar*);

};
#endif // NICK_AVATAR_H
