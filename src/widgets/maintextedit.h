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
#ifdef HAVE_V4L2
    class DlgCam;
#endif
#endif
class AnimatedEmoticonWidget;
class DlgUserProfile;
class Network;
#include <QTcpSocket>
#include <QTextEdit>

class MainTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MainTextEdit(Network *, QString, QTcpSocket *, DlgUserProfile *);
#ifndef Q_WS_WIN
#ifdef HAVE_V4L2
    void set_dlg_cam(DlgCam *);
#endif
#endif
    void update_background_image();
    inline QString get_current_nick() { return strNick; }

private:
    // params
    Network *pNetwork;
    QTcpSocket *camSocket;
    DlgUserProfile *pDlgUserProfile;
#ifndef Q_WS_WIN
#ifdef HAVE_V4L2
    DlgCam *pDlgCam;
#endif
#endif
    // other
    QString strChannel;
    QString strNick;
    QString strWebsite;
    enum { maxOpenChannels = 30 };
    QAction *openChannelsActs[maxOpenChannels];
    AnimatedEmoticonWidget *pAnimatedEmoticonWidget;

    void menu_nick(QString, QContextMenuEvent *);
    void menu_channel(QString, QContextMenuEvent *);
    void menu_website(QContextMenuEvent *);
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
    void text_changed();
    void open_webbrowser();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
};

#endif // MAINTEXTEDIT_H
