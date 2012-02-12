/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "config.h"
#ifdef Q_WS_X11
#include "debug.h"
#endif
#include "core.h"

#ifdef Q_WS_WIN
#include <windows.h>
#endif

void displayVersion()
{
    printf("Simple Chat Client %s\n\n"
           "Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>\n"
           "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
           "This is free software: you are free to change and redistribute it.\n\n",
           qPrintable(Core::instance()->version()));
}

void displayOptions()
{
    printf("Options:\n"
           "  --help                     Display this information\n"
           "  --version                  Display version information\n"
           "  --debug                    Enable debug output\n\n");
}

int main(int argc, char *argv[])
{
#ifdef Q_WS_WIN
    /* Call SetProcessDEPPolicy to permanently enable DEP.
       The function will not resolve on earlier versions of Windows,
       and failure is not dangerous. */
    HMODULE hMod = GetModuleHandleA("Kernel32.dll");
    if (hMod)
    {
        typedef BOOL (WINAPI *PSETDEP)(DWORD);
        PSETDEP setdeppolicy = (PSETDEP)GetProcAddress(hMod, "SetProcessDEPPolicy");
        if (setdeppolicy) setdeppolicy(1); /* PROCESS_DEP_ENABLE */
    }
#endif

    Q_INIT_RESOURCE(scc);

    // linux crash handler
#ifdef Q_WS_X11
    linuxCrashHandler();
#endif

    // message handler
#ifdef Q_WS_X11
    qInstallMsgHandler(messageHandler);
#endif

    // app
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    // set codec
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("ISO-8859-2"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // set organization
    QCoreApplication::setOrganizationName("scc");
    QCoreApplication::setOrganizationDomain("simplechatclien.sourceforge.net");
    QCoreApplication::setApplicationName("scc");

    // debug off
    Core::instance()->setDebug(false);

    // get language
    Config *pConfig = new Config();
    QString strLanguage = pConfig->getValue("language");
    delete pConfig;

    // set translate
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = SCC_DATA_DIR;
#endif

    // load translate
    QTranslator qtTranslator;
    qtTranslator.load(QString("%1/translations/qt_%2").arg(path, strLanguage));
    app.installTranslator(&qtTranslator);

    QTranslator sccTranslator;
    sccTranslator.load(QString("%1/translations/scc_%2").arg(path, strLanguage));
    app.installTranslator(&sccTranslator);

    // args
    for (int i = 1; i < app.argc(); i++)
    {
        QString param = app.argv()[i];
        if (param == "--debug")
        {
            printf("Debug enabled\n\n");
            Core::instance()->setDebug(true);
        }
        else if (param == "--version")
        {
            displayVersion();
            app.deleteLater();
            return 0;
        }
        else if (param == "--help")
        {
            displayVersion();
            displayOptions();
            app.deleteLater();
            return 0;
        }
    }

    // core
    Core::instance()->createGui();
    Core::instance()->showSccWindow();
    Core::instance()->checkUpdate();

    int ret = app.exec();
    Core::instance()->quit();
    app.deleteLater();
    return ret;
}
