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
    QMapIterator<QString, TabWidget*> i(tw);
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

void TabContainer::logOpened(QString strChannel)
{
    QString strData = "--- Log opened "+QDateTime::currentDateTime().toString(Qt::TextDate);
    Log *l = new Log();
    l->save(strChannel, strData);
    delete l;
}

void TabContainer::logClosed(QString strChannel)
{
    QString strData = "--- Log closed "+QDateTime::currentDateTime().toString(Qt::TextDate);
    Log *l = new Log();
    l->save(strChannel, strData);
    delete l;
}

bool TabContainer::isHighlightMessage(QString strMessage)
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


bool TabContainer::existTab(QString strChannel)
{
    return tw.contains(strChannel);
}

void TabContainer::addTab(QString strChannel)
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

void TabContainer::removeTab(QString strChannel)
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

void TabContainer::renameTab(QString strChannel, QString strNewName)
{
    int index = Core::instance()->getIndexFromChannelName(strChannel);

    if ((index != -1) && (pTabM->tabText(index)[0] == '^'))
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
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        tw[strChannel]->refreshColors();

        // update tab name color
        int index = Core::instance()->getIndexFromChannelName(strChannel);
        if (index != -1)
            pTabM->setColor(index, QColor(Core::instance()->settings.value("default_font_color")));
    }
}

void TabContainer::refreshCSS()
{
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        tw[strChannel]->pChatView->refreshCSS();
    }
}

void TabContainer::showMessage(QString strChannel, QString strData, MessageCategory eMessageCategory, QString strTime, QString strNick)
{
    if (!existTab(strChannel))
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
        Core::instance()->addAwaylog(strTime, strChannel, strAwaylogData);

        // update awaylog status
        emit updateAwaylogStatus();

        // update message category
        eMessageCategory = HighlightMessage;

        // highlight
        if ((index != -1) && (index != pTabM->currentIndex()))
            pTabM->setAlert(index, ChannelHighlight);
    }

    // set color
    if ((index != -1) && (index != pTabM->currentIndex()))
    {
        if (eMessageCategory != DefaultMessage)
            pTabM->setAlert(index, ChannelGreen); // green
        else
            pTabM->setAlert(index, ChannelRed); // red
    }

    // display
    tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory, strTime, strNick);
}

void TabContainer::showMessageAll(QString strData, MessageCategory eMessageCategory)
{
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        int index = Core::instance()->getIndexFromChannelName(strChannel);
        if ((index != -1) && (index != pTabM->currentIndex()))
        {
            if (eMessageCategory != DefaultMessage)
                pTabM->setAlert(index, ChannelGreen); // green
            else
                pTabM->setAlert(index, ChannelRed); // red
        }

        tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory);
    }
}

void TabContainer::showMessageActive(QString strData, MessageCategory eMessageCategory)
{
    QString strChannel = Core::instance()->getCurrentChannelName();

    if (!strChannel.isEmpty())
        tw[strChannel]->pChatView->displayMessage(strData, eMessageCategory);
}

void TabContainer::setTopic(QString strChannel, QString strTopic)
{
    if (!existTab(strChannel))
        return;

    // replace
    strTopic.replace("&", "&amp;");
    strTopic.replace("<", "&lt;");
    strTopic.replace(">", "&gt;");
    strTopic.replace("\"", "&quot;");
    strTopic.replace("'", "&#039;");
    strTopic.replace("\\", "&#92;");

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
    strTopic.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
    strTopic.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
    strTopic.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

    tw[strChannel]->topic->setToolTip(strTopic);
}

void TabContainer::authorTopic(QString strChannel, QString strNick)
{
    if (!existTab(strChannel))
        return;

    QString strTopicDetails = QString(tr("Topic set by %1")).arg(strNick);
    tw[strChannel]->topic->setToolTip(strTopicDetails);
}

void TabContainer::setChannelAvatar(QString strChannel)
{
    if (!existTab(strChannel))
        return;

    QByteArray bAvatar = Core::instance()->mChannelAvatar.value(strChannel);
    QPixmap pixmap;
    pixmap.loadFromData(bAvatar);

    QIcon icon(pixmap);
    int index = Core::instance()->getIndexFromChannelName(strChannel);
    if (index != -1)
        pTabM->setTabIcon(index, icon);
}

void TabContainer::clearContent(QString strChannel)
{
    if (existTab(strChannel))
        tw[strChannel]->pChatView->clearMessages();
}

// nicklist

void TabContainer::addUser(QString strChannel, QString strNick, QString strModes, bool bFastAdd)
{
    if (!existTab(strChannel))
        return;

    tw[strChannel]->pNickListWidget->addUser(strNick, strModes);
    tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));

    if (!bFastAdd)
        tw[strChannel]->pNickListWidget->sortItems(Qt::AscendingOrder);
}

void TabContainer::delUser(QString strChannel, QString strNick)
{
    if (!existTab(strChannel))
        return;

    tw[strChannel]->pNickListWidget->delUser(strNick);
    tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));
}

void TabContainer::quitUser(QString strNick, QString strDisplay)
{
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
        {
            showMessage(strChannel, strDisplay, QuitMessage);
            tw[strChannel]->pNickListWidget->delUser(strNick);
            tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));

            int index = Core::instance()->getIndexFromChannelName(strChannel);
            if (index != pTabM->currentIndex())
                pTabM->setAlert(index, ChannelGreen); // green
        }
    }
}

void TabContainer::changeFlag(QString strNick, QString strChannel, QString strFlag)
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

void TabContainer::changeFlag(QString strNick, QString strFlag)
{
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
            changeFlag(strNick, strChannel, strFlag);
    }
}

void TabContainer::nicklistRefresh(QString strChannel)
{
    if (existTab(strChannel))
        tw[strChannel]->pNickListWidget->sortItems(Qt::AscendingOrder);
}

void TabContainer::clearAllNicklist()
{
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        tw[strChannel]->pNickListWidget->clear();
        tw[strChannel]->users->setText(QString(tr("Users (%1)").arg(tw[strChannel]->pNickListWidget->count())));
    }
}

void TabContainer::setUserAvatarPath(QString strNick, QString strValue)
{
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
            tw[strChannel]->pNickListWidget->setUserAvatarPath(strNick, strValue);
    }
}

QString TabContainer::getUserAvatarPath(QString strNick)
{
    QMapIterator<QString, TabWidget*> i(tw);
    while (i.hasNext())
    {
        i.next();
        QString strChannel = i.key();

        if (tw[strChannel]->pNickListWidget->existUser(strNick))
            return tw[strChannel]->pNickListWidget->getUserAvatarPath(strNick);
    }
    return QString::null;
}

int TabContainer::getUserCount(QString strChannel)
{
    if (existTab(strChannel))
        return tw[strChannel]->pNickListWidget->count();
    else
        return 0;
}

QString TabContainer::getUserModes(QString strNick, QString strChannel)
{
    if ((existTab(strChannel)) && (tw[strChannel]->pNickListWidget->existUser(strNick)))
        return tw[strChannel]->pNickListWidget->getUserModes(strNick);
    else
        return QString::null;
}

QList<QString> TabContainer::getUserList(QString strChannel)
{
    QList<QString> userList;

    if (existTab(strChannel))
        userList = tw[strChannel]->pNickListWidget->getUserList();

    return userList;
}

void TabContainer::resizeMainWindow(QSize s)
{
    QMapIterator<QString, TabWidget*> i(tw);
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
