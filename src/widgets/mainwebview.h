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

#ifndef MAINWEBVIEW_H
#define MAINWEBVIEW_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "dlg_user_profile.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QSettings>
#include <QWebHitTestResult>
#include <QWebView>
#include "dlg_cam.h"
#include "dlg_kick.h"
#ifdef Q_WS_WIN
#include "kamerzysta.h"
#endif
#include "network.h"

class MainWebView : public QWebView
{
    Q_OBJECT
public:
    MainWebView(QWidget *, Network *, QSettings *, QString);
    ~MainWebView();
    void set_open_channels(QStringList);
    void set_user_info(QString, QString, QString);

private:
    QWidget *myparent;
    Network *pNetwork;
    QSettings *settings;
    QString strChannel;
    QString strNick;
    QStringList strOpenChannels;
    enum { maxOpenChannels = 50 };
    QAction *openChannelsActs[maxOpenChannels];
    sNickInfo sCurrentUserInfo;

private slots:
    void join_channel();
    void priv();
    void whois();
    void profile();
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
    void contextMenuEvent(QContextMenuEvent *);

};

#endif // MAINWEBVIEW_H
