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
#include <QFile>
#include <QSettings>
#include <QTcpSocket>
#include <QTimer>
#include "config.h"
#include "mainwindow.h"
#include "update.h"
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
    pUpdate = new Update();

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
    delete pUpdate;
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
    createSettings();

    kamerzystaSocket = new QTcpSocket();
    kamerzystaSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    kamerzystaSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);

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
    // default settings
    settings["version"] = "1.1.2.1015";
    settings["logged"] = "off";
    settings["busy"] = "off";
    settings["away"] = "off";
    settings["override"] = "off";
    settings["ignore_raw_141"] = "off";
    settings["age_check"] = "on";
    settings["first_run"] = "true";
    settings["uokey"] = "";
    settings["uo_nick"] = "";
    settings["onet_ubi"] = "";
    settings["onet_cid"] = "";
    settings["onet_sid"] = "";
    settings["onet_uid"] = "";
    settings["onetzuo_ticket"] = "";
    settings["last_active"] = "0";

    // config
    configValues();
    configProfileValues();

    // remove old config
    removeOldConfig();
    // clear old settings
    QSettings oldSettings;
    oldSettings.clear();

    // check settings
    checkSettings();
}

void Core::configValues()
{
    // config values
    Config *pConfig = new Config(false);
    QMap<QString,QString> mConfigValues = pConfig->readConfig();
    delete pConfig;

    // set settings
    QMapIterator <QString, QString> i(mConfigValues);
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
    QMap<QString,QString> mConfigProfileValues = pConfigProfile->readConfig();
    delete pConfigProfile;

    // set profile settings
    QMapIterator <QString, QString> ip(mConfigProfileValues);
    while (ip.hasNext())
    {
        ip.next();
        settings[ip.key()] = ip.value();
    }
}

void Core::removeOldConfig()
{
    QString path;
#ifdef Q_WS_WIN
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    // remove file
    if (QFile::exists(path+"/scc.conf"))
        QFile::remove(path+"/scc.conf");
}

void Core::checkSettings()
{
    QString strSoundBeep = settings.value("sound_beep");
    QString strSoundQuery = settings.value("sound_query");
    QString strBackgroundImage = settings.value("background_image");

    Config *pConfig = new Config();

    QFile fSoundBeep(strSoundBeep);
    QFile fSoundQuery(strSoundQuery);
    QFile fBackgroundImage(strBackgroundImage);

    if (!fSoundBeep.exists())
    {
        pConfig->setValue("sound_beep", "");
        settings["sound_beep"] = "";
    }
    if (!fSoundQuery.exists())
    {
        pConfig->setValue("sound_query", "");
        settings["sound_query"] = "";
    }
    if (!fBackgroundImage.exists())
    {
        pConfig->setValue("background_image", "");
        settings["background_image"] = "";
    }

    delete pConfig;
}

QString Core::version()
{
    return settings.value("version");
}

void Core::setDebug(bool b)
{
    if (b == true)
        settings["debug"] = "on";
    else
        settings["debug"] = "off";
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
void Core::refreshBackgroundImage()
{
    window->refreshBackgroundImage();
}

// update
void Core::checkUpdate()
{
    pUpdate->checkUpdate();
}

void Core::updateNickAvatar(QString strAvatar)
{
    window->updateNickAvatar(strAvatar);
}

void Core::addAwaylog(QString strChannel, QString strAwayData)
{
    if (settings.value("away") == "off")
        return;

    // fix /me
    QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01"))).arg("(.*)").arg(QString(QByteArray("\x01")));
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

    lAwaylog.append(QString("%1\n%2 %3").arg(strChannel).arg(strDT).arg(strAwayData));
}

QString Core::getChannelNameFromIndex(int index)
{
    QList<QString> lAllOpenChannels = lOpenChannels;
    lAllOpenChannels.insert(0, "Status");

    for (int i = 0; i < lAllOpenChannels.size(); i++)
    {
        QString strChannel = lAllOpenChannels.at(i);
        if (index == i)
            return strChannel;
    }
    return QString::null;
}

QString Core::getCurrentChannelName()
{
    int index = window->getCurrentTabIndex();
    return getChannelNameFromIndex(index);
}

// for avatars (if nick not in any channels -> remove avatar)
int Core::getNickChannels(QString strNick)
{
    int iResult = 0;

    for (int i = 0; i < lUsers.size(); i++)
    {
        if (lUsers.at(i).nick == strNick)
            iResult++;
    }

    // fix for self nick
    QString strMe = settings.value("nick");
    if (strNick == strMe)
        iResult = 1;

    return iResult;
}

QList<QString> Core::getNicksFromChannel(QString strChannel)
{
    QList<QString> usersList;
    for (int i = 0; i < lUsers.size(); i++)
    {
        if (lUsers.at(i).channel == strChannel)
        {
            QString strUser = lUsers.at(i).nick;
            usersList.append(strUser);
        }
    }
    return usersList;
}

QString Core::getUserModes(QString strNick, QString strChannel)
{
    for (int i = 0; i < lUsers.size(); i++)
    {
        if ((lUsers.at(i).nick == strNick) && (lUsers.at(i).channel == strChannel))
            return lUsers.at(i).modes;
    }
    return QString::null;
}

int Core::getUserMaxModes(QString strNick, QString strChannel)
{
    QString modes = getUserModes(strNick, strChannel);

    if (modes.contains("O")) { return 64; }
    if (modes.contains("o")) { return 32; }
    if (modes.contains("`")) { return 16; }
    if (modes.contains("@")) { return 8; }
    if (modes.contains("%")) { return 4; }
    if (modes.contains("!")) { return 2; }
    if (modes.contains("=")) { return 1; }
    if (modes.contains("+")) { return 0; }

    return -1;
}
