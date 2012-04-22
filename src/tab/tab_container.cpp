/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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
#include "awaylogs.h"
#include "convert.h"
#include "core.h"
#include "log.h"
#include "nicklist_widget.h"
#include "tab_manager.h"
#include "tab_widget.h"
#include "tab_container.h"

TabContainer::TabContainer(TabManager *_pTabM) : pTabM(_pTabM)
{
}

TabContainer::~TabContainer()
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        // remove from open channels
        Core::instance()->lOpenChannels.removeOne(strChannel);

        // remove tab
        delete tw.take(strChannel);

        // log
        logClosed(strChannel);
    }
}

void TabContainer::logOpened(const QString &strChannel)
{
    QString strData = "--- Log opened "+QDateTime::currentDateTime().toString(Qt::TextDate);
    Log::save(strChannel, strData);
}

void TabContainer::logClosed(const QString &strChannel)
{
    QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
    Log::save(strChannel, strData);
}

bool TabContainer::isHighlightMessage(const QString &strMessage)
{
    QString strMe = Core::instance()->settings["nick"];
    QStringList lData = strMessage.split(" ");
    QStringList lHighlight = Core::instance()->settings["highlight"].split(";", QString::SkipEmptyParts);
    lHighlight.append(strMe);

    for (int i = 0; i < lData.size(); i++)
    {
        QString strData = lData[i];
        strData.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
        strData.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
        strData.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

        QStringListIterator lHighlightIterator(lHighlight);
        while (lHighlightIterator.hasNext())
        {
            if (strData.toLower().contains(lHighlightIterator.next().toLower()))
                return true;
        }
    }
    return false;
}

void TabContainer::saveMessage(const QString &strChannel, const QString &strData, QString strTime, QString strNick)
{
    QDateTime dt;
    if (!strTime.isEmpty())
        dt = QDateTime::fromTime_t(strTime.toInt());
    else
        dt = QDateTime::currentDateTime();

    if (Core::instance()->settings.value("disable_logs") == "false")
    {
        QString strSaveData;
        if (!strNick.isEmpty())
            strSaveData = QString("%1 <%2> %3").arg(dt.toString("[hh:mm:ss]"), strNick, strData);
        else
            strSaveData = QString("%1 %2").arg(dt.toString("[hh:mm:ss]"), strData);

        // fix /me
        QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01")), "(.*)", QString(QByteArray("\x01")));
        if (strSaveData.contains(QRegExp(strRegExpMe)))
            strSaveData.replace(QRegExp(strRegExpMe), "\\1");

        Log::save(strChannel, strSaveData);
    }
}

bool TabContainer::hideJoinPart(const QString &strChannel, MessageCategory eMessageCategory)
{
    if ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage))
    {
        if (Core::instance()->settings.value("hide_join_part") == "true")
            return true;

        int iNickCount = Core::instance()->getUserCount(strChannel);
        if ((Core::instance()->settings.value("hide_join_part_200") == "true") && (iNickCount > 200))
            return true;
    }
    return false;
}

bool TabContainer::existTab(const QString &strChannel)
{
    return tw.contains(strChannel);
}

void TabContainer::addTab(const QString &strChannel)
{
    if (existTab(strChannel))
        return;

    // update open channels
    Core::instance()->lOpenChannels.append(strChannel);

    // create tab
    tw.insert(strChannel, new TabWidget(strChannel));
    pTabM->addTab(tw[strChannel], strChannel);
    pTabM->setCurrentIndex(tw.size()-1);

    // if priv
    if (strChannel[0] == '^')
    {
        if (Core::instance()->mPrivNames.contains(strChannel))
        {
            pTabM->setTabText(tw.size()-1, Core::instance()->convertPrivName(strChannel));

            logOpened(strChannel);
        }
    }
    else
        logOpened(strChannel);
}

void TabContainer::removeTab(const QString &strChannel)
{
    if ((!existTab(strChannel)) || (strChannel == STATUS))
        return;

    // remove from open channels
    Core::instance()->lOpenChannels.removeOne(strChannel);

    // remove tab
    delete tw.take(strChannel);

    // log
    logClosed(strChannel);
}

void TabContainer::renameTab(const QString &strChannel, const QString &strNewName)
{
    int index = Core::instance()->getIndexFromChannelName(strChannel);

    if ((index >= 0 && index <= pTabM->count()) && (pTabM->tabText(index)[0] == '^'))
    {
        pTabM->setTabText(index, strNewName);

        // log
        logOpened(strChannel);
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
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        // update tab name color
        int index = Core::instance()->getIndexFromChannelName(strChannel);
        pTabM->setColor(index, QColor(Core::instance()->settings.value("default_font_color")));
    }
}

void TabContainer::refreshCSS()
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        tw[strChannel]->pChatView->refreshCSS();
    }
}

void TabContainer::showMessage(const QString &strChannel, const QString &strData, MessageCategory eMessageCategory, QString strTime, QString strNick)
{
    if (!existTab(strChannel))
        return;

    // hide join part
    if (hideJoinPart(strChannel, eMessageCategory))
        return;

    int index = Core::instance()->getIndexFromChannelName(strChannel);

    // highlight
    bool bHighlightMessage = isHighlightMessage(strData);

    if ((bHighlightMessage) && (eMessageCategory == DefaultMessage))
    {
        QString strAwaylogData = strData;
        if (!strNick.isEmpty())
            strAwaylogData = QString("<%1> %2").arg(strNick, strData);

        // awaylog
        Awaylog::instance()->add(strTime, strChannel, strAwaylogData);

        // update awaylog status
        emit updateAwaylogStatus();

        // update message category
        eMessageCategory = HighlightMessage;

        // highlight
        pTabM->setAlert(index, ChannelHighlight);
    }

    // set color
    if (eMessageCategory != DefaultMessage)
        pTabM->setAlert(index, ChannelGreen);
    else
        pTabM->setAlert(index, ChannelRed);

    // save message
    saveMessage(strChannel, strData, strTime, strNick);

    // display
    tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory, strTime, strNick);
}

void TabContainer::showMessageAll(const QString &strData, MessageCategory eMessageCategory)
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        // hide join part
        if (!hideJoinPart(strChannel, eMessageCategory))
        {
            int index = Core::instance()->getIndexFromChannelName(strChannel);

            if (eMessageCategory != DefaultMessage)
                pTabM->setAlert(index, ChannelGreen);
            else
                pTabM->setAlert(index, ChannelRed);

            // save message
            saveMessage(strChannel, strData);

            // display
            tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory);
        }
    }
}

void TabContainer::showMessageActive(const QString &strData, MessageCategory eMessageCategory)
{
    QString strChannel = Core::instance()->getCurrentChannelName();

    if (strChannel.isEmpty())
        return;

    // hide join part
    if (hideJoinPart(strChannel, eMessageCategory))
        return;

    // save message
    saveMessage(strChannel, strData);

    // display
    tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory);
}

void TabContainer::setTopic(const QString &strChannel, const QString &strTopic)
{
    if (!existTab(strChannel))
        return;

    QString strTopicOriginal = strTopic;

    // replace
    strTopicOriginal.replace("&", "&amp;");
    strTopicOriginal.replace("<", "&lt;");
    strTopicOriginal.replace(">", "&gt;");
    strTopicOriginal.replace("\"", "&quot;");
    strTopicOriginal.replace("'", "&#039;");
    strTopicOriginal.replace("\\", "&#92;");

    QString strContent = strTopic;

    // convert emoticons, font
    Convert *convertText = new Convert();
    convertText->convertText(strContent);
    delete convertText;

    // fix length bug
    if (strContent.contains(QRegExp("\\S{100}")))
        strContent.replace(QRegExp("(\\S{100})"), "\\1 ");

    // set topic
    tw[strChannel]->topic->setText(QString("<b>%1</b> %2").arg(tr("Topic:"), strContent));

    // tooltip
    strTopicOriginal.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
    strTopicOriginal.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
    strTopicOriginal.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

    tw[strChannel]->topic->setToolTip(strTopicOriginal);
}

void TabContainer::authorTopic(const QString &strChannel, const QString &strNick)
{
    if (!existTab(strChannel))
        return;

    QString strTopicDetails = QString(tr("Topic set by %1")).arg(strNick);
    tw[strChannel]->topic->setToolTip(strTopicDetails);
}

void TabContainer::setChannelAvatar(const QString &strChannel)
{
    if (!existTab(strChannel))
        return;

    QByteArray bAvatar = Core::instance()->mChannelAvatar.value(strChannel);
    QPixmap pixmap;
    pixmap.loadFromData(bAvatar);

    QIcon icon(pixmap);
    int index = Core::instance()->getIndexFromChannelName(strChannel);
    if (index >= 0 && index <= pTabM->count())
        pTabM->setTabIcon(index, icon);
}

void TabContainer::clearContent(const QString &strChannel)
{
    if (existTab(strChannel))
        tw[strChannel]->pChatView->clearMessages();
}

// nicklist

void TabContainer::addUser(const QString &strChannel, const QString &strNick, const QString &strModes, bool bFastAdd)
{
    if (!existTab(strChannel))
        return;

    tw[strChannel]->pNickListWidget->addUser(strNick, strModes);
    tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));

    if (!bFastAdd)
        tw[strChannel]->pNickListWidget->sortItems(Qt::AscendingOrder);
}

void TabContainer::delUser(const QString &strChannel, const QString &strNick)
{
    if (!existTab(strChannel))
        return;

    tw[strChannel]->pNickListWidget->delUser(strNick);
    tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));
}

void TabContainer::quitUser(const QString &strNick, const QString &strDisplay)
{
    MessageCategory eMessageCategory = QuitMessage;

    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
        {
            showMessage(strChannel, strDisplay, eMessageCategory);
            tw[strChannel]->pNickListWidget->delUser(strNick);
            tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));

            int index = Core::instance()->getIndexFromChannelName(strChannel);

            if (!hideJoinPart(strChannel, eMessageCategory))
                pTabM->setAlert(index, ChannelGreen);
        }
    }
}

void TabContainer::changeFlag(const QString &strNick, const QString &strChannel, const QString &strFlag)
{
    if (!existTab(strChannel))
        return;

    if (tw[strChannel]->pNickListWidget->existUser(strNick))
    {
        tw[strChannel]->pNickListWidget->changeUserFlag(strNick, strFlag);
        tw[strChannel]->pNickListWidget->sortItems(Qt::AscendingOrder);
    }

    QString strCurrentChannel = Core::instance()->getCurrentChannelName();

    if ((!strCurrentChannel.isEmpty()) && (strCurrentChannel == strChannel))
    {
        QString strMe = Core::instance()->settings.value("nick");
        if (strNick == strMe)
        {
            if (strFlag == "+X") emit setModeration(true);
            else if (strFlag == "-X") emit setModeration(false);
        }
    }
}

void TabContainer::changeFlag(const QString &strNick, const QString &strFlag)
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
            changeFlag(strNick, strChannel, strFlag);
    }
}

void TabContainer::nicklistRefresh(const QString &strChannel)
{
    if (existTab(strChannel))
        tw[strChannel]->pNickListWidget->sortItems(Qt::AscendingOrder);
}

void TabContainer::clearAllNicklist()
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        tw[strChannel]->pNickListWidget->clear();
        tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));
    }
}

void TabContainer::setUserAvatarPath(const QString &strNick, const QString &strValue)
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
            tw[strChannel]->pNickListWidget->setUserAvatarPath(strNick, strValue);
    }
}

QString TabContainer::getUserAvatarPath(const QString &strNick)
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
            return tw[strChannel]->pNickListWidget->getUserAvatarPath(strNick);
    }
    return QString::null;
}

int TabContainer::getUserCount(const QString &strChannel)
{
    if (existTab(strChannel))
        return tw[strChannel]->pNickListWidget->count();
    else
        return 0;
}

QString TabContainer::getUserModes(const QString &strNick, const QString &strChannel)
{
    if ((existTab(strChannel)) && (tw[strChannel]->pNickListWidget->existUser(strNick)))
        return tw[strChannel]->pNickListWidget->getUserModes(strNick);
    else
        return QString::null;
}

QList<QString> TabContainer::getUserList(const QString &strChannel)
{
    QList<QString> userList;

    if (existTab(strChannel))
        userList = tw[strChannel]->pNickListWidget->getUserList();

    return userList;
}

void TabContainer::resizeMainWindow(QSize s)
{
    QHashIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (!tw[strChannel]->pNickListWidget->isHidden())
        {
            int width = s.width();
            if (width > 250)
            {
                QList<int> currentSizes = tw[strChannel]->splitter->sizes();

                currentSizes[0] = width-200;
                currentSizes[1] = 200;

                tw[strChannel]->splitter->setSizes(currentSizes);
            }
        }
    }
}
