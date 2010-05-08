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

#ifndef QNICKLIST_H
#define QNICKLIST_H

#include <QContextMenuEvent>
#ifdef Q_WS_X11
#include <QDebug>
#endif
#include <QListWidget>
#include <QMenu>
#include <QQueue>
#include <QSettings>
#include <QTcpSocket>
#include "dlg_cam.h"

class qnicklist : public QListWidget
{
    Q_OBJECT
public:
    qnicklist(QTcpSocket *, QSettings *, QString);
    ~qnicklist();
    void set_open_channels(QStringList);
    void nicklist_add(QString, QString, QHash <QString, QString> *, QQueue <QString> *, QQueue <QString> *);
    void nicklist_remove(QString, QHash <QString, QString> *, QQueue <QString> *, QQueue <QString> *);
    bool nicklist_exist(QString, QHash <QString, QString> *);
    void nicklist_clear(QHash <QString, QString> *, QQueue <QString> *, QQueue <QString> *);
    void nicklist_refresh(QHash <QString, QString> *, QQueue <QString> *, QQueue <QString> *);
    void nicklist_sort(QHash <QString, QString> *, QQueue <QString> *, QQueue <QString> *);
    void nicklist_quicksort(QString strStatus, QHash <QString, QString> *, QQueue <QString> *, QQueue <QString> *);

private:
    QTcpSocket *socket;
    QSettings *settings;
    QString strChannel;
    QStringList strOpenChannels;
    enum { MaxOpenChannels = 50 };
    QAction *openChannelsActs[MaxOpenChannels];

    void send(QString);

private slots:
    void priv();
    void whois();
    void cam();
    void friends_add();
    void friends_del();
    void ignore_add();
    void ignore_del();
    void kick();
    void ban();
    void kban();
    void op_add();
    void op_del();
    void halfop_add();
    void halfop_del();
    void moderator_add();
    void moderator_del();
    void voice_add();
    void voice_del();
    void invite();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *);

};

#endif // QNICKLIST_H
