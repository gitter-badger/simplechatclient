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

    topic = new QLabel(this);
    topic->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    topic->setWordWrap(true);
    topic->setMinimumHeight(25);
    topic->show();

    pMainTextEdit = new MainTextEdit(pNetwork, strName, pDlgUserProfile);
    pMainTextEdit->document()->setMaximumBlockCount(1000);
    pMainTextEdit->setReadOnly(true);
    pMainTextEdit->setAcceptRichText(false);
    pMainTextEdit->setParent(this);
    pMainTextEdit->show();

    if (strName[0] == '#')
    {
        mainLayout->addWidget(topic);
        mainLayout->addWidget(pMainTextEdit);
        mainWidget->setLayout(mainLayout);
    }
    else if (strName[0] == '^')
    {
        topic->hide();

        mainLayout->addWidget(pMainTextEdit);
        mainWidget->setLayout(mainLayout);
    }
    else
    {
        topic->hide();

        mainLayout->addWidget(pMainTextEdit);
        mainWidget->setLayout(mainLayout);
    }

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

void TabWidget::displayMsg(QString &strTime, QString &strData, MessageCategory eMessageCategory)
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

    displayMessage(strData, eMessageCategory);
}

void TabWidget::displayMsg(QString &strData, MessageCategory eMessageCategory)
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

    displayMessage(strData, eMessageCategory);
}

void TabWidget::displayMessage(QString &strData, MessageCategory eMessageCategory)
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
                    if (strAwayData.contains("<")) strAwayData = strAwayData.remove(strAwayData.indexOf("<"),1);
                    if (strAwayData.contains(">")) strAwayData = strAwayData.remove(strAwayData.indexOf(">"),1);
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
    convertText->convertText(strData, strContentLast);
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

void TabWidget::setTopic(QString &strTopic)
{
    // replace
    strTopic.replace("&", "&amp;");
    strTopic.replace("<", "&lt;");
    strTopic.replace(">", "&gt;");

    QString strContent = strTopic;
    QString strLastContent;

    // convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convertText(strContent,strLastContent);
    delete convertText;

    // set topic
    topic->setText("<b>"+tr("Topic:")+"</b> "+strContent+strLastContent);

    // tooltip
    strTopic.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
    strTopic.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
    strTopic.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");
    topic->setToolTip(strTopic);
}

void TabWidget::authorTopic(QString &strAuthor)
{
    QString strTopicDetails = QString(tr("Topic set by %1")).arg(strAuthor);
    topic->setToolTip(strTopicDetails);
}

void TabWidget::clearContent()
{
    pMainTextEdit->clear();
}

void TabWidget::refreshColors()
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
}

void TabWidget::refreshBackgroundImage()
{
    pMainTextEdit->updateBackgroundImage();
}
