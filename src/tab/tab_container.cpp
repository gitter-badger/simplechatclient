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

#include <QDateTime>
#include "convert.h"
#include "core.h"
#include "log.h"
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
        Core::instance()->lOpenChannels.removeOne(strChannel);

        // remove tab
        delete Core::instance()->tw.take(strChannel);

        // log
        Log::logClosed(strChannel);
    }
}

bool TabContainer::existTab(const QString &strChannel)
{
    return Core::instance()->tw.contains(strChannel);
}

void TabContainer::addTab(const QString &strChannel)
{
    if (existTab(strChannel))
        return;

    // update open channels
    Core::instance()->lOpenChannels.append(strChannel);

    // create tab
    Core::instance()->tw.insert(strChannel, new TabWidget(strChannel));
    pTabM->addTab(Core::instance()->tw[strChannel], strChannel);
    pTabM->setCurrentIndex(Core::instance()->tw.size()-1);

    // if priv
    if (strChannel[0] == '^')
    {
        if (Core::instance()->mPrivNames.contains(strChannel))
        {
            pTabM->setTabText(Core::instance()->tw.size()-1, Core::instance()->convertPrivName(strChannel));

            Log::logOpened(strChannel);
        }
    }
    else
        Log::logOpened(strChannel);
}

void TabContainer::removeTab(const QString &strChannel)
{
    if ((!existTab(strChannel)) || (strChannel == DEBUG) || (strChannel == STATUS))
        return;

    // remove from open channels
    Core::instance()->lOpenChannels.removeOne(strChannel);

    // remove tab
    delete Core::instance()->tw.take(strChannel);

    // log
    Log::logClosed(strChannel);
}

void TabContainer::renameTab(const QString &strChannel, const QString &strNewName)
{
    int index = Core::instance()->getIndexFromChannelName(strChannel);

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
    QString strChannel = Core::instance()->getChannelNameFromIndex(index);

    if (!strChannel.isEmpty())
    {
        if (Core::instance()->pNetwork->isConnected())
            Core::instance()->pNetwork->send(QString("PART %1").arg(strChannel));
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
        int index = Core::instance()->getIndexFromChannelName(strChannel);
        pTabM->setColor(index, QColor(Core::instance()->settings.value("default_color")));
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

void TabContainer::setTopic(const QString &strChannel, const QString &strTopicContent)
{
    if (!existTab(strChannel))
        return;

    QString strTopic = strTopicContent;
    QString strTooltip = strTopicContent;

    // convert emoticons, font
    Convert::fixHtmlChars(strTopic);
    Convert::convertText(strTopic);

    // fix length bug
    if (strTopic.contains(QRegExp("\\S{100}")))
        strTopic.replace(QRegExp("(\\S{100})"), "\\1 ");

    // set topic
    Core::instance()->tw[strChannel]->topic->setText(QString("<b>%1</b> %2").arg(tr("Topic:"), strTopic));

    // tooltip
    Convert::fixHtmlChars(strTooltip);
    Convert::simpleConvert(strTooltip);

    Core::instance()->tw[strChannel]->topic->setToolTip(strTooltip);
}

void TabContainer::authorTopic(const QString &strChannel, const QString &strNick)
{
    if (!existTab(strChannel))
        return;

    QString strTopicDetails = QString(tr("Topic set by %1")).arg(strNick);
    Core::instance()->tw[strChannel]->topic->setToolTip(strTopicDetails);
}

void TabContainer::setChannelAvatar(const QString &strChannel)
{
    if (!existTab(strChannel))
        return;

    QString strAvatarpath = Core::instance()->mChannelAvatar.value(strChannel);

    int index = Core::instance()->getIndexFromChannelName(strChannel);
    if (index >= 0 && index <= pTabM->count())
        pTabM->setTabIcon(index, QIcon(strAvatarpath));
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
