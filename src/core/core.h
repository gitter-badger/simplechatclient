/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
class QTcpSocket;
class QTimer;
class MainWindow;
class Updates;
#include <QObject>
#include "network.h"

/**
 * Core
 */
class Core : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Core)
    static Core *Instance;
public:
    static Core *instance();

    Core();
    virtual ~Core();
    void createAndShowGui();
    void createSettings();
    // old
    void removeOldConfig();
    void convertOldProfiles();
    void convertOldConfig();
    // config
    void configValues();
    void configProfileValues();
    // from main
    QString version();
    void setDebug(bool b);
    // window
    MainWindow *mainWindow();
    // from options
    void refreshColors();
    void refreshCSS();
    void refreshToolWidgetValues();
    // for window
    QString getChannelNameFromIndex(int index);
    int getIndexFromChannelName(const QString &strChannel);
    QString getCurrentChannelName();
    // get users per channel
    QList<QString> getUserListFromChannel(const QString &strChannel);
    // get user modes
    QString getUserModes(const QString &strNick, const QString &strChannel);
    // get user max modes - for sorting
    int getUserMaxModes(const QString &strModes);
    // get users count
    int getUserCount(const QString &strChannel);
    // show message
    void showMessage(const QString &strChannel, const QString &strData, MessageCategory eMessageCategory, QString strTime = QString::null, QString strNick = QString::null);
    // get user avatar path
    QString getUserAvatarPath(const QString &strNick);
    // priv
    QString convertPrivName(const QString &strChannel);
    // remove dir
    bool removeDir(const QString &dirName);

    QHash<QString, bool> mFriends;
    QList<QString> lIgnore;
    QList<QString> lChannelFavourites;
    QList<ChannelList> lChannelList;
    QHash<QString, QString> mMyStats;
    QHash<QString, QString> mMyProfile;
    QList<QString> lChannelHomes;
    QAction *lagAct;
    QAction *busyAct;
    QAction *awayAct;
    // channels
    QList<QString> lOpenChannels;
    QHash<QString, QByteArray> mChannelAvatar;
    // offline
    QAction *offlineMsgAct;
    QList<OfflineMsg> lOfflineMsg;
    QList<QString> lOfflineNicks;
    // kamerzysta socket
    QTcpSocket *kamerzystaSocket;
    // settings
    QHash<QString,QString> settings;
    // network
    Network *pNetwork;
    // auto-away
    QTimer *autoAwayTimer;
    // user profile
    QString strUserProfile;
    QHash<QString,QString> mUserProfile;
    bool bUserProfile;
    // channel settings
    QString strChannelSettings;
    QHash<QString,QString> mChannelSettingsInfo;
    QMultiHash<QString,QString> mChannelSettingsPermissions;
    bool bChannelSettingsInfo;
    QHash<QString,QString> mChannelSettingsStats;
    bool bChannelSettingsStats;
    // moderation
    QMultiHash<QString,ModerateMsg> mModerateMessages;
    // priv
    QHash<QString,QString> mPrivNames;
    // empty user avatar
    QString strEmptyUserAvatarPath;

public slots:
    void quit();

private:
    MainWindow *window;

    void init();
    void checkSettings();
    void readEmptyUserAvatar();
};

#endif // CORE_H
