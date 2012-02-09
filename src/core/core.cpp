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

#include <QAction>
#include <QDateTime>
#include <QDir>
#include <QSettings>
#include <QTcpSocket>
#include <QTimer>
#include "config.h"
#include "log.h"
#include "mainwindow.h"
#include "updates.h"
#include "core.h"

#ifdef Q_WS_WIN
    #include <QCoreApplication>
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
    pUpdates = new Updates();

    // lag
    lagAct = new QAction("Lag: ?", this);
    lagAct->setEnabled(false);

    busyAct = new QAction(this);
    busyAct->setIcon(QIcon(":/images/oxygen/16x16/im-user-offline.png"));
    awayAct = new QAction(this);
    awayAct->setIcon(QIcon(":/images/oxygen/16x16/im-user-away.png"));

    // checkable
    busyAct->setCheckable(true);
    awayAct->setCheckable(true);
    busyAct->setChecked(false);
    awayAct->setChecked(false);
}

Core::~Core()
{
    delete pUpdates;
    delete window;
    window = 0;

    kamerzystaSocket->deleteLater();

    // close network
    settings["reconnect"] = "false";
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

    // read empty user avatar
    readEmptyUserAvatar();

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

void Core::createGui()
{
    busyAct->setText(tr("Mark as busy"));
    awayAct->setText(tr("Mark as away"));

    window = new MainWindow(0);
}

void Core::createSettings()
{
    // debug
    QString strDebug = settings["debug"];
    settings.clear();
    settings["debug"] = strDebug;

    // default settings
    settings["version"] = "1.1.4.1167";
    settings["available_version"] = "";
    settings["logged"] = "false";
    settings["busy"] = "false";
    settings["away"] = "false";
    settings["override"] = "false";
    settings["ignore_raw_141"] = "false";
    settings["age_check"] = "true";
    settings["first_run"] = "true";
    settings["uokey"] = "";
    settings["uo_nick"] = "";
    settings["onet_ubi"] = "";
    settings["onet_cid"] = "";
    settings["onet_sid"] = "";
    settings["onet_uid"] = "";
    settings["onetzuo_ticket"] = "";
    settings["last_active"] = "0";
    settings["reconnect"] = "true";

    // read config
    configValues();

    // convert old config profile
    convertOldConfig();

    // read config profile
    configProfileValues();

    // check settings
    checkSettings();
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

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

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
    for (int i = 0; i < list.size(); ++i)
    {
        QString profile = list.at(i).fileName().remove(".xml");
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
    QHash<QString,QString> mConfigValues = pConfig->readConfig();

    QHashIterator <QString, QString> i(mConfigValues);
    while (i.hasNext())
    {
        i.next();

        if (i.value() == "on")
            pConfig->setValue(i.key(), "true");
        else if (i.value() == "off")
            pConfig->setValue(i.key(), "false");
   }

    delete pConfig;
}

void Core::configValues()
{
    // config values
    Config *pConfig = new Config(false);
    QHash<QString,QString> mConfigValues = pConfig->readConfig();
    delete pConfig;

    // set settings
    QHashIterator <QString, QString> i(mConfigValues);
    while (i.hasNext())
    {
        i.next();
        settings[i.key()] = i.value();
    }
}

void Core::configProfileValues()
{
    // config profile values
    Config *pConfigProfile = new Config();
    QHash<QString,QString> mConfigProfileValues = pConfigProfile->readConfig();
    delete pConfigProfile;

    // set profile settings
    QHashIterator <QString, QString> ip(mConfigProfileValues);
    while (ip.hasNext())
    {
        ip.next();
        settings[ip.key()] = ip.value();
    }
}

void Core::checkSettings()
{
    QString strSoundBeep = settings.value("sound_beep");
    QString strSoundQuery = settings.value("sound_query");
    QString strBackgroundImage = settings.value("background_image");

    Config *pConfig = new Config();

    if (!QFile::exists(strSoundBeep))
    {
        pConfig->setValue("sound_beep", "");
        settings["sound_beep"] = "";
    }
    if (!QFile::exists(strSoundQuery))
    {
        pConfig->setValue("sound_query", "");
        settings["sound_query"] = "";
    }
    if (!QFile::exists(strBackgroundImage))
    {
        pConfig->setValue("background_image", "");
        settings["background_image"] = "";
    }
    if ((settings.value("themes") != "Standard") && (settings.value("themes") != "Origin") && (settings.value("themes") != "Alhena") && (settings.value("themes") != "Adara"))
    {
        pConfig->setValue("themes", "Standard");
        settings["themes"] = "Standard";
    }

    QStringList lHighlight;
    lHighlight << "&" << "<" << ">" << "\"" << "'" << "\\";
    QStringListIterator lHighlightIterator(lHighlight);
    while (lHighlightIterator.hasNext())
    {
        QString strChar = lHighlightIterator.next();

        if (settings["highlight"].contains(strChar))
        {
            settings["highlight"].remove(strChar);
            pConfig->setValue("highlight", settings["highlight"]);
        }
    }

    delete pConfig;
}

void Core::readEmptyUserAvatar()
{
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = SCC_DATA_DIR;
#endif

    strEmptyUserAvatarPath = path+"/images/user_avatar.png";
}

QString Core::version()
{
    return settings.value("version");
}

void Core::setDebug(bool b)
{
    if (b == true)
        settings["debug"] = "true";
    else
        settings["debug"] = "false";
}

void Core::showSccWindow()
{
    window->show();
}

MainWindow *Core::sccWindow()
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

// from options
void Core::refreshColors()
{
    window->refreshColors();
}

// from options
void Core::refreshCSS()
{
    window->refreshCSS();
}

// from options
void Core::refreshToolWidgetValues()
{
    window->refreshToolWidgetValues();
}

// update
void Core::checkUpdate()
{
    pUpdates->checkUpdate();
}

void Core::addAwaylog(QString strTime, QString strChannel, QString strAwayData)
{
    if (settings.value("away") == "false")
        return;

    // save awaylog
    if (Core::instance()->settings.value("disable_logs") == "false")
    {
        QDateTime dt;
        if (!strTime.isEmpty())
            dt = QDateTime::fromTime_t(strTime.toInt());
        else
            dt = QDateTime::currentDateTime();

        QString strAwaylogFileData = QString("%1 %2 %3").arg(dt.toString("[dd/MM/yyyy] [hh:mm:ss]"), strChannel, strAwayData);
        Log::save("awaylog", strAwaylogFileData);
    }

    // fix /me
    QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01")), "(.*)", QString(QByteArray("\x01")));
    if (strAwayData.contains(QRegExp(strRegExpMe)))
    {
        strAwayData.replace(QRegExp(strRegExpMe), "\\1");
        if (strAwayData.contains("<")) strAwayData = strAwayData.remove(strAwayData.indexOf("<"),1);
        if (strAwayData.contains(">")) strAwayData = strAwayData.remove(strAwayData.indexOf(">"),1);
        strAwayData = "*"+strAwayData;
    }

    // remove color, font, emots
    strAwayData.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
    strAwayData.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
    strAwayData.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

    QString strDT = QDateTime::currentDateTime().toString("[hh:mm:ss]");

    lAwaylog.append(QString("%1\n%2 %3").arg(strChannel, strDT, strAwayData));
}

QString Core::getChannelNameFromIndex(int index)
{
    if (index < lOpenChannels.size())
        return lOpenChannels.at(index);
    else
        return QString::null;
}

int Core::getIndexFromChannelName(QString strChannel)
{
    for (int i = 0; i < lOpenChannels.size(); i++)
    {
        QString strOpenChannel = lOpenChannels.at(i);
        if (strOpenChannel == strChannel)
            return i;
    }

    return -1;
}

QString Core::getCurrentChannelName()
{
    int index = window->getCurrentTabIndex();
    return getChannelNameFromIndex(index);
}

// InputLineWidget::event
QList<QString> Core::getUserListFromChannel(QString strChannel)
{
    return window->getUserList(strChannel);
}

QString Core::getUserModes(QString strNick, QString strChannel)
{
    return window->getUserModes(strNick, strChannel);
}

int Core::getUserMaxModes(QString strModes)
{
    if (strModes.contains("O")) { return 64; }
    if (strModes.contains("o")) { return 32; }
    if (strModes.contains("`")) { return 16; }
    if (strModes.contains("@")) { return 8; }
    if (strModes.contains("%")) { return 4; }
    if (strModes.contains("!")) { return 2; }
    if (strModes.contains("=")) { return 1; }
    if (strModes.contains("+")) { return 0; }

    return -1;
}

// ChatView::displayMessage
int Core::getUserCount(QString strChannel)
{
    return window->getUserCount(strChannel);
}

// DlgModeration::buttonAccept
void Core::showMessage(QString strChannel, QString strData, MessageCategory eMessageCategory, QString strTime, QString strNick)
{
    window->showMessage(strChannel, strData, eMessageCategory, strTime, strNick);
}

QString Core::getUserAvatarPath(QString strNick)
{
    return window->getUserAvatarPath(strNick);
}

QString Core::convertPrivName(QString strChannel)
{
    if (mPrivNames.contains(strChannel))
        strChannel = mPrivNames.value(strChannel);

    return strChannel;
}

bool Core::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName))
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
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
