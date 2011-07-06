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

#ifndef ANIMATEDEMOTICONWIDGET_H
#define ANIMATEDEMOTICONWIDGET_H

#include <QLabel>
#include <QMovie>
#include <QTextEdit>
#include <QVBoxLayout>

class AnimatedEmoticonWidget : public QWidget
{
    Q_OBJECT
public:
    AnimatedEmoticonWidget(QTextEdit*);
    void startEmoticon(QString, int, int);
    void stopEmoticon();

private:
    QTextEdit *parentWidget;
    QString strCurrentEmoticon;
    QMovie *movie;
    QVBoxLayout *layout;
    QLabel *label;

    int getCorrectX(int, int);
    int getCorrectY(int, int);
};

#endif // ANIMATEDEMOTICONWIDGET_H
