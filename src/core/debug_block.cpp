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

#include "debug_block.h"

QMutex DebugBlock::sm_mutex;

int s_colors[] = { 1, 2, 4, 5, 6 }; // no yellow and white for sanity
int s_colorIndex = 0;

QString colorize( const QString &text, int color = s_colorIndex )
{
    return QString( "\x1b[00;3%1m%2\x1b[00;39m" ).arg( QString::number(s_colors[color]), text );
}

DebugBlock::DebugBlock(const char* label): mp_label(label), m_color(s_colorIndex)
{
    sm_mutex.lock();
    s_colorIndex = (s_colorIndex + 1) % 5;

    qDebug() << qPrintable( colorize( QLatin1String( "BEGIN:" ), m_color ) ) << mp_label;

    sm_mutex.unlock();
}

DebugBlock::~DebugBlock()
{
    qDebug() << qPrintable( colorize( QLatin1String( "END:" ), m_color ) ) << mp_label;
}
