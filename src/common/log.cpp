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
#include <QSettings>
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
    strData.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
    strData.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
    strData.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");
    return strData;
}

void Log::save(QString strChannel, QString strData)
{
    QString path;

#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    QSettings winSettings(QSettings::UserScope, "Microsoft", "Windows");
    winSettings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    path = winSettings.value("Personal").toString();
    path += "/scc";
#endif

    // create scc dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    // create logs dir if not exist
    path += "/log";
    if (!QDir().exists(path))
        QDir().mkdir(path);

    // save logs by date
    QSettings settings;
    bool bSaveLogsByDate = settings.value("save_logs_by_date").toString() == "on" ? true : false;

    if (bSaveLogsByDate)
    {
        QString strDate = QDate().currentDate().toString("yyyy-MM");

        // create logs dir if not exist
        path += "/"+strDate;
        if (!QDir().exists(path))
            QDir().mkdir(path);
    }

    QFile f(path+"/"+strChannel+".txt");
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
