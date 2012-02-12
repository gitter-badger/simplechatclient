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
    Convert(bool _bInsertWidthHeight = false);
    void convertText(QString &);
    void removeFont(QString &);
    void removeColor(QString &);

    bool getRemovedBold() { return bRemovedBold; }
    bool getRemovedItalic() { return bRemovedItalic; }
    QString getRemovedFont() { return strRemovedFont; }
    int getRemovedColor() { return iRemovedColor; }

private:
    bool bInsertWidthHeight;
    bool bRemovedBold;
    bool bRemovedItalic;
    QString strRemovedFont;
    int iRemovedColor;

    void convertColor(QString &);
    void convertFont(QString &);
    void convertEmoticons(QString &);
    QString findEmoticon(const QString&);
};

#endif // CONVERT_H
