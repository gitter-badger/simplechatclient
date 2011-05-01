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
// for abort()
#include <stdio.h>
#include <stdlib.h>
// debug
#include "debug.h"

#ifdef Q_WS_X11
#include <QProcess>
#include <signal.h>
#include <QDebug>
#include <cstdio>
#include <pwd.h>
#include <stdlib.h>
#include <execinfo.h>

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

void crashHandler()
{
    QString path = QDir::homePath()+"/.scc";

    // create dir if not exist
    QDir d(path);
    if (d.exists(path) == false)
        d.mkdir(path);

    QDir d1(path);
    if (d1.exists(path+"/log") == false)
        d1.mkdir(path+"/log");

    QProcess pProcess;

    int pid = QCoreApplication::applicationPid();
    QString strPid = QString::number(pid);
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

    QString strCommand = "gdb --pid "+ strPid +" -ex \"set logging overwrite on\" -ex \"set logging file "+ path +"/log/crash-"+ strRandom +".txt\" -ex \"set logging on\" -ex \"backtrace\" -ex \"info registers\" -ex \"x/16i $pc\" -ex \"thread apply all backtrace\" -ex \"up\" -ex \"list\" -ex \"quit\"";

    pProcess.start(strCommand);
    pProcess.waitForFinished();

    exit(1);
}

void got_bus(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGBUS: BUS ERROR -- CRASHING!");
    crashHandler();
}

void got_segv(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGSEGV: SEGMENT VIOLATION -- CRASHING!");
    crashHandler();
}

void got_fpe(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGFPE: FLOATING POINT ERROR -- CRASHING!");
    crashHandler();
}

void got_term(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGTERM: TERMINATE SIGNAL -- SIGNING OFF!");
    crashHandler();
}

void got_hup(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGHUP: HANGUP SIGNAL -- SIGNING OFF!");
    crashHandler();
}

void got_ill(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGILL: ILL SIGNAL -- SIGNING OFF!");
    crashHandler();
}

void got_abrt(int z)
{
    Q_UNUSED (z);
    saveMessage("debug", "error: SIGABRT: ABRT SIGNAL -- SIGNING OFF!");
    crashHandler();
}

void linux_crash_handler()
{
    struct sigaction sv;

    // sigbus
    sv.sa_handler = got_bus;
    sigemptyset(&sv.sa_mask);
    sv.sa_flags = 0;
    sigaction(SIGBUS, &sv, NULL);
    // sigsegv
    sv.sa_handler = got_segv;
    sigaction(SIGSEGV, &sv, NULL);
    // sigfpe
    sv.sa_handler = got_fpe;
    sigaction(SIGFPE, &sv, NULL);
    // sigterm
    sv.sa_handler = got_term;
    sigaction(SIGTERM, &sv, NULL);
    // sighup
    sv.sa_handler = got_hup;
    sigaction(SIGHUP, &sv, NULL);
    // sigpipe
    sv.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sv, NULL);
    // sigill
    sv.sa_handler = got_ill;
    sigaction(SIGILL, &sv, NULL);
    // sigabrt
    sv.sa_handler = got_abrt;
    sigaction(SIGABRT, &sv, NULL);
}
#endif

void saveMessage(QString strFilename, QString strData)
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

    QFile f(path+"/log/"+strFilename+".txt");
    if (!f.open(QIODevice::Append))
    {
#ifdef Q_WS_X11
        qDebug() << "Error: log: Cannot open file " << strFilename << ".txt";
#endif
        return;
    }

    QTextStream out(&f);
    out << strData << "\r\n";

    f.close();
}

void messageHandler(QtMsgType type, const char *msg)
{
    switch (type)
    {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s\n", msg);
            saveMessage("debug", QString("Debug: %1").arg(msg));
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s\n", msg);
            saveMessage("debug", QString("Warning: %1").arg(msg));
#ifdef Q_WS_X11
            printBacktrace("warning from Qt");
#endif
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s\n", msg);
            saveMessage("debug", QString("Critical: %1").arg(msg));
#ifdef Q_WS_X11
            printBacktrace("critical error from Qt");
#endif
#ifndef Q_WS_X11
            abort();
#endif
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s\n", msg);
            saveMessage("debug", QString("Fatal: %1").arg(msg));
#ifdef Q_WS_X11
            printBacktrace("fatal error from Qt");
#endif
            abort();
            break;
    }
}
