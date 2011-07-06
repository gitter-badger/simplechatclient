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

#ifndef SIMPLERANKWIDGET_H
#define SIMPLERANKWIDGET_H

#include <QWidget>

class SimpleRankWidget : public QWidget
{
    Q_OBJECT
public:
    SimpleRankWidget(QWidget *parent = 0, int r = 0, int p1 = 250, int p2 = 15);
    void setRank(int);

    QSize sizeHint() const;

private:
    int iWidth;
    int iHeight;
    int iRank;

protected:
     void paintEvent(QPaintEvent *);
};

#endif // SIMPLERANKWIDGET_H
