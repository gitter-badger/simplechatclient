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

#ifndef TAB_CONTAINER_H
#define TAB_CONTAINER_H

#include "defines.h"
class TabManager;
class TabWidget;
#include <QObject>

/**
 * Class for displaying messages in tabs or change main window settings
 */
class TabContainer : public QObject
{
    Q_OBJECT
public:
    TabContainer(TabManager *);
    virtual ~TabContainer();
    bool existTab(const QString&);
    void addTab(const QString&);
    void removeTab(const QString&);
    void renameTab(const QString&, const QString&);
    void partTab(int);
    void resizeMainWindow(QSize);

public slots:
    void refreshColors();
    void refreshCSS();
    void showMessage(const QString&, const QString&, MessageCategory, QString strTime = QString::null, QString strNick = QString::null);
    void showMessageAll(const QString&, MessageCategory);
    void showMessageActive(const QString&, MessageCategory);
    void setTopic(const QString&, const QString&);
    void authorTopic(const QString&, const QString&);
    void setChannelAvatar(const QString&);
    void clearContent(const QString&);
    // nicklist
    void addUser(const QString&, const QString&, const QString&, bool);
    void delUser(const QString&, const QString&);
    void quitUser(const QString&, const QString&);
    void changeFlag(const QString&, const QString&, const QString&);
    void changeFlag(const QString&, const QString&);
    void nicklistRefresh(const QString&);
    void clearAllNicklist();
    void setUserAvatarPath(const QString&, const QString&);
    QString getUserAvatarPath(const QString&);
    int getUserCount(const QString&);
    QString getUserModes(const QString&, const QString&);
    QList<QString> getUserList(const QString&);

private:
    // params
    TabManager *pTabM;
    // other
    QHash<QString, TabWidget*> tw;

    void logOpened(const QString&);
    void logClosed(const QString&);
    bool isHighlightMessage(const QString&);

signals:
    void setModeration(bool);
    void updateAwaylogStatus();
};

#endif // TAB_CONTAINER_H
