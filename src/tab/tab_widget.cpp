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

#include <QDateTime>
#include <QFile>
#include <QSettings>
#include "convert.h"
#include "core.h"
#include "log.h"
#include "network.h"
#include "maintextedit.h"
#include "notify.h"
#include "tab_widget.h"

#ifndef Q_WS_WIN
    #include "dlg_cam.h"
#endif

TabWidget::TabWidget(Network *param1, QString param2, QTcpSocket *param3, DlgUserProfile *param4)
{
    pNetwork = param1;
    strName = param2;
    camSocket = param3;
    pDlgUserProfile = param4;

    QSettings settings;
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());
    strFontSize = settings.value("font_size").toString();

    iNickCount = 0;

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainWidget = new QWidget(this);

    topic = new QTextEdit(this);
    topic->setReadOnly(true);
    topic->setAcceptRichText(false);
    topic->setMinimumHeight(30);
    topic->setMinimumWidth(16777215);
    topic->setMaximumHeight(30);
    topic->setMaximumWidth(16777215);
    topic->show();

    topicDetails = new QLabel(this);
    topicDetails->setMargin(0);
    topicDetails->setOpenExternalLinks(false);
    topicDetails->setAlignment(Qt::AlignLeft);
    topicDetails->show();

    websiteLink = new QLabel(this);
    websiteLink->setMargin(0);
    websiteLink->setOpenExternalLinks(true);
    websiteLink->setAlignment(Qt::AlignRight);
    websiteLink->show();

    avatar = new QLabel(this);
    avatar->setMaximumSize(QSize(50,50));
    avatar->setMargin(0);
    avatar->show();

    detailsWidget = new QWidget(this);
    detailsLayout = new QHBoxLayout();
    detailsLayout->setMargin(0);
    detailsLayout->addWidget(topicDetails);
    detailsLayout->addWidget(websiteLink);
    detailsWidget->setLayout(detailsLayout);

    topRightWidget = new QWidget(this);
    topRightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    topRightLayout = new QVBoxLayout();
    topRightLayout->setMargin(0);
    topRightLayout->setAlignment(Qt::AlignTop);
    topRightLayout->addWidget(topic);
    topRightLayout->addWidget(detailsWidget);
    topRightWidget->setLayout(topRightLayout);

    topLeftWidget = new QWidget(this);
    topLeftLayout = new QVBoxLayout();
    topLeftLayout->setMargin(0);
    topLeftLayout->setAlignment(Qt::AlignTop);
    topLeftLayout->addWidget(avatar);
    topLeftWidget->setLayout(topLeftLayout);

    topWidget = new QWidget(this);
    topWidget->setMaximumHeight(50);
    topLayout = new QHBoxLayout();
    topLayout->setMargin(0);
    topLayout->setAlignment(Qt::AlignLeft);
    topLayout->addWidget(topLeftWidget);
    topLayout->addWidget(topRightWidget);
    topWidget->setLayout(topLayout);

    pMainTextEdit = new MainTextEdit(pNetwork, strName, camSocket, pDlgUserProfile);
    pMainTextEdit->document()->setMaximumBlockCount(1000);
    pMainTextEdit->setReadOnly(true);
    pMainTextEdit->setAcceptRichText(false);
    pMainTextEdit->setParent(this);
    pMainTextEdit->show();

    if (strName[0] == '#')
    {
        if (settings.value("style") == "classic")
        {
            avatar->hide();
            topic->hide();
            topicDetails->hide();
            websiteLink->hide();
            detailsWidget->hide();
            topRightWidget->hide();
            topLeftWidget->hide();
            topWidget->hide();
        }
        else
            mainLayout->addWidget(topWidget);

        mainLayout->addWidget(pMainTextEdit);
        mainWidget->setLayout(mainLayout);
    }
    else if (strName[0] == '^')
    {
        avatar->hide();
        topic->hide();
        topicDetails->hide();
        websiteLink->hide();
        detailsWidget->hide();
        topRightWidget->hide();
        topLeftWidget->hide();
        topWidget->hide();

        mainLayout->addWidget(pMainTextEdit);
        mainWidget->setLayout(mainLayout);
    }
    else
    {
        avatar->hide();
        topic->hide();
        topicDetails->hide();
        websiteLink->hide();
        detailsWidget->hide();
        topRightWidget->hide();
        topLeftWidget->hide();
        topWidget->hide();

        mainLayout->addWidget(pMainTextEdit);
        mainWidget->setLayout(mainLayout);
    }

    // hide avatar
    topLeftWidget->hide();
    // hide topic details
    topicDetails->hide();
    // hide website link
    websiteLink->hide();

    // show layout
    this->setLayout(mainLayout);

    // set colors
    if ((strBackgroundColor.toLower() != "#ffffff") && (strBackgroundColor.isEmpty() == false))
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);
}

TabWidget::~TabWidget()
{
    pMainTextEdit->clear();
}

#ifndef Q_WS_WIN
void TabWidget::set_dlg_cam(DlgCam *param1)
{
    pDlgCam = param1;
    pMainTextEdit->set_dlg_cam(pDlgCam);
}
#endif

QString TabWidget::addslashes(QString strData)
{
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");
    strData.replace("\"", "");
    strData.replace("\'", "");
    strData.replace(";", "");
    strData.replace("%", "");

    return strData;
}

void TabWidget::display_msg(QString strTime, QString strData, int iLevel)
{
    QDateTime dt = QDateTime::fromTime_t(strTime.toUInt());
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    QSettings settings;
    if (settings.value("disable_logs").toString() == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings.value("hide_join_part").toString() == "on")
            return;

        if ((settings.value("hide_join_part_200").toString() == "on") && (iNickCount > 200))
            return;
    }

    display_message(strData, iLevel);
}

void TabWidget::display_msg(QString strData, int iLevel)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    QSettings settings;
    if (settings.value("disable_logs").toString() == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

    if ((iLevel == 1) || (iLevel == 2) || (iLevel == 3))
    {
        if (settings.value("hide_join_part").toString() == "on")
            return;

        if ((settings.value("hide_join_part_200").toString() == "on") && (iNickCount > 200))
            return;
    }

    display_message(strData, iLevel);
}

void TabWidget::display_message(QString strData, int iLevel)
{
    QSettings settings;

    // awaylog
    if (iLevel == 8)
    {
        if (settings.value("away").toString() == "on")
        {
            QString strAwayData = strData;

            // if /me remove time,action <>
            if (strAwayData.indexOf(QString(QByteArray("\x01"))) != -1)
            {
                strAwayData = strAwayData.right(strAwayData.length() - 11);
                if (strAwayData.indexOf("ACTION ") != -1) strAwayData = strAwayData.replace("ACTION ", QString::null);
                if (strAwayData.indexOf("<") != -1) strAwayData = strAwayData.remove(strAwayData.indexOf("<"),4);
                if (strAwayData.indexOf(">") != -1) strAwayData = strAwayData.remove(strAwayData.indexOf(">"),4);
            }

            // remove color, font, emots
            strAwayData.replace(QRegExp("%C([a-zA-Z0-9]+)%"),"");
            strAwayData.replace(QRegExp("%F([a-zA-Z0-9:]+)%"),"");
            strAwayData.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

            Core::instance()->lAwaylog.append(QString("%1\n%2").arg(strName).arg(strAwayData));
        }
    }

    // fix data
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");

    // channels
    strData.replace(QRegExp("#([~-_a-zA-Z0-9\xa1\xaf\xa6\xac\xca\xc6\xd1\xd3\xa3\xb1\xbf\xb6\xbc\xea\xe6\xf1\xf3\xb3]+)"), "<span style=\"color:"+addslashes(settings.value("channel_font_color").toString())+";text-decoration:none;\">#\\1</span>");

    // content last
    QString strContentLast;

    // colors
    QString strFontColor;

    if (iLevel == 0)
        strFontColor = addslashes(settings.value("default_font_color").toString()); // default black
    else if (iLevel == 1) // join
        strFontColor = addslashes(settings.value("font_color_level_1").toString()); // default green
    else if (iLevel == 2) // part
        strFontColor = addslashes(settings.value("font_color_level_2").toString()); // default light blue
    else if (iLevel == 3) // quit
        strFontColor = addslashes(settings.value("font_color_level_3").toString()); // default dark blue
    else if (iLevel == 4) // kick
        strFontColor = addslashes(settings.value("font_color_level_4").toString()); // default dark blue
    else if (iLevel == 5) // mode
        strFontColor = addslashes(settings.value("font_color_level_5").toString()); // default green
    else if (iLevel == 6) // notice
        strFontColor = addslashes(settings.value("font_color_level_6").toString()); // default blue
    else if (iLevel == 7) // info
        strFontColor = addslashes(settings.value("font_color_level_7").toString()); // default gray
    else if (iLevel == 8) // hilight no color
        strFontColor = addslashes(settings.value("default_font_color").toString()); // default black
    else if (iLevel == 9) // error
        strFontColor = addslashes(settings.value("font_color_level_9").toString()); // default red
    else
    {
        iLevel = 0;
        strFontColor = addslashes(settings.value("default_font_color").toString()); // default black
    }

    strData.insert(11, "<span style=\"color:"+strFontColor+";\">");
    strContentLast = "</span>"+strContentLast;

    // if /me remove time,action <>
    if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
    {
        strData = strData.right(strData.length() - 11);
        if (strData.indexOf("ACTION ") != -1) strData = strData.replace("ACTION ", QString::null);
        if (strData.indexOf("&lt;") != -1) strData = strData.remove(strData.indexOf("&lt;"),4);
        if (strData.indexOf("&gt;") != -1) strData = strData.remove(strData.indexOf("&gt;"),4);
    }

    // convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convert_text(&strData, &strContentLast);
    delete convertText;

    // hilight
    QString strTextDecoration = "none";
    if (iLevel == 8)
    {
        strTextDecoration = "underline";

        // sound
        if (settings.value("disable_sounds").toString() == "off")
            Notify::instance()->play(Beep);
    }

    // /me
    QString strAlign = "left";
    if (settings.value("hide_formating").toString() == "off")
    {
        if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
        {
            strAlign = "center";
            strData.remove(QString(QByteArray("\x01")));
        }
    }

    // init text
    QString strContent = "<span style=\"color:"+addslashes(settings.value("default_font_color").toString())+";font-size:"+strFontSize+";text-decoration:"+strTextDecoration+";\">";
    strContent = strContent+strData+strContentLast+"</span>";

    // move cursor - fix align bug
    QTextCursor cursor(pMainTextEdit->document());
    cursor.movePosition(QTextCursor::End);

    // text align
    QTextBlockFormat format;
    if (strAlign == "center")
        format.setAlignment(Qt::AlignCenter);
    else
        format.setAlignment(Qt::AlignLeft);

    // insert text
    cursor.insertBlock(format);
    cursor.insertHtml(strContent);

    // move cursor
    if (pMainTextEdit->textCursor().selectedText().isEmpty() == true)
        pMainTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

// window options

void TabWidget::set_topic(QString strTopic)
{
    // colors
    QSettings settings;
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());
    topic->setTextColor(QColor(strDefaultFontColor));
    topic->setTextBackgroundColor(QColor(strBackgroundColor));

    // replace
    strTopic.replace("&", "&amp;");
    strTopic.replace("<", "&lt;");
    strTopic.replace(">", "&gt;");

    QString strContent = strTopic;
    QString strLastContent;

    // convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convert_text(&strContent,&strLastContent);
    delete convertText;

    // set topic
    topic->setHtml(strContent+strLastContent);

    // tooltip
    strTopic.replace(QRegExp("%C(\\S+)%"),"");
    strTopic.replace(QRegExp("%F(\\S+)%"),"");
    strTopic.replace(QRegExp("%I(\\S+)%"),"<\\1>");

    if (settings.value("style") == "modern")
        topic->setToolTip(strTopic);
    else if (settings.value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::author_topic(QString strAuthor)
{
    topicDetails->show();
    topicDetails->setText(QString(tr("Topic set by %1")).arg(strAuthor));

    QSettings settings;
    if (settings.value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::set_link(QString strUrl)
{
    websiteLink->show();

    websiteLink->setText(QString("<a href=\"%1\" style=\"color:#0000FF;text-decoration:none;\" >&nbsp;&nbsp;"+tr("Channel website")+"&nbsp;&nbsp;</a>").arg(strUrl));
    websiteLink->setToolTip(strUrl);
}

void TabWidget::update_channel_avatar()
{
    if (Core::instance()->mChannelAvatar.contains(strName) == true)
    {
        // show widget
        topLeftWidget->show();

        // display avatar
        QPixmap pixmap;
        pixmap.loadFromData(Core::instance()->mChannelAvatar.value(strName));
        avatar->setPixmap(pixmap);
    }
}

void TabWidget::set_open_channels(QStringList strOpenChannels)
{
    pMainTextEdit->set_open_channels(strOpenChannels);
}

void TabWidget::clear_content()
{
    pMainTextEdit->clear();
}

void TabWidget::refresh_colors()
{
    // get values
    QSettings settings;
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());

    // this
    if (strBackgroundColor.toLower() != "#ffffff")
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);

    // mainTextEdit
    pMainTextEdit->setTextBackgroundColor(QColor(strBackgroundColor));

    // topic
    topic->setTextColor(QColor(strDefaultFontColor));
    topic->setTextBackgroundColor(QColor(strBackgroundColor));
}

void TabWidget::refresh_background_image()
{
    pMainTextEdit->update_background_image();
}
