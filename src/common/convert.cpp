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
#include <QDir>
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
    QRegExp rx("%F(b|i|bi)?:?(arial|times|verdana|tahoma|courier)?%");

    int pos = 0;
    while ((pos = rx.indexIn(strData, pos)) != -1)
    {
        int first = pos;
        pos += rx.matchedLength();
        int second = pos;

        if (Core::instance()->settings.value("hide_formating") == "off")
        {
            QString strAtributes;
            QString strFontStyle = rx.cap(1);
            QString strFontName = rx.cap(2);

            if (strFontStyle.contains("b")) strAtributes += "font-weight:bold;";
            if (strFontStyle.contains("i")) strAtributes += "font-style:italic;";

            if (strFontName == "arial") strAtributes += "font-family:Arial;";
            else if (strFontName == "times") strAtributes += "font-family:Times New Roman;";
            else if (strFontName == "verdana") strAtributes += "font-family:Verdana;";
            else if (strFontName == "tahoma") strAtributes += "font-family:Tahoma;";
            else if (strFontName == "courier") strAtributes += "font-family:Courier New;";
            else strAtributes += "font-family:Verdana;";

            if (!strAtributes.isEmpty())
            {
                strData.replace(first, second-first, QString("<span style=\"%1\">").arg(strAtributes));
                strData += "</span>";
            }
        }
        else
            strData.remove(first, second-first);

        pos--;
    }
}

void Convert::convertEmoticons(QString &strData)
{
    QRegExp rx("%I([a-zA-Z0-9_-]+)%");

    int pos = 0;
    while ((pos = rx.indexIn(strData, pos)) != -1)
    {
        int first = pos;
        pos += rx.matchedLength();
        int second = pos;

        QString strEmoticon = rx.cap(1);

        if (Core::instance()->settings.value("disable_emots") == "off")
        {
            QString strEmoticonPath = findEmoticon(strEmoticon);

            if (!strEmoticonPath.isEmpty())
            {
                QString strWidthHeight;
                if (bInsertWidthHeight)
                {
                    QPixmap p(strEmoticonPath);
                    strWidthHeight = "width=\""+QString::number(p.width())+"px\" height=\""+QString::number(p.height())+"px\"";
                }

                strData.replace(first, second-first, QString("<img src=\"%1\" alt=\"%2\" %3 />").arg(strEmoticonPath).arg(strEmoticon).arg(strWidthHeight));
            }
            else
                strData.replace(first, second-first, QString("//%1").arg(strEmoticon));
        }
        else
            strData.replace(first, second-first, QString("//%1").arg(strEmoticon));

        pos--;
    }
}

void Convert::convertText(QString &strData)
{
    convertFont(strData);
    convertColor(strData);
    convertEmoticons(strData);
}

void Convert::removeFont(QString &strData)
{
    QRegExp rx("%F(b|i|bi)?:?(arial|times|verdana|tahoma|courier)?%");

    int pos = 0;
    while ((pos = rx.indexIn(strData, pos)) != -1)
    {
        int first = pos;
        pos += rx.matchedLength();
        int second = pos;

        QString strFontStyle = rx.cap(1);
        QString strFontName = rx.cap(2);

        if (strFontStyle.contains("b")) bRemovedBold = true;
        if (strFontStyle.contains("i")) bRemovedItalic = true;

        strRemovedFont = strFontName;

        strData.remove(first, second-first);

        pos--;
    }
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

QString Convert::findEmoticon(QString strEmoticon)
{
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = "/usr/share/scc";
#endif

    QDir dAllEmoticonsDirs = path+"/3rdparty/emoticons";
    QStringList lDirs = dAllEmoticonsDirs.entryList(QStringList("*"), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    for (int i = 0; i < lDirs.size(); i++)
    {
        QString strDir = lDirs[i];
        QString strEmoticonCheck = QString("%1/%2/%3.gif").arg(dAllEmoticonsDirs.path()).arg(strDir).arg(strEmoticon);

        if (QFile::exists(strEmoticonCheck))
            return strEmoticonCheck;
    }
    return QString::null;
}
