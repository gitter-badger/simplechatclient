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
    TabContainer(TabManager *_pTabM);
    virtual ~TabContainer();
    bool existTab(const QString &strChannel);
    void addTab(const QString &strChannel);
    void removeTab(const QString &strChannel);
    void renameTab(const QString &strChannel, const QString &strNewName);
    void partTab(int index);
    void resizeMainWindow(QSize s);

public slots:
    void refreshColors();
    void refreshCSS();
    void showMessage(const QString &strChannel, const QString &strData, MessageCategory eMessageCategory, QString strTime = QString::null, QString strNick = QString::null);
    void showMessageAll(const QString &strData, MessageCategory eMessageCategory);
    void showMessageActive(const QString &strData, MessageCategory eMessageCategory);
    void setTopic(const QString &strChannel, const QString &strTopic);
    void authorTopic(const QString &strChannel, const QString &strNick);
    void setChannelAvatar(const QString &strChannel);
    void clearContent(const QString &strChannel);
    // nicklist
    void addUser(const QString &strChannel, const QString &strNick, const QString &strModes, bool bFastAdd);
    void delUser(const QString &strChannel, const QString &strNick);
    void quitUser(const QString &strNick, const QString &strDisplay);
    void changeFlag(const QString &strNick, const QString &strChannel, const QString &strFlag);
    void changeFlag(const QString &strNick, const QString &strFlag);
    void nicklistRefresh(const QString &strChannel);
    void clearAllNicklist();
    void setUserAvatarPath(const QString &strNick, const QString &strValue);
    QString getUserAvatarPath(const QString &strNick);
    int getUserCount(const QString &strChannel);
    QString getUserModes(const QString &strNick, const QString &strChannel);
    QList<QString> getUserList(const QString &strChannel);

private:
    // params
    TabManager *pTabM;
    // other
    QHash<QString, TabWidget*> tw;

    void logOpened(const QString &strChannel);
    void logClosed(const QString &strChannel);
    bool isHighlightMessage(const QString &strMessage);

signals:
    void setModeration(bool);
    void updateAwaylogStatus();
};

#endif // TAB_CONTAINER_H
