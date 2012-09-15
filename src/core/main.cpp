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

#include <QtGui/QApplication>
#include <QTextCodec>
#include "config.h"
#include "core.h"
#include "settings.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

/*
#ifdef Q_WS_X11
    #include "linux_debug.h"
#endif
*/

void displayVersion()
{
    printf("Simple Chat Client %s\n\n"
           "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
           "This is free software: you are free to change and redistribute it.\n\n",
           qPrintable(Settings::instance()->get("version")));
}

void displayOptions()
{
    printf("Options:\n"
           "  --help                     Display this information\n"
           "  --version                  Display version information\n"
           "  --debug                    Enable debug output\n\n");
}

void enableDEP()
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
}

int main(int argc, char *argv[])
{
    // enable DEP
    enableDEP();

    Q_INIT_RESOURCE(scc);

/*
    // linux crash handler
#ifdef Q_WS_X11
    linuxCrashHandler();
#endif

    // message handler
#ifdef Q_WS_X11
    qInstallMsgHandler(messageHandler);
#endif
*/

    // app
    QApplication app(argc, argv);
    // minimize to tray
    QApplication::setQuitOnLastWindowClosed(false);

    // set codec
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("ISO-8859-2"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // set organization
    QCoreApplication::setOrganizationName("scc");
    QCoreApplication::setOrganizationDomain("simplechatclien.sourceforge.net");
    QCoreApplication::setApplicationName("scc");

    // debug off
    Settings::instance()->set("debug", "false");

    // args
    for (int i = 1; i < app.argc(); i++)
    {
        QString param = app.argv()[i];
        if (param == "--debug")
        {
            printf("Debug enabled\n\n");
            Settings::instance()->set("debug", "true");
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
    Core::instance()->createAndShowGui();

    int ret = app.exec();
    Core::instance()->quit();
    app.deleteLater();
    return ret;
}
