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
    bool existTab(QString);
    void addTab(QString);
    void removeTab(QString);
    void renameTab(QString, QString);
    void partTab(int);
    void resizeMainWindow(QSize);

public slots:
    void refreshColors();
    void refreshCSS();
    void showMessage(QString &, QString &, MessageCategory, QString strTime = QString::null, QString strNick = QString::null);
    void showMessageAll(QString &, MessageCategory);
    void showMessageActive(QString &, MessageCategory);
    void setTopic(QString &, QString &);
    void authorTopic(QString &, QString &);
    void setChannelAvatar(QString);
    void clearContent(QString);
    // nicklist
    void addUser(QString, QString, QString, bool);
    void delUser(QString, QString);
    void quitUser(QString, QString);
    void changeFlag(QString, QString, QString);
    void changeFlag(QString, QString);
    void nicklistRefresh(QString);
    void clearAllNicklist();
    void setUserAvatarPath(QString, QString);
    QString getUserAvatarPath(QString);
    int getUserCount(QString);
    QString getUserModes(QString, QString);
    QList<QString> getUserList(QString);

private:
    // params
    TabManager *pTabM;
    // other
    QList<TabWidget*> tw;

    void logOpened(QString);
    void logClosed(QString);
    int getIndex(QString);
    bool isHighlightMessage(QString);

signals:
    void setModeration(bool);
    void updateAwaylogStatus();
};

#endif // TAB_CONTAINER_H
