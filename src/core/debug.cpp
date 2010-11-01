/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "debug.h"

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
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s\n", msg);
            saveMessage("debug", QString("Critical: %1").arg(msg));
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s\n", msg);
            saveMessage("debug", QString("Fatal: %1").arg(msg));
            abort();
    }
}
