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

TabContainer::TabContainer(QWidget *parent, Network *param1, QSettings *param2, TabManager *param3, Notify *param4, QMap <QString, QByteArray> *param5, QMap <QString, QByteArray> *param6, QTcpSocket *param7)
{
    myparent = parent;
    pNetwork = param1;
    settings = param2;
    tabm = param3;
    pNotify = param4;
    mNickAvatar = param5;
    mChannelAvatar = param6;
    camSocket = param7;
    strFreeUsedList = "ffffffffffffffffffffffffffffffffffffffffffffffffff"; // f = free  u = used
}

TabContainer::~TabContainer()
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            QString strChannel = tw[i]->get_name();

            // log
            QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
            Log *l = new Log();
            l->save(strChannel, strData);
            delete l;

            // remove
            int iTab = tabm->tab_pos(strChannel);
            if (iTab != -1)
                tabm->removeTab(iTab);
            strFreeUsedList[i] = 'f';
            delete tw[i];
        }
    }
}

void TabContainer::set_dlg(DlgChannelSettings *param1, DlgModeration *param2)
{
    dlgchannel_settings = param1;
    dlgmoderation = param2;
}

bool TabContainer::exist_tab(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
                return true;
        }
    }
    return false;
}

int TabContainer::free_list_get()
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'f')
            return i;
    }
    return -1;
}

void TabContainer::add_tab(QString strChannel)
{
    if (exist_tab(strChannel) == false)
    {
        int iFree = free_list_get();
        if (iFree != -1)
        {
            // log
            QString strData = "--- Log opened "+QDateTime::currentDateTime().toString(Qt::TextDate);
            Log *l = new Log();
            l->save(strChannel, strData);
            delete l;

            // create
            tw[iFree] = new TabWidget(myparent, pNetwork, settings, strChannel, pNotify, mNickAvatar, mChannelAvatar, dlgchannel_settings, dlgmoderation, camSocket);
            int iTab = tabm->addTab(tw[iFree], strChannel);
            strFreeUsedList[iFree] = 'u';
            tabm->setCurrentIndex(iTab);
            create_channel_names();
            update_open_channels();
        }
        else
            show_msg("Status", tr("Error: Exceeded allowed number of channels in the client"), 9);
    }
}

void TabContainer::remove_tab(QString strChannel)
{
    if (strChannel == "Status") return;

    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    // log
                    QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
                    Log *l = new Log();
                    l->save(strChannel, strData);
                    delete l;

                    // remove
                    strChannel = tw[i]->get_name();
                    strFreeUsedList[i] = 'f';
                    int iTab = tabm->tab_pos(strChannel);
                    if (iTab != -1)
                        tabm->removeTab(iTab);
                    delete tw[i];
                    create_channel_names();
                    update_open_channels();
                    return;
                }
            }
        }
    }
}

void TabContainer::rename_tab(QString strChannel, QString strNewName)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    int iTab = tabm->tab_pos(strChannel);
                    if (iTab != -1)
                    {
                        tabm->setTabText(iTab, strNewName);
                        update_open_channels();
                    }
                    return;
                }
            }
        }
    }
}

void TabContainer::show_msg(QString strTime, QString strChannel, QString strData, int iLevel)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    strChannel = tw[i]->get_name();

                    // hilight
                    Config *pConfig = new Config();
                    QString strMe = pConfig->get_value("login-nick");
                    delete pConfig;

                    if (strData.indexOf(QRegExp("(\\s|\\W)"+strMe+"\\b")) != -1)
                    {
                        if (iLevel == 0)
                        {
                            tw[i]->display_msg(strTime, strData, 8);
                            if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                                tabm->set_hilight(strChannel);
                        }
                        else
                        {
                            tw[i]->display_msg(strTime, strData, iLevel);
                            if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                                tabm->set_alert(strChannel, QColor(0, 147, 0, 255)); // green
                        }
                    }
                    else
                    {
                        tw[i]->display_msg(strTime, strData, iLevel);
                        if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                        {
                            if (iLevel != 0)
                                tabm->set_alert(strChannel, QColor(0, 147, 0, 255)); // green
                            else
                                tabm->set_alert(strChannel, QColor(255, 0, 0, 255)); // red
                        }
                    }

                    return;
                }
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
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    strChannel = tw[i]->get_name();

                    // hilight
                    Config *pConfig = new Config();
                    QString strMe = pConfig->get_value("login-nick");
                    delete pConfig;

                    if (strData.indexOf(QRegExp("(\\s|\\W)"+strMe+"\\b")) != -1)
                    {
                        if (iLevel == 0)
                        {
                            tw[i]->display_msg(strData, 8);
                            if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                                tabm->set_hilight(strChannel);
                        }
                        else
                        {
                            tw[i]->display_msg(strData, iLevel);
                            if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                                tabm->set_alert(strChannel, QColor(0, 147, 0, 255)); // green
                        }
                    }
                    else
                    {
                        tw[i]->display_msg(strData, iLevel);
                        if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                        {
                            if (iLevel != 0)
                                tabm->set_alert(strChannel, QColor(0, 147, 0, 255)); // green
                            else
                                tabm->set_alert(strChannel, QColor(255, 0, 0, 255)); // red
                        }
                    }

                    return;
                }
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
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            tw[i]->display_msg(strData, iLevel);
            if (tabm->tab_pos(tw[i]->get_name()) != tabm->currentIndex())
            {
                if (iLevel != 0)
                    tabm->set_alert(tw[i]->get_name(), QColor(0, 147, 0, 255)); // green
                else
                    tabm->set_alert(tw[i]->get_name(), QColor(255, 0, 0, 255)); // red
            }
        }
    }
}

void TabContainer::show_msg_active(QString strData, int iLevel)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tabm->tab_pos(tw[i]->get_name()) == tabm->currentIndex())
            {
                tw[i]->display_msg(strData, iLevel);
                return;
            }
        }
    }
}

void TabContainer::set_topic(QString strChannel, QString strTopic)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    tw[i]->set_topic(strTopic);
                    return;
                }
            }
        }
    }
}

void TabContainer::enable_topic(QString strChannel)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    tw[i]->enable_topic();
                    return;
                }
            }
        }
    }
}

void TabContainer::disable_topic(QString strChannel)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    tw[i]->disable_topic();
                    return;
                }
            }
        }
    }
}

void TabContainer::author_topic(QString strChannel, QString strNick)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    tw[i]->author_topic(strNick);
                    return;
                }
            }
        }
    }
}

void TabContainer::set_link(QString strChannel, QString strLink)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (strFreeUsedList[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    tw[i]->set_link(strLink);
                    return;
                }
            }
        }
    }
}

void TabContainer::add_user(QString strChannel, QString strNick, QString strSuffix, int iRefresh)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
            {
                tw[i]->add_user(strNick, strSuffix, iRefresh);
                return;
            }
        }
    }
}

void TabContainer::del_user(QString strChannel, QString strNick)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
            {
                tw[i]->del_user(strNick);
                return;
            }
        }
    }
}

void TabContainer::quit_user(QString strNick, QString strDisplay)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->nicklist_exist(strNick) == true)
            {
                int iLevel = 3;
                tw[i]->display_msg(strDisplay, iLevel);
                tw[i]->del_user(strNick);

                if (tabm->tab_pos(tw[i]->get_name()) != tabm->currentIndex())
                    tabm->set_alert(tw[i]->get_name(), QColor(0, 147, 0, 255)); // green
            }
        }
    }
}

void TabContainer::change_flag(QString strNick, QString strChannel, QString strFlag)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if ((tw[i]->get_name() == strChannel) && (tw[i]->nicklist_exist(strNick) == true))
            {
                tw[i]->change_flag(strNick, strFlag);
                return;
            }
        }
    }
}

void TabContainer::change_flag(QString strNick, QString strFlag)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->nicklist_exist(strNick) == true)
                tw[i]->change_flag(strNick, strFlag);
        }
    }
}

void TabContainer::update_nick(QString strNick)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
            tw[i]->update_nick(strNick);
    }
}

void TabContainer::clear_nicklist(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
            {
                tw[i]->clear_nicklist();
                return;
            }
        }
    }
}

void TabContainer::clear_all_nicklist()
{
    mNickAvatar->clear();
    mChannelAvatar->clear();

    QStringList strlOpenChannels = this->get_open_channels();
    for (int i = 0; i < strlOpenChannels.size(); i++)
        this->clear_nicklist(strlOpenChannels[i]);
}

void TabContainer::clear_channel_all_nick_avatars(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
            {
                QStringList strlList = tw[i]->get_nicklist();

                for (int i = 0; i < strlList.count(); i++)
                {
                    QString strNick = strlList.at(i);

                    // remove nick from avatars if not exist on open channels; must be 1 (current channel)
                    if ((mNickAvatar->contains(strNick) == true) && (get_nick_channels(strNick) == 1))
                        mNickAvatar->remove(strNick);
                }

                return;
            }
        }
    }
}

void TabContainer::refresh_nicklist(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
            {
                tw[i]->nicklist_refresh_all();
                return;
            }
        }
    }
}

void TabContainer::update_open_channels()
{
    QStringList strOpenChannels = get_open_channels();

    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
                tw[i]->set_open_channels(strOpenChannels);
    }
}

void TabContainer::update_nick_avatar(QString strNick)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->nicklist_exist(strNick) == true)
            {
                tw[i]->update_nick_avatar();
                return;
            }
        }
    }
}

void TabContainer::update_channel_avatar(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
            {
                tw[i]->update_channel_avatar();
                return;
            }
        }
    }
}

void TabContainer::set_user_info(QString strNick, QString strKey, QString strValue)
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->nicklist_exist(strNick) == true)
                tw[i]->set_user_info(strNick, strKey, strValue);
        }
    }
}

void TabContainer::sshow_msg(QString strChannel, QString strData, int iLevel)
{
    show_msg(strChannel, strData, iLevel);
}

void TabContainer::refresh_colors()
{
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
            tw[i]->refresh_colors();
    }
}

void TabContainer::create_channel_names()
{
    QStringList strlChannelNames;

    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
            strlChannelNames.append(tw[i]->get_name());
    }

    settings->setValue("channel_names", strlChannelNames);
}

QStringList TabContainer::get_open_channels()
{
    QStringList strlResult;
    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->get_name() != "Status")
                strlResult.append(tw[i]->get_name());
        }
    }
    return strlResult;
}

int TabContainer::get_nick_channels(QString strNick)
{
    int iResult = 0;

    for (int i = 0; i < 50; i++)
    {
        if (strFreeUsedList[i] == 'u')
        {
            if (tw[i]->nicklist_exist(strNick) == true)
                iResult++;
        }
    }

    return iResult;
}
