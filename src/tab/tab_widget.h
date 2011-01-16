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

#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include "defines.h"
class DlgCam;
class DlgUserProfile;
class MainWebView;
class Network;
class Notify;
class TabManager;
#include <QLabel>
#include <QTcpSocket>
#include <QtWebKit/QWebView>
#include <QVBoxLayout>
#include <QWidget>

/**
 * Main widget used for displaying all text messages from users
 */
class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *, Network *, QString, Notify *, QMap <QString, QByteArray> *, QTcpSocket *, sChannelNickStatus *, DlgUserProfile *, DlgCam *, QList<QString> *);
    ~TabWidget();
    inline QString get_name() { return strName; }
    inline bool get_scroll() { return bScroll; }
    inline void add_user() { iNickCount++; } // update nick count for option hide join/part when > 200
    inline void del_user() { iNickCount--; } // update nick count for option hide join/part when > 200
    inline void clear_users() { iNickCount = 0; } // update nick count for option hide join/part when > 200
    inline int get_users() { return iNickCount; }
    QString addslashes(QString);
    void display_msg(QString, QString, int);
    void display_msg(QString, int);
    void display_message(QString, int); // private, exteption: inputlinewidget
    void set_topic(QString);
    void author_topic(QString);
    void set_link(QString);
    void update_channel_avatar();
    void set_open_channels(QStringList);
    void change_font_size(QString);
    void clear_content();
    void set_scroll(bool);
    void refresh_colors();

private:
    // params
    QWidget *myparent;
    Network *pNetwork;
    QString strName;
    Notify *pNotify;
    QMap <QString, QByteArray> *mChannelAvatar;
    QTcpSocket *camSocket;
    sChannelNickStatus *mChannelNickStatus;
    DlgUserProfile *pDlg_user_profile;
    DlgCam *pDlg_cam;
    QList<QString> *lAwaylog;

    // other
    QString strContent;
    QString strContentStart;
    QString strContentEnd;
    QString strFontSize;

    QVBoxLayout *mainLayout;
    QWidget *mainWidget;
    QWidget *topLeftWidget;
    QVBoxLayout *topLeftLayout;
    QWidget *topRightWidget;
    QVBoxLayout *topRightLayout;
    QWidget *detailsWidget;
    QHBoxLayout *detailsLayout;
    QWidget *topWidget;
    QHBoxLayout *topLayout;

    QWebView *topic;
    QLabel *avatar;
    QLabel *topicDetails;
    QLabel *websiteLink;
    MainWebView *mainWebView;

    QString strTopicContent;
    bool bCursorPositionChanged;
    int iScrollBarValue;
    bool bScroll;
    int iNickCount;

    void replace_color(QString, QString);

private slots:
    void change_scroll_position();
};

#endif // TAB_WIDGET_H
