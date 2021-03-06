/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2015 Piotr Łuczko <piotr.luczko@gmail.com>
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

#ifndef SIMPLE_PERCENTAGE_WIDGET_H
#define SIMPLE_PERCENTAGE_WIDGET_H

#include <QWidget>

class SimplePercentageWidget : public QWidget
{
    Q_OBJECT
public:
    SimplePercentageWidget(QWidget *parent = 0, int _iWidth = 400, int _iHeight = 30, double _iRank = 0);

    void setRank(double _iRank);
    QSize sizeHint() const;

private:
    int iWidth;
    int iHeight;
    double iRank;

protected:
     void paintEvent(QPaintEvent *);
};

#endif // SIMPLE_PERCENTAGE_WIDGET_H
