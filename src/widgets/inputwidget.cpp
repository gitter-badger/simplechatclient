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
#include "highlighter.h"
#include "inputlinewidget.h"
#include "log.h"
#include "network.h"
#include "replace.h"
#include "inputwidget.h"

InputWidget::InputWidget(QWidget *parent, Network *param1, sChannelList *param2) : QWidget(parent)
{
    pNetwork = param1;
    stlChannelList = param2;

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

    // highlighter
    pHighlighter = new Highlighter(pInputLine->document());

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputline_return_pressed()));
    QObject::connect(pInputLine, SIGNAL(returnPressed()), this, SLOT(inputline_return_pressed()));
    QObject::connect(pInputLine, SIGNAL(rehighlight()), pHighlighter, SLOT(rehighlight()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moder_button_clicked()));
    QObject::connect(showHideToolWidget, SIGNAL(clicked()), this, SLOT(show_hide_toolwidget_clicked()));
}

void InputWidget::set_active(QString strName)
{
    strChannel = strName;
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

void InputWidget::set_userslist(QTreeWidget *treeWidget)
{
    QList <QString> usersList;

    for (int i = 0; i < treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *parent_item = treeWidget->topLevelItem(i);
        for (int x = 0; x < parent_item->childCount(); x++)
        {
            QTreeWidgetItem *child_item = parent_item->child(x);
            QString strChild = child_item->text(0);

            if (usersList.contains(strChild) == false) // add if not exist
                usersList.append(strChild);
        }
    }

    pInputLine->set_userslist(usersList);
}

void InputWidget::set_moderation(bool m)
{
    if (m == true)
        moderSendButton->show();
    else
        moderSendButton->hide();
}

void InputWidget::set_toolwidget_icon(bool bShowHide)
{
    if (bShowHide == true)
        showHideToolWidget->setIcon(QIcon(":/images/oxygen/16x16/text-frame-unlink.png"));
    else
        showHideToolWidget->setIcon(QIcon(":/images/oxygen/16x16/text-frame-link.png"));
}

QString InputWidget::convert_emots(QString strData)
{
    strData.replace(QRegExp("(http:|https:)//"), "\\1\\\\"); // fix http/s
    strData.replace(QRegExp("//([a-zA-Z0-9_-]+)\\b"), "%I\\1%");
    strData.replace(QRegExp("(http:|https:)\\\\\\\\"), "\\1//"); // fix http/s
    return strData;
}

QString InputWidget::replace_emots(QString strData)
{
    Replace *pReplace = new Replace();
    strData = pReplace->replace_emots(strData);
    delete pReplace;

    return strData;
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
    if (strText.isEmpty() == true) return; // empty!

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

        Commands *pCommands = new Commands(stlChannelList, strChannel, strText);
        strText = pCommands->execute();
        delete pCommands;

        // help
        if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
        {
            QStringList strlHelp = strText.split(";");
            for (int i = 0; i < strlHelp.size(); i++)
                emit show_msg(strChannel, strlHelp.at(i), 7);
        }
        // me
        else if (strTextList[0] == "me")
        {
            if (strTextOriginal.length() > 3)
            {
                QString strTextSend = strText;
                QString strTextDisplay = strTextOriginal.right(strTextOriginal.length()-3);

                QString weight;
                QString font = strFontFamily.toLower();

                if (bMyBold == true) weight += "b";
                if (bMyItalic == true) weight += "i";

                if (font == "verdana")
                    font = "";
                if ((strCurrentColor != "#000000") && (strCurrentColor.isEmpty() == false))
                    strTextDisplay = "%C"+strCurrentColor.right(6)+"%"+strTextDisplay;
                if (font.isEmpty() == false)
                    font = ":"+font;
                if ((weight.isEmpty() == false) || (font.isEmpty() == false))
                    strTextDisplay = "%F"+weight+font+"%"+strTextDisplay;

                strTextSend = convert_emots(strTextSend);
                strTextSend = replace_emots(strTextSend);
                strTextDisplay = convert_emots(strTextDisplay);
                strTextDisplay = replace_emots(strTextDisplay);

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                QSettings settings;
                if (settings.value("disable_logs").toString() == "off")
                {
                    Log *l = new Log();
                    l->save(strChannel, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strTextDisplay));
                    delete l;
                }

                pNetwork->send(strTextSend);
                emit display_message(strChannel, QString("%1<%2> %3ACTION %4%5").arg(strDT).arg(strMe).arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01"))), 0);
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
        QString weight;
        QString font = strFontFamily.toLower();

        if (bMyBold == true) weight += "b";
        if (bMyItalic == true) weight += "i";

        if (font == "verdana")
            font = "";
        if ((strCurrentColor != "#000000") && (strCurrentColor.isEmpty() == false))
            strText = "%C"+strCurrentColor.right(6)+"%"+strText;
        if (font.isEmpty() == false)
            font = ":"+font;
        if ((weight.isEmpty() == false) || (font.isEmpty() == false))
            strText = "%F"+weight+font+"%"+strText;

        strText = convert_emots(strText);
        strText = replace_emots(strText);

        QDateTime dt = QDateTime::currentDateTime();
        QString strDT = dt.toString("[hh:mm:ss] ");

        // standard text
        if (bModeration == false)
        {
            if (settings.value("disable_logs").toString() == "off")
            {
                Log *l = new Log();
                l->save(strChannel, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText));
                delete l;
            }

            strText = QString("PRIVMSG %1 :%2").arg(strChannel).arg(strText);
            pNetwork->send(strText);
            emit display_message(strChannel, QString("%1<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-10-strChannel.length())), 0);
        }
        // moder notice
        else if (bModeration == true)
        {
            if (settings.value("disable_logs").toString() == "off")
            {
                Log *l = new Log();
                l->save(strChannel, QString("%1 *<%2> %3").arg(strDT).arg(strMe).arg(strText));
                delete l;
            }

            strText = QString("MODERNOTICE %1 :%2").arg(strChannel).arg(strText);
            pNetwork->send(strText);
            emit display_message(strChannel, QString("%1 *<%2> %3").arg(strDT).arg(strMe).arg(strText.right(strText.length()-14-strChannel.length())), 6);
        }
    }
}

void InputWidget::update_nick(QString strNick)
{
    nickLabel->setText(QString("<b>%1</b>").arg(strNick));
}

void InputWidget::inputline_return_pressed()
{
    QString strText = pInputLine->toPlainText().trimmed();
    paste_multi_line(strText, false);
    pInputLine->clear();
}

void InputWidget::moder_button_clicked()
{
    QString strText = pInputLine->toPlainText().trimmed();
    paste_multi_line(strText, true);
    pInputLine->clear();
}

void InputWidget::show_hide_toolwidget_clicked()
{
    emit show_hide_toolwidget();
}
