/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "inputline.h"

Inputline::Inputline(QWidget *parent) : QLineEdit(parent)
{
    index = 0;
    strLastWord = QString::null;
}

Inputline::~Inputline()
{
    strLastWord = QString::null;
}

void Inputline::set_userslist(QListWidget *param1)
{
    users_list.clear();

    for (int i = 0; i < param1->count(); i++)
        users_list.append(param1->item(i)->data(Qt::UserRole).toString());
}

void Inputline::insert_text(QString strText)
{
    // pos
    int iPos = this->cursorPosition();

    // insert text
    QString strLine = this->text();
    strLine.insert(iPos, strText);
    this->setText(strLine);
}

QString Inputline::get_word()
{
    QString strWord = this->text();
    if (strWord.isEmpty() == false)
    {
        int iLength = 0;
        int iStart = 0;

        iStart = strWord.lastIndexOf(' ', this->cursorPosition()-1)+1;
        iLength = this->cursorPosition()-iStart;
        strWord = strWord.mid(iStart, iLength);

        strWord.replace(":", "");
    }
    return strWord;
}

void Inputline::set_word(QString strSetWord)
{
    QString strWord = this->text();
    if (strWord.isEmpty() == false)
    {
        int iLength = 0;
        int iStart = 0;

        iStart = strWord.lastIndexOf(' ', this->cursorPosition()-1)+1;
        iLength = this->cursorPosition()-iStart;
        strWord = strWord.mid(iStart, iLength);

        if (iStart == 0)
            strSetWord += ":";

        QString strNewLine = this->text();
        strNewLine = strNewLine.replace(iStart, iLength, strSetWord);
        this->setText(strNewLine);

        // set cursor
        this->setCursorPosition(iStart+strSetWord.length());
    }
}

bool Inputline::event(QEvent *e)
{
    if (e->type() != QEvent::KeyPress)
        return QLineEdit::event(e);

    QKeyEvent *k = static_cast<QKeyEvent*>(e);

    if (k->key() == Qt::Key_Tab)
    {
        if (users_list.count() == 0)
            return true;

        QString strWord = get_word();

        if (strWord.isEmpty() == true)
            return true;

        if (strLastWord.isEmpty() == true)
        {
            find.clear();
            for (int i = 0; i < users_list.count(); i++)
            {
                if (users_list.at(i).startsWith(strWord, Qt::CaseInsensitive) == true)
                    find.append(users_list.at(i));
                if (users_list.at(i).startsWith("~"+strWord, Qt::CaseInsensitive) == true)
                    find.append(users_list.at(i));
            }

            strLastWord = strWord;
        }

        if (find.isEmpty() == false)
        {
            if (index > find.count())
                index = 0;

            if (find.isEmpty() == false)
            {
                QString strSetWord = find.at(index);
                set_word(strSetWord);
            }

            index++;
            if (index >= find.count())
                index = 0;
        }

        return true;
    }
    else
    {
        index = 0;
        strLastWord = QString::null;
        return QLineEdit::event(e);
    }
}
