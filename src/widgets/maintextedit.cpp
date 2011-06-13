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

    update_background_image();

    setMouseTracking(true);
    pAnimatedEmoticonWidget = new AnimatedEmoticonWidget(this);
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(text_changed()));
}

void MainTextEdit::update_background_image()
{
    QSettings settings;
    QString strBackgroundImage = settings.value("background_image").toString();
    QString strDisableBackgroundImage = settings.value("disable_background_image").toString();

    if ((strDisableBackgroundImage == "off") && (!strBackgroundImage.isEmpty()))
        this->setStyleSheet("QTextEdit{background-image: url("+strBackgroundImage+"); background-attachment: fixed; background-position: center; background-repeat: no-repeat; background-color: #ffffff; }");
    else
        this->setStyleSheet("");
}

void MainTextEdit::join_channel()
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
        pDlgUserProfile->set_nick(strNick);
        pDlgUserProfile->show();
    }
}

void MainTextEdit::cam()
{
#ifdef Q_WS_WIN
    (new Kamerzysta(Core::instance()->kamerzystaSocket))->show(strNick);
#else
    new DlgWebcam(strNick, true);
#endif
}

void MainTextEdit::friends_add()
{
    pNetwork->send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void MainTextEdit::friends_del()
{
    pNetwork->send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void MainTextEdit::ignore_add()
{
    pNetwork->send(QString("NS IGNORE ADD %1").arg(strNick));
}

void MainTextEdit::ignore_del()
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

void MainTextEdit::op_add()
{
    pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::op_del()
{
    pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::halfop_add()
{
    pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::halfop_del()
{
    pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::moderator_add()
{
    pNetwork->send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::moderator_del()
{
    pNetwork->send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::voice_add()
{
    pNetwork->send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void MainTextEdit::voice_del()
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

void MainTextEdit::open_webbrowser()
{
    QDesktopServices::openUrl(QUrl(strWebsite, QUrl::TolerantMode));
}

void MainTextEdit::send_to_notes()
{
    QString path;

#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    QSettings winSettings(QSettings::UserScope, "Microsoft", "Windows");
    winSettings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    path = winSettings.value("Personal").toString();
    path += "/scc";
#endif

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    QString strNotesFile = path+"/notes.txt";
    QString strContent;

    // read
    QFile *file = new QFile(strNotesFile);
    if (file->exists())
    {
        if (!file->open(QIODevice::ReadWrite))
        {
#ifdef Q_WS_X11
            qDebug() << tr("Error: Cannot read notes file!");
#endif
            return;
        }

        // set content
        strContent = file->readAll();
    }

    file->close();
    delete file;

    // content
    strContent += this->textCursor().selectedText()+"\r\n";
    QByteArray baContent = strContent.toAscii();

    // save
    QFile *fs = new QFile(strNotesFile);
    fs->open(QIODevice::WriteOnly | QIODevice::Truncate);
    fs->write(baContent);
    fs->close();
    delete fs;
}

void MainTextEdit::search()
{
    DlgFindText(Core::instance()->sccWindow(), this).exec();
}

void MainTextEdit::menu_channel(QString strChannel, QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *nameAct = new QAction(strChannel, &menu);
    nameAct->setIcon(QIcon(":/images/oxygen/16x16/irc-join-channel.png"));
    nameAct->setDisabled(true);

    menu.addAction(nameAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/legalmoves.png"), tr("Join channel"), this, SLOT(join_channel()));
    menu.exec(event->globalPos());
}

void MainTextEdit::menu_nick(QString strNick, QContextMenuEvent *event)
{
    QString strModes = Core::instance()->get_user_modes(strNick, strChannel);

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
    friends->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to friends"), this, SLOT(friends_add()));
    friends->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from friends"), this, SLOT(friends_del()));

    QMenu *ignore = new QMenu(tr("Ignore list"));
    ignore->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"));
    ignore->addAction(QIcon(":/images/oxygen/16x16/list-add.png"), tr("Add to Ignore list"), this, SLOT(ignore_add()));
    ignore->addAction(QIcon(":/images/oxygen/16x16/list-remove.png"), tr("Remove from Ignore list"), this, SLOT(ignore_del()));

    QMenu *privilege = new QMenu(tr("Actions"));
    privilege->setIcon(QIcon(":/images/oxygen/16x16/irc-operator.png"));

    if (strModes.contains("@"))
        privilege->addAction(QIcon(":/images/op.png"), tr("Take super operator status"), this, SLOT(op_del()));
    else
        privilege->addAction(QIcon(":/images/op.png"), tr("Give super operator status"), this, SLOT(op_add()));

    if (strModes.contains("%"))
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Take operator status"), this, SLOT(halfop_del()));
    else
        privilege->addAction(QIcon(":/images/halfop.png"), tr("Give operator status"), this, SLOT(halfop_add()));

    if (strModes.contains("!"))
        privilege->addAction(QIcon(":/images/mod.png"), tr("Take moderator status"), this, SLOT(moderator_del()));
    else
        privilege->addAction(QIcon(":/images/mod.png"), tr("Give moderator status"), this, SLOT(moderator_add()));

    if (strModes.contains("+"))
        privilege->addAction(QIcon(":/images/voice.png"), tr("Take guest status"), this, SLOT(voice_del()));
    else
        privilege->addAction(QIcon(":/images/voice.png"), tr("Give guest status"), this, SLOT(voice_add()));

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
        if ((strModes.contains("W")) || (strModes.contains("V")))
            menu.addAction(QIcon(":/images/pubcam.png"), tr("Webcam"), this, SLOT(cam()));
    }
    menu.addMenu(minvite);
    menu.addMenu(friends);
    menu.addMenu(ignore);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/im-kick-user.png"), tr("Kick From Channel"), this, SLOT(kick()));
    menu.addAction(QIcon(":/images/oxygen/16x16/im-ban-user.png"), tr("Ban From Channel"), this, SLOT(ban()));
    menu.addAction(QIcon(":/images/oxygen/16x16/im-ban-kick-user.png"), tr("Kick & Ban"), this, SLOT(kban()));
    menu.addAction(QIcon(":/images/oxygen/16x16/im-user-busy.png"), tr("IP Ban"), this, SLOT(ipban()));
    menu.addSeparator();
    menu.addMenu(privilege);

    menu.exec(event->globalPos());
}

void MainTextEdit::menu_website(QContextMenuEvent *event)
{
    QString strShortLink = strWebsite;
    if (strShortLink.size() > 40) strShortLink = strShortLink.left(20)+"..."+strShortLink.right(20);

    QMenu menu(this);

    QAction *websiteLinkAct = new QAction(strShortLink, this);
    websiteLinkAct->setIcon(QIcon(":/images/oxygen/16x16/preferences-web-browser-shortcuts.png"));
    websiteLinkAct->setDisabled(true);

    menu.addAction(websiteLinkAct);
    menu.addSeparator();
    menu.addAction(QIcon(":/images/oxygen/16x16/applications-internet.png"), tr("Open link"), this, SLOT(open_webbrowser()));

    menu.exec(event->globalPos());
}

void MainTextEdit::menu_standard(QContextMenuEvent *event)
{
    QMenu menu(this);

    if (!this->textCursor().selectedText().isEmpty())
    {
        QAction *sendToNotes = new QAction(QIcon(":/images/oxygen/16x16/story-editor.png"), tr("Send to notes"), &menu);
        connect(sendToNotes, SIGNAL(triggered()), this, SLOT(send_to_notes()));
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

int MainTextEdit::get_word_index(QString strLine, int iPos)
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

QString MainTextEdit::get_word(QTextCursor cursor)
{
    // get pos
    int iPos = cursor.position() - cursor.block().position(); // cursor.positionInBlock()
    // get line
    cursor.select(QTextCursor::BlockUnderCursor);
    QString strBlock = cursor.selectedText().trimmed();
    QStringList strlBlock = strBlock.split(" ");

    int index = get_word_index(strBlock, iPos);
    if (index != -1)
        return strlBlock.at(index);
    else
        return "";
}

QString MainTextEdit::get_first_word(QTextCursor cursor)
{
    // get line
    cursor.select(QTextCursor::BlockUnderCursor);
    QString strBlock = cursor.selectedText().trimmed();
    QStringList strlBlock = strBlock.split(" ");

    return strlBlock.at(1);
}

void MainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    if (this->textCursor().selectedText().isEmpty()) // if nothing selected
    {
        QTextCursor cursor = QTextEdit::cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        if (!cursor.selectedText().isEmpty())
        {
            int iPos = cursor.position() - cursor.block().position(); // cursor.positionInBlock()
            QString strFirstWord = get_first_word(cursor);
            QString strWord = get_word(cursor);

            if ((!strFirstWord.isEmpty()) && (!strWord.isEmpty()))
            {
                // channel
                if (strWord.at(0) == '#')
                {
                    strChannel = strWord;
                    menu_channel(strChannel, event);
                    return;
                }

                // nick
                if ((iPos > 11) && (iPos < 11+strFirstWord.length()))
                {
                    QString strFixedWord = strWord.mid(1,strWord.length()-2);
                    QString strFixedFirstWord = strFirstWord.mid(1,strFirstWord.length()-2);

                    if (strFixedWord == strFixedFirstWord)
                    {
                        strNick = strFixedWord;
                        menu_nick(strNick, event);
                        return;
                    }
                }

                // website
                if ((strWord.contains("http")) || (strWord.contains("www")))
                {
                    strWebsite = strWord;
                    menu_website(event);
                    return;
                }
            }
        }
    }

    menu_standard(event);
}

void MainTextEdit::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QTextCursor cursor = cursorForPosition(pos);
    QTextFormat format = cursor.charFormat();

    if (!format.isImageFormat())
    {
        pAnimatedEmoticonWidget->stop_emoticon();
        QTextEdit::mouseMoveEvent(event);
    }
    else
    {
        QString strName = format.toImageFormat().name();
        int x = pos.x();
        int y = pos.y();
        pAnimatedEmoticonWidget->start_emoticon(strName, x, y);
    }

    QTextEdit::mouseMoveEvent(event);
}

void MainTextEdit::text_changed()
{
    pAnimatedEmoticonWidget->stop_emoticon();
}
