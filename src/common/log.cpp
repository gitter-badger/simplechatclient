/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2014 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include "convert.h"
#include "html_messages_renderer.h"
#include "settings.h"
#include "log.h"

#ifdef Q_OS_WIN
    #include <QStandardPaths>
#endif

void Log::logOpened(const QString &strChannel)
{
    QString strData = "--- Log opened "+QDateTime::currentDateTime().toString(Qt::TextDate);

    QString strLogsFormat = Settings::instance()->get("logs_format");
    if (strLogsFormat == "html")
    {
        strData = "<div><strong>"+strData+"</strong></div>";
        Log::save(strChannel, strData, Log::Html);
    }
    else
    {
        Log::save(strChannel, strData, Log::Txt);
    }
}

void Log::logClosed(const QString &strChannel)
{
    QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);

    QString strLogsFormat = Settings::instance()->get("logs_format");
    if (strLogsFormat == "html")
    {
        strData = "<div><strong>"+strData+"</strong></div>";
        Log::save(strChannel, strData, Log::Html);
    }
    else
    {
        Log::save(strChannel, strData, Log::Txt);
    }
}

void Log::save(const QString &strChannel, const QString &strMessage, Log::LogsFormat currentLogFormat)
{
    if (Settings::instance()->get("logs") == "false") return;
    if ((strChannel.isEmpty()) || (strMessage.isEmpty())) return;

    QString strCurrentProfile = Settings::instance()->get("current_profile");
    QString path;
#ifdef Q_OS_WIN
    path = QFileInfo(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).absoluteFilePath();
    path += "/scc/";
#else
    path = QDir::homePath()+"/.scc/";
#endif

    path += "profiles/"+strCurrentProfile+"/log/";

    // save logs by date
    bool bLogsByDate = Settings::instance()->get("logs_by_date") == "true" ? true : false;
    if (bLogsByDate)
    {
        QString strDate = QDate().currentDate().toString("yyyy-MM");
        path += strDate+"/";
    }

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    QString strFileName = strChannel;

    // fixed priv name
    if (strFileName.at(0) == '^')
        strFileName = Channel::instance()->getAlternativeName(strFileName);

    // convert
    QString strSaveMessage = strMessage;

    // file
    QString strFileExtension = (currentLogFormat == Log::Html ? "html" : "txt");
    QFile f(path+strFileName+"."+strFileExtension);

    if (currentLogFormat == Log::Html)
    {
        if (!f.exists())
        {
            HtmlMessagesRenderer *pHtmlMessagesRenderer = new HtmlMessagesRenderer();
            QString strHeadCSS = pHtmlMessagesRenderer->headCSS();
            QString strBodyCSS = pHtmlMessagesRenderer->bodyCSS();
            delete pHtmlMessagesRenderer;

            strSaveMessage = "<!DOCTYPE html><html><head><title>"+strFileName+"</title><style type=\"text/css\">"+strHeadCSS+"</style></head><body style=\""+strBodyCSS+"\">"+strSaveMessage;
        }

        QString path;
#ifdef Q_OS_WIN
        path = QCoreApplication::applicationDirPath();
#else
        path = SCC_DATA_DIR;
#endif

        strSaveMessage = strSaveMessage.replace("qrc:/images/oxygen/16x16", "file://"+path+"/images");
        strSaveMessage = strSaveMessage.replace("qrc:", "file://"+path);
    }

    if (f.open(QIODevice::Append))
    {

        if (strChannel != DEBUG_WINDOW)
            Convert::simpleConvert(strSaveMessage);

        // save
        QTextStream out(&f);
        out << strSaveMessage << "\r\n";

        f.close();
    }
}
