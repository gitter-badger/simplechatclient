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

#include "tab_widget.h"

TabWidget::TabWidget(QWidget *parent, Network *param1, QString param2, Notify *param3, QMap <QString, QByteArray> *param4, QTcpSocket *param5, sChannelNickStatus *param6, DlgUserProfile *param7)
{
    myparent = parent;
    pNetwork = param1;
    strName = param2;
    pNotify = param3;
    mChannelAvatar = param4;
    camSocket = param5;
    mChannelNickStatus = param6;
    pDlg_user_profile = param7;

    QSettings settings;
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());
    strFontSize = settings.value("font_size").toString();

    iNickCount = 0;
    bScroll = true;
    bCursorPositionChanged = false;
    strContentStart = "<html><body style=\"background-color:"+strBackgroundColor+";\">";
    strContentEnd = "</body></html>";

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainWidget = new QWidget(this);

    topic = new QWebView(this);
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

    avatar = new QLabel(this);
    avatar->setMaximumSize(QSize(50,50));
    avatar->setMargin(0);
    avatar->show();

    topRightWidget = new QWidget(this);
    //topRightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    topRightLayout = new QVBoxLayout();
    topRightLayout->setMargin(0);
    topRightLayout->setAlignment(Qt::AlignTop);
    topRightLayout->addWidget(topic);
    topRightLayout->addWidget(topicDetails);
    topRightWidget->setLayout(topRightLayout);

    topLeftWidget = new QWidget(this);
    topLeftLayout = new QVBoxLayout();
    topLeftLayout->setMargin(0);
    topLeftLayout->setAlignment(Qt::AlignTop);
    topLeftLayout->addWidget(avatar);
    topLeftWidget->setLayout(topLeftLayout);

    topWidget = new QWidget(this);
    topLayout = new QHBoxLayout();
    topLayout->setMargin(0);
    topLayout->setAlignment(Qt::AlignLeft);
    topLayout->addWidget(topLeftWidget);
    topLayout->addWidget(topRightWidget);
    topWidget->setLayout(topLayout);

    mainWebView = new MainWebView(myparent, pNetwork, strName, camSocket, mChannelNickStatus, pDlg_user_profile);
    mainWebView->setParent(this);
    mainWebView->show();

    if (strName[0] == '#')
    {
        if (settings.value("style") == "classic")
        {
            topicDetails->hide();
            topLeftWidget->hide();
        }

        mainLayout->addWidget(topWidget);
        mainLayout->addWidget(mainWebView);
        mainWidget->setLayout(mainLayout);
    }
    else if (strName[0] == '^')
    {
        avatar->hide();
        topic->hide();
        topicDetails->hide();
        topRightWidget->hide();
        topLeftWidget->hide();
        topWidget->hide();

        mainLayout->addWidget(mainWebView);
        mainWidget->setLayout(mainLayout);
    }
    else
    {
        avatar->hide();
        topic->hide();
        topicDetails->hide();
        topRightWidget->hide();
        topLeftWidget->hide();
        topWidget->hide();

        mainLayout->addWidget(mainWebView);
        mainWidget->setLayout(mainLayout);
    }

    // hide avatar
    topLeftWidget->hide();
    // hide topic details
    topicDetails->hide();

    // show layout
    this->setLayout(mainLayout);

    // set colors
    if (strBackgroundColor.toLower() != "#ffffff")
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);

    // signals
    QObject::connect(mainWebView, SIGNAL(loadFinished(bool)), this, SLOT(change_scroll_position()));
}

TabWidget::~TabWidget()
{
    strContent.clear();
    mainWebView->setHtml(strContent,QUrl(""));
}

QString TabWidget::addslashes(QString strData)
{
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");
    strData.replace("\"", "");
    strData.replace("\'", "");
    strData.replace(";", "");
    strData.replace("%", "");

    if (strData.isEmpty() == true)
        return "#000000";
    else
        return strData;
}

void TabWidget::display_msg(QString strTime, QString strData, int iLevel)
{
    QDateTime dt = QDateTime::fromTime_t(strTime.toInt());
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

    // fix max size
    if (strContent.count("</p>") > 150)
    {
        QStringList list = strContent.split("</p>");
        int iCount = strContent.count("</p>");
        strContent.clear();
        for (int i = iCount-150; i < iCount; i++)
            strContent.append(list.at(i)+"</p>");
        list.clear();
    }

    // fix data
    strData += " ";
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");
    // channels
    strData.replace(QRegExp("#([~-_a-zA-Z0-9\xa1\xaf\xa6\xac\xca\xc6\xd1\xd3\xa3\xb1\xbf\xb6\xbc\xea\xe6\xf1\xf3\xb3]+)"), "<a id=\"level_chan\" style=\"color:"+addslashes(settings.value("channel_font_color").toString())+";text-decoration:none;\" href=\"chan#\\1\">#\\1</a>");
    // nicks
    strData.replace(QRegExp("&lt;([~-_a-zA-Z0-9\xa1\xaf\xa6\xac\xca\xc6\xd1\xd3\xa3\xb1\xbf\xb6\xbc\xea\xe6\xf1\xf3\xb3]+)&gt;"), "<a id=\"level_0\" style=\"color:"+addslashes(settings.value("default_font_color").toString())+";text-decoration:none;\" href=\"nick\\1\">&lt;\\1&gt;</a>");

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

    strData.insert(11, "<span id=\"level_"+QString::number(iLevel)+"\" style=\"color:"+strFontColor+";\">");
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

        if (settings.value("disable_sounds").toString() == "off")
            pNotify->play("beep");
    }

    // /me
    QString strTextAlign = "left";
    if (settings.value("hide_formating").toString() == "off")
    {
        if (strData.indexOf(QString(QByteArray("\x01"))) != -1)
        {
            strTextAlign = "center";
            strData.replace(QString(QByteArray("\x01")), "");
        }
    }

    // init text
    strContent.append("<p id=\"level_0\" style=\"color:"+addslashes(settings.value("default_font_color").toString())+";margin:0;padding:0;font-style:normal;text-align:"+strTextAlign+";font-family:Verdana;font-weight:normal;font-size:"+strFontSize+";text-decoration:"+strTextDecoration+";\">");

    // text
    strContent.append(strData);
    strContent = strContent+strContentLast;
    strContent.append("</p>");

    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
}

// window options

void TabWidget::set_topic(QString strTopic)
{
    QString strData = strTopic;

    // replace
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");

    // colors
    QSettings settings;
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());

    // content last
    QString strContentLast;
    QString strContentStart = "<html><body style=\"margin:0;padding:0;font-style:normal;color:"+strDefaultFontColor+";text-align:left;font-family:Verdana;font-weight:normal;font-size:12px;background-color:"+strBackgroundColor+";\">";
    QString strContentEnd = "</body></html>";

    // convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convert_text(&strData, &strContentLast);
    delete convertText;

    // init text
    strTopicContent = strData;
    strTopicContent = strTopicContent+strContentLast;

    // set topic
    topic->setHtml(strContentStart+strTopicContent+strContentEnd,QUrl(""));

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

void TabWidget::replace_color(QString level, QString color)
{
    strContent.replace(QRegExp(QString("id=\"level_%1\" style=\"color:#(......);").arg(level)), QString("id=\"level_%1\" style=\"color:%2;").arg(level).arg(color));
}

void TabWidget::update_channel_avatar()
{
    if (mChannelAvatar->contains(strName) == true)
    {
        // show widget
        topLeftWidget->show();

        // display avatar
        QPixmap pixmap;
        pixmap.loadFromData(mChannelAvatar->value(strName));
        avatar->setPixmap(pixmap);
    }
}

void TabWidget::set_open_channels(QStringList strOpenChannels)
{
    mainWebView->set_open_channels(strOpenChannels);
}

void TabWidget::change_font_size(QString strSize)
{
    strContent = strContent.replace("font-size:"+strFontSize, "font-size:"+strSize);
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
    strFontSize = strSize;
}

void TabWidget::clear_content()
{
    strContent.clear();
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));
}

void TabWidget::set_scroll(bool bSetScroll)
{
    bScroll = bSetScroll;
}

void TabWidget::refresh_colors()
{
    // get values
    QSettings settings;
    QString strBackgroundColor = addslashes(settings.value("background_color").toString());
    QString strDefaultFontColor = addslashes(settings.value("default_font_color").toString());
    QString strJoinFontColor = addslashes(settings.value("font_color_level_1").toString());
    QString strPartFontColor = addslashes(settings.value("font_color_level_2").toString());
    QString strQuitFontColor = addslashes(settings.value("font_color_level_3").toString());
    QString strKickFontColor = addslashes(settings.value("font_color_level_4").toString());
    QString strModeFontColor = addslashes(settings.value("font_color_level_5").toString());
    QString strNoticeFontColor = addslashes(settings.value("font_color_level_6").toString());
    QString strInfoFontColor = addslashes(settings.value("font_color_level_7").toString());
    QString strErrorFontColor = addslashes(settings.value("font_color_level_9").toString());
    QString strChannelFontColor = addslashes(settings.value("channel_font_color").toString());

    // refresh colors
    replace_color("0", strDefaultFontColor);
    replace_color("1", strJoinFontColor);
    replace_color("2", strPartFontColor);
    replace_color("3", strQuitFontColor);
    replace_color("4", strKickFontColor);
    replace_color("5", strModeFontColor);
    replace_color("6", strNoticeFontColor);
    replace_color("7", strInfoFontColor);
    replace_color("8", strDefaultFontColor);
    replace_color("9", strErrorFontColor);
    replace_color("chan", strChannelFontColor);

    // this
    if (strBackgroundColor.toLower() != "#ffffff")
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);

    // mainwebview
    strContentStart = "<html><body style=\"background-color:"+strBackgroundColor+";\">";
    iScrollBarValue = mainWebView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mainWebView->setHtml(strContentStart+strContent+strContentEnd,QUrl(""));

    // topic
    QString strTopicContentStart = "<html><body style=\"margin:0;padding:0;font-style:normal;color:"+strDefaultFontColor+";text-align:left;font-family:Verdana;font-weight:normal;font-size:12px;background-color:"+strBackgroundColor+";\">";
    QString strTopicContentEnd = "</body></html>";
    topic->setHtml(strTopicContentStart+strTopicContent+strTopicContentEnd,QUrl(""));
}

void TabWidget::change_scroll_position()
{
    if (bScroll == true)
        mainWebView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, mainWebView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
    else
        mainWebView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, iScrollBarValue);
}
