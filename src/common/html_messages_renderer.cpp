/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDateTime>
#include <QFileInfo>
#include "avatar.h"
#include "convert.h"
#include "nick.h"
#include "settings.h"
#include "html_messages_renderer.h"

HtmlMessagesRenderer::HtmlMessagesRenderer()
{
}

void HtmlMessagesRenderer::fixContextMenu(QString &strData, MessageCategory eMessageCategory)
{
    QStringList strDataList = strData.split(" ");
    QStringList lUrlImages;
    QStringList lYoutubeImages;

    // nick
    if ((strDataList.at(0) == "*") && ((eMessageCategory == MessageJoin) || (eMessageCategory == MessagePart) || (eMessageCategory == MessageQuit)  || (eMessageCategory == MessageKick)))
    {
        QString strWord = strDataList.at(1); Convert::removeStyles(strWord);
        strDataList[1] = QString("<a onclick=\"return false\" name=\"nick\" href=\"#\">%1</a>").arg(strWord);
    }

    for (int i = 0; i < strDataList.size(); ++i)
    {
        QString strWord = strDataList.at(i);

        // channel
        if ((!strWord.isEmpty()) && (strWord.at(0) == '#'))
        {
            Convert::removeStyles(strWord);
            if (eMessageCategory == MessageInfo)
                Convert::convertPrefix(strWord);

            strDataList[i] = QString("<a onclick=\"return false\" name=\"channel\" href=\"#\" class=\"ChannelColor\">%1</a>").arg(strWord);
        }

        // web
        if ((strWord.startsWith("http:")) || (strWord.startsWith("https:")) || (strWord.startsWith("www.")))
        {
            Convert::removeStyles(strWord);

            QRegExp exYoutube_1("youtube.com/watch\\?.*v=([a-zA-Z0-9_-]{11})");
            QRegExp exYoutube_2("youtube.com/v/([a-zA-Z0-9_-]{11})");
            QRegExp exYoutube_3("youtu.be/([a-zA-Z0-9_-]{11})");

            QList<QString> lSupportedImages;
            lSupportedImages << "jpg" << "jpeg" << "png" << "bmp";

            if ((strWord.contains(exYoutube_1)) || (strWord.contains(exYoutube_2)) || (strWord.contains(exYoutube_3)))
            {
                if (!exYoutube_1.cap(1).isEmpty()) lYoutubeImages << exYoutube_1.cap(1);
                if (!exYoutube_2.cap(1).isEmpty()) lYoutubeImages << exYoutube_2.cap(1);
                if (!exYoutube_3.cap(1).isEmpty()) lYoutubeImages << exYoutube_3.cap(1);

                QString strYoutubeLink = QString("<a onclick=\"return false\" name=\"youtube\" href=\"%1\"><img src=\"qrc:/images/oxygen/16x16/media-playback-start.png\" alt=\"%2\"></a>").arg(strWord, tr("Watch video"));
                strDataList[i] = QString("<a onclick=\"return false\" name=\"website\" href=\"%1\">%2</a> %3").arg(strWord, strWord, strYoutubeLink);
            }
            else if (lSupportedImages.contains(QFileInfo(strWord).completeSuffix().toLower()))
            {
                lUrlImages << strWord;

                strDataList[i] = QString("<a onclick=\"return false\" name=\"website\" href=\"%1\"> %2</a>").arg(strWord, strWord);
            }
            else
            {
                strDataList[i] = QString("<a onclick=\"return false\" name=\"website\" href=\"%1\">%2</a>").arg(strWord, strWord);
            }
        }
        else if (((strWord.contains("http:")) && (!strWord.startsWith("http:"))) || ((strWord.contains("https:")) && (!strWord.startsWith("https:"))) || ((strWord.contains("www.")) && (!strWord.startsWith("www."))))
        {
            if ((strWord.contains("http:")) || (strWord.contains("https:")))
            {
                strWord.replace("http:", " http:");
                strWord.replace("https:", " https:");
            }
            else
                strWord.replace("www.", " www.");

            QStringList strWords = strWord.split(" ");
            if (strWords.size() == 2)
            {
                QString strBeforeLink = strWords.at(0);
                QString strAfterLink = strWords.at(1);

                Convert::removeStyles(strAfterLink);

                QRegExp exYoutube_1("youtube.com/watch\\?.*v=([a-zA-Z0-9_-]{11})");
                QRegExp exYoutube_2("youtube.com/v/([a-zA-Z0-9_-]{11})");
                QRegExp exYoutube_3("youtu.be/([a-zA-Z0-9_-]{11})");

                QList<QString> lSupportedImages;
                lSupportedImages << "jpg" << "jpeg" << "png" << "bmp";

                if ((strAfterLink.contains(exYoutube_1)) || (strAfterLink.contains(exYoutube_2)) || (strAfterLink.contains(exYoutube_3)))
                {
                    if (!exYoutube_1.cap(1).isEmpty()) lYoutubeImages << exYoutube_1.cap(1);
                    if (!exYoutube_2.cap(1).isEmpty()) lYoutubeImages << exYoutube_2.cap(1);
                    if (!exYoutube_3.cap(1).isEmpty()) lYoutubeImages << exYoutube_3.cap(1);

                    QString strYoutubeLink = QString("<a onclick=\"return false\" name=\"youtube\" href=\"%1\"><img src=\"qrc:/images/oxygen/16x16/media-playback-start.png\" alt=\"%2\"></a>").arg(strAfterLink, tr("Watch video"));
                    strAfterLink = QString("<a onclick=\"return false\" name=\"website\" href=\"%1\">%2</a> %3").arg(strAfterLink, strAfterLink, strYoutubeLink);
                }
                else if (lSupportedImages.contains(QFileInfo(strAfterLink).completeSuffix().toLower()))
                {
                    lUrlImages << strAfterLink;

                    strAfterLink = QString("<a onclick=\"return false\" name=\"website\" href=\"%1\">%2</a>").arg(strAfterLink, strAfterLink);
                }
                else
                {
                    strAfterLink = QString("<a onclick=\"return false\" name=\"website\" href=\"%1\">%2</a>").arg(strAfterLink, strAfterLink);
                }

                strDataList[i] = strBeforeLink+strAfterLink;
            }
        }
    }

    if (((!lUrlImages.isEmpty()) || (!lYoutubeImages.isEmpty())) && (Settings::instance()->get("img_thumbs") == "true"))
    {
        strDataList << "<br/><span class=\"thumbs\">";

        foreach (const QString strImage, lUrlImages)
            strDataList << QString("<a onclick=\"return false\" name=\"website\" href=\"%1\"><img class=\"thumb\" src=\"%2\" alt=\"image\"></a>").arg(strImage, strImage);

        foreach (const QString strImage, lYoutubeImages)
        {
            QString strFullImage = QString("http://youtu.be/%1").arg(strImage);
            QString strThumbImage = QString("http://img.youtube.com/vi/%1/default.jpg").arg(strImage);
            strDataList << QString("<a onclick=\"return false\" name=\"website\" href=\"%1\"><img class=\"thumb\" src=\"%2\" alt=\"image\"></a>").arg(strFullImage, strThumbImage);
        }

        strDataList << "</span>";
    }

    strData = strDataList.join(" ");
}

QString HtmlMessagesRenderer::renderer(QString strData, MessageCategory eMessageCategory, qint64 iTime, QString strNick)
{
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(iTime);

    // fix html chars
    Convert::fixHtmlChars(strData);

    // fix for context menu
    fixContextMenu(strData, eMessageCategory);

    // /me
    QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01")), "(.*)", QString(QByteArray("\x01")));
    if (strData.contains(QRegExp(strRegExpMe)))
    {
        strData.replace(QRegExp(strRegExpMe), "\\1");
        eMessageCategory = MessageMe;
    }

    // convert emoticons, font
    Convert::convertText(strData, true, true);

    // fix utf8
    strData = Convert::fixUtf8Chars(strData);

    // font class
    QString strFontClass;
    if (eMessageCategory == MessageDefault) strFontClass = "DefaultColor";
    else if (eMessageCategory == MessageJoin) strFontClass = "MessageJoin";
    else if (eMessageCategory == MessagePart) strFontClass = "MessagePart";
    else if (eMessageCategory == MessageQuit) strFontClass = "MessageQuit";
    else if (eMessageCategory == MessageKick) strFontClass = "MessageKick";
    else if (eMessageCategory == MessageMode) strFontClass = "MessageMode";
    else if (eMessageCategory == MessageNotice) strFontClass = "MessageNotice";
    else if (eMessageCategory == MessageInfo) strFontClass = "MessageInfo";
    else if (eMessageCategory == MessageMe) strFontClass = "MessageMe";
    else if (eMessageCategory == MessageError) strFontClass = "MessageError";
    else if (eMessageCategory == MessageHighlight) strFontClass = "DefaultColor";
    else if (eMessageCategory == MessageModerNotice) strFontClass = "NoticeColor";

    // themes
    QString strThemes = Settings::instance()->get("themes");

    // highlight
    QString strExtraClass;
    if (eMessageCategory == MessageHighlight)
        strExtraClass = "underline";

    // me & modernotice
    QString strBeforeNick;
    QString strAfterNick;
    if ((eMessageCategory == MessageMe) || (eMessageCategory == MessageModerNotice))
        strBeforeNick = "* ";
    else
    {
        if ((strThemes == "Adara") || (strThemes == "Alhena"))
            strAfterNick = ":";
        else
        {
            strBeforeNick = "&lt;";
            strAfterNick = "&gt;";
        }
    }

    if (strThemes == "Adara")
    {
        if (!strNick.isEmpty())
        {
            QString strUserAvatar = Nick::instance()->getAvatar(strNick);

            // is valid avatar
            if (strUserAvatar.isEmpty())
            {
                if (strNick.at(0) == '~')
                    strUserAvatar = Avatar::instance()->getEmptyUnregisteredUserAvatarBig();
                else
                    strUserAvatar = Avatar::instance()->getEmptyRegisteredUserAvatarBig();
            }
            else
                strUserAvatar = Avatar::instance()->getAvatarPath(strUserAvatar);

#ifndef Q_WS_WIN
            strUserAvatar = "file://"+strUserAvatar;
#endif
            QString strUserAvatarImg = QString("<img src=\"%1\" alt=\"avatar\" class=\"avatar\" />").arg(strUserAvatar);
            return QString("<table><tr><td class=\"message\">%1<span class=\"DefaultColor\">%2<a href=\"#\" onclick=\"return false\" name=\"nick\">%3</a>%4&nbsp;</span><span class=\"%5 %6\">%7</span></td><td class=\"time\">%8</td></tr></table>").arg(strUserAvatarImg, strBeforeNick, strNick, strAfterNick, strFontClass, strExtraClass, strData, dt.toString(Settings::instance()->get("time_format")));
        }
        else
        {
            return QString("<table><tr><td class=\"message\">&nbsp;<span class=\"%1\">%2</span></td><td class=\"time\">%3</td></tr></table>").arg(strFontClass, strData, dt.toString(Settings::instance()->get("time_format")));
        }
    }
    else if (strThemes == "Alhena")
    {
        if (!strNick.isEmpty())
            return QString("<table><tr><td class=\"message\"><span class=\"DefaultColor\">%1<a href=\"#\" onclick=\"return false\" name=\"nick\">%2</a>%3&nbsp;</span><span class=\"%4 %5\">%6</span></td><td class=\"time\">[%7]</td></tr></table>").arg(strBeforeNick, strNick, strAfterNick, strFontClass, strExtraClass, strData, dt.toString(Settings::instance()->get("time_format")));
        else
            return QString("<table><tr><td class=\"message\">&nbsp;<span class=\"%1\">%2</span></td><td class=\"time\">[%3]</td></tr></table>").arg(strFontClass, strData, dt.toString(Settings::instance()->get("time_format")));
    }
    else
    {
        if (!strNick.isEmpty())
            return QString("<table>" \
                             "<tr>" \
                               "<td class=\"time\">[%1]</td>" \
                               "<td class=\"message DefaultColor\">" \
                                 "&nbsp;%2<a href=\"#\" onclick=\"return false\" name=\"nick\">%3</a>%4" \
                                 "&nbsp;<span class=\"%5 %6\">%7</span>" \
                               "</td>" \
                             "</tr>" \
                           "</table>")
                    .arg(dt.toString(Settings::instance()->get("time_format")), strBeforeNick, strNick, strAfterNick, strFontClass, strExtraClass, strData);
        else
            return QString("<table>" \
                             "<tr>" \
                               "<td class=\"time\">" \
                                 "[%1]" \
                               "</td>" \
                               "<td class=\"message DefaultColor\">" \
                                 "&nbsp;<span class=\"%2\">%3</span>" \
                               "</td>" \
                             "</tr>" \
                           "</table>")
                    .arg(dt.toString(Settings::instance()->get("time_format")), strFontClass, strData);
    }
}

QString HtmlMessagesRenderer::rendererDebug(QString strData, qint64 iTime)
{
    QString strColor = "#000000";
    if (strData.startsWith("<- "))
    {
        strColor = "#ee0000";
        strData.remove(0,3);
    }
    else if (strData.startsWith("-> "))
    {
        strColor = "#0000ee";
        strData.remove(0,3);
    }

    // fix html chars
    Convert::fixHtmlChars(strData);

    // time
    QString strTime = QDateTime::fromMSecsSinceEpoch(iTime).toString("[hh:mm:ss]");

    // display
    return QString("%1 <span style=\"color:%2;\">%3</span>").arg(strTime, strColor, strData);
}

QString HtmlMessagesRenderer::headCSS()
{
    QString strThemes = Settings::instance()->get("themes");

    QString strDefaultColor = Settings::instance()->get("default_color");
    QString strChannelColor = Settings::instance()->get("channel_color");
    QString strTimeColor = Settings::instance()->get("time_color");
    QString strFontSize = Settings::instance()->get("font_size");

    QString strMessageJoin = Settings::instance()->get("message_join_color");
    QString strMessagePart = Settings::instance()->get("message_part_color");
    QString strMessageQuit = Settings::instance()->get("message_quit_color");
    QString strMessageKick = Settings::instance()->get("message_kick_color");
    QString strMessageMode = Settings::instance()->get("message_mode_color");
    QString strMessageNotice = Settings::instance()->get("message_notice_color");
    QString strMessageInfo = Settings::instance()->get("message_info_color");
    QString strMessageMe = Settings::instance()->get("message_me_color");
    QString strMessageError = Settings::instance()->get("message_error_color");

    QString strHeadCSS = "div {margin-bottom:2px; vertical-align:bottom;}";
    strHeadCSS.append(QString("table {border-spacing:0; margin:0; padding:0; font-family:sans; font-size:%1;}").arg(strFontSize));
    strHeadCSS.append("table td {vertical-align:bottom;}");
    strHeadCSS.append(".message {width:100%; word-break: break-word;}");
    strHeadCSS.append("a {color:inherit; text-decoration:none;}");
    strHeadCSS.append(".thumb {max-width:75px; max-height:75px;}");
    strHeadCSS.append(".thumbs {margin-left:100px;}");
    strHeadCSS.append(".underline {text-decoration:underline;}");
    strHeadCSS.append(".avatar {margin-left:4px; margin-right:4px; width:30px; height:30px;}");

    if (strThemes == "Origin") {
        strHeadCSS.append(QString(".time{font-size:0.95em; color:%1;}").arg(strTimeColor));
    } else {
        strHeadCSS.append(QString(".time{font-weight:normal; text-decoration:none; color:%1; padding-right:5px;}").arg(strTimeColor));
    }

    strHeadCSS.append(QString(".DefaultColor{color:%1;}").arg(strDefaultColor));
    strHeadCSS.append(QString(".ChannelColor{color:%1;}").arg(strChannelColor));

    strHeadCSS.append(QString(".MessageDefault{color:%1;}").arg(strDefaultColor));
    strHeadCSS.append(QString(".MessageJoin{color:%1;}").arg(strMessageJoin));
    strHeadCSS.append(QString(".MessagePart{color:%1;}").arg(strMessagePart));
    strHeadCSS.append(QString(".MessageQuit{color:%1;}").arg(strMessageQuit));
    strHeadCSS.append(QString(".MessageKick{color:%1;}").arg(strMessageKick));
    strHeadCSS.append(QString(".MessageMode{color:%1;}").arg(strMessageMode));
    strHeadCSS.append(QString(".MessageNotice{color:%1;}").arg(strMessageNotice));
    strHeadCSS.append(QString(".MessageInfo{color:%1;}").arg(strMessageInfo));
    strHeadCSS.append(QString(".MessageMe{color:%1;}").arg(strMessageMe));
    strHeadCSS.append(QString(".MessageError{color:%1;}").arg(strMessageError));
    strHeadCSS.append(QString(".MessageHighlight{color:%1;}").arg(strDefaultColor));
    strHeadCSS.append(QString(".MessageModerNotice{color:%1;}").arg(strDefaultColor));

    return strHeadCSS;
}

QString HtmlMessagesRenderer::bodyCSS()
{
    QString strFontSize = Settings::instance()->get("font_size");
    QString strBackgroundImage = Settings::instance()->get("background_image");
    QString strShowBackgroundImage = Settings::instance()->get("show_background_image");

#ifndef Q_WS_WIN
    strBackgroundImage = "file://"+strBackgroundImage;
#endif

    QString strBackground;
    if ((strShowBackgroundImage == "true") && (!strBackgroundImage.isEmpty()))
        strBackground = QString("background-image: url('%1'); background-attachment: fixed; background-position: center; background-repeat: no-repeat;").arg(strBackgroundImage);

    return QString("margin: 0; padding: 0; font-family: sans; font-size:%1; %2").arg(strFontSize, strBackground);
}
