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

#ifndef Q_WS_WIN
#ifdef HAVE_V4L2
    class DlgCam;
#endif
#endif
class DlgUserProfile;
class MainTextEdit;
class MainWindow;
class Network;
class TabManager;
#include <QLabel>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

/**
 * Main widget used for displaying all text messages from users
 */
class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(Network *, QString, QTcpSocket *, DlgUserProfile *);
    virtual ~TabWidget();
#ifndef Q_WS_WIN
#ifdef HAVE_V4L2
    void set_dlg_cam(DlgCam *);
#endif
#endif
    inline QString get_name() { return strName; }
    QString addslashes(QString);
    void display_msg(QString, QString, int);
    void display_msg(QString, int);
    void display_message(QString, int); // private, exteption: inputlinewidget
    void set_topic(QString);
    void author_topic(QString);
    void set_link(QString);
    void update_channel_avatar();
    inline void change_font_size(QString f) { strFontSize = f; }
    void clear_content();
    void refresh_colors();
    void refresh_background_image();

private:
    // params
    Network *pNetwork;
    QString strName;
    QTcpSocket *camSocket;
    DlgUserProfile *pDlgUserProfile;
#ifndef Q_WS_WIN
#ifdef HAVE_V4L2
    DlgCam *pDlgCam;
#endif
#endif

    // other
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

    QTextEdit *topic;
    QLabel *avatar;
    QLabel *topicDetails;
    QLabel *websiteLink;
    MainTextEdit *pMainTextEdit;
};

#endif // TAB_WIDGET_H
