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

#ifndef CHATVIEW_H
#define CHATVIEW_H

#include "defines.h"
class DlgUserProfile;
#include <QTextEdit>

class ChatView : public QTextEdit
{
    Q_OBJECT
public:
    ChatView(QString, DlgUserProfile *);
    void displayMessage(QString &, MessageCategory, QString strTime = QString::null);
    void updateBackgroundImage();
    inline QString getCurrentNick() { return strNick; }

private:
    // params
    DlgUserProfile *pDlgUserProfile;
    // other
    QString strChannel;
    QString strNick;
    QString strWebsite;
    enum { maxOpenChannels = 30 };
    QAction *openChannelsActs[maxOpenChannels];

    void menuNick(QString, QContextMenuEvent *);
    void menuChannel(QString, QContextMenuEvent *);
    void menuWebsite(QContextMenuEvent *);
    void menuStandard(QContextMenuEvent *);

    int getWordIndex(QString, int);
    QString getWord(QTextCursor);
    QString getWordN(QTextCursor, int);
    int getWordPosIndex(QTextCursor);
    bool isJoinPartQuit(QTextCursor);

private slots:
    void joinChannel();
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
    void openWebbrowser();
    void sendToNotes();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *);
};

#endif // CHATVIEW_H
