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

#ifndef DEBUG_H
#define DEBUG_H

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#ifdef Q_WS_X11
#include <QDebug>
#endif
#ifndef Q_WS_X11
#include <QSettings>
#endif
// for abort()
#include <stdio.h>
#include <stdlib.h>

void saveMessage(QString, QString);
void messageHandler(QtMsgType, const char *);

#endif // DEBUG_H
