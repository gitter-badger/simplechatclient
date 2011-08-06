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
#include <QLabel>
#include <QVBoxLayout>
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

    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
    addslashes(strDefaultFontColor);
    QString strBackgroundColor = Core::instance()->settings.value("background_color");
    addslashes(strBackgroundColor);
    strFontSize = Core::instance()->settings.value("font_size");

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
    delete pMainTextEdit;
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

    if (Core::instance()->settings.value("disable_logs") == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

    if ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage))
    {
        if (Core::instance()->settings.value("hide_join_part") == "on")
            return;

        int iNickCount = Core::instance()->mChannelNicks[strName];
        if ((Core::instance()->settings.value("hide_join_part_200") == "on") && (iNickCount > 200))
            return;
    }

    displayMessage(strData, eMessageCategory);
}

void TabWidget::displayMsg(QString &strData, MessageCategory eMessageCategory)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("[hh:mm:ss] ");
    strData = strDT+strData;

    if (Core::instance()->settings.value("disable_logs") == "off")
    {
        Log *l = new Log();
        l->save(strName, strData);
        delete l;
    }

    if ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage))
    {
        if (Core::instance()->settings.value("hide_join_part") == "on")
            return;

        int iNickCount = Core::instance()->mChannelNicks[strName];
        if ((Core::instance()->settings.value("hide_join_part_200") == "on") && (iNickCount > 200))
            return;
    }

    displayMessage(strData, eMessageCategory);
}

void TabWidget::displayMessage(QString &strData, MessageCategory eMessageCategory)
{
    // awaylog
    if (eMessageCategory == HilightMessage)
    {
        if (Core::instance()->settings.value("away") == "on")
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
    QString strChannelFontColor = Core::instance()->settings.value("channel_font_color");
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
        strFontColor = Core::instance()->settings.value("default_font_color"); // default black
    else if (eMessageCategory == JoinMessage) // join
        strFontColor = Core::instance()->settings.value("font_color_level_1"); // default green
    else if (eMessageCategory == PartMessage) // part
        strFontColor = Core::instance()->settings.value("font_color_level_2"); // default light blue
    else if (eMessageCategory == QuitMessage) // quit
        strFontColor = Core::instance()->settings.value("font_color_level_3"); // default dark blue
    else if (eMessageCategory == KickMessage) // kick
        strFontColor = Core::instance()->settings.value("font_color_level_4"); // default dark blue
    else if (eMessageCategory == ModeMessage) // mode
        strFontColor = Core::instance()->settings.value("font_color_level_5"); // default green
    else if (eMessageCategory == NoticeMessage) // notice
        strFontColor = Core::instance()->settings.value("font_color_level_6"); // default blue
    else if (eMessageCategory == InfoMessage) // info
        strFontColor = Core::instance()->settings.value("font_color_level_7"); // default gray
    else if (eMessageCategory == MeMessage) // me
        strFontColor = Core::instance()->settings.value("font_color_level_8"); // default violet
    else if (eMessageCategory == ErrorMessage) // error
        strFontColor = Core::instance()->settings.value("font_color_level_9"); // default red
    else if (eMessageCategory == HilightMessage) // hilight no color
        strFontColor = Core::instance()->settings.value("default_font_color"); // default black
    else
    {
        eMessageCategory = DefaultMessage;
        strFontColor = Core::instance()->settings.value("default_font_color"); // default black
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
        if (Core::instance()->settings.value("disable_sounds") == "off")
            Notify::instance()->play(Beep);
    }

    // default font color
    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
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
    QString strBackgroundColor = Core::instance()->settings.value("background_color");
    addslashes(strBackgroundColor);
    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
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
