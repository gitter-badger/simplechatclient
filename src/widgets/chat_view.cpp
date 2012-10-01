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

#include <QContextMenuEvent>
#include <QDesktopServices>
#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QUrl>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include "channel.h"
#include "core.h"
#include "find_text.h"
#include "friends_model.h"
#include "html_messages_renderer.h"
#include "ignore_model.h"
#include "notes_model.h"
#include "nicklist.h"
#include "punish_reason.h"
#include "settings.h"
#include "user_profile.h"
#include "webbrowser.h"
#include "chat_view.h"

#ifdef Q_WS_WIN
    #include <QCoreApplication>
#endif

#if WITH_KAMERZYSTA
    #include "kamerzysta.h"
#else
    #include "webcam.h"
#endif

ChatView::ChatView(const QString &_strChatViewChannel) : strChatViewChannel(_strChatViewChannel), bAtBottom(true)
{
    setFocusPolicy(Qt::NoFocus);
    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    connect(this->page()->mainFrame(), SIGNAL(contentsSizeChanged(const QSize &)), this, SLOT(scrollToBottom()));

    createBody();
    refreshCSS();
}

void ChatView::createBody()
{
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = SCC_DATA_DIR;
#endif

    QString jsCode;
    QFile file(path+"/scripts/chat.js");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        jsCode = file.readAll();
        file.close();
    }

    if (jsCode.isEmpty())
    {
        jsCode = QByteArray::fromBase64("ZnVuY3Rpb24gYXBwZW5kTWVzc2FnZShodG1sKQ0Kew0KCXZhciBjaGF0RWxlbWVudCA9IGRvY3VtZW50LmdldEVsZW1lbnRCeUlkKCdDaGF0Jyk7DQoJdmFyIG4gPSBkb2N1bWVudC5jcmVhdGVFbGVtZW50KCdkaXYnKTsNCgluLmlubmVySFRNTCA9IGh0bWw7DQoJY2hhdEVsZW1lbnQuYXBwZW5kQ2hpbGQobik7IA0KfQ0KDQpmdW5jdGlvbiByZW1vdmVGaXJzdE1lc3NhZ2UoKQ0Kew0KCXZhciBjaGF0RWxlbWVudCA9IGRvY3VtZW50LmdldEVsZW1lbnRCeUlkKCdDaGF0Jyk7DQoJY2hhdEVsZW1lbnQucmVtb3ZlQ2hpbGQoY2hhdEVsZW1lbnQuZmlyc3RDaGlsZCk7DQp9DQoNCmZ1bmN0aW9uIGNsZWFyTWVzc2FnZXMoKQ0Kew0KCXZhciBjaGF0RWxlbWVudCA9IGRvY3VtZW50LmdldEVsZW1lbnRCeUlkKCdDaGF0Jyk7DQoJY2hhdEVsZW1lbnQuaW5uZXJIVE1MID0gJyc7DQp9DQo=").data();

        if (Settings::instance()->get("debug") == "true")
            qWarning() << "Error: chat.js does not exist!";
    }

    QString strMainHtml = "<html><head><style type=\"text/css\"></style></head><body><div id=\"Chat\"></div></body></html>";
    this->setHtml(strMainHtml);
    this->page()->mainFrame()->evaluateJavaScript(jsCode);
}

void ChatView::refreshCSS()
{
    QString strHeadCSS = HtmlMessagesRenderer::headCSS();
    QWebElement head = this->page()->mainFrame()->findFirstElement("head");
    QWebElement headStyle = head.findFirst("style");
    headStyle.setPlainText(strHeadCSS);

    QString strBodyCSS = HtmlMessagesRenderer::bodyCSS();
    QWebElement body = this->page()->mainFrame()->findFirstElement("body");
    body.setAttribute("style", strBodyCSS);
}

void ChatView::clearMessages()
{
    this->page()->mainFrame()->evaluateJavaScript("clearMessages()");

    bAtBottom = true;
}

void ChatView::displayMessage(const QString &strData, MessageCategory eMessageCategory, qint64 iTime, QString strNick)
{
    QString strContent;
    if (strChatViewChannel == DEBUG_WINDOW)
        strContent = HtmlMessagesRenderer::rendererDebug(strData, iTime);
    else
        strContent = HtmlMessagesRenderer::renderer(strData, eMessageCategory, iTime, strNick);

    // scroll
    bAtBottom = (this->page()->mainFrame()->scrollBarValue(Qt::Vertical) >= this->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));

    // remove first message
    QWebElement document = this->page()->mainFrame()->documentElement();
    QWebElementCollection allDiv = document.findAll("div");
    if (allDiv.count() > 350)
        this->page()->mainFrame()->evaluateJavaScript("removeFirstMessage()");

    // append
    this->page()->mainFrame()->evaluateJavaScript("appendMessage(\'"+strContent+"\')");
}

void ChatView::joinChannel()
{
    Core::instance()->network->send(QString("JOIN %1").arg(strChannel));
}

void ChatView::priv()
{
    Core::instance()->network->send(QString("PRIV %1").arg(strNick));
}

void ChatView::whois()
{
    Core::instance()->network->send(QString("WHOIS %1 :%1").arg(strNick));
}

void ChatView::profile()
{
    DlgUserProfile(strNick).exec();
}

void ChatView::cam()
{
#if WITH_KAMERZYSTA
    (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strNick);
#else
    new DlgWebcam(strNick, true);
#endif
}

void ChatView::friendsAdd()
{
    Core::instance()->network->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void ChatView::friendsDel()
{
    Core::instance()->network->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void ChatView::ignoreAdd()
{
    Core::instance()->network->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void ChatView::ignoreDel()
{
    Core::instance()->network->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void ChatView::kick()
{
    Core::instance()->network->send(QString("KICK %1 %2 :%3").arg(strChatViewChannel, strNick, tr("No reason")));
}

void ChatView::kickWithReason()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strPunishReason = action->data().toString();
        Core::instance()->network->send(QString("KICK %1 %2 :%3").arg(strChatViewChannel, strNick, strPunishReason));
    }
}

void ChatView::ban()
{
    Core::instance()->network->send(QString("CS BAN %1 ADD %2").arg(strChatViewChannel, strNick));
}

void ChatView::kban()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick & Ban"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->network->send(QString("CS BAN %1 ADD %2").arg(strChatViewChannel, strNick));
        Core::instance()->network->send(QString("KICK %1 %2 :%3").arg(strChatViewChannel, strNick, strText));
    }
}

void ChatView::ipban()
{
    Core::instance()->network->send(QString("CS BANIP %1 ADD %2").arg(strChatViewChannel, strNick));
}

void ChatView::opAdd()
{
    Core::instance()->network->send(QString("CS OP %1 ADD %2").arg(strChatViewChannel, strNick));
}

void ChatView::opDel()
{
    Core::instance()->network->send(QString("CS OP %1 DEL %2").arg(strChatViewChannel, strNick));
}

void ChatView::halfopAdd()
{
    Core::instance()->network->send(QString("CS HALFOP %1 ADD %2").arg(strChatViewChannel, strNick));
}

void ChatView::halfopDel()
{
    Core::instance()->network->send(QString("CS HALFOP %1 DEL %2").arg(strChatViewChannel, strNick));
}

void ChatView::moderatorAdd()
{
    Core::instance()->network->send(QString("CS MODERATOR %1 ADD %2").arg(strChatViewChannel, strNick));
}

void ChatView::moderatorDel()
{
    Core::instance()->network->send(QString("CS MODERATOR %1 DEL %2").arg(strChatViewChannel, strNick));
}

void ChatView::voiceAdd()
{
    Core::instance()->network->send(QString("CS VOICE %1 ADD %2").arg(strChatViewChannel, strNick));
}

void ChatView::voiceDel()
{
    Core::instance()->network->send(QString("CS VOICE %1 DEL %2").arg(strChatViewChannel, strNick));
}

void ChatView::invite()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        Core::instance()->network->send(QString("INVITE %1 %2").arg(strNick, strInviteChannel));
    }
}

void ChatView::watchVideo()
{
    QString strYoutubeLink;

    QRegExp exYoutube_1("youtube.com/watch\\?.*v=([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_2("youtube.com/v/([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_3("youtu.be/([a-zA-Z0-9_-]{11})");

    if (exYoutube_1.indexIn(strWebsite) > -1)
        strYoutubeLink = "http://www.youtube.com/embed/"+exYoutube_1.cap(1);
    if (exYoutube_2.indexIn(strWebsite) > -1)
        strYoutubeLink = "http://www.youtube.com/embed/"+exYoutube_2.cap(1);
    if (exYoutube_3.indexIn(strWebsite) > -1)
        strYoutubeLink = "http://www.youtube.com/embed/"+exYoutube_3.cap(1);

    if (QUrl(strYoutubeLink).isValid())
        (new DlgWebBrowser(QUrl(strYoutubeLink)))->show();
}

void ChatView::openWebbrowser()
{
    QDesktopServices::openUrl(QUrl(strWebsite));
}

void ChatView::sendToNotes()
{
    QString strNotesContent = Notes::instance()->get();
    strNotesContent += this->selectedText()+"\r\n";
    Notes::instance()->set(strNotesContent);
}

void ChatView::search()
{
    DlgFindText(this).exec();
}

void ChatView::clear()
{
    clearMessages();
}

void ChatView::menuChannel(QContextMenuEvent *event)
{
    QMenu menu;

    QAction *nameAct = new QAction(strChannel, &menu);
    nameAct->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    nameAct->setDisabled(true);

    menu.addAction(nameAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/legalmoves.png"), tr("Join channel"), this, SLOT(joinChannel()));
    menu.exec(event->globalPos());
}

void ChatView::menuNick(QContextMenuEvent *event)
{
    QString strMe = Settings::instance()->get("nick");
    QString strSelfModes = Nicklist::instance()->getUserModes(strMe, strChatViewChannel);
    int iSelfMaxModes = Nicklist::instance()->getUserMaxModes(strSelfModes);
    QString strNickModes = Nicklist::instance()->getUserModes(strNick, strChatViewChannel);

    QMenu *mInvite = new QMenu(tr("Invite"));
    mInvite->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));

    QList<QString> lChannelsCleared = Channel::instance()->getCleared();
    for (int i = 0; i < lChannelsCleared.size(); ++i)
    {
        QString strOpenChannel = lChannelsCleared[i];
        if (strOpenChannel[0] == '^')
            strOpenChannel = Channel::instance()->getPriv(strOpenChannel);

        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
        openChannelsActs[i]->setVisible(false);
        openChannelsActs[i]->setText(strOpenChannel);
        openChannelsActs[i]->setData(lChannelsCleared[i]);
        openChannelsActs[i]->setVisible(true);

        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
        mInvite->addAction(openChannelsActs[i]);
     }

    QMenu *mKick = new QMenu(tr("Kick From Channel"));
    mKick->setIcon(QIcon(":/images/oxygen/16x16/im-kick-user.png"));

    QList<QString> lPunishReasons = PunishReason::instance()->get();
    for (int i = 0; i < lPunishReasons.size(); ++i)
    {
        QString strPunishReasons = lPunishReasons[i];

        punishReasonActs[i] = new QAction(this);
        punishReasonActs[i]->setIcon(QIcon(":/images/oxygen/16x16/view-conversation-balloon.png"));
        punishReasonActs[i]->setVisible(false);
        punishReasonActs[i]->setText(strPunishReasons);
        punishReasonActs[i]->setData(lPunishReasons[i]);
        punishReasonActs[i]->setVisible(true);

        connect(punishReasonActs[i], SIGNAL(triggered()), this, SLOT(kickWithReason()));
        mKick->addAction(punishReasonActs[i]);
    }

    QMenu *friends = new QMenu(tr("Friends list"));
    friends->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending.png"));
    if (FriendsModel::instance()->contains(strNick))
        friends->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from friends"), this, SLOT(friendsDel()));
    else
        friends->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to friends"), this, SLOT(friendsAdd()));

    QMenu *ignore = new QMenu(tr("Ignore list"));
    ignore->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"));
    if (IgnoreModel::instance()->contains(strNick))
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from Ignore list"), this, SLOT(ignoreDel()));
    else
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to Ignore list"), this, SLOT(ignoreAdd()));

    QMenu *privilege = new QMenu(tr("Actions"));
    privilege->setIcon(QIcon(":/images/oxygen/16x16/irc-operator.png"));

    if ((strNickModes.contains(FLAG_OP)) && ((iSelfMaxModes >= 16) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(opDel()));
    else if ((!strNickModes.contains(FLAG_OP)) && (iSelfMaxModes >= 16))
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(opAdd()));

    if ((strNickModes.contains(FLAG_HALFOP)) && ((iSelfMaxModes >= 8) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfopDel()));
    else if ((!strNickModes.contains(FLAG_HALFOP)) && (iSelfMaxModes >= 8))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfopAdd()));

    if ((strNickModes.contains(FLAG_MOD)) && ((iSelfMaxModes >= 4) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderatorDel()));
    else if ((!strNickModes.contains(FLAG_MOD)) && (iSelfMaxModes >= 4))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderatorAdd()));

    if ((strNickModes.contains(FLAG_VOICE)) && ((iSelfMaxModes >= 4) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Take guest status"), this, SLOT(voiceDel()));
    else if ((!strNickModes.contains(FLAG_VOICE)) && (iSelfMaxModes >= 4))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Give guest status"), this, SLOT(voiceAdd()));

    QAction *nickAct = new QAction(strNick, this);
    nickAct->setIcon(QIcon(":/images/oxygen/16x16/user-identity.png"));
    nickAct->setDisabled(true);

    QMenu menu;
    menu.addAction(nickAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/list-add-user.png"), tr("Priv"), this, SLOT(priv()));
    menu.addAction(QIcon(":/images/oxygen/16x16/text-field.png"), tr("Whois"), this, SLOT(whois()));
    if (!strNick.startsWith('~'))
    {
        menu.addAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"), tr("Profile"), this, SLOT(profile()));
        if ((strNickModes.contains(FLAG_CAM_PUB)) || (strNickModes.contains(FLAG_CAM_PRIV)))
            menu.addAction(QIcon(":/images/pubcam.png"), tr("Webcam"), this, SLOT(cam()));
    }
    menu.addMenu(mInvite);
    if (strSelfModes.contains(FLAG_REGISTERED))
    {
        menu.addMenu(friends);
        menu.addMenu(ignore);
    }
    if (iSelfMaxModes >= 4)
    {
        menu.addSeparator();
        menu.addAction(QIcon(":/images/oxygen/16x16/im-kick-user.png"), tr("Kick From Channel"), this, SLOT(kick()));
        menu.addMenu(mKick);
        menu.addAction(QIcon(":/images/oxygen/16x16/im-ban-user.png"), tr("Ban From Channel"), this, SLOT(ban()));
        menu.addAction(QIcon(":/images/oxygen/16x16/im-ban-kick-user.png"), tr("Kick & Ban"), this, SLOT(kban()));
        menu.addAction(QIcon(":/images/oxygen/16x16/im-user-busy.png"), tr("IP Ban"), this, SLOT(ipban()));
    }
    if (!privilege->isEmpty())
    {
        menu.addSeparator();
        menu.addMenu(privilege);
    }

    menu.exec(event->globalPos());
}

void ChatView::menuWebsite(QContextMenuEvent *event)
{
    QString strShortLink = strWebsite;
    if (strShortLink.size() > 40) strShortLink = strShortLink.left(20)+"..."+strShortLink.right(20);

    QMenu menu;

    QAction *websiteLinkAct = new QAction(strShortLink, this);
    websiteLinkAct->setIcon(QIcon(":/images/oxygen/16x16/preferences-web-browser-shortcuts.png"));
    websiteLinkAct->setDisabled(true);

    menu.addAction(websiteLinkAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/applications-internet.png"), tr("Open link"), this, SLOT(openWebbrowser()));

    QRegExp exYoutube_1("youtube.com/watch\\?.*v=([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_2("youtube.com/v/([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_3("youtu.be/([a-zA-Z0-9_-]{11})");

    if ((strWebsite.contains(exYoutube_1)) || (strWebsite.contains(exYoutube_2)) || (strWebsite.contains(exYoutube_3)))
        menu.addAction(QIcon(":/images/oxygen/16x16/tool-animator.png"), tr("Watch video"), this, SLOT(watchVideo()));

    QAction *copyLinkToClipboard = this->pageAction(QWebPage::CopyLinkToClipboard);
    copyLinkToClipboard->setIcon(QIcon(":/images/oxygen/16x16/edit-copy.png"));
    menu.addAction(copyLinkToClipboard);

    menu.exec(event->globalPos());
}

void ChatView::menuStandard(QContextMenuEvent *event)
{
    QMenu menu;

    if (!this->selectedText().isEmpty())
    {
        QAction *sendToNotes = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Send to notes"), &menu);
        connect(sendToNotes, SIGNAL(triggered()), this, SLOT(sendToNotes()));
        menu.addAction(sendToNotes);
    }

    QAction *search = new QAction(QIcon(":/images/oxygen/16x16/edit-find.png"), tr("Find..."), &menu);
    connect(search, SIGNAL(triggered()), this, SLOT(search()));
    menu.addAction(search);

    QAction *clear = new QAction(QIcon(":/images/oxygen/16x16/draw-eraser.png"), tr("Clear"), &menu);
    connect(clear, SIGNAL(triggered()), this, SLOT(clear()));
    menu.addAction(clear);

    menu.addSeparator();

    if (!this->selectedText().isEmpty())
    {
        QAction *copy = this->pageAction(QWebPage::Copy);
        copy->setIcon(QIcon(":/images/oxygen/16x16/edit-copy.png"));
        menu.addAction(copy);
    }

    QAction *selectAll = this->pageAction(QWebPage::SelectAll);
    selectAll->setIcon(QIcon(":/images/oxygen/16x16/edit-select-all.png"));
    menu.addAction(selectAll);

    menu.exec(event->globalPos());
}

void ChatView::contextMenuEvent(QContextMenuEvent *event)
{
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    if (r.linkUrl().isEmpty() == false)
    {
        QString strWord = r.linkText();
        QString strCategory = r.linkElement().attribute("name");

        // website
        if (strCategory == "website")
        {
            strWebsite = strWord;
            menuWebsite(event);
            return;
        }

        // channel
        if (strCategory == "channel")
        {
            strChannel = strWord;
            menuChannel(event);
            return;
        }

        // nick
        if (strCategory == "nick")
        {
            strNick = strWord;
            menuNick(event);
            return;
        }
    }

    menuStandard(event);
}

void ChatView::scrollToBottom()
{
    if (bAtBottom)
        page()->mainFrame()->setScrollBarValue(Qt::Vertical, page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}
