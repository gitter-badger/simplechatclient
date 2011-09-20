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

#ifndef INPUTLINE_WIDGET_H
#define INPUTLINE_WIDGET_H

#include <QLineEdit>

class InputLineWidget : public QLineEdit
{
    Q_OBJECT
public:
    InputLineWidget(QWidget*);
    virtual ~InputLineWidget();
    void insertText(QString);

private:
    int index;
    QString strLastWord;
    QList<QString> find;
    QString strLastMsg;

    QString getWord();
    void setWord(QString);

protected:
    virtual bool event(QEvent *);

signals:
    void returnPressed();
    void ctrlTabPressed();
    void ctrlShiftTabPressed();
};

#endif // INPUTLINE_WIDGET_H