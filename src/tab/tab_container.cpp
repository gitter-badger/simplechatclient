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

#include "tab_container.h"

TabContainer::TabContainer(QWidget *parent, Network *param1, TabManager *param2, Notify *param3, QMap <QString, QByteArray> *param4, QTcpSocket *param5, InputLineDockWidget *param6, sChannelNickStatus *param7)
{
    myparent = parent;
    pNetwork = param1;
    pTabM = param2;
    pNotify = param3;
    mChannelAvatar = param4;
    camSocket = param5;
    inputlinewidget = param6;
    mChannelNickStatus = param7;
}

TabContainer::~TabContainer()
{
    for (int i = 0; i < tw.count(); i++)
    {
        QString strChannel = tw[i]->get_name();

        // remove nicklist
        emit remove_nicklist(strChannel);

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

void TabContainer::set_dlg(DlgChannelSettings *param1, DlgModeration *param2)
{
    dlgchannel_settings = param1;
    dlgmoderation = param2;
}

int TabContainer::get_index(QString strName)
{
    for (int i = 0; i < tw.count(); i++)
    {
        if (tw[i]->get_name() == strName)
            return i;
    }
    return -1;
}

bool TabContainer::exist_tab(QString strChannel)
{
    for (int i = 0; i < tw.count(); i++)
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

        // create nicklist
        emit create_nicklist(strChannel);

        // create tab
        tw.append(new TabWidget(myparent, pNetwork, strChannel, pNotify, mChannelAvatar, dlgchannel_settings, dlgmoderation, camSocket, inputlinewidget, mChannelNickStatus));
        pTabM->addTab(tw.at(tw.count()-1), strChannel);
        pTabM->setCurrentIndex(tw.count()-1);

        // update
        update_open_channels();
    }
}

void TabContainer::remove_tab(QString strChannel)
{
    if (strChannel == "Status") return;

    int i = get_index(strChannel);
    if (i != -1)
    {
        // remove nicklist
        emit remove_nicklist(strChannel);

        // remove tab
        delete tw.at(i);
        tw.removeAt(i);

        // update
        update_open_channels();

        // log
        QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
        Log *l = new Log();
        l->save(strChannel, strData);
        delete l;

        // change tab if not active
        if (i != pTabM->currentIndex())
            emit currentChanged(pTabM->currentIndex());

        return;
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

            update_open_channels();

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
    else
    {
        //pNetwork->send(QString("JOIN %1").arg(strChannel));
        add_tab(strChannel);
        show_msg(strChannel, strData, iLevel);
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
    else
    {
        //pNetwork->send(QString("JOIN %1").arg(strChannel));
        add_tab(strChannel);
        show_msg(strChannel, strData, iLevel);
    }
}

void TabContainer::show_msg_all(QString strData, int iLevel)
{
    for (int i = 0; i < tw.count(); i++)
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
    for (int i = 0; i < tw.count(); i++)
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

void TabContainer::enable_moderation(QString strChannel)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->enable_moderation();
}

void TabContainer::disable_moderation(QString strChannel)
{
    int i = get_index(strChannel);
    if (i != -1)
        tw[i]->disable_moderation();
}

void TabContainer::update_open_channels()
{
    QStringList strOpenChannels = get_open_channels();

    for (int i = 0; i < tw.count(); i++)
        tw[i]->set_open_channels(strOpenChannels);
}

void TabContainer::update_channel_avatar(QString strChannel)
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

void TabContainer::refresh_colors()
{
    for (int i = 0; i < tw.count(); i++)
        tw[i]->refresh_colors();
}

QStringList TabContainer::get_open_channels()
{
    QStringList strlResult;
    for (int i = 0; i < tw.count(); i++)
    {
        if (tw[i]->get_name() != "Status")
            strlResult.append(tw[i]->get_name());
    }
    return strlResult;
}
