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
#include <QSettings>
#include <QStringListIterator>
#include "convert.h"

Convert::Convert()
{
}

void Convert::convert_text(QString *strData, QString *strLastContent)
{
    (*strData) += " "; // fix convert algorithm

    // fonts
    if (strData->indexOf("%F") != -1)
    {
        int iCount = strData->count("%F");

        while (strData->indexOf("%F") != -1)
        {
            int iStartPos = strData->indexOf("%F");
            int iEndPos = strData->indexOf("%", iStartPos+1);
            int iSpacePos = strData->indexOf(" ", iStartPos);

            if (iEndPos != -1)
            {
                if ((iEndPos < iSpacePos) || (iSpacePos == -1))
                {
                    iEndPos++;
                    QString strFontFull = strData->mid(iStartPos, iEndPos-iStartPos);
                    QString strFont = strFontFull.mid(2,strFontFull.length()-3);
                    strFont = strFont.toLower();

                    QString strFontStyle = "normal";
                    QString strFontWeight;
                    QString strFontName;

                    if (strFont.indexOf(":") != -1)
                    {
                        strFontWeight = strFont.left(strFont.indexOf(":"));
                        strFontName = strFont.right(strFont.length()-strFont.indexOf(":")-1);
                    }
                    else
                    {
                        QRegExp rx("((b|i)?)((b|i)?)");
                        if (rx.exactMatch(strFont) == true)
                            strFontWeight = strFont;
                    }

                    if (strFontWeight.isEmpty() == false)
                    {
                        for (int fw = 0; fw < strFontWeight.length(); fw++)
                        {
                            if (strFontWeight[fw] == 'b') strFontWeight = "bold";
                            else if (strFontWeight[fw] == 'i') strFontStyle = "italic";
                        }
                    }

                    if ((strFontName.isEmpty() == false) || (strFontWeight.isEmpty() == false))
                    {
                        QString strFontFamily;
                        if (strFontName == "arial") strFontFamily = "Arial";
                        else if (strFontName == "times") strFontFamily = "Times New Roman";
                        else if (strFontName == "verdana") strFontFamily = "Verdana";
                        else if (strFontName == "tahoma") strFontFamily = "Tahoma";
                        else if (strFontName == "courier") strFontFamily = "Courier New";
                        else strFontFamily = "Verdana";

                        if ((strFontWeight.isEmpty() == true) || (strFontWeight == "i"))
                            strFontWeight = "normal";

                        QString strInsert = "<span style=\"font-weight:"+strFontWeight+";font-style:"+strFontStyle+";font-family:"+strFontFamily+";\">";
                        strData->replace(strFontFull, strInsert);
                    }
                    else
                        strData->insert(iStartPos+1, " "); // fix wrong %F
                }
                else
                    strData->insert(iStartPos+1, " "); // fix wrong %F
            }
            else
                break;
        }

        QString strSpan;
        for (int i = 0; i < iCount; i++)
            strSpan += "</span>";

        (*strLastContent) = strSpan+(*strLastContent);
    }

    // colors
    QSettings settings;
    if (settings.value("hide_formating").toString() == "off")
    {
        int iCount = strData->count("%C");

        strData->replace("%C000000%", "<span style=\"color:#000000;\">");
        strData->replace("%C623c00%", "<span style=\"color:#623c00;\">");
        strData->replace("%Cc86c00%", "<span style=\"color:#c86c00;\">");
        strData->replace("%Cff6500%", "<span style=\"color:#ff6500;\">");
        strData->replace("%Cff0000%", "<span style=\"color:#ff0000;\">");
        strData->replace("%Ce40f0f%", "<span style=\"color:#e40f0f;\">");
        strData->replace("%C990033%", "<span style=\"color:#990033;\">");
        strData->replace("%C8800ab%", "<span style=\"color:#8800ab;\">");
        strData->replace("%Cce00ff%", "<span style=\"color:#ce00ff;\">");
        strData->replace("%C0f2ab1%", "<span style=\"color:#0f2ab1;\">");
        strData->replace("%C3030ce%", "<span style=\"color:#3030ce;\">");
        strData->replace("%C006699%", "<span style=\"color:#006699;\">");
        strData->replace("%C1a866e%", "<span style=\"color:#1a866e;\">");
        strData->replace("%C008100%", "<span style=\"color:#008100;\">");
        strData->replace("%C959595%", "<span style=\"color:#959595;\">");

        QString strSpan;
        for (int i = 0; i < iCount; i++)
            strSpan += "</span>";

        (*strLastContent) = strSpan+(*strLastContent);
    }
    else
    {
        // remove colors
        QStringList strlReplace;
        strlReplace << "%C000000%" << "%C623c00%" << "%Cc86c00%" << "%Cff6500%" << "%Cff0000%" << "%Ce40f0f%" << "%C990033%" << "%C8800ab%" << "%Cce00ff%" << "%C0f2ab1%" << "%C3030ce%" << "%C006699%" << "%C1a866e%" << "%C008100%" << "%C959595%";

        QStringListIterator strliReplace (strlReplace);
        while (strliReplace.hasNext())
            strData->replace(strliReplace.next(), "");
    }

    // emoticons
    if (strData->indexOf("%I") != -1)
    {
        QString strPath = QCoreApplication::applicationDirPath();

        while (strData->indexOf("%I") != -1)
        {
            int iStartPos = strData->indexOf("%I");
            int iEndPos = strData->indexOf("%", iStartPos+1);
            int iSpacePos = strData->indexOf(" ", iStartPos);

            if (iEndPos != -1)
            {
                if ((iEndPos < iSpacePos) || (iSpacePos == -1))
                {
                    iEndPos++;
                    QString strEmoticonFull = strData->mid(iStartPos, iEndPos-iStartPos);
                    QString strEmoticon = strEmoticonFull.mid(2,strEmoticonFull.length()-3);
                    QString strInsert;

                    QString strEmoticonFull1 = strPath+"/3rdparty/emoticons/"+strEmoticon+".gif";
                    QString strEmoticonFull2 = strPath+"/3rdparty/emoticons_other/"+strEmoticon+".gif";
                    QFile f1(strEmoticonFull1);
                    QFile f2(strEmoticonFull2);
                    if ((f1.exists() == true) && (settings.value("hide_formating").toString() == "off") && (settings.value("disable_emots").toString() == "off"))
                    {
                        strInsert = "<img src=\""+strEmoticonFull1+"\" alt=\""+strEmoticon+"\" />";
                    }
                    else if ((f2.exists() == true) && (settings.value("hide_formating").toString() == "off") && (settings.value("disable_emots").toString() == "off"))
                    {
                        strInsert = "<img src=\""+strEmoticonFull2+"\" alt=\""+strEmoticon+"\" />";
                    }
                    // emoticon not exist or hide formating or disabled emots
                    else
                        strInsert = "//"+strEmoticon;

                    strData->replace(strEmoticonFull, strInsert);
                }
                else
                    strData->insert(iStartPos+1, " "); // fix wrong %I
            }
            else
                break;
        }
    }
}
