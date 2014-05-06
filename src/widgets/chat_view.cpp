/*
 * Simple Chat Client
 *
 *   Copyright (C) 2009-2014 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include <QTimer>
#include <QUrl>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include "channel.h"
#include "core.h"
#include "find_text_gui.h"
#include "friends.h"
#include "html_messages_renderer.h"
#include "ignore.h"
#include "log.h"
#include "mainwindow.h"
#include "notes.h"
#include "nick.h"
#include "punish_reason.h"
#include "settings.h"
#include "user_profile_gui.h"
#include "webbrowser_gui.h"
#include "chat_view.h"

#ifdef Q_WS_WIN
    #include <QCoreApplication>
#endif

#if WITH_KAMERZYSTA
    #include "kamerzysta.h"
#else
    #include "webcam_gui.h"
#endif

ChatView::ChatView(const QString &_strChatViewChannel) : strChatViewChannel(_strChatViewChannel), bScrollToBottom(true)
{
    setFocusPolicy(Qt::NoFocus);

    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false);
    settings()->setAttribute(QWebSettings::PrintElementBackgrounds, false);
    //settings()->setAttribute(QWebSettings::NotificationsEnabled, false);
    settings()->setObjectCacheCapacities(0, 0, 0);
    settings()->setMaximumPagesInCache(0);

    connect(this->page()->mainFrame(), SIGNAL(contentsSizeChanged(const QSize &)), this, SLOT(autoScrollToBottom()));

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

    QString jsCode = "function appendMessage(html) { "
                         "var elements = document.getElementById('Chat').getElementsByTagName('article'); "
                         "if (elements.length > 350) { "
                             "var chatElement = document.getElementById('Chat'); "
                             "chatElement.removeChild(chatElement.firstChild); "
                         "} "
                         "var chatElement = document.getElementById('Chat'); "
                         "var n = document.createElement('article'); "
                         "n.innerHTML = html; "
                         "chatElement.appendChild(n); "
                     "} "

                     "function clearMessages() { "
                         "var chatElement = document.getElementById('Chat'); "
                         "chatElement.innerHTML = ''; "
                     "}";

    QString strMainHtml = "<html>"
                              "<head>"
                                  "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />"
                                  "<style type=\"text/css\"></style>"
                              "</head>"
                              "<body>"
                                  "<main id=\"Chat\"></main>"
                              "</body>"
                          "</html>";
    this->setHtml(strMainHtml);
    this->page()->mainFrame()->evaluateJavaScript(jsCode);
}

void ChatView::refreshCSS()
{
    HtmlMessagesRenderer *pHtmlMessagesRenderer = new HtmlMessagesRenderer();
    QString strHeadCSS = pHtmlMessagesRenderer->headCSS();
    QString strBodyCSS = pHtmlMessagesRenderer->bodyCSS();
    delete pHtmlMessagesRenderer;

    QWebElement head = this->page()->mainFrame()->findFirstElement("head");
    QWebElement headStyle = head.findFirst("style");
    headStyle.setPlainText(strHeadCSS);

    QWebElement body = this->page()->mainFrame()->findFirstElement("body");
    body.setAttribute("style", strBodyCSS);

    // html logs
    if (Settings::instance()->get("debug") == "true")
    {
        Log::save(strChatViewChannel, "<html><head>", "html");
        Log::save(strChatViewChannel, QString("<style type=\"text/css\">%1</style>").arg(strHeadCSS), "html");
        Log::save(strChatViewChannel, QString("<style type=\"text/css\">%1</style>").arg(strBodyCSS), "html");
        Log::save(strChatViewChannel, "</head><body>", "html");
    }
}

void ChatView::clearMessages()
{
    this->page()->mainFrame()->evaluateJavaScript("clearMessages()");

    bScrollToBottom = true;
}

void ChatView::displayMessage(const QString &strData, MessageCategory eMessageCategory, qint64 iTime, QString strNick)
{
    QString strContent;

    HtmlMessagesRenderer *pHtmlMessagesRenderer = new HtmlMessagesRenderer();
    if (strChatViewChannel == DEBUG_WINDOW)
        strContent = pHtmlMessagesRenderer->rendererDebug(strData, iTime);
    else
        strContent = pHtmlMessagesRenderer->renderer(strData, eMessageCategory, iTime, strNick);
    delete pHtmlMessagesRenderer;

    // append
    this->page()->mainFrame()->evaluateJavaScript("appendMessage(\'"+strContent+"\')");

    // html logs
    if (Settings::instance()->get("debug") == "true")
        Log::save(strChatViewChannel, strContent, "html");
}

void ChatView::joinChannel()
{
    Core::instance()->network->send(QString("JOIN %1").arg(strChannel));
}

void ChatView::nick()
{
    Core::instance()->mainWindow()->insertTextToInputLine(strNick+" ");
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
    UserProfileGui(strNick).exec();
}

void ChatView::cam()
{
#if WITH_KAMERZYSTA
    (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strNick);
#else
    new WebcamGui(strNick, true);
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
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->network->send(QString("KICK %1 %2 :%3").arg(strChatViewChannel, strNick, strText));
    }
}

void ChatView::kickWithSelectedReason()
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

void ChatView::kbanWithReason()
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

void ChatView::kbanWithSelectedReason()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strPunishReason = action->data().toString();

        Core::instance()->network->send(QString("CS BAN %1 ADD %2").arg(strChatViewChannel, strNick));
        Core::instance()->network->send(QString("KICK %1 %2 :%3").arg(strChatViewChannel, strNick, strPunishReason));
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
        (new WebBrowserGui(QUrl(strYoutubeLink)))->show();
}

void ChatView::openWebbrowser()
{
    QDesktopServices::openUrl(QUrl::fromUserInput(strWebsite));
}

void ChatView::sendToNotes()
{
    QString strNotesContent = Notes::instance()->get();
    strNotesContent += this->selectedText()+"\r\n";
    Notes::instance()->set(strNotesContent);
}

void ChatView::search()
{
    FindTextGui(this).exec();
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
    QString strSelfModes = Nick::instance()->getModes(strMe, strChatViewChannel);
    int iSelfMaxModes = Nick::instance()->getMaxModes(strMe, strChatViewChannel);
    QString strNickModes = Nick::instance()->getModes(strNick, strChatViewChannel);
    QList<QString> lPunishReasons = PunishReason::instance()->get();

    QMenu *mInvite = new QMenu(tr("Invite"));
    mInvite->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));
    QList<CaseIgnoreString> lChannelsCleared = Channel::instance()->getListClearedSorted();
    for (int i = 0; i < lChannelsCleared.size(); ++i)
    {
        QString strOpenChannel = lChannelsCleared.at(i);
        if (strOpenChannel.at(0) == '^')
            strOpenChannel = Channel::instance()->getAlternativeName(strOpenChannel);

        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
        openChannelsActs[i]->setVisible(false);
        openChannelsActs[i]->setText(strOpenChannel);
        openChannelsActs[i]->setData(lChannelsCleared.at(i));
        openChannelsActs[i]->setVisible(true);

        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
        mInvite->addAction(openChannelsActs[i]);
     }

    QMenu *mKick = new QMenu(tr("Kick From Channel"));
    mKick->setIcon(QIcon(":/images/oxygen/16x16/im-kick-user.png"));
    mKick->addAction(QIcon(":/images/oxygen/16x16/view-conversation-balloon.png"), tr("(reason)"), this, SLOT(kickWithReason()));
    if (!lPunishReasons.isEmpty()) mKick->addSeparator();
    for (int i = 0; i < lPunishReasons.size(); ++i)
    {
        QString strPunishReasons = lPunishReasons.at(i);

        kickReasonAct[i] = new QAction(this);
        kickReasonAct[i]->setIcon(QIcon(":/images/oxygen/16x16/view-conversation-balloon.png"));
        kickReasonAct[i]->setVisible(false);
        kickReasonAct[i]->setText(strPunishReasons);
        kickReasonAct[i]->setData(lPunishReasons.at(i));
        kickReasonAct[i]->setVisible(true);

        connect(kickReasonAct[i], SIGNAL(triggered()), this, SLOT(kickWithSelectedReason()));
        mKick->addAction(kickReasonAct[i]);
    }

    QMenu *mKickAndBan = new QMenu(tr("Kick & Ban"));
    mKickAndBan->setIcon(QIcon(":/images/oxygen/16x16/im-ban-kick-user.png"));
    mKickAndBan->addAction(QIcon(":/images/oxygen/16x16/view-conversation-balloon.png"), tr("(reason)"), this, SLOT(kbanWithReason()));
    if (!lPunishReasons.isEmpty()) mKickAndBan->addSeparator();
    for (int i = 0; i < lPunishReasons.size(); ++i)
    {
        QString strPunishReasons = lPunishReasons.at(i);

        kbanReasonAct[i] = new QAction(this);
        kbanReasonAct[i]->setIcon(QIcon(":/images/oxygen/16x16/view-conversation-balloon.png"));
        kbanReasonAct[i]->setVisible(false);
        kbanReasonAct[i]->setText(strPunishReasons);
        kbanReasonAct[i]->setData(lPunishReasons.at(i));
        kbanReasonAct[i]->setVisible(true);

        connect(kbanReasonAct[i], SIGNAL(triggered()), this, SLOT(kbanWithSelectedReason()));
        mKickAndBan->addAction(kbanReasonAct[i]);
    }

    QMenu *friends = new QMenu(tr("Friends list"));
    friends->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending.png"));
    if (Friends::instance()->contains(strNick))
        friends->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from friends"), this, SLOT(friendsDel()));
    else
        friends->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to friends"), this, SLOT(friendsAdd()));

    QMenu *ignore = new QMenu(tr("Ignore list"));
    ignore->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"));
    if (Ignore::instance()->contains(strNick))
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from Ignore list"), this, SLOT(ignoreDel()));
    else
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to Ignore list"), this, SLOT(ignoreAdd()));

    QMenu *privilege = new QMenu(tr("Actions"));
    privilege->setIcon(QIcon(":/images/oxygen/16x16/irc-operator.png"));

    if ((strNickModes.contains(FLAG_OP)) && ((iSelfMaxModes >= FLAG_OWNER_INT) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(opDel()));
    else if ((!strNickModes.contains(FLAG_OP)) && (iSelfMaxModes >= FLAG_OWNER_INT))
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(opAdd()));

    if ((strNickModes.contains(FLAG_HALFOP)) && ((iSelfMaxModes >= FLAG_OP_INT) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfopDel()));
    else if ((!strNickModes.contains(FLAG_HALFOP)) && (iSelfMaxModes >= FLAG_OP_INT))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfopAdd()));

    if ((strNickModes.contains(FLAG_MOD)) && ((iSelfMaxModes >= FLAG_HALFOP_INT) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderatorDel()));
    else if ((!strNickModes.contains(FLAG_MOD)) && (iSelfMaxModes >= FLAG_HALFOP_INT))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderatorAdd()));

    if ((strNickModes.contains(FLAG_VOICE)) && ((iSelfMaxModes >= FLAG_HALFOP_INT) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Take guest status"), this, SLOT(voiceDel()));
    else if ((!strNickModes.contains(FLAG_VOICE)) && (iSelfMaxModes >= FLAG_HALFOP_INT))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Give guest status"), this, SLOT(voiceAdd()));

    QMenu menu;
    menu.addAction(QIcon(":/images/oxygen/16x16/user-identity.png"), strNick, this, SLOT(nick()));
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/list-add-user.png"), tr("Priv"), this, SLOT(priv()));
    menu.addAction(QIcon(":/images/oxygen/16x16/text-field.png"), tr("Whois"), this, SLOT(whois()));
    if (strNick.at(0) != '~')
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
    if (iSelfMaxModes >= FLAG_HALFOP_INT)
    {
        menu.addSeparator();
        menu.addAction(QIcon(":/images/oxygen/16x16/im-kick-user.png"), tr("Kick From Channel"), this, SLOT(kick()));
        menu.addMenu(mKick);
        menu.addAction(QIcon(":/images/oxygen/16x16/im-ban-user.png"), tr("Ban From Channel"), this, SLOT(ban()));
        menu.addMenu(mKickAndBan);
        menu.addAction(QIcon(":/images/oxygen/16x16/im-user-busy.png"), tr("IP Ban"), this, SLOT(ipban()));
    }
    if (!privilege->isEmpty())
    {
        menu.addSeparator();
        menu.addMenu(privilege);
    }

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

void ChatView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return QWebView::mousePressEvent(event);

    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    if (r.linkUrl().isEmpty() == false)
    {
        QString strWord = r.linkUrl().toString();
        QString strCategory = r.linkElement().attribute("name");

        // website
        if (strCategory == "website")
        {
            strWebsite = strWord;
            openWebbrowser();
            return;
        }

        // youtube
        if (strCategory == "youtube")
        {
            strWebsite = strWord;
            watchVideo();
            return;
        }
    }

    QWebView::mousePressEvent(event);
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
        menu.addAction(QIcon(":/images/oxygen/16x16/media-playback-start.png"), tr("Watch video"), this, SLOT(watchVideo()));

    QAction *copyLinkToClipboard = this->pageAction(QWebPage::CopyLinkToClipboard);
    copyLinkToClipboard->setIcon(QIcon(":/images/oxygen/16x16/edit-copy.png"));
    menu.addAction(copyLinkToClipboard);

    menu.exec(event->globalPos());
}

void ChatView::contextMenuEvent(QContextMenuEvent *event)
{
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    if (r.linkUrl().isEmpty() == false)
    {
        QString strCategory = r.linkElement().attribute("name");

        // website
        if (strCategory == "website")
        {
            strWebsite = r.linkUrl().toString();
            menuWebsite(event);
            return;
        }

        // channel
        if (strCategory == "channel")
        {
            strChannel = r.linkText();
            menuChannel(event);
            return;
        }

        // nick
        if (strCategory == "nick")
        {
            strNick = r.linkText();
            menuNick(event);
            return;
        }
    }

    menuStandard(event);
}

bool ChatView::copySelectedText()
{
    if (page()->hasSelection())
    {
        page()->triggerAction(QWebPage::Copy);
        return true;
    }
    else
        return false;
}

void ChatView::forceScrollToBottom()
{
    QTimer::singleShot(50, this, SLOT(scrollToBottom())); // 0.05 sec
}

void ChatView::autoScrollToBottom()
{
    if (bScrollToBottom)
        QTimer::singleShot(50, this, SLOT(scrollToBottom())); // 0.05 sec
}

void ChatView::scrollToBottom()
{
    page()->mainFrame()->setScrollBarValue(Qt::Vertical, page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}

void ChatView::wheelEvent(QWheelEvent *event)
{
    // event
    QWebView::wheelEvent(event);

    // scroll
    bScrollToBottom = (this->page()->mainFrame()->scrollBarValue(Qt::Vertical) >= this->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}
