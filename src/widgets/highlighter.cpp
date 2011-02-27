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
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    init();
}

void Highlighter::init()
{
    iMax = 0;
    bSpacePressed = false;

    QSettings settings;
    if (settings.value("spellchecker").toString() == "on")
    {
        if (settings.value("language").toString() == "pl")
            read_dict("pl_PL");
        else
            read_dict("en_US");
    }
}

void Highlighter::read_dict(QString strFileName)
{
    QString strPath = QCoreApplication::applicationDirPath();
    QString strDicFile = strPath+"/"+strFileName+".dic";

    QFile file(strDicFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd())
    {
             QString strLine = file.readLine().trimmed();
             if (strLine.length() > iMax) iMax = strLine.length();

             lKeywords.append(strLine);
    }

    file.close();
}

void Highlighter::highlightBlock(const QString &text)
{
    // check is spellchecker active
    QSettings settings;
    if (settings.value("spellchecker").toString() == "off")
    {
        // clear if not active
        if (lKeywords.count() != 0)
            lKeywords.clear();
        return;
    }

    // is dict empty
    if (lKeywords.count() == 0) init();

    // spellchecker
    if (bSpacePressed == true)
    {
        QTextCharFormat format;
        format.setFontUnderline(true);
        format.setUnderlineColor(QColor("#ff0000"));
        format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

        QStringList strlWords = text.split(QRegExp("\\W+"), QString::SkipEmptyParts);
        for (int i = 0; i < strlWords.size(); ++i)
        {
            QString strWord = strlWords.at(i);
            QString strWordUtf8 = strlWords.at(i).toUtf8();
            int length = strWord.length();

            if ((strWord.length() > 2) && (strWord.length() < iMax))
            {
                if (lKeywords.contains(strWordUtf8) == false)
                {
                    int count = text.count(strWord);
                    int last = 0;
                    for (int x = 0; x < count; x++)
                    {
                        int index = text.indexOf(strWord, last);
                        setFormat(index, length, format);

                        last = index+1;
                    }
                }
            }
        }

        setCurrentBlockState(0);

        bSpacePressed = false;
    }
}

void Highlighter::rehighlight()
{
    bSpacePressed = true;
}
