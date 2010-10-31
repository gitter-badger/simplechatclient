/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include <QDateTime>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMap>
#include <QWidget>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>
#include "convert.h"
#include "log.h"
#include "network.h"
#include "mainwebview.h"
#include "notify.h"

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *, Network *, QString, Notify *, QMap <QString, QByteArray> *, QTcpSocket *, sChannelNickStatus *);
    ~TabWidget();
    inline QString get_name() { return strName; }
    inline MainWebView* get_webview() { return mainWebView; }
    QString addslashes(QString);
    void display_msg(QString, QString, int);
    void display_msg(QString, int);
    void display_message(QString, int); // private, exteption: inputlinewidget
    void set_topic(QString);
    void author_topic(QString);
    void update_channel_avatar();
    void set_open_channels(QStringList);
    void change_font_size(QString);
    void clear_content();
    void set_scroll(bool);
    void refresh_colors();
    void send_message(bool);

private:
    // params
    QWidget *myparent;
    Network *pNetwork;
    QString strName;
    Notify *pNotify;
    QMap <QString, QByteArray> *mChannelAvatar;
    QTcpSocket *camSocket;
    sChannelNickStatus *mChannelNickStatus;

    // other
    QString strContent;
    QString strContentStart;
    QString strContentEnd;
    QString strFontSize;

    QWidget *topWidget;
    MainWebView *mainWebView;

    QVBoxLayout *mainLayout;
    QWidget *mainWidget;
    QWidget *topLeftWidget;
    QVBoxLayout *topLeftLayout;
    QWidget *topRightWidget;
    QVBoxLayout *topRightLayout;
    QHBoxLayout *topLayout;
    QWebView *topic;
    QLabel *logo;
    QLabel *topicDetails;

    QString strTopicContent;
    bool bCursorPositionChanged;
    int iScrollBarValue;
    bool bScroll;

    void replace_color(QString, QString);

private slots:
    void change_scroll_position();

};

#endif // TAB_WIDGET_H
