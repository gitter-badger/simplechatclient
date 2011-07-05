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

#include <QComboBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include "commands.h"
#include "core.h"
#include "inputlinewidget.h"
#include "log.h"
#include "network.h"
#include "replace.h"
#include "inputwidget.h"

InputWidget::InputWidget(QWidget *parent, Network *param1) : QWidget(parent)
{
    pNetwork = param1;

    nickLabel = new QLabel(parent);
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(tr("(Unregistered)")));
    nickLabel->show();

    pInputLine = new InputLineWidget(parent);
    pInputLine->setMinimumWidth(350);
    pInputLine->setFont(QFont("Verdana", -1, -1, false));
    pInputLine->show();

    sendButton = new QPushButton(QIcon(":/images/oxygen/16x16/go-next.png"), tr("Send"), parent);
    sendButton->setToolTip(tr("Send"));
    sendButton->setMaximumWidth(85);
    sendButton->setMaximumHeight(25);
    sendButton->show();

    moderSendButton = new QPushButton(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Send to moderators"), parent);
    moderSendButton->setToolTip(tr("Send to moderators"));
    moderSendButton->setMaximumHeight(25);
    moderSendButton->show();

    showHideToolWidget = new QPushButton(QIcon(":/images/oxygen/16x16/text-frame-unlink.png"), "", parent);
    showHideToolWidget->setToolTip(tr("Show/Hide tool widget"));
    showHideToolWidget->setMaximumWidth(25);
    showHideToolWidget->setMaximumHeight(25);
    showHideToolWidget->show();

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(nickLabel);
    mainLayout->addWidget(pInputLine);
    mainLayout->addWidget(sendButton);
    mainLayout->addWidget(moderSendButton);
    mainLayout->addWidget(showHideToolWidget);
    setLayout(mainLayout);

    // default hidden
    moderSendButton->hide();

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputline_return_pressed()));
    QObject::connect(pInputLine, SIGNAL(returnPressed()), this, SLOT(inputline_return_pressed()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moder_button_clicked()));
    QObject::connect(showHideToolWidget, SIGNAL(clicked()), this, SLOT(show_hide_toolwidget_clicked()));
    QObject::connect(pInputLine, SIGNAL(ctrlTabPressed()), this, SLOT(ctrl_tab_pressed()));
    QObject::connect(pInputLine, SIGNAL(ctrlShiftTabPressed()), this, SLOT(ctrl_shift_tab_pressed()));
}

void InputWidget::set_active(QString strName)
{
    strChannel = strName;

    // update nicklist
    QList<QString> usersList = Core::instance()->get_nicks_from_channel(strChannel);
    pInputLine->set_userslist(usersList);
}

void InputWidget::update_nicklist()
{
    QList<QString> usersList = Core::instance()->get_nicks_from_channel(strChannel);
    pInputLine->set_userslist(usersList);
}

void InputWidget::insert_text(QString strText)
{
    pInputLine->insert_text(strText);
}

void InputWidget::set_font(QFont font)
{
    pInputLine->setFont(font);
}

void InputWidget::set_color(QString color)
{
    pInputLine->setStyleSheet(QString("color:%1").arg(color));
}

void InputWidget::set_moderation(bool value)
{
    if (value)
        moderSendButton->show();
    else
        moderSendButton->hide();
}

void InputWidget::set_toolwidget_icon(bool bShowHide)
{
    if (bShowHide)
        showHideToolWidget->setIcon(QIcon(":/images/oxygen/16x16/text-frame-unlink.png"));
    else
        showHideToolWidget->setIcon(QIcon(":/images/oxygen/16x16/text-frame-link.png"));
}

void InputWidget::paste_multi_line(QString strText, bool bModeration)
{
    QStringList list = strText.split(QRegExp("(\n|\r)"));
    int len = 400;

    for (int i = 0; i < list.size(); i++)
    {
        QString line = list.at(i);
        if (line.size() > len)
        {
            while (line.size() > len)
            {
                QString short_line = line.left(len);
                send_message(short_line, bModeration);
                line.remove(0, len);
            }
        }
        if ((line.size() < len) && (line.size() != 0))
            send_message(line, bModeration);
    }
}

void InputWidget::send_message(QString strText, bool bModeration)
{
    if (strText.isEmpty()) return; // empty text!
    if (strChannel.isEmpty()) return; // empty channel!

    QString strTextOriginal = strText;

    QSettings settings;
    QString strMe = settings.value("nick").toString();
    QString strCurrentColor = settings.value("my_color").toString();
    QString strFontFamily = settings.value("my_font").toString();
    bool bMyBold = settings.value("my_bold").toString() == "on" ? true : false;
    bool bMyItalic = settings.value("my_italic").toString() == "on" ? true : false;

    // if command
    if ((strText[0] == '/') && (strText[1] != '/'))
    {
        if (strText[0] == '/')
            strText = strText.right(strText.length()-1);
        strTextOriginal = strText;
        QStringList strTextList = strText.split(" ");

        Commands *pCommands = new Commands(strChannel, strText);
        strText = pCommands->execute();
        delete pCommands;

        // help
        if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
        {
            QStringList strlHelp = strText.split(";");
            for (int i = 0; i < strlHelp.size(); i++)
            {
                QString strDisplay = strlHelp.at(i);
                emit show_msg(strChannel, strDisplay, InfoMessage);
            }
        }
        else if ((strTextList[0] == "mp3") || (strTextList[0] == "winamp"))
        {
            pNetwork->send(QString("PRIVMSG %1 :%2").arg(strChannel).arg(strText));

            QDateTime dt = QDateTime::currentDateTime();
            QString strDT = dt.toString("[hh:mm:ss] ");

            if (settings.value("disable_logs").toString() == "off")
            {
                Log *l = new Log();
                QString strSave = QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText);
                l->save(strChannel, strSave);
                delete l;
            }

            QString strDisplay = QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText);
            emit display_message(strChannel, strDisplay, DefaultMessage);
        }
        // me
        else if (strTextList[0] == "me")
        {
            if (strChannel == "Status") return; // return if status

            if (strTextOriginal.length() > 3)
            {
                QString strTextSend = strText;
                QString strTextDisplay = strTextOriginal.right(strTextOriginal.length()-3);

                QString weight;
                QString font = strFontFamily.toLower();

                if (bMyBold) weight += "b";
                if (bMyItalic) weight += "i";

                if (font == "verdana")
                    font = "";
                if ((strCurrentColor != "#000000") && (!strCurrentColor.isEmpty()))
                    strTextDisplay = "%C"+strCurrentColor.right(6)+"%"+strTextDisplay;
                if (!font.isEmpty())
                    font = ":"+font;
                if ((!weight.isEmpty()) || (!font.isEmpty()))
                    strTextDisplay = "%F"+weight+font+"%"+strTextDisplay;

                Replace *pReplace = new Replace();
                pReplace->convert_and_replace_emots(strTextSend);
                pReplace->convert_and_replace_emots(strTextDisplay);
                delete pReplace;

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                QSettings settings;
                if (settings.value("disable_logs").toString() == "off")
                {
                    Log *l = new Log();
                    QString strSave = QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strTextDisplay);
                    l->save(strChannel, strSave);
                    delete l;
                }

                pNetwork->send(strTextSend);
                QString strDisplay = QString("%1<%2> %3ACTION %4%5").arg(strDT).arg(strMe).arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01")));
                emit display_message(strChannel, strDisplay, MeMessage);
            }
        }
        // other command
        else
        {
            if (strText.length() > 0)
                pNetwork->send(strText);
        }
    }
    else
    {
        if (strChannel == "Status") return; // return if status

        QString weight;
        QString font = strFontFamily.toLower();

        if (bMyBold) weight += "b";
        if (bMyItalic) weight += "i";

        if (font == "verdana")
            font = "";
        if ((strCurrentColor != "#000000") && (!strCurrentColor.isEmpty()))
            strText = "%C"+strCurrentColor.right(6)+"%"+strText;
        if (!font.isEmpty())
            font = ":"+font;
        if ((!weight.isEmpty()) || (!font.isEmpty()))
            strText = "%F"+weight+font+"%"+strText;

        Replace *pReplace = new Replace();
        pReplace->convert_and_replace_emots(strText);
        delete pReplace;

        QDateTime dt = QDateTime::currentDateTime();
        QString strDT = dt.toString("[hh:mm:ss] ");

        // standard text
        if (!bModeration)
        {
            if (settings.value("disable_logs").toString() == "off")
            {
                Log *l = new Log();
                QString strSave = QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText);
                l->save(strChannel, strSave);
                delete l;
            }

            strText = QString("PRIVMSG %1 :%2").arg(strChannel).arg(strText);
            pNetwork->send(strText);
            QString strDisplay = QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-10-strChannel.length()));
            emit display_message(strChannel, strDisplay, DefaultMessage);
        }
        // moder notice
        else if (bModeration)
        {
            if (settings.value("disable_logs").toString() == "off")
            {
                Log *l = new Log();
                QString strSave = QString("%1*<%2> %3").arg(strDT).arg(strMe).arg(strText);
                l->save(strChannel, strSave);
                delete l;
            }

            strText = QString("MODERNOTICE %1 :%2").arg(strChannel).arg(strText);
            pNetwork->send(strText);
            QString strDisplay = QString("%1*<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-14-strChannel.length()));
            emit display_message(strChannel, strDisplay, NoticeMessage);
        }
    }
}

void InputWidget::update_nick(QString strNick)
{
    nickLabel->setText(QString("<b>%1</b>").arg(strNick));
}

void InputWidget::inputline_return_pressed()
{
    QSettings settings;

    // update last active
    QDateTime cdt = QDateTime::currentDateTime();
    int t = (int)cdt.toTime_t(); // seconds that have passed since 1970
    settings.setValue("last_active", QString::number(t));

    // disable away
    bool bAway = settings.value("away").toString() == "on" ? true : false;
    if (bAway)
        pNetwork->send("AWAY :");

    // text
    QString strText = pInputLine->text().trimmed();
    paste_multi_line(strText, false);
    pInputLine->clear();
}

void InputWidget::moder_button_clicked()
{
    QSettings settings;

    // update last active
    QDateTime cdt = QDateTime::currentDateTime();
    int t = (int)cdt.toTime_t(); // seconds that have passed since 1970
    settings.setValue("last_active", QString::number(t));

    // disable away
    bool bAway = settings.value("away").toString() == "on" ? true : false;
    if (bAway)
        pNetwork->send("AWAY :");

    // text
    QString strText = pInputLine->text().trimmed();
    paste_multi_line(strText, true);
    pInputLine->clear();
}

void InputWidget::show_hide_toolwidget_clicked()
{
    emit show_hide_toolwidget();
}

void InputWidget::ctrl_tab_pressed()
{
    emit ctrlTabPressed();
}

void InputWidget::ctrl_shift_tab_pressed()
{
    emit ctrlShiftTabPressed();
}
