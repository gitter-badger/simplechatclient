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

TabWidget::TabWidget(Network *param1, QString param2, DlgUserProfile *param3)
{
    pNetwork = param1;
    strName = param2;
    pDlgUserProfile = param3;

    QSettings settings;
    QString strDefaultFontColor = settings.value("default_font_color").toString();
    addslashes(strDefaultFontColor);
    QString strBackgroundColor = settings.value("background_color").toString();
    addslashes(strBackgroundColor);
    strFontSize = settings.value("font_size").toString();

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

    pMainTextEdit = new MainTextEdit(pNetwork, strName, pDlgUserProfile);
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
    if ((strBackgroundColor.toLower() != "#ffffff") && (!strBackgroundColor.isEmpty()))
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);
}

TabWidget::~TabWidget()
{
    pMainTextEdit->clear();
}

void TabWidget::addslashes(QString &strData)
{
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");
    strData.remove("\"");
    strData.remove("\'");
    strData.remove(";");
    strData.remove("%");
}

void TabWidget::display_msg(QString &strTime, QString &strData, MessageCategory eMessageCategory)
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

    if ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage))
    {
        if (settings.value("hide_join_part").toString() == "on")
            return;

        int iNickCount = Core::instance()->mChannelNicks[strName];
        if ((settings.value("hide_join_part_200").toString() == "on") && (iNickCount > 200))
            return;
    }

    display_message(strData, eMessageCategory);
}

void TabWidget::display_msg(QString &strData, MessageCategory eMessageCategory)
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

    if ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage))
    {
        if (settings.value("hide_join_part").toString() == "on")
            return;

        int iNickCount = Core::instance()->mChannelNicks[strName];
        if ((settings.value("hide_join_part_200").toString() == "on") && (iNickCount > 200))
            return;
    }

    display_message(strData, eMessageCategory);
}

void TabWidget::display_message(QString &strData, MessageCategory eMessageCategory)
{
    QSettings settings;

    // awaylog
    if (eMessageCategory == HilightMessage)
    {
        if (settings.value("away").toString() == "on")
        {
            QString strAwayData = strData;

            // if /me
            if (strAwayData.contains((QString(QByteArray("\x01")))))
            {
                if (strAwayData.contains("ACTION "))
                {
                    strAwayData.remove(QString(QByteArray("\x01")));
                    strAwayData.remove("ACTION ");

                    strAwayData.insert(11, "* ");
                    if (strAwayData.contains("<")) strAwayData = strAwayData.remove(strAwayData.indexOf("<"),4);
                    if (strAwayData.contains(">")) strAwayData = strAwayData.remove(strAwayData.indexOf(">"),4);
                }
            }
            // remove color, font, emots
            strAwayData.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
            strAwayData.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
            strAwayData.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

            Core::instance()->lAwaylog.append(QString("%1\n%2").arg(strName).arg(strAwayData));
        }
    }

    // fix data
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");

    // channels
    QString strChannelFontColor = settings.value("channel_font_color").toString();
    addslashes(strChannelFontColor);
    strData.replace(QRegExp("#([~-_a-zA-Z0-9\xa1\xaf\xa6\xac\xca\xc6\xd1\xd3\xa3\xb1\xbf\xb6\xbc\xea\xe6\xf1\xf3\xb3]+)"), "<span style=\"color:"+strChannelFontColor+";text-decoration:none;\">#\\1</span>");

    // if /me
    if (strData.contains((QString(QByteArray("\x01")))))
    {
        if (strData.contains("ACTION "))
        {
            strData.remove(QString(QByteArray("\x01")));
            strData.remove("ACTION ");

            strData.insert(11, "* ");
            if (strData.contains("&lt;")) strData = strData.remove(strData.indexOf("&lt;"),4);
            if (strData.contains("&gt;")) strData = strData.remove(strData.indexOf("&gt;"),4);

            eMessageCategory = MeMessage;
        }
    }

    // content last
    QString strContentLast;

    // colors
    QString strFontColor;

    if (eMessageCategory == DefaultMessage)
        strFontColor = settings.value("default_font_color").toString(); // default black
    else if (eMessageCategory == JoinMessage) // join
        strFontColor = settings.value("font_color_level_1").toString(); // default green
    else if (eMessageCategory == PartMessage) // part
        strFontColor = settings.value("font_color_level_2").toString(); // default light blue
    else if (eMessageCategory == QuitMessage) // quit
        strFontColor = settings.value("font_color_level_3").toString(); // default dark blue
    else if (eMessageCategory == KickMessage) // kick
        strFontColor = settings.value("font_color_level_4").toString(); // default dark blue
    else if (eMessageCategory == ModeMessage) // mode
        strFontColor = settings.value("font_color_level_5").toString(); // default green
    else if (eMessageCategory == NoticeMessage) // notice
        strFontColor = settings.value("font_color_level_6").toString(); // default blue
    else if (eMessageCategory == InfoMessage) // info
        strFontColor = settings.value("font_color_level_7").toString(); // default gray
    else if (eMessageCategory == MeMessage) // me
        strFontColor = settings.value("font_color_level_8").toString(); // default violet
    else if (eMessageCategory == ErrorMessage) // error
        strFontColor = settings.value("font_color_level_9").toString(); // default red
    else if (eMessageCategory == HilightMessage) // hilight no color
        strFontColor = settings.value("default_font_color").toString(); // default black
    else
    {
        eMessageCategory = DefaultMessage;
        strFontColor = settings.value("default_font_color").toString(); // default black
    }

    addslashes(strFontColor);
    strData.insert(11, "<span style=\"color:"+strFontColor+";\">");
    strContentLast = "</span>"+strContentLast;

    // convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convert_text(strData, strContentLast);
    delete convertText;

    // hilight
    QString strTextDecoration = "none";
    if (eMessageCategory == HilightMessage)
    {
        strTextDecoration = "underline";

        // sound
        if (settings.value("disable_sounds").toString() == "off")
            Notify::instance()->play(Beep);
    }

    // default font color
    QString strDefaultFontColor = settings.value("default_font_color").toString();
    addslashes(strDefaultFontColor);

    // init text
    QString strContent = "<span style=\"color:"+strDefaultFontColor+";font-size:"+strFontSize+";text-decoration:"+strTextDecoration+";\">";
    strContent = strContent+strData+strContentLast+"</span>";

    // append
    pMainTextEdit->append(strContent);
}

// window options

void TabWidget::set_topic(QString &strTopic)
{
    // colors
    QSettings settings;
    QString strDefaultFontColor = settings.value("default_font_color").toString();
    addslashes(strDefaultFontColor);
    QString strBackgroundColor = settings.value("background_color").toString();
    addslashes(strBackgroundColor);
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
    convertText->convert_text(strContent,strLastContent);
    delete convertText;

    // set topic
    topic->setHtml(strContent+strLastContent);

    // tooltip
    strTopic.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
    strTopic.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
    strTopic.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

    if (settings.value("style") == "modern")
        topic->setToolTip(strTopic);
    else if (settings.value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::author_topic(QString &strAuthor)
{
    topicDetails->show();
    topicDetails->setText(QString(tr("Topic set by %1")).arg(strAuthor));

    QSettings settings;
    if (settings.value("style") == "classic")
        topic->setToolTip(topicDetails->text());
}

void TabWidget::set_link(QString &strUrl)
{
    websiteLink->show();

    websiteLink->setText(QString("<a href=\"%1\" style=\"color:#0000FF;text-decoration:none;\" >&nbsp;&nbsp;"+tr("Channel website")+"&nbsp;&nbsp;</a>").arg(strUrl));
    websiteLink->setToolTip(strUrl);
}

void TabWidget::update_channel_avatar()
{
    if (Core::instance()->mChannelAvatar.contains(strName))
    {
        // show widget
        topLeftWidget->show();

        // display avatar
        QPixmap pixmap;
        pixmap.loadFromData(Core::instance()->mChannelAvatar.value(strName));
        avatar->setPixmap(pixmap);
    }
}

void TabWidget::clear_content()
{
    pMainTextEdit->clear();
}

void TabWidget::refresh_colors()
{
    // get values
    QSettings settings;
    QString strBackgroundColor = settings.value("background_color").toString();
    addslashes(strBackgroundColor);
    QString strDefaultFontColor = settings.value("default_font_color").toString();
    addslashes(strDefaultFontColor);

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
