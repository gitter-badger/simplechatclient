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

#include <QtGui/QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QTextCodec>
#include <QTranslator>
#include "config.h"
#ifdef Q_WS_X11
#include "debug.h"
#endif
#include "core.h"
#include "singleapplication.h"

void displayVersion()
{
    printf("Simple Chat Client %s\n\n"
           "Copyright (C) 2011 Piotr Łuczko <piotr.luczko@gmail.com>\n"
           "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
           "This is free software: you are free to change and redistribute it.\n\n",
           qPrintable(Core::version()));
}

void displayOptions()
{
    printf("Options:\n"
           "  --help                     Display this information\n"
           "  --version                  Display version information\n"
           "  --debug                    Enable debug output\n\n");
}

// clear settings without debug
void clearSettings()
{
    QSettings settings;

    // read debug
    QString strOldDebug = settings.value("debug").toString();

    // clear
    settings.clear();

    // restore debug
    settings.setValue("debug", strOldDebug);
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(scc);

    // linux crash handler
#ifdef Q_WS_X11
    linux_crash_handler();
#endif

    // message handler
#ifdef Q_WS_X11
    qInstallMsgHandler(messageHandler);
#endif

    // single app
    SingleApplication app(argc, argv, "scc");

    // set codec
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("ISO-8859-2"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // set organization
    QCoreApplication::setOrganizationName("scc");
    QCoreApplication::setOrganizationDomain("simplechatclien.sourceforge.net");
    QCoreApplication::setApplicationName("scc");

    // debug off
    QSettings settings;
    settings.setValue("debug", "off");

    // get translate
    Config *pConfig = new Config();
    QString strLanguage = pConfig->get_value("language");
    delete pConfig;

    // set translate
    QString strPath = QCoreApplication::applicationDirPath();

    // load translate
    QTranslator qtTranslator;
    qtTranslator.load(QString("%1/i18n/qt_%2").arg(strPath).arg(strLanguage));
    app.installTranslator(&qtTranslator);

    QTranslator sccTranslator;
    sccTranslator.load(QString("%1/i18n/scc_%2").arg(strPath).arg(strLanguage));
    app.installTranslator(&sccTranslator);

    // args
    for (int i = 1; i < app.argc(); i++)
    {
        QString param = app.argv()[i];
        if (param == "--debug")
        {
            printf("Debug enabled\n\n");
            Core::enableDebug();
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

    // detect already running
    if (app.isRunning())
    {
        QMessageBox::critical(0, app.translate("@default", "Error"), app.translate("@default", "Simple Chat Client is already running"));
        return 0;
    }

    // clear settings;
    clearSettings();

    // core
    Core::instance()->createGui();
    Core::instance()->showSccWindow();
    Core::instance()->check_update();

    return app.exec();
}
