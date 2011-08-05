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
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QSettings>
#include <QTextBlock>
#include <QUrl>
#include "animatedemoticonwidget.h"
#include "core.h"
#include "dlg_find_text.h"
#include "dlg_user_profile.h"
#include "network.h"
#include "maintextedit.h"

#ifdef Q_WS_WIN
    #include "kamerzysta.h"
#else
    #include "dlg_webcam.h"
#endif

MainTextEdit::MainTextEdit(Network *param1, QString param2, DlgUserProfile *param3)
{
    pNetwork = param1;
    strChannel = param2;
    pDlgUserProfile = param3;

    strNick = QString::null;

    updateBackgroundImage();

    setMouseTracking(true);
    pAnimatedEmoticonWidget = new AnimatedEmoticonWidget(this);
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(textChanged()));
}

MainTextEdit::~MainTextEdit()
{
    setMouseTracking(false);
    pAnimatedEmoticonWidget->stopEmoticon();
    delete pAnimatedEmoticonWidget;
}

void MainTextEdit::updateBackgroundImage()
{
    QSettings settings;
    QString strBackgroundImage = settings.value("background_image").toString();
    QString strDisableBackgroundImage = settings.value("disable_background_image").toString();

    if ((strDisableBackgroundImage == "off") && (!strBackgroundImage.isEmpty()))
        this->setStyleSheet("QTextEdit{background-image: url("+strBackgroundImage+"); background-attachment: fixed; background-position: center; background-repeat: no-repeat; background-color: #ffffff; }");
    else
        this->setStyleSheet("");
}

void MainTextEdit::joinChannel()
{
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void MainTextEdit::priv()
{
    pNetwork->send(QString("PRIV %1").arg(strNick));
}

void MainTextEdit::whois()
{
    pNetwork->send(QString("WHOIS %1 :%1").arg(strNick));
}

void MainTextEdit::profile()
{
    if (strNick[0] != '~')
    {
        pDlgUserProfile->setNick(strNick);
        pDlgUserProfile->show();
    }
}

void MainTextEdit::cam()
{
#ifdef Q_WS_WIN
    (new Kamerzysta(Core::instance()->kamerzystaSocket, pNetwork))->show(strNick);
#else
    new DlgWebcam(strNick, true);
#endif
}

void MainTextEdit::friendsAdd()
{
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void MainTextEdit::friendsDel()
{
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void MainTextEdit::ignoreAdd()
{
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void MainTextEdit::ignoreDel()
{
    pNetwork->send(QString("NS IGNORE DEL %1").arg(strNick));
}

void MainTextEdit::kick()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick From Channel"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
}

void MainTextEdit::ban()
{
    pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::kban()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Kick & Ban"), tr("Reason for kicking:"), QLineEdit::Normal, tr("No reason"), &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
        pNetwork->send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strText));
    }
}

void MainTextEdit::ipban()
{
    pNetwork->send(QString("CS BANIP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::opAdd()
{
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::opDel()
{
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::halfopAdd()
{
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::halfopDel()
{
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::moderatorAdd()
{
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::moderatorDel()
{
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::voiceAdd()
{
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::voiceDel()
{
    pNetwork->send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::invite()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        pNetwork->send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

void MainTextEdit::openWebbrowser()
{
    QDesktopServices::openUrl(QUrl(strWebsite, QUrl::TolerantMode));
}

void MainTextEdit::sendToNotes()
{
    QString path;

#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#endif

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    QString strNotesFile = path+"/notes.txt";
    QString strContent;

    // read
    QFile fr(strNotesFile);
    if (fr.exists())
    {
        if (!fr.open(QIODevice::ReadOnly))
        {
#ifdef Q_WS_X11
            qDebug() << tr("Error: Cannot read notes file!");
#endif
            return;
        }

        // read content
        QTextStream ts(&fr);
        strContent = ts.readAll();
    }

    fr.close();

    // content
    strContent += this->textCursor().selectedText();

    // save
    QFile f(strNotesFile);
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream out(&f);
    out << strContent << "\r\n";

    f.close();
}

void MainTextEdit::search()
{
    DlgFindText(Core::instance()->sccWindow(), this).exec();
}

void MainTextEdit::menuChannel(QString strChannel, QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *nameAct = new QAction(strChannel, &menu);
    nameAct->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    nameAct->setDisabled(true);

    menu.addAction(nameAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/legalmoves.png"), tr("Join channel"), this, SLOT(joinChannel()));
    menu.exec(event->globalPos());
}

void MainTextEdit::menuNick(QString strNick, QContextMenuEvent *event)
{
    QSettings settings;
    QString strMe = settings.value("nick").toString();
    QString strSelfModes = Core::instance()->getUserModes(strMe, strChannel);
    int iSelfMaxModes = Core::instance()->getUserMaxModes(strMe, strChannel);
    QString strNickModes = Core::instance()->getUserModes(strNick, strChannel);

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
        openChannelsActs[i]->setText(lOpenChannels[i]);
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

    QMenu menu(this);
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

void MainTextEdit::menuWebsite(QContextMenuEvent *event)
{
    QString strShortLink = strWebsite;
    if (strShortLink.size() > 40) strShortLink = strShortLink.left(20)+"..."+strShortLink.right(20);

    QMenu menu(this);

    QAction *websiteLinkAct = new QAction(strShortLink, this);
    websiteLinkAct->setIcon(QIcon(":/images/oxygen/16x16/preferences-web-browser-shortcuts.png"));
    websiteLinkAct->setDisabled(true);

    menu.addAction(websiteLinkAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/applications-internet.png"), tr("Open link"), this, SLOT(openWebbrowser()));

    menu.exec(event->globalPos());
}

void MainTextEdit::menuStandard(QContextMenuEvent *event)
{
    QMenu menu(this);

    if (!this->textCursor().selectedText().isEmpty())
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

    if (!this->textCursor().selectedText().isEmpty())
    {
        QAction *copy = new QAction(QIcon(":/images/oxygen/16x16/edit-copy.png"), tr("Copy"), &menu);
        copy->setShortcut(QKeySequence::Copy);
        connect(copy, SIGNAL(triggered()), this, SLOT(copy()));
        menu.addAction(copy);
    }

    QAction *all = new QAction(QIcon(":/images/oxygen/16x16/edit-select-all.png"), tr("Select All"), &menu);
    all->setShortcut(QKeySequence::SelectAll);
    connect(all, SIGNAL(triggered()), this, SLOT(selectAll()));
    menu.addAction(all);

    menu.exec(event->globalPos());
}

int MainTextEdit::getWordIndex(QString strLine, int iPos)
{
    strLine +=" ";
    int iCount = strLine.count(" ");
    int iLast = 0;
    for (int i = 0; i < iCount; i++)
    {
        int iCurrentPos = strLine.indexOf(" ", iLast);

        if (iCurrentPos >= iPos)
            return i;

        iLast = iCurrentPos+1;
    }
    return -1;
}

QString MainTextEdit::getWord(QTextCursor cursor)
{
    // get pos
    int iPos = cursor.position() - cursor.block().position(); // cursor.positionInBlock()
    // get line
    cursor.select(QTextCursor::BlockUnderCursor);
    QString strBlock = cursor.selectedText().trimmed();
    QStringList strlBlock = strBlock.split(" ");

    int index = getWordIndex(strBlock, iPos);
    if (index != -1)
        return strlBlock.at(index);
    else
        return "";
}

QString MainTextEdit::getWordN(QTextCursor cursor, int n)
{
    // get line
    cursor.select(QTextCursor::BlockUnderCursor);
    QString strBlock = cursor.selectedText().trimmed();
    QStringList strlBlock = strBlock.split(" ");

    return strlBlock.at(n);
}

int MainTextEdit::getWordPosIndex(QTextCursor cursor)
{
    // get pos
    int iPos = cursor.position() - cursor.block().position(); // cursor.positionInBlock()
    // get line
    cursor.select(QTextCursor::BlockUnderCursor);
    QString strBlock = cursor.selectedText().trimmed();

    return getWordIndex(strBlock, iPos);
}

bool MainTextEdit::isJoinPartQuit(QTextCursor cursor)
{
    // get line
    cursor.select(QTextCursor::BlockUnderCursor);
    QString strBlock = cursor.selectedText().trimmed();

    if ((strBlock.contains(tr("has joined"))) || (strBlock.contains(tr("has joined priv"))) || (strBlock.contains(tr("has left"))) || (strBlock.contains(tr("has left priv"))) || (strBlock.contains(tr("has quit"))) || (strBlock.contains(tr("has been kicked"))))
        return true;
    else
        return false;
}

void MainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    if (this->textCursor().selectedText().isEmpty()) // if nothing selected
    {
        QTextCursor cursor = QTextEdit::cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        if (!cursor.selectedText().isEmpty())
        {
            int iWordPos = getWordPosIndex(cursor);
            QString strFirstWord = getWordN(cursor, 1);
            QString strWord = getWord(cursor);

            if ((!strFirstWord.isEmpty()) && (!strWord.isEmpty()))
            {
                // website
                if ((strWord.contains("http")) || (strWord.contains("www.")))
                {
                    strWebsite = strWord;
                    menuWebsite(event);
                    return;
                }

                // channel
                if (strWord.at(0) == '#')
                {
                    strChannel = strWord;
                    menuChannel(strChannel, event);
                    return;
                }

                // <nick>
                if (iWordPos == 1)
                {
                    if ((strWord.contains("<")) && (strWord.contains(">")))
                    {
                        strWord.remove("<"); strWord.remove(">");
                        strNick = strWord;
                        menuNick(strNick, event);
                        return;
                    }
                }

                // nick [join,part,quit,kick]
                if (iWordPos == 2)
                {
                    QString strStar = getWordN(cursor, 1);

                    if ((strStar == "*") && (isJoinPartQuit(cursor)))
                    {
                        strNick = strWord;
                        menuNick(strNick, event);
                        return;
                    }
                }
            }
        }
    }

    menuStandard(event);
}

void MainTextEdit::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QTextCursor cursor = cursorForPosition(pos);
    QTextFormat format = cursor.charFormat();

    if (!format.isImageFormat())
    {
        pAnimatedEmoticonWidget->stopEmoticon();
        QTextEdit::mouseMoveEvent(event);
    }
    else
    {
        QString strName = format.toImageFormat().name();
        int x = pos.x();
        int y = pos.y();
        pAnimatedEmoticonWidget->startEmoticon(strName, x, y);
    }

    QTextEdit::mouseMoveEvent(event);
}

void MainTextEdit::textChanged()
{
    pAnimatedEmoticonWidget->stopEmoticon();
}
