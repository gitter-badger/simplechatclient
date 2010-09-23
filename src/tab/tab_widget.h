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

#include <QAction>
#include <QtAlgorithms>
#include <QDateTime>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMap>
#include <QMenu>
#include <QPushButton>
#include <QQueue>
#include <QRgb>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>
#include <QWidget>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>
#include "commands.h"
#include "convert.h"
#include "dlg_channel_settings.h"
#include "dlg_emoticons.h"
#include "dlg_moderation.h"
#include "log.h"
#include "network.h"
#include "inputline.h"
#include "mainwebview.h"
#include "nicklistdelegate.h"
#include "nicklistwidget.h"
#include "notify.h"
#include "replace.h"

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *, Network *, QString, Notify *, QMap <QString, QByteArray> *, QMap <QString, QByteArray> *, DlgChannelSettings *, DlgModeration *, QTcpSocket *);
    ~TabWidget();
    QString get_name() { return strName; }
    void set_default();
    QString addslashes(QString);
    QString convert_emots(QString);
    QString replace_emots(QString);
    void display_msg(QString, QString, int);
    void display_msg(QString, int);
    void set_topic(QString);
    void enable_moderation();
    void disable_moderation();
    void author_topic(QString);
    void set_link(QString);
    void add_user(QString, QString, QString);
    void del_user(QString);
    void change_flag(QString, QString);
    void update_nick(QString);
    void clear_nicklist();
    void set_user_info(QString, QString, QString);
    void set_open_channels(QStringList);
    bool nicklist_exist(QString);
    void update_nick_avatar();
    void update_channel_avatar();
    void refresh_colors();
    void nicklist_refresh_all();
    QStringList get_nicklist();
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
    QLabel *nickCount;
    QLabel *webLink;
    NickListWidget *nicklist;
    MainWebView *mainWebView;
    QWidget *toolWidget;
    QWidget *bottomWidget;

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
    QLabel *separator;
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
    QAction *size18Act;
    QAction *size20Act;

    QHBoxLayout *bottomLayout;
    QLabel *nickLabel;
    Inputline *inputline;
    QPushButton *sendButton;
    QPushButton *moderSendButton;

    QSplitter *splitter;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QWidget *leftWidget;
    QWidget *rightWidget;
    QGridLayout *mainLayout;
    DlgChannelSettings *dlgchannel_settings;
    DlgModeration *dlgmoderation;
    Notify *pNotify;
    QMap <QString, QByteArray> *mNickAvatar;
    QMap <QString, QByteArray> *mChannelAvatar;
    QTcpSocket *camSocket;

    QWidget *myparent;
    Network *pNetwork;
    QString strName;
    QString strLast_msg;
    sNickStatus nickStatus;
    int iNickCount;
    bool bCursorPositionChanged;
    QString strCurrentColor;
    QString strTopicContent;
    bool bScroll;
    int iScrollBarValue;

    void replace_color(QString, QString);

    void nicklist_add(QString, QString, QString);
    void nicklist_remove(QString);

    void display_message(QString, int);

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
    void size18_triggered();
    void size20_triggered();

    void color_clicked(int);
    void emoticons_clicked();
    void channel_settings_clicked();
    void moderation_clicked();
    void clear_clicked();
    void scroll_clicked();
    void inputline_return_pressed();
    void moder_button_clicked();

    void change_scroll_position();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *);

};

#endif // TAB_WIDGET_H
