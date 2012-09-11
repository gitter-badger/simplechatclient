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

#include <QAction>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QTcpSocket>
#include "config.h"
#include "log.h"
#include "mainwindow.h"
#include "settings.h"
#include "updates.h"
#include "core.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

Core * Core::Instance = 0;

Core * Core::instance()
{
    if (!Instance)
    {
        Instance = new Core();
        Instance->init();
    }

    return Instance;
}

Core::Core() : window(0)
{
}

Core::~Core()
{
    delete window;
    window = 0;

    kamerzystaSocket->deleteLater();

    // close network
    Settings::instance()->set("reconnect", "false");
    pNetwork->disconnect();
    pNetwork->quit();
    pNetwork->wait();
    pNetwork->deleteLater();
    pNetwork->QObject::disconnect();
    delete pNetwork;
}

void Core::init()
{
    // clear old settings
    QSettings oldSettings;
    oldSettings.clear();

    // remove old config
    removeOldConfig();

    // convert old profiles
    convertOldProfiles();

    // create settings
    createSettings();

    // language
    refreshLanguage();

    // kamerzysta
    kamerzystaSocket = new QTcpSocket();
    kamerzystaSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    kamerzystaSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

    // network
    QString strServer = "czat-app.onet.pl";
    int iPort = 5015;
    pNetwork = new Network(strServer, iPort);
    pNetwork->start(QThread::InheritPriority);
}

void Core::createAndShowGui()
{
    window = new MainWindow(0);
    window->show();
    window->init();

    // updates
    Updates::instance()->checkUpdate();
}

void Core::createSettings()
{
    // default settings
    Settings::instance()->set("version", "1.6.0.dev");
    Settings::instance()->set("available_version", QString::null);
    Settings::instance()->set("whats_new", QString::null);
    Settings::instance()->set("motd", QString::null);
    Settings::instance()->set("version_status", "unknown");
    Settings::instance()->set("update_url", QString::null);
    Settings::instance()->set("logged", "false");
    Settings::instance()->set("busy", "false");
    Settings::instance()->set("away", "false");
    Settings::instance()->set("override", "false");
    Settings::instance()->set("ignore_raw_141", "false");
    Settings::instance()->set("age_check", "true");
    Settings::instance()->set("first_run", "true");
    Settings::instance()->set("uokey", QString::null);
    Settings::instance()->set("uo_nick", QString::null);
    Settings::instance()->set("onet_ubi", QString::null);
    Settings::instance()->set("onet_cid", QString::null);
    Settings::instance()->set("onet_sid", QString::null);
    Settings::instance()->set("onet_uid", QString::null);
    Settings::instance()->set("onetzuo_ticket", QString::null);
    Settings::instance()->set("last_active", "0");
    Settings::instance()->set("reconnect", "true");

    // read config
    configValues();

    // convert old config profile
    convertOldConfig();

    // read config profile
    configProfileValues();

    // check settings
    fixSettings();
}

void Core::removeOldConfig()
{
    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    // remove file
    if (QFile::exists(path+"/scc.conf"))
        QFile::remove(path+"/scc.conf");
}

void Core::convertOldProfiles()
{
    QString path;
#ifdef Q_WS_WIN
    path = QFileInfo(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).absoluteFilePath();
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    path += "/profiles";

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QStringList("*.xml"), QDir::Files | QDir::NoSymLinks);
    foreach (QFileInfo info, list)
    {
        QString profile = info.fileName().remove(".xml");
        if (!QDir().exists(path+"/"+profile))
        {
            QDir().mkpath(path+"/"+profile);
            QFile::rename(path+"/"+profile+".xml", path+"/"+profile+"/profile.xml");
        }
        else
            QFile::remove(path+"/"+profile+".xml");
    }
}

void Core::convertOldConfig()
{
    Config *pConfig = new Config();

    // change on off -> true false
    QHash<QString,QString> lConfigValues = pConfig->read();

    QHashIterator <QString, QString> i(lConfigValues);
    while (i.hasNext())
    {
        i.next();
        QString strKey = i.key();
        QString strValue = i.value();

        if (strValue == "on")
            pConfig->set(strKey, "true");
        else if (strValue == "off")
            pConfig->set(strKey, "false");
    }

    delete pConfig;
}

void Core::configValues()
{
    // config values
    Config *pConfig = new Config(false);
    QHash<QString,QString> lConfigValues = pConfig->read();
    delete pConfig;

    // set settings
    QHashIterator <QString, QString> i(lConfigValues);
    while (i.hasNext())
    {
        i.next();
        Settings::instance()->set(i.key(), i.value());
    }
}

void Core::configProfileValues()
{
    // config profile values
    Config *pConfigProfile = new Config();
    QHash<QString,QString> lConfigProfileValues = pConfigProfile->read();
    delete pConfigProfile;

    // set profile settings
    QHashIterator <QString, QString> ip(lConfigProfileValues);
    while (ip.hasNext())
    {
        ip.next();
        Settings::instance()->set(ip.key(), ip.value());
    }
}

void Core::fixSettings()
{
    Config *pConfig = new Config();

    if (!QFile::exists(Settings::instance()->get("sound_beep")))
    {
        pConfig->set("sound_beep", QString::null);
        Settings::instance()->set("sound_beep", QString::null);
    }
    if (!QFile::exists(Settings::instance()->get("sound_query")))
    {
        pConfig->set("sound_query", QString::null);
        Settings::instance()->set("sound_query", QString::null);
    }
    if (!QFile::exists(Settings::instance()->get("background_image")))
    {
        pConfig->set("background_image", QString::null);
        Settings::instance()->set("background_image", QString::null);
    }
    if ((Settings::instance()->get("themes") != "Standard") && (Settings::instance()->get("themes") != "Origin") && (Settings::instance()->get("themes") != "Alhena") && (Settings::instance()->get("themes") != "Adara"))
    {
        pConfig->set("themes", "Standard");
        Settings::instance()->set("themes", "Standard");
    }

    delete pConfig;
}

MainWindow *Core::mainWindow()
{
    return window;
}

void Core::quit()
{
    if (!Instance)
        return;

    delete Instance;
    Instance = 0;
}

void Core::refreshLanguage()
{
    // get language
    QString strLanguage = Settings::instance()->get("language");

    // set translate
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = SCC_DATA_DIR;
#endif

    // load translate
    if (qtTranslator.load(QString("%1/translations/qt_%2").arg(path, strLanguage)))
        qApp->installTranslator(&qtTranslator);

    if (sccTranslator.load(QString("%1/translations/scc_%2").arg(path, strLanguage)))
        qApp->installTranslator(&sccTranslator);
}

bool Core::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName))
    {
        QFileInfoList listInfo = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
        foreach(QFileInfo info, listInfo)
        {
            if (info.isDir())
                result = removeDir(info.absoluteFilePath());
            else
                result = QFile::remove(info.absoluteFilePath());

            if (!result)
                return result;
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
