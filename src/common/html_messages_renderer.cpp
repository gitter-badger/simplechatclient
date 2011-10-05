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

    // fix for context menu
    QStringList strDataList = strData.split(" ");
    for (int i = 0; i < strDataList.size(); i++)
    {
        QString strWord = strDataList[i];

        if ((i == 0) && (strWord.startsWith("&lt;")) && (strWord.endsWith("&gt;")))
            strDataList[i] = "<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">"+strDataList[i]+"</a>";
        if ((i == 1) && (strDataList[i-1] == "*") && ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage)  || (eMessageCategory == KickMessage)))
            strDataList[i] = "<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">"+strDataList[i]+"</a>";
        if (strWord[0] == '#')
            strDataList[i] = "<a href=\"#\" onclick=\"return false\" name=\"channel\" class=\"ChannelFontColor\" style=\"text-decoration:none;\">"+strDataList[i]+"</a>";
        if ((strWord.contains(QRegExp("(ftp:|http:|https:)//"))) || (strWord.contains("www.")))
        {
            int pos = strWord.indexOf(QRegExp("(ftp:|http:|https:|www.)"));
            QString strBeforeLink = strWord.left(pos);
            strDataList[i].remove(0, pos);
            strDataList[i] = strBeforeLink+"<a href=\"#\" onclick=\"return false\" name=\"website\" style=\"color:inherit;text-decoration:none;\">"+strDataList[i]+"</a>";
        }
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
    QString strFontClass;

    if (eMessageCategory == DefaultMessage)
        strFontClass = "DefaultFontColor";
    else if (eMessageCategory == JoinMessage)
        strFontClass = "JoinFontColor";
    else if (eMessageCategory == PartMessage)
        strFontClass = "PartFontColor";
    else if (eMessageCategory == QuitMessage)
        strFontClass = "QuitFontColor";
    else if (eMessageCategory == KickMessage)
        strFontClass = "KickFontColor";
    else if (eMessageCategory == ModeMessage)
        strFontClass = "ModeFontColor";
    else if (eMessageCategory == NoticeMessage)
        strFontClass = "NoticeFontColor";
    else if (eMessageCategory == InfoMessage)
        strFontClass = "InfoFontColor";
    else if (eMessageCategory == MeMessage)
        strFontClass = "MeFontColor";
    else if (eMessageCategory == ErrorMessage)
        strFontClass = "ErrorFontColor";
    else if (eMessageCategory == HilightMessage)
        strFontClass = "DefaultFontColor";
    else
        strFontClass = "DefaultFontColor";

    // convert emoticons, font
    Convert *convertText = new Convert(true);
    convertText->convertText(strData);
    delete convertText;

    // hilight
    QString strTextDecoration;
    if (eMessageCategory == HilightMessage)
        strTextDecoration = "style=\"text-decoration:underline;\"";

    return QString("<span class=\"DefaultFontColor\" %1>%2<span class=\"%3\">%4</span></span>").arg(strTextDecoration).arg(strDT).arg(strFontClass).arg(strData);
}
