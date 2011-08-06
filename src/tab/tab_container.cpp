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
#include "core.h"
#include "log.h"
#include "network.h"
#include "tab_manager.h"
#include "tab_widget.h"
#include "tab_container.h"

TabContainer::TabContainer(Network *param1, TabManager *param2)
{
    pNetwork = param1;
    pTabM = param2;
}

TabContainer::~TabContainer()
{
    for (int i = 0; i < tw.size(); i++)
    {
        QString strChannel = tw[i]->getName();

        // clear avatars
        emit clearChannelAllNickAvatars(strChannel);

        // clear nicklist
        emit clearNicklist(strChannel);

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

void TabContainer::setDlg(DlgUserProfile *param1)
{
    pDlgUserProfile = param1;
}

int TabContainer::getIndex(QString strName)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (tw[i]->getName() == strName)
            return i;
    }
    return -1;
}

bool TabContainer::existTab(QString strChannel)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (tw[i]->getName() == strChannel)
            return true;
    }
    return false;
}

void TabContainer::addTab(QString strChannel)
{
    if (!existTab(strChannel))
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
        tw.append(new TabWidget(pNetwork, strChannel, pDlgUserProfile));
        pTabM->addTab(tw.at(tw.size()-1), strChannel);
        pTabM->setCurrentIndex(tw.size()-1);
    }
}

void TabContainer::removeTab(QString strChannel)
{
    if (strChannel == "Status") return;

    int i = getIndex(strChannel);
    if (i != -1)
    {
        // clear avatars
        emit clearChannelAllNickAvatars(strChannel);

        // clear nicklist
        emit clearNicklist(strChannel);

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

bool TabContainer::renameTab(QString strChannel, QString strNewName)
{
    int i = getIndex(strChannel);
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

void TabContainer::partTab(int index)
{
    QString strChannel = tw[index]->getName();

    if (pNetwork->isConnected())
        pNetwork->send(QString("PART %1").arg(strChannel));
    else
        removeTab(strChannel);
}

void TabContainer::showMsg(QString &strTime, QString &strChannel, QString &strData, MessageCategory eMessageCategory)
{
    int i = getIndex(strChannel);
    if (i != -1)
    {
        // hilight
        QString strMe = Core::instance()->settings.value("nick");

        if (strData.contains(QRegExp("(\\s|\\W)"+strMe+"\\b")))
        {
            if (eMessageCategory == DefaultMessage)
            {
                tw[i]->displayMsg(strTime, strData, HilightMessage);
                if (i != pTabM->currentIndex())
                    pTabM->setHilight(i);
            }
            else
            {
                tw[i]->displayMsg(strTime, strData, eMessageCategory);
                if (i != pTabM->currentIndex())
                    pTabM->setAlert(i, QColor(0, 147, 0, 255)); // green
            }

            // update awaylog status
            emit updateAwaylogStatus();
        }
        else
        {
            tw[i]->displayMsg(strTime, strData, eMessageCategory);
            if (i != pTabM->currentIndex())
            {
                if (eMessageCategory != DefaultMessage)
                    pTabM->setAlert(i, QColor(0, 147, 0, 255)); // green
                else
                    pTabM->setAlert(i, QColor(255, 0, 0, 255)); // red
            }
        }
    }
}

void TabContainer::showMsg(QString &strChannel, QString &strData, MessageCategory eMessageCategory)
{
    int i = getIndex(strChannel);
    if (i != -1)
    {
        // hilight
        QString strMe = Core::instance()->settings.value("nick");

        if (strData.contains(QRegExp("(\\s|\\W)"+strMe+"\\b")))
        {
            if (eMessageCategory == DefaultMessage)
            {
                tw[i]->displayMsg(strData, HilightMessage);
                if (i != pTabM->currentIndex())
                    pTabM->setHilight(i);
            }
            else
            {
                tw[i]->displayMsg(strData, eMessageCategory);
                if (i != pTabM->currentIndex())
                    pTabM->setAlert(i, QColor(0, 147, 0, 255)); // green
            }

            // update awaylog status
            emit updateAwaylogStatus();
        }
        else
        {
            tw[i]->displayMsg(strData, eMessageCategory);
            if (i != pTabM->currentIndex())
            {
                if (eMessageCategory != DefaultMessage)
                    pTabM->setAlert(i, QColor(0, 147, 0, 255)); // green
                else
                    pTabM->setAlert(i, QColor(255, 0, 0, 255)); // red
            }
        }
    }
}

void TabContainer::showMsgAll(QString &strData, MessageCategory eMessageCategory)
{
    for (int i = 0; i < tw.size(); i++)
    {
        QString strDataAll = strData;
        tw[i]->displayMsg(strDataAll, eMessageCategory);
        if (i != pTabM->currentIndex())
        {
            if (eMessageCategory != DefaultMessage)
                pTabM->setAlert(i, QColor(0, 147, 0, 255)); // green
            else
                pTabM->setAlert(i, QColor(255, 0, 0, 255)); // red
        }
    }
}

void TabContainer::showMsgActive(QString &strData, MessageCategory eMessageCategory)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (i == pTabM->currentIndex())
        {
            tw[i]->displayMsg(strData, eMessageCategory);
            return;
        }
    }
}

void TabContainer::setTopic(QString &strChannel, QString &strTopic)
{
    int i = getIndex(strChannel);
    if (i != -1)
        tw[i]->setTopic(strTopic);
}

void TabContainer::authorTopic(QString &strChannel, QString &strNick)
{
    int i = getIndex(strChannel);
    if (i != -1)
        tw[i]->authorTopic(strNick);
}

void TabContainer::slotUpdateNickAvatar(QString strNick)
{
    emit updateNickAvatar(strNick);
}

void TabContainer::slotUpdateChannelAvatar(QString strChannel)
{
    int i = getIndex(strChannel);
    if (i != -1)
    {
        QByteArray bAvatar = Core::instance()->mChannelAvatar.value(strChannel);
        QPixmap pixmap;
        pixmap.loadFromData(bAvatar);

        QIcon icon(pixmap);
        pTabM->setTabIcon(i, icon);
    }
}

void TabContainer::slotShowMsg(QString &strChannel, QString &strData, MessageCategory eMessageCategory)
{
    showMsg(strChannel, strData, eMessageCategory);
}

void TabContainer::slotShowMsgActive(QString &strData, MessageCategory eMessageCategory)
{
    showMsgActive(strData, eMessageCategory);
}

void TabContainer::slotShowMsgAll(QString &strData, MessageCategory eMessageCategory)
{
    showMsgAll(strData, eMessageCategory);
}

void TabContainer::slotDisplayMessage(QString &strChannel, QString &strData, MessageCategory eMessageCategory)
{
    int i = getIndex(strChannel);
    if (i != -1)
        tw[i]->displayMessage(strData, eMessageCategory);
}

void TabContainer::slotChangeFontSize(QString strSize)
{
    for (int i = 0; i < tw.size(); i++)
        tw[i]->changeFontSize(strSize);
}

void TabContainer::slotClearContent(QString strChannel)
{
    int i = getIndex(strChannel);
    if (i != -1)
        tw[i]->clearContent();
}

void TabContainer::refreshColors()
{
    for (int i = 0; i < tw.size(); i++)
    {
        tw[i]->refreshColors();
        // update tab name color
        pTabM->setColor(i, QColor(Core::instance()->settings.value("default_font_color")));
    }
}

void TabContainer::refreshBackgroundImage()
{
    for (int i = 0; i < tw.size(); i++)
        tw[i]->refreshBackgroundImage();
}
