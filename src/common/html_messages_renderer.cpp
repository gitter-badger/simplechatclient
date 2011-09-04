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

QString HtmlMessagesRenderer::renderer(QString strData, MessageCategory eMessageCategory)
{
    // fix data
    strData.replace("&", "&amp;");
    strData.replace("<", "&lt;");
    strData.replace(">", "&gt;");

    // channels
    QString strChannelFontColor = Core::instance()->settings.value("channel_font_color");
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

    strData.insert(11, "<span style=\"color:"+strFontColor+";\">");
    strContentLast = "</span>"+strContentLast;

    // convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convertText(strData, strContentLast);
    delete convertText;

    // hilight
    QString strTextDecoration = "none";
    if (eMessageCategory == HilightMessage)
        strTextDecoration = "underline";

    // default font color
    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
    QString strFontSize = Core::instance()->settings.value("font_size");

    // init text
    QString strContent = "<span style=\"color:"+strDefaultFontColor+";font-size:"+strFontSize+";text-decoration:"+strTextDecoration+";\">";
    strContent = strContent+strData+strContentLast+"</span>";

    return strContent;
}
