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

#ifndef MAINTEXTEDIT_H
#define MAINTEXTEDIT_H

#ifndef Q_WS_WIN
    class DlgCam;
#endif
class DlgUserProfile;
class Network;
#include <QTcpSocket>
#include <QTextEdit>

class MainTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MainTextEdit(Network *, QString, QTcpSocket *, DlgUserProfile *);
    virtual ~MainTextEdit();
#ifndef Q_WS_WIN
    void set_dlg_cam(DlgCam *);
#endif
    void update_background_image();
    void set_open_channels(QStringList);
    inline QString get_current_nick() { return strNick; }

private:
    // params
    Network *pNetwork;
    QString strChannel;
    QTcpSocket *camSocket;
    DlgUserProfile *pDlgUserProfile;
#ifndef Q_WS_WIN
    DlgCam *pDlgCam;
#endif
    // other
    QString strNick;
    QStringList strOpenChannels;
    enum { maxOpenChannels = 30 };
    QAction *openChannelsActs[maxOpenChannels];

    void menu_nick(QString, QContextMenuEvent *);
    void menu_channel(QString, QContextMenuEvent *);
    void menu_standard(QContextMenuEvent *);

    int get_word_index(QString, int);
    QString get_word(QTextCursor);
    QString get_first_word(QTextCursor);


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
    void ipban();
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

#endif // MAINTEXTEDIT_H
