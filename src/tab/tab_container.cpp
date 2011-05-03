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

#include <QDateTime>
#include <QSettings>
#include "core.h"
#include "log.h"
#include "network.h"
#include "tab_manager.h"
#include "tab_widget.h"
#include "tab_container.h"

TabContainer::TabContainer(Network *param1, TabManager *param2, QTcpSocket *param3)
{
    pNetwork = param1;
    pTabM = param2;
    camSocket = param3;
}

TabContainer::~TabContainer()
{
    for (int i = 0; i < tw.size(); i++)
    {
        QString strChannel = tw[i]->get_name();

        // clear avatars
        emit clear_channel_all_nick_avatars(strChannel);

        // clear nicklist
        emit clear_nicklist(strChannel);

        // remove from open channels
        Core::instance()->lOpenChannels.removeAll(strChannel);

        // remove from nick count
        Core::instance()->mChannelNicks.remove(strChannel);

        // remove tab
        delete tw.at(i);
        tw.removeAt(i);

        // log
        QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
        Log *l = new Log();
        l->save(strChannel, strData);
        delete l;
    }
}

void TabContainer::set_dlg(DlgUserProfile *param1)
{
    pDlgUserProfile = param1;
}

#ifndef Q_WS_WIN
#ifdef HAVE_V4L2
void TabContainer::set_dlg_cam(DlgCam *param1)
{
    pDlgCam = param1;
}
#endif
#endif

int TabContainer::get_index(QString strName)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (tw[i]->get_name() == strName)
            return i;
    }
    return -1;
}

bool TabContainer::exist_tab(QString strChannel)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (tw[i]->get_name() == strChannel)
            return true;
    }
    return false;
}

void TabContainer::add_tab(QString strChannel)
{
    if (exist_tab(strChannel) == false)
    {
        // log
        QString strData = "--- Log opened "+QDateTime::currentDateTime().toString(Qt::TextDate);
        Log *l = new Log();
        l->save(strChannel, strData);
        delete l;

        // update open channels
        if (strChannel != "Status")
            Core::instance()->lOpenChannels.append(strChannel);

        // update nick count
        Core::instance()->mChannelNicks.insert(strChannel, 0);

        // create tab
        tw.append(new TabWidget(pNetwork, strChannel, camSocket, pDlgUserProfile));
#ifndef Q_WS_WIN
#ifdef HAVE_V4L2
        tw.at(tw.size()-1)->set_dlg_cam(pDlgCam);
#endif
#endif
        pTabM->addTab(tw.at(tw.size()-1), strChannel);
        pTabM->setCurrentIndex(tw.size()-1);
    }
}

void TabContainer::remove_tab(QString strChannel)
{
    if (strChannel == "Status") return;

    int i = get_index(strChannel);
    if (i != -1)
    {
        // clear avatars
        emit clear_channel_all_nick_avatars(strChannel);

        // clear nicklist
        emit clear_nicklist(strChannel);

        // remove from open channels
        Core::instance()->lOpenChannels.removeAll(strChannel);

        // remove from nick count
        Core::instance()->mChannelNicks.remove(strChannel);

        // remove tab
        delete tw.at(i);
        tw.removeAt(i);

        // log
        QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
        Log *l = new Log();
        l->save(strChannel, strData);
        delete l;
    }
}

bool TabContainer::rename_tab(QString strChannel, QString strNewName)
{
    int i = get_index(strChannel);
    if (i != -1)
    {
        if (pTabM->tabText(i)[0] == '^')
        {
            pTabM->setTabText(i, strNewName);

            return true;
        }
    }
    return false;
}

void TabContainer::part_tab(int index)
{
    QString strChannel = tw[index]->get_name();

    if (pNetwork->is_connected() == true)
        pNetwork->send(QString("PART %1").arg(strChannel));
    else
        remove_tab(strChannel);
}

void TabContainer::show_msg(QString strTime, QString strChannel, QString strData, int iLevel)
{
    int i = get_index(strChannel);
    if (i != -1)
    {
        // hilight
        QSettings settings;
        QString strMe = settings.value("nick").toString();

        if (strData.indexOf(QRegExp("(\\s|\\W)"+strMe+"\\b")) != -1)
        {
            if (iLevel == 0)
            {
                tw[i]->display_msg(strTime, strData, 8);
                if (i != pTabM->currentIndex())
                    pTabM->set_hilight(i);
            }
            else
            {
                tw[i]->display_msg(strTime, strData, iLevel);
                if (i != pTabM->currentIndex())
                    pTabM->set_alert(i, QColor(0, 147, 0, 255)); // green
            }

            // update awaylog status
            emit update_awaylog_status();
        }
        else
        {
            tw[i]->display_msg(strTime, strData, iLevel);
            if (i != pTabM->currentIndex())
            {
                if (iLevel != 0)
                    pTabM->set_alert(i, QColor(0, 147, 0, 255)); // green
                else
                    pTabM->set_alert(i, QColor(255, 0, 0, 255)); // red
            }
        }
    }
}

void TabContainer::show_msg(QString strChannel, QString strData, int iLevel)
{
    int i = get_index(strChannel);
    if (i != -1)
    {
        // hilight
        QSettings settings;
        QString strMe = settings.value("nick").toString();

        if (strData.indexOf(QRegExp("(\\s|\\W)"+strMe+"\\b")) != -1)
        {
            if (iLevel == 0)
            {
                tw[i]->display_msg(strData, 8);
                if (i != pTabM->currentIndex())
                    pTabM->set_hilight(i);
            }
            else
            {
                tw[i]->display_msg(strData, iLevel);
                if (i != pTabM->currentIndex())
                    pTabM->set_alert(i, QColor(0, 147, 0, 255)); // green
            }

            // update awaylog status
            emit update_awaylog_status();
        }
        else
        {
            tw[i]->display_msg(strData, iLevel);
            if (i != pTabM->currentIndex())
            {
                if (iLevel != 0)
                    pTabM->set_alert(i, QColor(0, 147, 0, 255)); // green
                else
                    pTabM->set_alert(i, QColor(255, 0, 0, 255)); // red
            }
        }
    }
}

void TabContainer::show_msg_all(QString strData, int iLevel)
{
    for (int i = 0; i < tw.size(); i++)
    {
        tw[i]->display_msg(strData, iLevel);
        if (i != pTabM->currentIndex())
        {
            if (iLevel != 0)
                pTabM->set_alert(i, QColor(0, 147, 0, 255)); // green
            else
                pTabM->set_alert(i, QColor(255, 0, 0, 255)); // red
        }
    }
}

void TabContainer::show_msg_active(QString strData, int iLevel)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (i == pTabM->currentIndex())
        {
            tw[i]->display_msg(strData, iLevel);
            return;
        }
    }
}

void TabContainer::set_topic(QString strChannel, QString strTopic)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->set_topic(strTopic);
}

void TabContainer::author_topic(QString strChannel, QString strNick)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->author_topic(strNick);
}

void TabContainer::set_link(QString strChannel, QString strLink)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->set_link(strLink);
}

void TabContainer::slot_update_nick_avatar(QString strNick)
{
    emit update_nick_avatar(strNick);
}

void TabContainer::slot_update_channel_avatar(QString strChannel)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->update_channel_avatar();
}

void TabContainer::slot_show_msg(QString strChannel, QString strData, int iLevel)
{
    show_msg(strChannel, strData, iLevel);
}

void TabContainer::slot_show_msg_active(QString strData, int iLevel)
{
    show_msg_active(strData, iLevel);
}

void TabContainer::slot_show_msg_all(QString strData, int iLevel)
{
    show_msg_all(strData, iLevel);
}

void TabContainer::slot_display_message(QString strChannel, QString strData, int iLevel)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->display_message(strData, iLevel);
}

void TabContainer::slot_change_font_size(QString strSize)
{
    for (int i = 0; i < tw.size(); i++)
        tw[i]->change_font_size(strSize);
}

void TabContainer::slot_clear_content(QString strChannel)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->clear_content();
}

void TabContainer::refresh_colors()
{
    QSettings settings;
    for (int i = 0; i < tw.size(); i++)
    {
        tw[i]->refresh_colors();
        // update tab name color
        pTabM->set_color(i, QColor(settings.value("default_font_color").toString()));
    }
}

void TabContainer::refresh_background_image()
{
    for (int i = 0; i < tw.size(); i++)
        tw[i]->refresh_background_image();
}
