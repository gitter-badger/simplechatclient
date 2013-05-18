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

#include <QEvent>
#include <QKeyEvent>
#include "channel.h"
#include "core.h"
#include "mainwindow.h"
#include "nick.h"
#include "inputline_widget.h"

InputLineWidget::InputLineWidget(QWidget *parent) : QLineEdit(parent), index(0), strLastWord(QString::null), iLastMessage(-1), iLastMessageLimit(30)
{
}

InputLineWidget::~InputLineWidget()
{
    strLastWord = QString::null;
}

void InputLineWidget::insertText(const QString &strText)
{
    // pos
    int iPos = this->cursorPosition();

    // insert text
    QString strLine = this->text();
    strLine.insert(iPos, strText);
    this->setText(strLine);
}

QString InputLineWidget::getWord()
{
    QString strWord = this->text();
    if (!strWord.isEmpty())
    {
        int iLength = 0;
        int iStart = 0;

        // if first word -1, if next -2
        if (this->cursorPosition()-1 == 0)
            iStart = strWord.lastIndexOf(' ', this->cursorPosition()-1)+1;
        else
            iStart = strWord.lastIndexOf(' ', this->cursorPosition()-2)+1;

        iLength = this->cursorPosition()-iStart;
        strWord = strWord.mid(iStart, iLength);

        strWord.remove(" ");
    }
    return strWord;
}

void InputLineWidget::setWord(const QString &strSetWord)
{
    QString strWord = this->text();
    if (!strWord.isEmpty())
    {
        int iLength = 0;
        int iStart = 0;

        // if first word -1, if next -2
        if (this->cursorPosition()-1 == 0)
            iStart = strWord.lastIndexOf(' ', this->cursorPosition()-1)+1;
        else
            iStart = strWord.lastIndexOf(' ', this->cursorPosition()-2)+1;

        iLength = this->cursorPosition()-iStart;
        strWord = strWord.mid(iStart, iLength);

        QString strFixedSetWord = strSetWord+" ";

        QString strNewLine = this->text();
        strNewLine = strNewLine.replace(iStart, iLength, strFixedSetWord);
        this->setText(strNewLine);

        // set cursor
        this->setCursorPosition(iStart+strFixedSetWord.length());
    }
}

bool InputLineWidget::event(QEvent *e)
{
    if (e->type() != QEvent::KeyPress)
        return QLineEdit::event(e);

    QKeyEvent *k = static_cast<QKeyEvent*>(e);

    // key event to main window
    Core::instance()->mainWindow()->inputLineKeyEvent(k);

    // key event
    if (k->key() == Qt::Key_Tab)
    {
        iLastMessage = -1;

        QString strChannel = Channel::instance()->getCurrentName();
        QList<CaseIgnoreString> usersList = Nick::instance()->getFromChannel(strChannel);

        if (usersList.size() == 0)
            return true;

        QString strWord = getWord();

        if (strWord.isEmpty())
            return true;

        if (strLastWord.isEmpty())
        {
            find.clear();
            foreach (const QString &strUser, usersList)
            {
                if (strUser.startsWith(strWord, Qt::CaseInsensitive))
                    find.append(strUser);
                if (strUser.startsWith("~"+strWord, Qt::CaseInsensitive))
                    find.append(strUser);
            }

            strLastWord = strWord;
        }

        if (!find.isEmpty())
        {
            if (index > find.size())
                index = 0;

            if (!find.isEmpty())
            {
                QString strSetWord = find.value(index, QString::null);
                setWord(strSetWord);
            }

            index++;
            if (index >= find.size())
                index = 0;
        }

        return true;
    }
    else if ((k->key() == Qt::Key_Enter) || (k->key() == Qt::Key_Return))
    {
        QString strText = this->text().simplified();

        if (!strText.isEmpty())
        {
            if (lLastMessages.size() >= iLastMessageLimit)
                lLastMessages.removeLast();

            lLastMessages.push_front(strText);

            iLastMessage = -1;

            emit returnPressed();
        }

        return true;
    }
    else if (k->key() == Qt::Key_Up)
    {
        iLastMessage++;
        if (iLastMessage > iLastMessageLimit)
            iLastMessage = iLastMessageLimit;

        if (iLastMessage > lLastMessages.size()-1)
            iLastMessage = lLastMessages.size()-1;

        QString strLastMessage = lLastMessages.value(iLastMessage, QString::null);

        // text
        this->setText(strLastMessage);

        // cursor
        this->setCursorPosition(strLastMessage.size());

        return true;
    }
    else if (k->key() == Qt::Key_Down)
    {
        iLastMessage--;
        if (iLastMessage < -1)
            iLastMessage = -1;

        QString strLastMessage = lLastMessages.value(iLastMessage, QString::null);

        // text
        this->setText(strLastMessage);

        // cursor
        this->setCursorPosition(strLastMessage.size());

        return true;
    }
    else
    {
        iLastMessage = -1;
        index = 0;
        strLastWord = QString::null;
        return QLineEdit::event(e);
    }
}
