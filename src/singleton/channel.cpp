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

#include <QLabel>
#include "core.h"
#include "convert.h"
#include "mainwindow.h"
#include "nicklist_widget.h"
#include "channel.h"

Channel * Channel::Instance = 0;

Channel * Channel::instance()
{
    if (!Instance)
    {
        Instance = new Channel();
        Instance->clear();
    }

    return Instance;
}

Channel::Channel()
{
}

void Channel::clear()
{
    lChannelInfo.clear();
    lChannels.clear();
    lAvatar.clear();
    lPriv.clear();
}

void Channel::add(const QString &channel)
{
    if (!lChannels.contains(channel))
        lChannels.append(channel);
}

void Channel::remove(const QString &channel)
{
    if (lChannels.contains(channel))
        lChannels.removeOne(channel);
}

QList<QString> Channel::get()
{
    return lChannels;
}

void Channel::move(int from, int to)
{
    lChannels.move(from, to);
}

bool Channel::contains(const QString &channel)
{
    return lChannels.contains(channel);
}

QList<QString> Channel::getCleared()
{
    QList<QString> lChannelsCleared = lChannels;
    lChannelsCleared.removeOne(DEBUG_WINDOW);
    lChannelsCleared.removeOne(STATUS_WINDOW);
    return lChannelsCleared;
}

QString Channel::getFromIndex(int index)
{
    return lChannels.value(index, QString::null);
}

int Channel::getIndex(const QString &channel)
{
    return lChannels.indexOf(channel);
}

QString Channel::getCurrent()
{
    int index = Core::instance()->mainWindow()->getCurrentTabIndex();
    return lChannels.value(index, QString::null);
}

QList<CaseIgnoreString> Channel::getSorted()
{
    QList<CaseIgnoreString> lChannelsCaseIgnore;

    // copy to new list
    QList<QString> lChannels = getCleared();

    foreach (QString strChannel, lChannels)
        lChannelsCaseIgnore.append(strChannel);

    // sort
    qSort(lChannelsCaseIgnore.begin(), lChannelsCaseIgnore.end());

    return lChannelsCaseIgnore;
}

// topic
void Channel::setTopic(const QString &strChannel, const QString &strTopicContent)
{
    if (!Core::instance()->tw.contains(strChannel))
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

void Channel::setAuthorTopic(const QString &strChannel, const QString &strNick)
{
    if (!Core::instance()->tw.contains(strChannel))
        return;

    QString strTopicDetails = QString(tr("Topic set by %1")).arg(strNick);
    Core::instance()->tw[strChannel]->topic->setToolTip(strTopicDetails);
}

// channel info
void Channel::addChannelInfo(const QString &channel)
{
    if (!lChannelInfo.contains(channel))
        lChannelInfo.append(channel);
}

bool Channel::containsChannelInfo(const QString &channel)
{
    return lChannelInfo.contains(channel);
}

void Channel::removeChannelInfo(const QString &channel)
{
    if (lChannelInfo.contains(channel))
        lChannelInfo.removeOne(channel);
}

void Channel::clearChannelInfo()
{
    lChannelInfo.clear();
}

// avatar
QString Channel::getAvatar(const QString &channel)
{
    return lAvatar.value(channel, QString::null);
}

void Channel::setAvatar(const QString &channel, const QString &path)
{
    lAvatar[channel] = path;

    Core::instance()->mainWindow()->updateChannelIcon(channel);
}

// priv
void Channel::setPriv(const QString &channel, const QString &name)
{
    lPriv[channel] = name;
}

QString Channel::getPriv(const QString &channel)
{
    return lPriv.value(channel, channel);
}

bool Channel::containsPriv(const QString &channel)
{
    return lPriv.contains(channel);
}

QList<QString> Channel::getUserList(const QString &strChannel)
{
    QList<QString> userList;

    if (Core::instance()->tw.contains(strChannel))
        userList = Core::instance()->tw[strChannel]->pNickListWidget->getUserList();

    return userList;
}

int Channel::getUserCount(const QString &strChannel)
{
    if (Core::instance()->tw.contains(strChannel))
        return Core::instance()->tw[strChannel]->pNickListWidget->count();
    else
        return 0;
}
