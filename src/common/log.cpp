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

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#ifndef Q_WS_X11
#include <QSettings>
#endif
#include <QTextStream>
#include "log.h"

#ifdef Q_WS_X11
#include <QDebug>
#endif

Log::Log()
{
}

QString Log::convert(QString strData)
{
    strData.replace(QRegExp("%C([a-zA-Z0-9]+)%"),"");
    strData.replace(QRegExp("%F([a-zA-Z0-9:]+)%"),"");
    strData.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");
    return strData;
}

void Log::save(QString strChannel, QString strData)
{
    QString path;

#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
    settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    path = settings.value("Personal").toString();
    path += "/scc";
#endif

    // create dir if not exist
    QDir d(path);
    if (d.exists(path) == false)
        d.mkdir(path);

    QDir d1(path);
    if (d1.exists(path+"/log") == false)
        d1.mkdir(path+"/log");

    QFile f(path+"/log/"+strChannel+".txt");
    if (!f.open(QIODevice::Append))
    {
#ifdef Q_WS_X11
        qDebug() << tr("Error: log: Cannot open file ") << strChannel;
#endif
        return;
    }

    strData = convert(strData);

    QTextStream out(&f);
    out << strData << "\r\n";
    
    f.close();
}
