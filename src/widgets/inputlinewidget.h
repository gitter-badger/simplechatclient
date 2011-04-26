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

#ifndef INPUTLINEWIDGET_H
#define INPUTLINEWIDGET_H

#include <QPlainTextEdit>

class InputLineWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    InputLineWidget(QWidget*);
    virtual ~InputLineWidget();
    void set_userslist(QList <QString>);
    void insert_text(QString);

private:
    int index;
    QString strLastWord;
    QList <QString> usersList;
    QList <QString> find;
    QString strLastMsg;

    QString get_word();
    void set_word(QString);
    void paste_multi_line(QString);

protected:
    virtual bool event(QEvent *);
    virtual void insertFromMimeData(const QMimeData *);

signals:
    void returnPressed();
};

#endif // INPUTLINEWIDGET_H
