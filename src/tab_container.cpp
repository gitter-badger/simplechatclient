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

#include "tab_container.h"

tab_container::tab_container(tab_manager *param1, QWidget *param2, QTcpSocket *param3, QSettings *param4)
{
    tabm = param1;
    mainWin = param2;
    socket = param3;
    settings = param4;
    free_list = "ffffffffffffffffffffffffffffffffffffffffffffffffff"; // f = free  u = used
}

tab_container::~tab_container()
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            QString strChannel = tw[i]->get_name();
            tabm->removeTab(tabm->tab_pos(strChannel));
            free_list[i] = 'f';
            delete tw[i];
            tab_container::update_open_channels();
            return;
        }
    }
}

void tab_container::set_dlg(dlg_channel_settings *param1, dlg_moderation *param2)
{
    dlgchannel_settings = param1;
    dlgmoderation = param2;
}

bool tab_container::exist_tab(QString strChannel)
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

int tab_container::free_list_get()
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'f')
            return i;
    }
    return -1;
}

void tab_container::add_tab(QString strChannel,QWidget *parent)
{
    if (exist_tab(strChannel) == false)
    {
        int iFree = tab_container::free_list_get();
        if (iFree != -1)
        {
            tw[iFree] = new tab_widget(strChannel, parent, socket, settings, dlgchannel_settings, dlgmoderation);
            int iTab = tabm->addTab(tw[iFree], strChannel);
            tabm->setCurrentIndex(iTab);
            free_list[iFree] = 'u';
            tab_container::update_open_channels();
        }
        else
            tab_container::show_msg("Status", "Error: Przekroczona dopuszczalna ilo¶æ kana³ów w kliencie", 9);
    }
}

void tab_container::remove_tab(QString strChannel)
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
                    tab_container::update_open_channels();
                    return;
                }
            }
        }
    }
}

void tab_container::rename_tab(QString strChannel, QString strNewName)
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
                    tab_container::update_open_channels();
                    return;
                }
            }
        }
    }
}

void tab_container::show_msg(QString strTime, QString strChannel, QString strData, int iLevel)
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
                    config *pConfig = new config();
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
        tab_container::add_tab(strChannel, mainWin);
        tab_container::show_msg(strChannel, strData, iLevel);
    }
}

void tab_container::show_msg(QString strChannel, QString strData, int iLevel)
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
                    config *pConfig = new config();
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
        tab_container::add_tab(strChannel, mainWin);
        tab_container::show_msg(strChannel, strData, iLevel);
    }
}

void tab_container::show_msg_all(QString strData, int iLevel)
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


void tab_container::show_msg_active(QString strData, int iLevel)
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

void tab_container::set_topic(QString strChannel, QString strTopic)
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

void tab_container::enable_topic(QString strChannel)
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

void tab_container::disable_topic(QString strChannel)
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

void tab_container::author_topic(QString strChannel, QString strNick)
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

void tab_container::set_link(QString strChannel, QString strLink)
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

void tab_container::add_user(QString strChannel, QString strNick, QString strSuffix)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
                tw[i]->add_user(strNick, strSuffix);
        }
    }
}

void tab_container::del_user(QString strChannel, QString strNick)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
                tw[i]->del_user(strNick);
        }
    }
}

void tab_container::quit_user(QString strNick, QString strDisplay)
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

void tab_container::change_flag(QString strNick, QString strChannel, QString strFlag)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            if ((tw[i]->get_name() == strChannel) && (tw[i]->nicklist_exist(strNick)))
                tw[i]->change_flag(strNick, strFlag);
        }
    }
}

void tab_container::change_flag(QString strNick, QString strFlag)
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

void tab_container::update_nick(QString strNick)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
            tw[i]->update_nick(strNick);
    }
}

void tab_container::clear_nicklist(QString strChannel)
{
    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
        {
            if (tw[i]->get_name() == strChannel)
                tw[i]->clear_nicklist();
        }
    }
}

void tab_container::update_open_channels()
{
    QStringList strOpenChannels = tab_container::get_open_channels();

    for (int i = 0; i < 50; i++)
    {
        if (free_list[i] == 'u')
                tw[i]->set_open_channels(strOpenChannels);
    }
}

QStringList tab_container::get_open_channels()
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
