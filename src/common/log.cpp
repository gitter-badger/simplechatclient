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

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "channel.h"
#include "core.h"
#include "convert.h"
#include "log.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

void Log::logOpened(const QString &strChannel)
{
    QString strData = "--- Log opened "+QDateTime::currentDateTime().toString(Qt::TextDate);
    Log::save(strChannel, strData);
}

void Log::logClosed(const QString &strChannel)
{
    QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
    Log::save(strChannel, strData);
}

void Log::save(const QString &strChannel, const QString &strData)
{
    QString path;

#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    QString strCurrentProfile = Core::instance()->settings.value("current_profile");
    path += "/profiles/"+strCurrentProfile+"/log";

    // save logs by date
    bool bSaveLogsByDate = Core::instance()->settings.value("save_logs_by_date") == "true" ? true : false;
    if (bSaveLogsByDate)
    {
        QString strDate = QDate().currentDate().toString("yyyy-MM");
        path += "/"+strDate;
    }

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    QString strFileName = strChannel;

    // fixed priv name
    if (strFileName[0] == '^')
        strFileName = Channel::instance()->getPriv(strFileName);

    QFile f(path+"/"+strFileName+".txt");
    if (f.open(QIODevice::Append))
    {
        // convert
        QString strSaveData = strData; 

        if (strChannel != DEBUG)
            Convert::simpleConvert(strSaveData);

        // save
        QTextStream out(&f);
        out << strSaveData << "\r\n";

        f.close();
    }
}
