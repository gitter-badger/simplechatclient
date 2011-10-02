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

#ifndef NICKLIST_WIDGET_H
#define NICKLIST_WIDGET_H

#include "defines.h"
#include <QListWidget>

class NickListWidget : public QListWidget
{
    Q_OBJECT
public:
    NickListWidget(QString);
    void addUser(QString, QString);
    void delUser(QString);
    bool existUser(QString);
    void updateUserAvatar(QString, QByteArray);
    void changeUserFlag(QString, QString);
    QString getUserModes(QString);
    QList<QString> getUserList();

private:
    // other
    QString strChannel;
    QAction *openChannelsActs[maxOpenChannels];
    QString strSelectedNick;

    QString convertFlag(QString);

private slots:
    void priv();
    void whois();
    void profile();
    void cam();
    void friendsAdd();
    void friendsDel();
    void ignoreAdd();
    void ignoreDel();
    void kick();
    void ban();
    void kban();
    void ipban();
    void opAdd();
    void opDel();
    void halfopAdd();
    void halfopDel();
    void moderatorAdd();
    void moderatorDel();
    void voiceAdd();
    void voiceDel();
    void invite();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *);
};

#endif // NICKLIST_WIDGET_H
