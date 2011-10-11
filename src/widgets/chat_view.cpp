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

#include <QContextMenuEvent>
#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QUrl>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include "core.h"
#include "user_profile.h"
#include "find_text.h"
#include "html_messages_renderer.h"
#include "log.h"
#include "notify.h"
#include "webbrowser.h"
#include "chat_view.h"

#ifdef Q_WS_WIN
    #include "kamerzysta.h"
#else
    #include "webcam.h"
#endif

ChatView::ChatView(QString _strChatViewChannel) : strChatViewChannel(_strChatViewChannel), bScroll(true)
{
    setFocusPolicy(Qt::NoFocus);
    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    QObject::connect(this->page()->mainFrame(), SIGNAL(contentsSizeChanged(const QSize &)), this, SLOT(scrollToBottom()));

    createBody();
    refreshCSS();
}

void ChatView::createBody()
{
    QString strPath;
#ifdef Q_WS_WIN
    strPath = QCoreApplication::applicationDirPath();
#else
    strPath = "/usr/share/scc";
#endif

    QString jsCode;
    QFile file(strPath+"/scripts/chat.js");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        jsCode = file.readAll();
        file.close();
    }

    QString strHtml = "<html><head><style type=\"text/css\"></style></head><body><div id=\"Chat\"></div></body></html>";
    this->setHtml(strHtml);
    this->page()->mainFrame()->evaluateJavaScript(jsCode);
}

void ChatView::refreshCSS()
{
    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
    QString strJoinFontColor = Core::instance()->settings.value("font_color_level_1");
    QString strPartFontColor = Core::instance()->settings.value("font_color_level_2");
    QString strQuitFontColor = Core::instance()->settings.value("font_color_level_3");
    QString strKickFontColor = Core::instance()->settings.value("font_color_level_4");
    QString strModeFontColor = Core::instance()->settings.value("font_color_level_5");
    QString strNoticeFontColor = Core::instance()->settings.value("font_color_level_6");
    QString strInfoFontColor = Core::instance()->settings.value("font_color_level_7");
    QString strMeFontColor = Core::instance()->settings.value("font_color_level_8");
    QString strErrorFontColor = Core::instance()->settings.value("font_color_level_9");
    QString strChannelFontColor = Core::instance()->settings.value("channel_font_color");

    QString strFontSize = Core::instance()->settings.value("font_size");

    QString strBackgroundImage = Core::instance()->settings.value("background_image");
    QString strDisableBackgroundImage = Core::instance()->settings.value("disable_background_image");

#ifndef Q_WS_WIN
    strBackgroundImage = "file://"+strBackgroundImage;
#endif

    QString strBackground;
    if ((strDisableBackgroundImage == "off") && (!strBackgroundImage.isEmpty()))
        strBackground = "background-image: url("+strBackgroundImage+"); background-attachment: fixed; background-position: center; background-repeat: no-repeat;";

    QString strBodyCSS = QString("margin: 0; padding: 0; font-family: sans; word-wrap: break-word; font-size:%1; %2").arg(strFontSize).arg(strBackground);
    QWebElement body = this->page()->mainFrame()->findFirstElement("body");
    body.setAttribute("style", strBodyCSS);

    QString strHeadCSS = "div{margin-bottom: 2px;}";
    strHeadCSS.append(QString(".time{font-weight:normal;text-decoration:none;color:%1;float:right;clear:right;margin-right:5px;}").arg(strDefaultFontColor));
    strHeadCSS.append(QString(".avatar{border-radius:5px;vertical-align:middle;margin-left:4px;margin-right:4px;width:30px;height:30px;}"));
    strHeadCSS.append(QString(".DefaultFontColor{color:%1;}").arg(strDefaultFontColor));
    strHeadCSS.append(QString(".JoinFontColor{color:%1;}").arg(strJoinFontColor));
    strHeadCSS.append(QString(".PartFontColor{color:%1;}").arg(strPartFontColor));
    strHeadCSS.append(QString(".QuitFontColor{color:%1;}").arg(strQuitFontColor));
    strHeadCSS.append(QString(".KickFontColor{color:%1;}").arg(strKickFontColor));
    strHeadCSS.append(QString(".ModeFontColor{color:%1;}").arg(strModeFontColor));
    strHeadCSS.append(QString(".NoticeFontColor{color:%1;}").arg(strNoticeFontColor));
    strHeadCSS.append(QString(".InfoFontColor{color:%1;}").arg(strInfoFontColor));
    strHeadCSS.append(QString(".MeFontColor{color:%1;}").arg(strMeFontColor));
    strHeadCSS.append(QString(".ErrorFontColor{color:%1;}").arg(strErrorFontColor));
    strHeadCSS.append(QString(".ChannelFontColor{color:%1;}").arg(strChannelFontColor));

    QWebElement head = this->page()->mainFrame()->findFirstElement("head");
    QWebElement headStyle = head.findFirst("style");
    headStyle.setPlainText(strHeadCSS);
}

void ChatView::clearMessages()
{
    this->page()->mainFrame()->evaluateJavaScript("clearMessages()");
}

void ChatView::displayMessage(QString &strData, MessageCategory eMessageCategory, QString strTime, QString strNick)
{
    QDateTime dt;
    if (!strTime.isEmpty())
        dt = QDateTime::fromTime_t(strTime.toUInt());
    else
        dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("[hh:mm:ss]");
    QString strShortDT = dt.toString("hh:mm");

    if (Core::instance()->settings.value("disable_logs") == "off")
    {
        QString strSaveData;
        if (!strNick.isEmpty())
            strSaveData = QString("%1 <%2> %3").arg(strDT).arg(strNick).arg(strData);
        else
            strSaveData = QString("%1 %2").arg(strDT).arg(strData);

        // fix /me
        QString strRegExpMe = QString("%1ACTION %2%3").arg(QString(QByteArray("\x01"))).arg("(.*)").arg(QString(QByteArray("\x01")));
        if (strSaveData.contains(QRegExp(strRegExpMe)))
            strSaveData.replace(QRegExp(strRegExpMe), "\\1");

        Log *l = new Log();
        l->save(strChatViewChannel, strSaveData);
        delete l;
    }

    if ((eMessageCategory == JoinMessage) || (eMessageCategory == PartMessage) || (eMessageCategory == QuitMessage))
    {
        if (Core::instance()->settings.value("hide_join_part") == "on")
            return;

        int iNickCount = Core::instance()->getUserCount(strChatViewChannel);
        if ((Core::instance()->settings.value("hide_join_part_200") == "on") && (iNickCount > 200))
            return;
    }

    HtmlMessagesRenderer *r = new HtmlMessagesRenderer();
    QString strContent = r->renderer(strDT, strData, eMessageCategory, strShortDT, strNick);
    delete r;

    // scroll
    if (this->page()->mainFrame()->scrollBarValue(Qt::Vertical) ==  this->page()->mainFrame()->scrollBarMaximum(Qt::Vertical))
        bScroll = true;
    else
        bScroll = false;

    // remove first message
    QWebElement document = this->page()->mainFrame()->documentElement();
    QWebElementCollection allDiv = document.findAll("div");
    if (allDiv.count() > 500)
        this->page()->mainFrame()->evaluateJavaScript("removeFirstMessage()");

    // append
    this->page()->mainFrame()->evaluateJavaScript("appendMessage(\'"+strContent+"\')");

    /* in Qt 4.6.3 / WebKit there is a bug making the following call not working */
    /* according to: https://bugs.webkit.org/show_bug.cgi?id=35633 */
    /* the proper refreshing behaviour should occur once the bug is fixed */
    /* possible temporary solution: use QWebElements API to randomly change */
    /* URLs in the HTML/CSS content. */
    this->page()->triggerAction(QWebPage::ReloadAndBypassCache, false);

    // hilight
    if (eMessageCategory == HilightMessage)
    {
        // sound
        if (Core::instance()->settings.value("disable_sounds") == "off")
            Notify::instance()->play(Beep);
    }
}

void ChatView::joinChannel()
{
    Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void ChatView::priv()
{
    Core::instance()->pNetwork->send(QString("PRIV %1").arg(strNick));
}

void ChatView::whois()
{
    Core::instance()->pNetwork->send(QString("WHOIS %1 :%1").arg(strNick));
}

void ChatView::profile()
{
    if (strNick[0] != '~')
        DlgUserProfile(this, strNick).exec();
}

void ChatView::cam()
{
#ifdef Q_WS_WIN
    (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strNick);
#else
    new DlgWebcam(strNick, true);
#endif
}

void ChatView::friendsAdd()
{
    Core::instance()->pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void ChatView::friendsDel()
{
    Core::instance()->pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void ChatView::ignoreAdd()
{
    Core::instance()->pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void ChatView::ignoreDel()
{
    Core::instance()->pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void ChatView::kick()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
        Core::instance()->pNetwork->send(QString("KICK %1 %2 :%3").arg(strChatViewChannel).arg(strNick).arg(strText));
}

void ChatView::ban()
{
    Core::instance()->pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::kban()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick & Ban"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChatViewChannel).arg(strNick));
        Core::instance()->pNetwork->send(QString("KICK %1 %2 :%3").arg(strChatViewChannel).arg(strNick).arg(strText));
    }
}

void ChatView::ipban()
{
    Core::instance()->pNetwork->send(QString("CS BANIP %1 ADD %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::opAdd()
{
    Core::instance()->pNetwork->send(QString("CS OP %1 ADD %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::opDel()
{
    Core::instance()->pNetwork->send(QString("CS OP %1 DEL %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::halfopAdd()
{
    Core::instance()->pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::halfopDel()
{
    Core::instance()->pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::moderatorAdd()
{
    Core::instance()->pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::moderatorDel()
{
    Core::instance()->pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::voiceAdd()
{
    Core::instance()->pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::voiceDel()
{
    Core::instance()->pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChatViewChannel).arg(strNick));
}

void ChatView::invite()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        Core::instance()->pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

void ChatView::watchVideo()
{
    QString strYoutubeLink;

    QRegExp exYoutube_1("youtube.com/watch\\?v=([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_2("youtube.com/v/([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_3("youtu.be/([a-zA-Z0-9_-]{11})");

    if (exYoutube_1.indexIn(strWebsite) > -1)
        strYoutubeLink = "http://www.youtube.com/embed/"+exYoutube_1.cap(1);
    if (exYoutube_2.indexIn(strWebsite) > -1)
        strYoutubeLink = "http://www.youtube.com/embed/"+exYoutube_2.cap(1);
    if (exYoutube_3.indexIn(strWebsite) > -1)
        strYoutubeLink = "http://www.youtube.com/embed/"+exYoutube_3.cap(1);

    if (QUrl(strYoutubeLink).isValid())
        DlgWebBrowser(this, QUrl(strYoutubeLink)).exec();
}

void ChatView::openWebbrowser()
{
    QDesktopServices::openUrl(QUrl(strWebsite, QUrl::TolerantMode));
}

void ChatView::sendToNotes()
{
    QString path;

#ifdef Q_WS_WIN
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    QString strCurrentProfile = Core::instance()->settings.value("current_profile");
    path += "/profiles/"+strCurrentProfile;

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkpath(path);

    QString strNotesFile = path+"/notes.txt";
    QString strContent;

    // read
    QFile fr(strNotesFile);
    if (fr.exists())
    {
        if (fr.open(QIODevice::ReadOnly))
        {
            // read content
            QTextStream ts(&fr);
            strContent = ts.readAll();
            fr.close();
        }
        else
        {
#ifdef Q_WS_X11
            qDebug() << tr("Error: Cannot read notes file!");
#endif
            return;
        }
    }

    // content
    strContent += this->selectedText();

    // save
    QFile f(strNotesFile);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&f);
        out << strContent << "\r\n";

        f.close();
    }
}

void ChatView::search()
{
    DlgFindText(Core::instance()->sccWindow(), this).exec();
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
    QString strMe = Core::instance()->settings.value("nick");
    QString strSelfModes = Core::instance()->getUserModes(strMe, strChatViewChannel);
    int iSelfMaxModes = Core::instance()->getUserMaxModes(strSelfModes);
    QString strNickModes = Core::instance()->getUserModes(strNick, strChatViewChannel);

    QMenu *minvite = new QMenu(tr("Invite"));
    minvite->setIcon(QIcon(":/images/oxygen/16x16/legalmoves.png"));

    for (int i = 0; i < maxOpenChannels; ++i)
    {
        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
        openChannelsActs[i]->setVisible(false);
        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
     }

    for (int i = 0; i < maxOpenChannels; ++i)
        minvite->addAction(openChannelsActs[i]);

    QList<QString> lOpenChannels = Core::instance()->lOpenChannels;
    for (int i = 0; i < lOpenChannels.size(); ++i)
    {
        QString strOpenChannel = lOpenChannels[i];
        if (strOpenChannel[0] == '^')
            strOpenChannel = Core::instance()->convertPrivName(strOpenChannel);

        openChannelsActs[i]->setText(strOpenChannel);
        openChannelsActs[i]->setData(lOpenChannels[i]);
        openChannelsActs[i]->setVisible(true);
    }
    for (int j = lOpenChannels.size(); j < maxOpenChannels; ++j)
        openChannelsActs[j]->setVisible(false);

    QMenu *friends = new QMenu(tr("Friends list"));
    friends->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending.png"));
    if (Core::instance()->mFriends.contains(strNick))
        friends->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from friends"), this, SLOT(friendsDel()));
    else
        friends->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to friends"), this, SLOT(friendsAdd()));

    QMenu *ignore = new QMenu(tr("Ignore list"));
    ignore->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"));
    if (Core::instance()->lIgnore.contains(strNick))
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from Ignore list"), this, SLOT(ignoreDel()));
    else
        ignore->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to Ignore list"), this, SLOT(ignoreAdd()));

    QMenu *privilege = new QMenu(tr("Actions"));
    privilege->setIcon(QIcon(":/images/oxygen/16x16/irc-operator.png"));

    if ((strNickModes.contains("@")) && ((iSelfMaxModes >= 16) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(opDel()));
    else if ((!strNickModes.contains("@")) && (iSelfMaxModes >= 16))
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(opAdd()));

    if ((strNickModes.contains("%")) && ((iSelfMaxModes >= 8) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfopDel()));
    else if ((!strNickModes.contains("%")) && (iSelfMaxModes >= 8))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfopAdd()));

    if ((strNickModes.contains("!")) && ((iSelfMaxModes >= 4) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderatorDel()));
    else if ((!strNickModes.contains("!")) && (iSelfMaxModes >= 4))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderatorAdd()));

    if ((strNickModes.contains("+")) && ((iSelfMaxModes >= 4) || (strNick == strMe)))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Take guest status"), this, SLOT(voiceDel()));
    else if ((!strNickModes.contains("+")) && (iSelfMaxModes >= 4))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Give guest status"), this, SLOT(voiceAdd()));

    QAction *nickAct = new QAction(strNick, this);
    nickAct->setIcon(QIcon(":/images/oxygen/16x16/user-identity.png"));
    nickAct->setDisabled(true);

    QMenu menu;
    menu.addAction(nickAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/list-add-user.png"), tr("Priv"), this, SLOT(priv()));
    menu.addAction(QIcon(":/images/oxygen/16x16/text-field.png"), tr("Whois"), this, SLOT(whois()));
    if (strNick[0] != '~')
    {
        menu.addAction(QIcon(":/images/oxygen/16x16/view-pim-contacts.png"), tr("Profile"), this, SLOT(profile()));
        if ((strNickModes.contains("W")) || (strNickModes.contains("V")))
            menu.addAction(QIcon(":/images/pubcam.png"), tr("Webcam"), this, SLOT(cam()));
    }
    menu.addMenu(minvite);
    if (strSelfModes.contains("r"))
    {
        menu.addMenu(friends);
        menu.addMenu(ignore);
    }
    if (iSelfMaxModes >= 4)
    {
        menu.addSeparator();
        menu.addAction(QIcon(":/images/oxygen/16x16/im-kick-user.png"), tr("Kick From Channel"), this, SLOT(kick()));
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

    QRegExp exYoutube_1("youtube.com/watch\\?v=([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_2("youtube.com/v/([a-zA-Z0-9_-]{11})");
    QRegExp exYoutube_3("youtu.be/([a-zA-Z0-9_-]{11})");

    if ((strWebsite.contains(exYoutube_1)) || (strWebsite.contains(exYoutube_2)) || (strWebsite.contains(exYoutube_3)))
        menu.addAction(QIcon(":/images/oxygen/16x16/tool-animator.png"), tr("Watch video"), this, SLOT(watchVideo()));

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

    QAction *all = this->pageAction(QWebPage::SelectAll);
    all->setIcon(QIcon(":/images/oxygen/16x16/edit-select-all.png"));
    menu.addAction(all);

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
    if (bScroll)
        page()->mainFrame()->setScrollBarValue(Qt::Vertical, page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}
