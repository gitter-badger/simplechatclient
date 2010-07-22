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
#include "config.h"
#include "dlg_channel_settings.h"
#include "dlg_moderation.h"
#include "log.h"
#include "network.h"
#include "qinputline.h"
#include "qnicklist.h"
#include "qnicklistdelegate.h"
#include "qnotify.h"
#include "replace.h"

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(Network *, QSettings *, QString, QWidget *, Notify *, QMap <QString, QByteArray> *, QMap <QString, QByteArray> *, DlgChannelSettings *, DlgModeration *);
    ~TabWidget();
    QString get_name() { return strName; }
    QString convert_emots(QString);
    QString replace_emots(QString);
    void display_msg(QString, QString, int);
    void display_msg(QString, int);
    void set_topic(QString);
    void enable_topic();
    void disable_topic();
    void enable_moderation();
    void disable_moderation();
    void author_topic(QString);
    void set_link(QString);
    void add_user(QString, QString, int);
    void del_user(QString);
    void change_flag(QString, QString);
    void update_nick(QString);
    void clear_nicklist();
    void set_open_channels(QStringList);
    bool nicklist_exist(QString);
    void update_nick_avatar();
    void update_channel_avatar();
    void nicklist_refresh_all();
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
    Nicklist *nick_list;
    QWebView *textEdit;
    QWidget *toolWidget;
    QWidget *bottomWidget;

    QWidget *topLeftWidget;
    QVBoxLayout *topLeftLayout;
    QWidget *topRightUpWidget;
    QHBoxLayout *topRightUpLayout;
    QWidget *topRightWidget;
    QVBoxLayout *topRightLayout;
    QHBoxLayout *topLayout;
    QLineEdit *topic;
    QPushButton *topicButton;
    QLabel *logo;
    QLabel *topicDetails;

    QHBoxLayout *toolLayout;
    QPushButton *bold;
    QPushButton *italic;
    QPushButton *fontfamily;
    QComboBox *color;
    QPushButton *channel_settings;
    QPushButton *moderation;

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

    Network *pNetwork;
    QSettings *settings;
    QString strName;
    QString strLast_msg;
    QMap <QString, QString> nick_flag;
    QMap <QString, QString> nick_suffix;
    QHash <QString, QString> nicklist;
    QQueue <QString> new_nicklist1;
    QQueue <QString> new_nicklist2;
    int iNickCount;
    bool bCursorPositionChanged;
    QString strCurrentColor;

    void nicklist_add(QString, QString);
    void nicklist_remove(QString);
    void nicklist_clear();
    void nicklist_refresh();
    void nicklist_sort();
    void nicklist_quicksort(QString);

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

    void channel_settings_clicked();
    void moderation_clicked();
    void inputline_return_pressed();
    void moder_button_clicked();
    void topic_return_pressed();

    void change_scroll_position();

protected:
    virtual void keyPressEvent(QKeyEvent*);

};

#endif // TAB_WIDGET_H
