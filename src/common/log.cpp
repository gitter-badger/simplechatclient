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
#include <QDate>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "core.h"
#include "log.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#else
    #include <QDebug>
#endif

Log::Log()
{
}

void Log::convert(QString &strData)
{
    strData.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
    strData.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
    strData.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");
}

void Log::save(QString &strChannel, QString &strData)
{
    QString path;

#ifdef Q_WS_WIN
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    // create scc dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    // create logs dir if not exist
    path += "/log";
    if (!QDir().exists(path))
        QDir().mkdir(path);

    // save logs by date
    bool bSaveLogsByDate = Core::instance()->settings.value("save_logs_by_date") == "on" ? true : false;

    if (bSaveLogsByDate)
    {
        QString strDate = QDate().currentDate().toString("yyyy-MM");

        // create logs dir if not exist
        path += "/"+strDate;
        if (!QDir().exists(path))
            QDir().mkdir(path);
    }

    QFile f(path+"/"+strChannel+".txt");
    if (f.open(QIODevice::Append))
    {
        // convert
        QString strSaveData = strData;
        convert(strSaveData);

        // save
        QTextStream out(&f);
        out << strSaveData << "\r\n";

        f.close();
    }
}
