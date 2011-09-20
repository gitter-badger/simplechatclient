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
#include "tab_manager.h"
#include "tab_widget.h"
#include "tab_container.h"

TabContainer::TabContainer(TabManager *_pTabM) : pTabM(_pTabM)
{
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
        tw.append(new TabWidget(strChannel, pDlgUserProfile));
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

    if (Core::instance()->pNetwork->isConnected())
        Core::instance()->pNetwork->send(QString("PART %1").arg(strChannel));
    else
        removeTab(strChannel);
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
        tw[i]->pChatView->updateBackgroundImage();
}

void TabContainer::showMsg(QString &strChannel, QString &strData, MessageCategory eMessageCategory, QString strTime)
{
    int i = getIndex(strChannel);
    if (i != -1)
    {
        // hilight
        QString strMe = Core::instance()->settings.value("nick");
        QRegExp re;
        if (strMe[0] == '~')
            re.setPattern(strMe+"\\b");
        else
            re.setPattern("[^~]\\b"+strMe+"\\b");
        bool bHilightMessage = strData.contains(re) ? true : false;

        if ((bHilightMessage) && (eMessageCategory == DefaultMessage))
        {
            // awaylog
            Core::instance()->addAwaylog(strChannel, strData);

            // update awaylog status
            emit updateAwaylogStatus();

            // hilight
            if (i != pTabM->currentIndex())
                pTabM->setHilight(i);

            // update message category
            eMessageCategory = HilightMessage;
        }

        // set color
        if (i != pTabM->currentIndex())
        {
            if (eMessageCategory != DefaultMessage)
                pTabM->setAlert(i, QColor(0, 147, 0, 255)); // green
            else
                pTabM->setAlert(i, QColor(255, 0, 0, 255)); // red
        }

        // display
        tw[i]->pChatView->displayMessage(strData, eMessageCategory, strTime);
    }
}

void TabContainer::showMsgAll(QString &strData, MessageCategory eMessageCategory)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (i != pTabM->currentIndex())
        {
            if (eMessageCategory != DefaultMessage)
                pTabM->setAlert(i, QColor(0, 147, 0, 255)); // green
            else
                pTabM->setAlert(i, QColor(255, 0, 0, 255)); // red
        }

        QString strDataAll = strData;
        tw[i]->pChatView->displayMessage(strDataAll, eMessageCategory);
    }
}

void TabContainer::showMsgActive(QString &strData, MessageCategory eMessageCategory)
{
    for (int i = 0; i < tw.size(); i++)
    {
        if (i == pTabM->currentIndex())
        {
            tw[i]->pChatView->displayMessage(strData, eMessageCategory);
            return;
        }
    }
}

void TabContainer::setTopic(QString &strChannel, QString &strTopic)
{
    int i = getIndex(strChannel);
    if (i != -1)
    {
        // replace
        strTopic.replace("&", "&amp;");
        strTopic.replace("<", "&lt;");
        strTopic.replace(">", "&gt;");

        QString strContent = strTopic;

        // convert emoticons, font
        Convert *convertText = new Convert();
        convertText->convertText(strContent);
        delete convertText;

        // set topic
        tw[i]->topic->setText(QString("<b>%1</b> %2").arg(tr("Topic:")).arg(strContent));

        // tooltip
        strTopic.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
        strTopic.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
        strTopic.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

        tw[i]->topic->setToolTip(strTopic);
    }
}

void TabContainer::authorTopic(QString &strChannel, QString &strNick)
{
    int i = getIndex(strChannel);
    if (i != -1)
    {
        QString strTopicDetails = QString(tr("Topic set by %1")).arg(strNick);
        tw[i]->topic->setToolTip(strTopicDetails);
    }
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
        tw[i]->pChatView->displayMessage(strData, eMessageCategory);
}

void TabContainer::slotClearContent(QString strChannel)
{
    int i = getIndex(strChannel);
    if (i != -1)
        tw[i]->pChatView->clearMessages();
}
