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

#include "channel.h"
#include "core.h"
#include "log.h"
#include "nicklist_widget.h"
#include "settings.h"
#include "tab_manager.h"
#include "tab_container.h"

TabContainer::TabContainer(TabManager *_pTabM) : pTabM(_pTabM)
{
}

TabContainer::~TabContainer()
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        // remove from open channels
        Channel::instance()->remove(strChannel);

        // remove tab
        delete Core::instance()->tw.take(strChannel);

        // log
        Log::logClosed(strChannel);
    }
}

void TabContainer::addTab(const QString &strChannel)
{
    if (Core::instance()->tw.contains(strChannel))
        return;

    // update open channels
    Channel::instance()->add(strChannel);

    // create tab
    Core::instance()->tw.insert(strChannel, new TabWidget(strChannel));
    pTabM->addTab(Core::instance()->tw[strChannel], strChannel);
    pTabM->setCurrentIndex(Core::instance()->tw.size()-1);

    // if priv
    if (strChannel[0] == '^')
    {
        QString strPrivName = Channel::instance()->getPriv(strChannel);

        if (strPrivName != strChannel)
        {
            pTabM->setTabText(Core::instance()->tw.size()-1, strPrivName);

            Log::logOpened(strChannel);
        }
    }
    else
        Log::logOpened(strChannel);
}

void TabContainer::removeTab(const QString &strChannel)
{
    if ((!Core::instance()->tw.contains(strChannel)) || (strChannel == DEBUG_WINDOW) || (strChannel == STATUS_WINDOW))
        return;

    // remove from open channels
    Channel::instance()->remove(strChannel);

    // remove tab
    delete Core::instance()->tw.take(strChannel);

    // log
    Log::logClosed(strChannel);
}

void TabContainer::renameTab(const QString &strChannel, const QString &strNewName)
{
    int index = Channel::instance()->getIndex(strChannel);

    if (index >= 0 && index <= pTabM->count())
    {
        if (pTabM->tabText(index)[0] == '^')
        {
            pTabM->setTabText(index, strNewName);

            // log
            Log::logOpened(strChannel);
        }
    }
}

void TabContainer::partTab(int index)
{
    QString strChannel = Channel::instance()->getFromIndex(index);

    if (!strChannel.isEmpty())
    {
        if (Core::instance()->network->isConnected())
            Core::instance()->network->send(QString("PART %1").arg(strChannel));
        else
            removeTab(strChannel);
    }
}

void TabContainer::refreshColors()
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        // update tab name color
        int index = Channel::instance()->getIndex(strChannel);
        pTabM->setColor(index, QColor(Settings::instance()->get("default_color")));
    }
}

void TabContainer::refreshCSS()
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        Core::instance()->tw[strChannel]->pChatView->refreshCSS();
    }
}

void TabContainer::resizeMainWindow(QSize s)
{
    QHashIterator<QString, TabWidget*> i(Core::instance()->tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (!Core::instance()->tw[strChannel]->pNickListWidget->isHidden())
        {
            int width = s.width();
            if (width > 250)
            {
                QList<int> currentSizes = Core::instance()->tw[strChannel]->splitter->sizes();

                currentSizes[0] = width-200;
                currentSizes[1] = 200;

                Core::instance()->tw[strChannel]->splitter->setSizes(currentSizes);
            }
        }
    }
}
