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

#ifndef NICKLIST_WIDGET_H
#define NICKLIST_WIDGET_H

#include "defines.h"
#include <QListWidget>

class NickListWidget : public QListWidget
{
    Q_OBJECT
public:
    NickListWidget(const QString &_strChannel);
    void addUser(const QString &strNick, QString strModes);
    void delUser(const QString &strNick);
    void renameUser(const QString &strNick, const QString &strNewNick);
    bool existUser(const QString &strNick);
    void setUserAvatar(const QString &strNick, const QString &strValue);
    QString getUserAvatar(const QString &strNick);
    void changeUserFlag(const QString &strNick, QString strFlag);
    QString getUserModes(const QString &strNick);
    QList<QString> getUserList();

private:
    // other
    QString strChannel;
    QAction *openChannelsActs[MAX_OPEN_CHANNELS];
    QAction *kickReasonAct[MAX_PUNISH_REASONS];
    QAction *kbanReasonAct[MAX_PUNISH_REASONS];
    QString strSelectedNick;
    enum NickListRoles
    {
        NickListModesRole = Qt::UserRole+20,
        NickListAvatarUrlRole = Qt::UserRole+21
    };

    void convertFlag(QString &strFlag);

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
    void kickWithReason();
    void kickWithSelectedReason();
    void ban();
    void kbanWithReason();
    void kbanWithSelectedReason();
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
    virtual void contextMenuEvent(QContextMenuEvent *e);
};

#endif // NICKLIST_WIDGET_H
