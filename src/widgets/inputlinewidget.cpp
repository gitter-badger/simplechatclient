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

#include <QEvent>
#include <QKeyEvent>
#include <QTreeWidget>
#include "inputlinewidget.h"

InputLineWidget::InputLineWidget(QWidget *parent) : QPlainTextEdit(parent)
{
    index = 0;
    strLastWord = QString::null;
    setMaximumHeight(26);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWordWrapMode(QTextOption::NoWrap);
    setStyleSheet("margin:0;padding:0;font-size:11px;");
}

InputLineWidget::~InputLineWidget()
{
    strLastWord = QString::null;
    usersList.clear();
}

void InputLineWidget::set_userslist(QList<QString> newUsersList)
{
    usersList = newUsersList;
}

void InputLineWidget::insert_text(QString strText)
{
    // pos
    int iPos = this->textCursor().position();

    // insert text
    QString strLine = this->toPlainText();
    strLine.insert(iPos, strText);
    this->setPlainText(strLine);
}

QString InputLineWidget::get_word()
{
    QString strWord = this->toPlainText();
    if (strWord.isEmpty() == false)
    {
        int iLength = 0;
        int iStart = 0;

        // if first word -1, if next -2
        if (this->textCursor().position()-1 == 0)
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-1)+1;
        else
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-2)+1;

        iLength = this->textCursor().position()-iStart;
        strWord = strWord.mid(iStart, iLength);

        strWord.replace(" ", "");
    }
    return strWord;
}

void InputLineWidget::set_word(QString strSetWord)
{
    QString strWord = this->toPlainText();
    if (strWord.isEmpty() == false)
    {
        int iLength = 0;
        int iStart = 0;

        // if first word -1, if next -2
        if (this->textCursor().position()-1 == 0)
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-1)+1;
        else
            iStart = strWord.lastIndexOf(' ', this->textCursor().position()-2)+1;

        iLength = this->textCursor().position()-iStart;
        strWord = strWord.mid(iStart, iLength);

        strSetWord += " ";

        QString strNewLine = this->toPlainText();
        strNewLine = strNewLine.replace(iStart, iLength, strSetWord);
        this->setPlainText(strNewLine);

        // set cursor
        QTextCursor cursor = this->textCursor();
        cursor.setPosition(iStart+strSetWord.length(), QTextCursor::MoveAnchor);
        this->setTextCursor(cursor);
    }
}

bool InputLineWidget::event(QEvent *e)
{
    if (e->type() != QEvent::KeyPress)
        return QPlainTextEdit::event(e);

    QKeyEvent *k = static_cast<QKeyEvent*>(e);

    if (k->key() == Qt::Key_Tab)
    {
        if (usersList.size() == 0)
            return true;

        QString strWord = get_word();

        if (strWord.isEmpty() == true)
            return true;

        if (strLastWord.isEmpty() == true)
        {
            find.clear();
            for (int i = 0; i < usersList.size(); i++)
            {
                if (usersList.at(i).startsWith(strWord, Qt::CaseInsensitive) == true)
                    find.append(usersList.at(i));
                if (usersList.at(i).startsWith("~"+strWord, Qt::CaseInsensitive) == true)
                    find.append(usersList.at(i));
            }

            strLastWord = strWord;
        }

        if (find.isEmpty() == false)
        {
            if (index > find.size())
                index = 0;

            if (find.isEmpty() == false)
            {
                QString strSetWord = find.at(index);
                set_word(strSetWord);
            }

            index++;
            if (index >= find.size())
                index = 0;
        }

        return true;
    }
    else if ((k->key() == Qt::Key_Enter) || (k->key() == Qt::Key_Return))
    {
        strLastMsg = this->toPlainText().simplified();
        emit returnPressed();
        return true;
    }
    else if (k->key() == Qt::Key_Up)
    {
        // text
        setPlainText(strLastMsg);

        // cursor
        QTextCursor cursor = this->textCursor();
        cursor.setPosition(strLastMsg.size(), QTextCursor::MoveAnchor);
        this->setTextCursor(cursor);

        return true;
    }
    else if (k->key() == Qt::Key_Down)
    {
        clear();
        return true;
    }
    else
    {
        index = 0;
        strLastWord = QString::null;
        return QPlainTextEdit::event(e);
    }
}

void InputLineWidget::paste_multi_line(QString strText)
{
    QStringList list = strText.split(QRegExp("(\n|\r)"));
    int len = 400;

    for (int i = 0; i < list.size(); i++)
    {
        QString line = list.at(i);
        if (line.size() > len)
        {
            while (line.size() > len)
            {
                QString short_line = line.left(len);

                QString old_text = this->toPlainText();
                this->setPlainText(old_text+short_line);
                emit returnPressed();

                line.remove(0, len);
            }
        }
        if ((line.size() < len) && (line.size() != 0))
        {
            QString old_text = this->toPlainText();
            this->setPlainText(old_text+line);

            // if not last text - send
            if (i != list.size()-1)
                emit returnPressed();
            else
            {
                // move cursor
                QTextCursor cursor = this->textCursor();
                cursor.setPosition(this->toPlainText().size(), QTextCursor::MoveAnchor);
                this->setTextCursor(cursor);
            }
        }
    }
}

// fixed pasting multi-line
void InputLineWidget::insertFromMimeData(const QMimeData *source)
{
    paste_multi_line(source->text());
}
