/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CORE_H
#define CORE_H

#include "defines.h"
#include "scc-config.h"
class QTcpSocket;
class MainWindow;
class Updates;
#include "nicklist_widget.h"
#include "tab_widget.h"
#include <QObject>
#include <QTranslator>
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
    // window
    MainWindow *mainWindow();
    // language
    void refreshLanguage();
    // remove dir
    bool removeDir(const QString &dirName);

    // kamerzysta socket
    QTcpSocket *kamerzystaSocket;
    // network
    Network *pNetwork;
    // TW (important!!!)
    QHash<QString, TabWidget*> tw;

    // TODO remove this
    // gui
    QHash<QString, bool> mFriends;
    QList<QString> lIgnore;
    QList<QString> lChannelFavourites;
    QHash<QString, QString> mMyStats;
    QHash<QString, QString> mMyProfile;
    // channel settings
    QString strChannelSettings;
    QHash<QString,QString> mChannelSettingsInfo;
    QMultiHash<QString,QString> mChannelSettingsPermissions;
    bool bChannelSettingsInfo;
    QHash<QString,QString> mChannelSettingsStats;
    bool bChannelSettingsStats;
    // moderation
    QMultiHash<QString,OnetModerateMessage> mModerateMessages;
    // channel info
    QList<QString> lChannelInfo; // moderated

public slots:
    void quit();

private:
    MainWindow *window;
    QTranslator qtTranslator;
    QTranslator sccTranslator;

    void init();
    void fixSettings();
};

#endif // CORE_H
