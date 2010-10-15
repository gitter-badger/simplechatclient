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
#include <QLineEdit>
#include <QMap>
#include <QMenu>
#include <QPushButton>
#include <QRgb>
#include <QTextEdit>
#include <QWidget>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>
#include "convert.h"
#include "dlg_channel_settings.h"
#include "dlg_emoticons.h"
#include "dlg_moderation.h"
#include "inputlinedockwidget.h"
#include "log.h"
#include "network.h"
#include "mainwebview.h"
#include "notify.h"

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *, Network *, QString, Notify *, QMap <QString, QByteArray> *, DlgChannelSettings *, DlgModeration *, QTcpSocket *, InputLineDockWidget *, sChannelNickStatus *);
    ~TabWidget();
    inline QString get_name() { return strName; }
    inline MainWebView* get_webview() { return mainWebView; }
    void set_default();
    QString addslashes(QString);
    void display_msg(QString, QString, int);
    void display_msg(QString, int);
    void display_message(QString, int); // private, exteption: inputlinewidget
    void set_topic(QString);
    void enable_moderation();
    void disable_moderation();
    void author_topic(QString);
    void update_channel_avatar();
    void set_open_channels(QStringList);
    void refresh_colors();
    void send_message(bool);

private:
    QString strContent;
    QString strContentStart;
    QString strContentEnd;
    QString strFontSize;

    bool bMyBold;
    bool bMyItalic;
    QString strMyFontWeight;
    QString strMyFontStyle;
    QString strMyFontFamily;

    QWidget *topWidget;
    MainWebView *mainWebView;
    QWidget *toolWidget;

    QWidget *topLeftWidget;
    QVBoxLayout *topLeftLayout;
    QWidget *topRightWidget;
    QVBoxLayout *topRightLayout;
    QHBoxLayout *topLayout;
    QWebView *topic;
    QLabel *logo;
    QLabel *topicDetails;

    QHBoxLayout *toolLayout;
    QPushButton *bold;
    QPushButton *italic;
    QPushButton *fontfamily;
    QComboBox *color;
    QPushButton *emoticons;
    QFrame *separator;
    QPushButton *channel_settings;
    QPushButton *moderation;
    QPushButton *clear;
    QPushButton *scroll;

    QMenu *fontMenu;
    QAction *arialAct;
    QAction *timesAct;
    QAction *verdanaAct;
    QAction *tahomaAct;
    QAction *courierAct;

    QPushButton *size;
    QMenu *sizeMenu;
    QAction *size8Act;
    QAction *size9Act;
    QAction *size10Act;
    QAction *size11Act;
    QAction *size12Act;
    QAction *size14Act;
    QAction *size16Act;
    QAction *size18Act;
    QAction *size20Act;

    QVBoxLayout *mainLayout;
    QWidget *mainWidget;
    DlgChannelSettings *dlgchannel_settings;
    DlgModeration *dlgmoderation;
    Notify *pNotify;
    QMap <QString, QByteArray> *mChannelAvatar;
    QTcpSocket *camSocket;
    InputLineDockWidget *inputLineWidget;
    sChannelNickStatus *mChannelNickStatus;

    QWidget *myparent;
    Network *pNetwork;
    QString strName;
    QString strCurrentColor;
    QString strTopicContent;
    bool bCursorPositionChanged;
    bool bScroll;
    int iScrollBarValue;

    void replace_color(QString, QString);

private slots:
    void bold_clicked();
    void italic_clicked();
    void arial_triggered();
    void times_triggered();
    void verdana_triggered();
    void tahoma_triggered();
    void courier_triggered();

    void size8_triggered();
    void size9_triggered();
    void size10_triggered();
    void size11_triggered();
    void size12_triggered();
    void size14_triggered();
    void size16_triggered();
    void size18_triggered();
    void size20_triggered();

    void color_clicked(int);
    void emoticons_clicked();
    void channel_settings_clicked();
    void moderation_clicked();
    void clear_clicked();
    void scroll_clicked();

    void change_scroll_position();

};

#endif // TAB_WIDGET_H
