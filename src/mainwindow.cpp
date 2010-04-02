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

#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
// init
    toolBar = new QToolBar();
    setWindowTitle("Simple Chat Client");
    setWindowIcon(QIcon(":/3rdparty/images/r2_32.png"));

// action
    showAct = new QAction(QIcon(":/3rdparty/images/r2_32.png"),("P&oka¿"),this);
    connectAct = new QAction(QIcon(":/3rdparty/images/connect.png"),("&Po³±cz"),this);
    closeAct = new QAction(QIcon(":/3rdparty/images/exit.png"),("Za&koñcz"),this);
    optionsAct = new QAction(QIcon(":/3rdparty/images/options.png"),("O&pcje programu"),this);
    channel_list_Act = new QAction(QIcon(":/3rdparty/images/list.png"),("&Lista kana³ów"),this);
    channel_homes_Act = new QAction(QIcon(":/3rdparty/images/list.png"),("U&stawienia kana³ów"),this);
    channel_favourites_Act = new QAction(QIcon(":/3rdparty/images/list.png"),("Ul&ubione kana³y"),this);
    friendsAct = new QAction(QIcon(":/3rdparty/images/people.png"),("Przy&jaciele"),this);
    ignoreAct = new QAction(QIcon(":/3rdparty/images/people.png"),("Ignorowa&ni"),this);
    aboutAct = new QAction(QIcon(":/3rdparty/images/r2_32.png"),("&O programie SCC..."),this);

// shortcut
    connectAct->setShortcuts(QKeySequence::New);
    closeAct->setShortcut(tr("Ctrl+E"));
    optionsAct->setShortcut(tr("Ctrl+O"));
    channel_list_Act->setShortcut(tr("Ctrl+L"));
    channel_homes_Act->setShortcut(tr("Ctrl+M"));
    channel_favourites_Act->setShortcut(tr("Ctrl+U"));
    friendsAct->setShortcut(tr("Ctrl+P"));
    ignoreAct->setShortcut(tr("Ctrl+I"));

//  init all
    config *pConfig = new config();
    QString strDebugAll = pConfig->get_value("debug_all");
    QString strShowZuo = pConfig->get_value("show_zuo");
    QString strHideFormating = pConfig->get_value("hide_formating");
    QString strHideJoinPart = pConfig->get_value("hide_join_part");
    delete pConfig;

    settings.clear();
    settings.setValue("busy", "off");
    settings.setValue("override", "off");
    settings.setValue("autojoin_favourites", "on");
    settings.setValue("debug_all", strDebugAll);
    settings.setValue("show_zuo", strShowZuo);
    settings.setValue("hide_formating", strHideFormating);
    settings.setValue("hide_join_part", strHideJoinPart);

    tabm = new tab_manager(this, &settings);
    setCentralWidget(tabm);

    pHttp = new QHttp(this);
    pNetwork = new network(pHttp, connectAct, &settings);
    pSocket = pNetwork->get_socket();
    tabc = new tab_container(tabm, this, pSocket, &settings);

    dlgchannel_settings = new dlg_channel_settings(&settings, pSocket);
    dlgmoderation = new dlg_moderation(&settings, pSocket);
    dlgchannel_list = new dlg_channel_list(&settings, pSocket, tabc);
    dlgchannel_homes = new dlg_channel_homes(&settings, pSocket, tabc, dlgchannel_settings);
    dlgchannel_favourites = new dlg_channel_favourites(&settings, pSocket, tabc);
    dlgfriends = new dlg_friends(&settings, pSocket, tabc);
    dlgignore = new dlg_ignore(&settings, pSocket, tabc);
    dlgcam = new dlg_cam(&settings, pSocket);
    pIrc_auth = new irc_auth(pHttp, &settings);

    pNetwork->set_dlg(tabc, pIrc_auth, dlgchannel_settings, dlgchannel_homes, dlgchannel_list, dlgchannel_favourites, dlgfriends, dlgignore, dlgmoderation);
    tabc->set_dlg(dlgchannel_settings, dlgmoderation, dlgcam);

    tabc->show_msg("Status", "%Fi:courier%Witaj w programie Simple Chat Client %Ixhehe%", 0);
    tabc->show_msg("Status", "%Fb:courier%%C008100%Oficjalna strona SCC%C000000%: http://simplechatclien.sourceforge.net/ %Ixluzak%", 0);
    tabc->show_msg("Status", "%Fbi:courier%%Cff0000%Lista b³êdów%C000000%: http://tinyurl.com/yg3fjb4 %Ixmm%", 0);
    tabc->show_msg("Status", "%Fbi:courier%%C8800ab%Zg³aszanie b³êdów%C000000%: http://tinyurl.com/yfcseeh %Ixco%", 0);

// main menu
    fileMenu = menuBar()->addMenu("&Plik");
    fileMenu->addAction(connectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);

    optionsMenu = menuBar()->addMenu("&Ustawienia");
    optionsMenu->addAction(optionsAct);

    chatMenu = menuBar()->addMenu("&Czat");
    chatMenu->addAction(channel_list_Act);
    chatMenu->addAction(channel_homes_Act);
    chatMenu->addAction(channel_favourites_Act);
    chatMenu->addAction(friendsAct);
    chatMenu->addAction(ignoreAct);

    helpMenu = menuBar()->addMenu("Pomo&c");
    helpMenu->addAction(aboutAct);

// toolbar
    toolBar = addToolBar("Pasek nawigacji");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(connectAct);
    toolBar->addAction(optionsAct);
    toolBar->addAction(channel_list_Act);
    toolBar->addAction(channel_homes_Act);
    toolBar->addAction(friendsAct);

// statusbar
    statusBar()->showMessage("Gotowy");

// signals
    QObject::connect(connectAct, SIGNAL(triggered()), this, SLOT(button_connect()));
    QObject::connect(optionsAct, SIGNAL(triggered()), this, SLOT(options_dlg()));
    QObject::connect(channel_list_Act, SIGNAL(triggered()), this, SLOT(channel_list_dlg()));
    QObject::connect(channel_homes_Act, SIGNAL(triggered()), this, SLOT(channel_homes_dlg()));
    QObject::connect(channel_favourites_Act, SIGNAL(triggered()), this, SLOT(channel_favourites_dlg()));
    QObject::connect(friendsAct, SIGNAL(triggered()), this, SLOT(friends_dlg()));
    QObject::connect(ignoreAct, SIGNAL(triggered()), this, SLOT(ignore_dlg()));
    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(about_dlg()));
    QObject::connect(showAct, SIGNAL(triggered()), this, SLOT(button_show()));
    QObject::connect(closeAct, SIGNAL(triggered()), this, SLOT(button_close()));
    QObject::connect(tabm, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
    QObject::connect(pHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(http_request_finished()));

// tray
    trayMenu = new QMenu();
    trayMenu->addAction(showAct);
    trayMenu->addAction(closeAct);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/3rdparty/images/r2_32.png"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_icon(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
    trayIcon->hide();
    settings.setValue("reconnect", "false");
    delete pIrc_auth;
    delete dlgcam;
    delete dlgignore;
    delete dlgfriends;
    delete dlgchannel_favourites;
    delete dlgchannel_homes;
    delete dlgchannel_list;
    delete dlgmoderation;
    delete dlgchannel_settings;
    delete tabc;
    delete pNetwork;
    delete pHttp;
    delete tabm;
}

void MainWindow::set_debug(bool param1)
{
    if (param1 == true)
        settings.setValue("debug", "on");
    else
        settings.setValue("debug", "off");
}

void MainWindow::button_close()
{
    this->close();
}

void MainWindow::button_connect()
{
    config *pConfig = new config();

    QString strNick = pConfig->get_value("login-nick");
    QString strPass = pConfig->get_value("login-pass");

    // nick & pass is null
    if ((strNick.isEmpty() == true) && (strPass.isEmpty() == true))
    {
        pConfig->set_value("login-nick", "~test");
        strNick = "~test";
    }

    // decrypt pass
    if (strPass.isEmpty() == false)
    {
        qcrypt *pCrypt = new qcrypt();
        strPass = pCrypt->decrypt(strNick, strPass);
        delete pCrypt;
    }

    // correct nick
    if ((strPass.isEmpty() == true) && (strNick[0] != '~'))
    {
        strNick = "~"+strNick;
        pConfig->set_value("login-nick", strNick);
    }
    if ((strPass.isEmpty() == false) && (strNick[0] == '~'))
    {
        strNick = strNick.right(strNick.length()-1);
        pConfig->set_value("login-nick", strNick);
    }

    delete pConfig;

    // update nick
    tabc->update_nick(strNick);

    // connect
    if (pNetwork->is_connected() == false)
    {
        connectAct->setText("&Roz³±cz");
        connectAct->setIconText("&Roz³±cz");
        pNetwork->set_hostport("czat-app.onet.pl",5015);
        settings.setValue("reconnect", "true");
        pNetwork->connect();
        if (pNetwork->is_connected() == true)
        {
            pNetwork->send(QString("NICK %1").arg(strNick));
            pNetwork->send("AUTHKEY");
            QString strNickCurrent = strNick;
            if (strNickCurrent[0] == '~')
                strNickCurrent = strNick.right(strNick.length()-1);
            pIrc_auth->request_uo(strNickCurrent, strPass);
        }
    }
    else
    {
        settings.setValue("reconnect", "false");
        pNetwork->send("QUIT");
        connectAct->setText("&Po³±cz");
        connectAct->setIconText("&Po³±cz");
        pNetwork->close();
        // update nick
        tabc->update_nick("(niezalogowany)");
    }
}

void MainWindow::options_dlg()
{
    (new dlg_options(&settings))->show();
}

void MainWindow::channel_list_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        dlgchannel_list->show();
}

void MainWindow::channel_homes_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        dlgchannel_homes->show();
}

void MainWindow::channel_favourites_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        dlgchannel_favourites->show();
}

void MainWindow::friends_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        dlgfriends->show();
}

void MainWindow::ignore_dlg()
{
    if ((pNetwork->is_connected() == true) && (pNetwork->is_writable() == true))
        dlgignore->show();
}

void MainWindow::about_dlg()
{
    (new dlg_about)->show();
}

// tray
void MainWindow::tray_icon(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::Trigger)
    {
        if (this->isVisible() == true)
            this->hide();
        else
            this->showNormal();
    }
}

// tray button
void MainWindow::button_show()
{
    this->showNormal();
}

// close tab
void MainWindow::tab_close_requested(int index)
{
    QString strName = tabm->tabText(index);
    if (strName != "Status")
    {
        if (strName[0] == '#')
        {
            tabc->remove_tab(strName);
            if (pNetwork->is_connected() == true)
                pNetwork->send(QString("PART %1").arg(strName));
        }
        else
        {
            if (tabc->exist_tab(strName) == false)
            {
                QString strNewName = settings.value("priv"+strName).toString();
                if (strNewName.isEmpty() == false)
                {
                    if (pNetwork->is_connected() == true)
                        pNetwork->send(QString("PART %1").arg(strNewName));
                }
            }
            else
            {
                if (pNetwork->is_connected() == true)
                    pNetwork->send(QString("PART %1").arg(strName));
            }
        }
    }
}

// http
void MainWindow::http_request_finished()
{
    QByteArray ba = pHttp->readAll();
    QString strData = ba;
    if (strData.isEmpty() == false)
    {
        // <?xml version="1.0" encoding="ISO-8859-2"?><root><uoKey>LY9j2sXwio0G_yo3PdpukDL8iZJGHXKs</uoKey><zuoUsername>~Succubi_test</zuoUsername><error err_code="TRUE"  err_text="warto¶æ prawdziwa" ></error></root>
        if (strData.indexOf("uoKey") != -1)
        {
            if (strData.indexOf("err_code=\"TRUE\"") != -1)
            {
                QString strUOKey = strData.mid(strData.indexOf("<uoKey>")+7, strData.indexOf("</uoKey>") - strData.indexOf("<uoKey>") -7);
                QString strNick = strData.mid(strData.indexOf("<zuoUsername>")+13, strData.indexOf("</zuoUsername>") - strData.indexOf("<zuoUsername>") -13);
                settings.setValue("uokey", strUOKey);
                if ((strUOKey.isEmpty() == false) && (strNick.isEmpty() == false))
                    pNetwork->send(QString("USER * %1  czat-app.onet.pl :%2").arg(strUOKey).arg(strNick));
                return;
            }
            else
            {
                tabc->show_msg("Status","Error: B³±d autoryzacji.", 9);
                pNetwork->close();
                return;
            }
        }
        // <?xml version="1.0" encoding="ISO-8859-2"?><root><error err_code="-2"  err_text="U.ytkownik nie zalogowany" ></error></root>
        else if (strData.indexOf("error err_code=") != -1)
        {
            if (strData.indexOf("err_code=\"TRUE\"") != -1)
            {
                tabc->show_msg("Status","Error: B³±d autoryzacji [Nick jest ju¿ zalogowany na czacie]", 9);
            }
            else
            {
                QString strReason = strData.mid(strData.indexOf("err_text=\"")+10, strData.indexOf("\" ></error>") - strData.indexOf("err_text=\"") -10);
                if (strReason.length() > 100) strReason = "Unknown error";
                tabc->show_msg("Status", QString("Error: B³±d autoryzacji [%1]").arg(strReason), 9);
            }
            pNetwork->close();

//          raw 433 -> reconnect
            if (strData.indexOf("err_code=\"TRUE\"") != -1)
                MainWindow::button_connect();

            return;
        }
    }
}
