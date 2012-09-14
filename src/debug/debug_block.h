/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#ifndef DEBUG_BLOCK_H
#define DEBUG_BLOCK_H

#include <QMutex>
#include <QtDebug>

#if QT_VERSION >= 0x040700
    #include <QElapsedTimer>
#else
    #include <QTime>
#endif

#ifdef _WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define DEBUG_BLOCK DebugBlock uniquelyNamedStackAllocatedStandardBlock( __PRETTY_FUNCTION__ );

class DebugBlock
{
public:
    DebugBlock(const char* label);
    ~DebugBlock();

private:
    const char* mp_label;
    int m_color;

    static QMutex sm_mutex;

#if QT_VERSION >= 0x040700
    QElapsedTimer m_startTime;
#else
    QTime m_startTime;
#endif
};

#endif // DEBUG_BLOCK_H
