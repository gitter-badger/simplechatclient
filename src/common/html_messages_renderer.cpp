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

#include "convert.h"
#include "core.h"
#include "html_messages_renderer.h"

HtmlMessagesRenderer::HtmlMessagesRenderer(QObject *parent) : QObject(parent)
{
}

QString HtmlMessagesRenderer::renderer(QString strDT, QString strData, MessageCategory eMessageCategory)
{
    // fix data
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");
    strData.replace("\"", "&quot;");
    strData.replace("'", "&#039;");
    strData.replace("\\", "&#92;");

    // default font color
    QString strFontSize = Core::instance()->settings.value("font_size");
    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
    QString strChannelFontColor = Core::instance()->settings.value("channel_font_color");

    // fix for context menu
    QStringList strDataList = strData.split(" ");
    for (int i = 0; i < strDataList.size(); i++)
    {
        QString strWord = strDataList[i];

        if ((i == 0) && (strWord.contains("&lt;")) && (strWord.contains("&gt;")))
            strDataList[i] = "<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">"+strDataList[i]+"</a>";
        if ((i == 1) && (strDataList[i-1] == "*") && ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage)  || (eMessageCategory == KickMessage)))
            strDataList[i] = "<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">"+strDataList[i]+"</a>";
        if (strWord[0] == '#')
            strDataList[i] = "<a href=\"#\" onclick=\"return false\" name=\"channel\" style=\"color:"+strChannelFontColor+";text-decoration:none;\">"+strDataList[i]+"</a>";
        if ((strWord.contains("http")) || (strWord.contains("www")))
            strDataList[i] = "<a href=\"#\" onclick=\"return false\" name=\"website\" style=\"color:inherit;text-decoration:none;\">"+strDataList[i]+"</a>";
    }
    strData = strDataList.join(" ");

    // if /me
    if (strData.contains((QString(QByteArray("\x01")))))
    {
        if (strData.contains("ACTION "))
        {
            strData.remove(QString(QByteArray("\x01")));
            strData.remove("ACTION ");

            strData.insert(0, "* ");
            if (strData.contains("&lt;")) strData = strData.remove(strData.indexOf("&lt;"),4);
            if (strData.contains("&gt;")) strData = strData.remove(strData.indexOf("&gt;"),4);

            eMessageCategory = MeMessage;
        }
    }

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
        strFontColor = Core::instance()->settings.value("default_font_color"); // default black

    strData = QString("<span style=\"color:%1;\">%2</span>").arg(strFontColor).arg(strData);

    // convert emoticons, font
    Convert *convertText = new Convert(true);
    convertText->convertText(strData);
    delete convertText;

    // hilight
    QString strTextDecoration;
    if (eMessageCategory == HilightMessage)
        strTextDecoration = "text-decoration:underline;";

    return QString("<span style=\"color:%1;font-size:%2;%3\">%4%5</span>").arg(strDefaultFontColor).arg(strFontSize).arg(strTextDecoration).arg(strDT).arg(strData);
}
