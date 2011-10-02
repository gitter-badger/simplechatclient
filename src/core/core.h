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

#ifndef CORE_H
#define CORE_H

#include "defines.h"
class QAction;
class QTcpSocket;
class QTimer;
class MainWindow;
class Updates;
#include <QObject>
#include "network.h"

/**
 * Core
 */
class Core: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Core);
    static Core *Instance;
public:
    static Core *instance();

    Core();
    virtual ~Core();
    void createGui();
    void createSettings();
    // config
    void configValues();
    void configProfileValues();
    void removeOldConfig();
    // from main
    QString version();
    void setDebug(bool);
    // window
    void showSccWindow();
    MainWindow *sccWindow();
    // from options
    void refreshColors();
    void refreshBackgroundImage();
    void refreshToolWidgetValues();
    // update
    void checkUpdate();
    // awaylog
    void addAwaylog(QString, QString);
    // for window
    QString getChannelNameFromIndex(int index);
    QString getCurrentChannelName();
    // get users per channel
    QList<QString> getUserListFromChannel(QString);
    // get user modes
    QString getUserModes(QString, QString);
    // get user max modes - for sorting
    int getUserMaxModes(QString);
    // get users count
    int getUserCount(QString);

    QMap<QString, bool> mFriends;
    QList<QString> lIgnore;
    QList<QString> lChannelFavourites;
    QList<ChannelList> lChannelList;
    QMap<QString, QString> mMyStats;
    QMap<QString, QString> mMyProfile;
    QList<QString> lChannelHomes;
    QAction *lagAct;
    QList<QString> lAwaylog;
    QAction *busyAct;
    QAction *awayAct;
    // channels
    QList<QString> lOpenChannels;
    QMap<QString, QByteArray> mChannelAvatar;
    // offline
    QAction *offlineMsgAct;
    QList<OfflineMsg> lOfflineMsg;
    QList<QString> lOfflineNicks;
    // kamerzysta socket
    QTcpSocket *kamerzystaSocket;
    // settings
    QMap<QString,QString> settings;
    // network
    Network *pNetwork;
    // auto-away
    QTimer *autoAwayTimer;
    // user profile
    QString strUserProfile;
    QMap<QString,QString> mUserProfile;
    bool bUserProfile;
    // channel settings
    QString strChannelSettings;
    QMap<QString,QString> mChannelSettingsInfo;
    QMultiMap<QString,QString> mChannelSettingsPermissions;
    bool bChannelSettingsInfo;
    QMap<QString,QString> mChannelSettingsStats;
    bool bChannelSettingsStats;
    // moderation
    QMultiHash<QString,ModerateMsg> mModerateMessages;

public slots:
    void quit();

private:
    MainWindow *window;
    Updates *pUpdates;

    void init();
    void checkSettings();
};

#endif // CORE_H
