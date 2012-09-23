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
#include <QDir>
#include <QProcess>
#include "log.h"
#include "settings.h"

// for abort()
#include <stdio.h>
#include <stdlib.h>
// debug

#include <signal.h>
#include <cstdio>
#include <pwd.h>
#include <stdlib.h>
#include <execinfo.h>

#include "linux_debug.h"

// function printBacktrace is from Kadu project (kadu.net)
void printBacktrace(const QString &header)
{
    if (header.isEmpty())
        fprintf(stderr, "\nbacktrace:\n");
    else
        fprintf(stderr, "\nbacktrace: ('%s')\n", qPrintable(header));

    void *bt_array[100];
    char **bt_strings;
    int num_entries;
    if ((num_entries = backtrace(bt_array, 100)) < 0) {
        fprintf(stderr, "could not generate backtrace\n");
        return;
    }
    if ((bt_strings = backtrace_symbols(bt_array, num_entries)) == NULL) {
        fprintf(stderr, "could not get symbol names for backtrace\n");
        return;
    }
    fprintf(stderr, "======= BEGIN OF BACKTRACE =====\n");
    for (int i = 0; i < num_entries; ++i)
        fprintf(stderr, "[%d] %s\n", i, bt_strings[i]);
    fprintf(stderr, "======= END OF BACKTRACE  ======\n");
    free(bt_strings);

    fflush(stderr);
}

QString createRandom6()
{
    QString strRandom;
    while(strRandom.length() < 6)
    {
        int cat = qrand() % 3;

        // 0-9
        if (cat == 0)
        {
            int i = qrand() % 9;
            QChar c = i+=48;
            strRandom += c.toAscii();
        }
        // A-Z
        else if (cat == 1)
        {
            int i = qrand() % 25;
            QChar c = i+=65;
            strRandom += c.toAscii();
        }
        // a-z
        else if (cat == 2)
        {
            int i = qrand() % 25;
            QChar c = i+=97;
            strRandom += c.toAscii();
        }
    }
    return strRandom;
}

void crashHandler()
{
    QString path = QDir::homePath()+"/.scc/";

    QString strCurrentProfile = Settings::instance()->get("current_profile");
    path += "profiles/"+strCurrentProfile+"/log/";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    int pid = QCoreApplication::applicationPid();
    QString strPid = QString::number(pid);
    QString strRandom = createRandom6();

    QString strCommand = "gdb --pid "+ strPid +" -ex \"set logging overwrite on\" -ex \"set logging file "+ path +"/crash-"+ strRandom +".txt\" -ex \"set logging on\" -ex \"backtrace\" -ex \"up\" -ex \"list\" -ex \"quit\"";

    QProcess pProcess;
    pProcess.start(strCommand);
    pProcess.waitForFinished();

    exit(1);
}

void gotSignal(int)
{
    QString strMessage = "Error: Got signal SIGSEGV";

    // save
    Log::save("debug", strMessage);

    // handler
    crashHandler();
}

void linuxCrashHandler()
{
    signal(SIGSEGV, gotSignal);
}

void messageHandler(QtMsgType type, const char *msg)
{
    std::string strCategory;

    if (type == QtDebugMsg) strCategory = "Debug";
    else if (type == QtWarningMsg) strCategory = "Warning";
    else if (type == QtCriticalMsg) strCategory = "Critical";
    else if (type == QtFatalMsg) strCategory = "Fatal";

    // display
    fprintf(stderr, "%s: %s\n", strCategory.c_str(), msg);

    QString strMessage = QString("%1: %2").arg(strCategory.c_str(), msg);

    // save
    Log::save("debug", strMessage);

    // backtrace
    if (type == QtWarningMsg) fprintf(stderr, "\nwarning from Qt: ('%s')\n", qPrintable(strMessage));
    else if (type == QtCriticalMsg) printBacktrace("critical error from Qt");
    else if (type == QtFatalMsg) printBacktrace("fatal error from Qt");
}
