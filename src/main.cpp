/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#include <QDesktopWidget>
#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    bool bDebug = false;

    if (argc > 1)
    {
        for (int i = 0; i < argc; i++)
        {
            // version
            if (strcmp(argv[i], "--version") == 0)
            {
#ifdef Q_WS_X11
                qDebug() << "Simple Chat Client version 1.0.3";
#endif
                return 0;
            }
            // debug
            if (strcmp(argv[i], "-debug") == 0)
            {
#ifdef Q_WS_X11
                qDebug() << "Debug: on";
#endif
                bDebug = true;
            }
        }
    }

    Q_INIT_RESOURCE(scc);

    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("ISO-8859-2"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    const int x = QApplication::desktop()->availableGeometry().width();
    const int y = QApplication::desktop()->availableGeometry().height();
    const int xo = 800;
    const int yo = 600;

    QCoreApplication::setOrganizationName("scc");
    QCoreApplication::setOrganizationDomain("simplechatclien.sourceforge.net");
    QCoreApplication::setApplicationName("scc");

    MainWindow mainWin;
    mainWin.set_debug(bDebug);
    mainWin.resize(xo,yo);
    mainWin.setGeometry((x-xo)/2,(y-yo)/2,xo,yo);
    mainWin.show();

    return app.exec();
}
