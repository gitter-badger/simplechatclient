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
    format.setFontUnderline(true);
    format.setUnderlineColor(QColor("#ff0000"));
    format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

    init();
}

void Highlighter::init()
{
    iMax = 0;

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

bool Highlighter::correct_word(QString strWord)
{
    QListIterator<QString> i(lKeywords);
    while (i.hasNext())
    {
        if (i.next() == strWord)
            return true;
    }
    return false;
}

void Highlighter::highlightBlock(const QString &text)
{
    // check is spellchecker active
    QSettings settings;
    if (settings.value("spellchecker").toString() == "off")
    {
        // clear if not active
        if (lKeywords.size() != 0)
            lKeywords.clear();
        return;
    }

    // is dict empty
    if (lKeywords.size() == 0) init();

    // spellchecker
    QRegExp word("\\b\\w+\\b");

    int index = 0;
    while ((index = word.indexIn(text, index)) != -1)
    {
        if (correct_word(word.cap().toUtf8()) == false)
            setFormat(index, word.matchedLength(), format);
        index += word.matchedLength();
    }
}
