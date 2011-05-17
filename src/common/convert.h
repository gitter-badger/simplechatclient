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

#ifndef CONVERT_H
#define CONVERT_H

#include <QObject>

/**
 * Convert text
 * %Fbi%%C008100%%Ihehe%
 */
class Convert : public QObject
{
    Q_OBJECT
public:
    Convert();
    void convert_text(QString *, QString *);
    void remove_font(QString *);
    void remove_color(QString *);

    bool get_removed_bold() { return bRemovedBold; }
    bool get_removed_italic() { return bRemovedItalic; }
    QString get_removed_font() { return strRemovedFont; }
    int get_removed_color() { return iRemovedColor; }

private:
    bool bRemovedBold;
    bool bRemovedItalic;
    QString strRemovedFont;
    int iRemovedColor;
};

#endif // CONVERT_H
