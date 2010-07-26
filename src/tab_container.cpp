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

TabContainer::TabContainer(Network *param1, QSettings *param2, TabManager *param3, QWidget *param4, Notify *param5, QMap <QString, QByteArray> *param6, QMap <QString, QByteArray> *param7)
{
    pNetwork = param1;
    settings = param2;
    tabm = param3;
    mainWin = param4;
    pNotify = param5;
    mNickAvatar = param6;
    mChannelAvatar = param7;
    free_list = "ffffffffffffffffffffffffffffffffffffffffffffffffff"; // f = free  u = used
}

TabContainer::~TabContainer()
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            QString strChannel = tw[i]->get_name();
            tabm->removeTab(tabm->tab_pos(strChannel));
            free_list[i] = 'f';
            delete tw[i];
            update_open_channels();
            return;
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
        if (free_list[i] == 'u')
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
        if (free_list[i] == 'f')
            return i;
    }
    return -1;
}

void TabContainer::add_tab(QString strChannel,QWidget *parent)
{
    if (exist_tab(strChannel) == false)
    {
        int iFree = free_list_get();
        if (iFree != -1)
        {
            tw[iFree] = new TabWidget(pNetwork, settings, strChannel, parent, pNotify, mNickAvatar, mChannelAvatar, dlgchannel_settings, dlgmoderation);
            int iTab = tabm->addTab(tw[iFree], strChannel);
            tabm->setCurrentIndex(iTab);
            free_list[iFree] = 'u';
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
            if (free_list[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    strChannel = tw[i]->get_name();
                    tabm->removeTab(tabm->tab_pos(strChannel));
                    free_list[i] = 'f';
                    delete tw[i];
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
            if (free_list[i] == 'u')
            {
                if (tw[i]->get_name() == strChannel)
                {
                    tabm->setTabText(tabm->tab_pos(strChannel), strNewName);
                    settings->setValue("priv"+strNewName, strChannel);
                    update_open_channels();
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
            if (free_list[i] == 'u')
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
                                tabm->set_alert(strChannel);
                        }
                    }
                    else
                    {
                        tw[i]->display_msg(strTime, strData, iLevel);
                        if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                            tabm->set_alert(strChannel);
                    }

                    return;
                }
            }
        }
    }
    else
    {
        pNetwork->send(QString("JOIN %1").arg(strChannel));
        add_tab(strChannel, mainWin);
        show_msg(strChannel, strData, iLevel);
    }
}

void TabContainer::show_msg(QString strChannel, QString strData, int iLevel)
{
    if (exist_tab(strChannel) == true)
    {
        for (int i = 0; i < 50; i++)
        {
            if (free_list[i] == 'u')
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
                                tabm->set_alert(strChannel);
                        }
                    }
                    else
                    {
                        tw[i]->display_msg(strData, iLevel);
                        if (tabm->tab_pos(strChannel) != tabm->currentIndex())
                            tabm->set_alert(strChannel);
                    }

                    return;
                }
            }
        }
    }
    else
    {
        pNetwork->send(QString("JOIN %1").arg(strChannel));
        add_tab(strChannel, mainWin);
        show_msg(strChannel, strData, iLevel);
    }
}

void TabContainer::show_msg_all(QString strData, int iLevel)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            tw[i]->display_msg(strData, iLevel);
            if (tabm->tab_pos(tw[i]->get_name()) != tabm->currentIndex())
                tabm->set_alert(tw[i]->get_name());
        }
    }
}


void TabContainer::show_msg_active(QString strData, int iLevel)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
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
            if (free_list[i] == 'u')
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
            if (free_list[i] == 'u')
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
            if (free_list[i] == 'u')
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
            if (free_list[i] == 'u')
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
            if (free_list[i] == 'u')
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
        if (free_list[i] == 'u')
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
        if (free_list[i] == 'u')
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
        if (free_list[i] == 'u')
        {
            if (tw[i]->nicklist_exist(strNick) == true)
            {
                tw[i]->display_msg(strDisplay, 3);
                tw[i]->del_user(strNick);
            }
        }
    }
}

void TabContainer::change_flag(QString strNick, QString strChannel, QString strFlag)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
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
        if (free_list[i] == 'u')
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
        if (free_list[i] == 'u')
            tw[i]->update_nick(strNick);
    }
}

void TabContainer::clear_nicklist(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
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

void TabContainer::refresh_nicklist(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
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
        if (free_list[i] == 'u')
                tw[i]->set_open_channels(strOpenChannels);
    }
}

void TabContainer::update_nick_avatar(QString strNick)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
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
        if (free_list[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
            {
                tw[i]->update_channel_avatar();
                return;
            }
        }
    }
}

void TabContainer::sshow_msg(QString strChannel, QString strData, int iLevel)
{
    show_msg(strChannel, strData, iLevel);
}

QStringList TabContainer::get_open_channels()
{
    QStringList strlResult;
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            if (tw[i]->get_name() != "Status")
                strlResult.append(tw[i]->get_name());
        }
    }
    return strlResult;
}
