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

#include <QCoreApplication>
#include <QFile>
#include "core.h"
#include "convert.h"

Convert::Convert(bool _bInsertWidthHeight) : bInsertWidthHeight(_bInsertWidthHeight), bRemovedBold(false), bRemovedItalic(false), iRemovedColor(-1)
{
}

void Convert::convertColor(QString &strData)
{
    QList<QString> lColors;
    lColors << "000000" << "623c00" << "c86c00" << "ff6500" << "ff0000" << "e40f0f" << "990033" << "8800ab" << "ce00ff" << "0f2ab1" << "3030ce" << "006699" << "1a866e" << "008100" << "959595";

    if (Core::instance()->settings.value("hide_formating") == "off")
    {
        foreach (QString strColor, lColors)
        {
            if (strData.contains(QString("%C%1%").arg(strColor)))
            {
                strData.replace(QString("%C%1%").arg(strColor), QString("<span style=\"color:#%1;\">").arg(strColor));
                strData += "</span>";
            }
        }
    }
    else
    {
        foreach (QString strColor, lColors)
            strData.remove(QString("%C%1%").arg(strColor));
    }
}

void Convert::convertFont(QString &strData)
{
    QStringList lData = strData.split(" ");
    QString strSpans;

    for (int i = 0; i < lData.size(); i++)
    {
        QRegExp rx("%F(b|i|bi)?:?(arial|times|verdana|tahoma|courier)?%");

        int pos = 0;
        while ((pos = rx.indexIn(lData[i], pos)) != -1)
        {
            pos += rx.matchedLength();

            QString strFont = rx.cap(0);
            strFont = strFont.mid(2,strFont.size()-3);

            if (!strFont.isEmpty())
            {
                QString strFontStyle;
                QString strFontName;
                QString strFontWeight;
                QString strShortFontWeight;

                if (strFont.contains(":"))
                {
                    strShortFontWeight = strFont.left(strFont.indexOf(":"));
                    strFontName = strFont.right(strFont.size()-strFont.indexOf(":")-1);
                }
                else
                {
                    QRegExp rx("((b|i)?)((b|i)?)");
                    if (rx.exactMatch(strFont))
                        strShortFontWeight = strFont;
                }

                if (!strShortFontWeight.isEmpty())
                {
                    for (int fw = 0; fw < strShortFontWeight.size(); fw++)
                    {
                        if (strShortFontWeight[fw] == 'b') strFontWeight = "bold";
                        else if (strShortFontWeight[fw] == 'i') strFontStyle = "italic";
                    }
                }

                if ((!strFontName.isEmpty()) || (!strFontWeight.isEmpty()) || (!strFontStyle.isEmpty()))
                {
                    QString strFontFamily;
                    if (strFontName == "arial") strFontFamily = "Arial";
                    else if (strFontName == "times") strFontFamily = "Times New Roman";
                    else if (strFontName == "verdana") strFontFamily = "Verdana";
                    else if (strFontName == "tahoma") strFontFamily = "Tahoma";
                    else if (strFontName == "courier") strFontFamily = "Courier New";
                    else strFontFamily = "Verdana";

                    if (strFontWeight.isEmpty())
                        strFontWeight = "normal";
                    if (strFontStyle.isEmpty())
                        strFontStyle = "normal";

                    lData[i].replace(QString("%F%1%").arg(strFont), QString("<span style=\"font-weight:%1;font-style:%2;font-family:%3;\">").arg(strFontWeight).arg(strFontStyle).arg(strFontFamily));
                    strSpans += "</span>";
                }
            }
        }
    }
    strData = lData.join(" ")+strSpans;
}

void Convert::convertEmoticons(QString &strData)
{
    QString strPath;
#ifdef Q_WS_WIN
    strPath = QCoreApplication::applicationDirPath();
#else
    strPath = "/usr/share/scc";
#endif

    QStringList lData = strData.split(" ");

    for (int i = 0; i < lData.size(); i++)
    {
        QRegExp rx("%I([a-zA-Z0-9_-]+)%");

        int pos = 0;
        while ((pos = rx.indexIn(lData[i], pos)) != -1)
        {
            pos += rx.matchedLength();

            QString strEmoticon = rx.cap(1);

            QString strEmoticonPath;
            if (QFile::exists(strPath+"/3rdparty/emoticons/"+strEmoticon+".gif"))
                strEmoticonPath = strPath+"/3rdparty/emoticons/"+strEmoticon+".gif";
            else if (QFile::exists(strPath+"/3rdparty/emoticons_other/"+strEmoticon+".gif"))
                strEmoticonPath = strPath+"/3rdparty/emoticons_other/"+strEmoticon+".gif";

            if (!strEmoticonPath.isEmpty())
            {
                QString strWidthHeight;
                if (bInsertWidthHeight)
                {
                    QPixmap p(strEmoticonPath);
                    strWidthHeight = "width=\""+QString::number(p.width())+"px\" height=\""+QString::number(p.height())+"px\"";
                }

                lData[i].replace(QString("%I%1%").arg(strEmoticon), QString("<img src=\"%1\" alt=\"%2\" %3 />").arg(strEmoticonPath).arg(strEmoticon).arg(strWidthHeight));
            }
            else
                lData[i].replace(QString("%I%1%").arg(strEmoticon), QString("//%1").arg(strEmoticon));
        }
    }
    strData = lData.join(" ");
}

void Convert::convertText(QString &strData)
{
    convertFont(strData);
    convertColor(strData);
    convertEmoticons(strData);
}

void Convert::removeFont(QString &strData)
{
    QStringList lData = strData.split(" ");

    for (int i = 0; i < lData.size(); i++)
    {
        QRegExp rx("%F(b|i|bi)?:?(arial|times|verdana|tahoma|courier)?%");

        int pos = 0;
        while ((pos = rx.indexIn(lData[i], pos)) != -1)
        {
            pos += rx.matchedLength();

            QString strFont = rx.cap(0);
            strFont = strFont.mid(2,strFont.size()-3);

            if (!strFont.isEmpty())
            {
                QString strFontName;
                QString strShortFontWeight;

                if (strFont.contains(":"))
                {
                    strShortFontWeight = strFont.left(strFont.indexOf(":"));
                    strFontName = strFont.right(strFont.size()-strFont.indexOf(":")-1);
                }
                else
                {
                    QRegExp rx("((b|i)?)((b|i)?)");
                    if (rx.exactMatch(strFont))
                        strShortFontWeight = strFont;
                }

                if (!strShortFontWeight.isEmpty())
                {
                    for (int fw = 0; fw < strShortFontWeight.size(); fw++)
                    {
                        if (strShortFontWeight[fw] == 'b') bRemovedBold = true;
                        else if (strShortFontWeight[fw] == 'i') bRemovedItalic = true;
                    }
                }

                if (!strFontName.isEmpty())
                {
                    if (strFontName == "arial") strRemovedFont = "arial";
                    else if (strFontName == "times") strRemovedFont = "times";
                    else if (strFontName == "verdana") strRemovedFont = "verdana";
                    else if (strFontName == "tahoma") strRemovedFont = "tahoma";
                    else if (strFontName == "courier") strRemovedFont = "courier";
                    else strRemovedFont = "verdana";
                }

                lData[i].remove(QString("%F%1%").arg(strFont));
            }
        }
    }
    strData = lData.join(" ");
}

void Convert::removeColor(QString &strData)
{
    QList<QString> lColors;
    lColors << "000000" << "623c00" << "c86c00" << "ff6500" << "ff0000" << "e40f0f" << "990033" << "8800ab" << "ce00ff" << "0f2ab1" << "3030ce" << "006699" << "1a866e" << "008100" << "959595";

    int iFontColor = 0;
    foreach (QString strColor, lColors)
    {
        if (strData.contains(QString("%C%1%").arg(strColor)))
        {
            strData.remove(QString("%C%1%").arg(strColor));
            iRemovedColor = iFontColor;
        }
        iFontColor++;
    }
}
