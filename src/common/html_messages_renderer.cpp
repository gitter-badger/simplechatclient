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

QString HtmlMessagesRenderer::renderer(QString strDT, QString strData, MessageCategory eMessageCategory, QString strShortDT, QString strNick)
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

        if ((i == 1) && (strDataList[0] == "*") && ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage)  || (eMessageCategory == KickMessage)))
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

    // /me
    QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01"))).arg("(.*)").arg(QString(QByteArray("\x01")));
    if (strData.contains(QRegExp(strRegExpMe)))
    {
        strData.replace(QRegExp(strRegExpMe), "\\1");
        eMessageCategory = MeMessage;
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
    else if (eMessageCategory == ModerNoticeMessage)
        strFontClass = "NoticeFontColor";
    else
        strFontClass = "DefaultFontColor";

    // themes
    QString strThemes = Core::instance()->settings["themes"];

    // convert emoticons, font
    Convert *convertText = new Convert(true);
    convertText->convertText(strData);
    delete convertText;

#ifndef Q_WS_WIN
    // fix linux img src
    strData.replace("img src=\"", "img src=\"file://");
#endif

    // hilight
    QString strTextDecoration;
    if (eMessageCategory == HilightMessage)
        strTextDecoration = "style=\"text-decoration:underline;\"";

    // me & modernotice
    QString strBeforeNick;
    QString strAfterNick;
    if ((eMessageCategory == MeMessage) || (eMessageCategory == ModerNoticeMessage))
        strBeforeNick = "* ";
    else
    {
        if (strThemes == "Adara")
            strAfterNick = ":";
        else
        {
            strBeforeNick = "&lt;";
            strAfterNick = "&gt;";
        }
    }

    if (!strNick.isEmpty())
    {
        if (strThemes == "Adara")
        {
            QString strUserAvatarPath = Core::instance()->getUserAvatarPath(strNick);

            // is valid avatar
            if ((strUserAvatarPath != Core::instance()->strEmptyUserAvatarPath) && (QImage(strUserAvatarPath).isNull()))
                strUserAvatarPath = Core::instance()->strEmptyUserAvatarPath;

#ifndef Q_WS_WIN
            strUserAvatarPath = "file://"+strUserAvatarPath;
#endif
            QString strUserAvatarImg = QString("<img src=\"%1\" alt=\"avatar\" class=\"avatar\" />").arg(strUserAvatarPath);
            return QString("<table><tr><td class=\"TableText\">%1<span class=\"DefaultFontColor\">%2<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">%3</a>%4 </span><span class=\"%5\" %6>%7</span></td><td class=\"time\">%8</td></tr></table>").arg(strUserAvatarImg).arg(strBeforeNick).arg(strNick).arg(strAfterNick).arg(strFontClass).arg(strTextDecoration).arg(strData).arg(strShortDT);
        }
        else
            return QString("<span class=\"DefaultFontColor\">%1 %2<a href=\"#\" onclick=\"return false\" name=\"nick\" style=\"color:inherit;text-decoration:none;\">%3</a>%4 <span class=\"%5\" %6>%7</span></span>").arg(strDT).arg(strBeforeNick).arg(strNick).arg(strAfterNick).arg(strFontClass).arg(strTextDecoration).arg(strData);
    }
    else
    {
        if (strThemes == "Adara")
            return QString("<table><tr><td class=\"TableText\">&nbsp;<span class=\"%1\">%2</span></td><td class=\"time\">%3</td></tr></table>").arg(strFontClass).arg(strData).arg(strShortDT);
        else
            return QString("<span class=\"DefaultFontColor\">%1 <span class=\"%2\">%3</span></span>").arg(strDT).arg(strFontClass).arg(strData);
    }
}
