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
#include "commands.h"
#include "core.h"
#include "inputline_widget.h"
#include "replace.h"
#include "input_widget.h"

InputWidget::InputWidget(QWidget *parent) : QWidget(parent)
{
    nickLabel = new QLabel(parent);
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(tr("(Unregistered)")));
    nickLabel->show();

    pInputLine = new InputLineWidget(parent);
    pInputLine->setMinimumWidth(350);
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

    // focus
    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(pInputLine);

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputlineReturnPressed()));
    QObject::connect(pInputLine, SIGNAL(returnPressed()), this, SLOT(inputlineReturnPressed()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moderButtonClicked()));
    QObject::connect(showHideToolWidget, SIGNAL(clicked()), this, SLOT(showHideToolwidgetClicked()));
    QObject::connect(pInputLine, SIGNAL(ctrlTabPressed()), this, SLOT(slotCtrlTabPressed()));
    QObject::connect(pInputLine, SIGNAL(ctrlShiftTabPressed()), this, SLOT(slotCtrlShiftTabPressed()));
}

InputWidget::~InputWidget()
{
    delete pInputLine;
}

void InputWidget::insertText(QString strText)
{
    pInputLine->insertText(strText);
}

void InputWidget::setFont(QFont font)
{
    pInputLine->setFont(font);
}

void InputWidget::setColor(QString color)
{
    pInputLine->setStyleSheet(QString("color:%1").arg(color));
}

void InputWidget::setModeration(bool value)
{
    if (value)
        moderSendButton->show();
    else
        moderSendButton->hide();
}

void InputWidget::setToolwidgetIcon(bool bShowHide)
{
    if (bShowHide)
        showHideToolWidget->setIcon(QIcon(":/images/oxygen/16x16/text-frame-unlink.png"));
    else
        showHideToolWidget->setIcon(QIcon(":/images/oxygen/16x16/text-frame-link.png"));
}

void InputWidget::pasteMultiLine(QString strText, bool bModeration)
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
                sendMessage(short_line, bModeration);
                line.remove(0, len);
            }
        }
        if ((line.size() < len) && (line.size() != 0))
            sendMessage(line, bModeration);
    }
}

void InputWidget::sendMessage(QString strText, bool bModeration)
{
    if (strText.isEmpty()) return; // empty text!
    QString strChannel = Core::instance()->getCurrentChannelName();

    QString strTextOriginal = strText;

    QString strMe = Core::instance()->settings.value("nick");
    QString strCurrentColor = Core::instance()->settings.value("my_color");
    QString strFontFamily = Core::instance()->settings.value("my_font");
    bool bMyBold = Core::instance()->settings.value("my_bold") == "on" ? true : false;
    bool bMyItalic = Core::instance()->settings.value("my_italic") == "on" ? true : false;

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
                emit showMsg(strChannel, strDisplay, InfoMessage);
            }
        }
        else if ((strTextList[0] == "mp3") || (strTextList[0] == "winamp"))
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

            Core::instance()->pNetwork->send(QString("PRIVMSG %1 :%2").arg(strChannel).arg(strText));
            QString strDisplay = QString("<%1> %2").arg(strMe).arg(strText);
            emit displayMessage(strChannel, strDisplay, DefaultMessage);
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
                pReplace->convertAndReplaceEmots(strTextSend);
                pReplace->convertAndReplaceEmots(strTextDisplay);
                delete pReplace;

                Core::instance()->pNetwork->send(strTextSend);
                QString strDisplay = QString("<%1> %2ACTION %3%4").arg(strMe).arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01")));
                emit displayMessage(strChannel, strDisplay, MeMessage);
            }
        }
        // other command
        else
        {
            if (strText.length() > 0)
                Core::instance()->pNetwork->send(strText);
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
        pReplace->convertAndReplaceEmots(strText);
        delete pReplace;

        // standard text
        if (!bModeration)
        {
            strText = QString("PRIVMSG %1 :%2").arg(strChannel).arg(strText);
            Core::instance()->pNetwork->send(strText);
            QString strDisplay = QString("<%1> %2").arg(strMe).arg(strText.right(strText.length()-10-strChannel.length()));
            emit displayMessage(strChannel, strDisplay, DefaultMessage);
        }
        // moder notice
        else if (bModeration)
        {
            strText = QString("MODERNOTICE %1 :%2").arg(strChannel).arg(strText);
            Core::instance()->pNetwork->send(strText);
            QString strDisplay = QString("*<%1> %2").arg(strMe).arg(strText.right(strText.length()-14-strChannel.length()));
            emit displayMessage(strChannel, strDisplay, NoticeMessage);
        }
    }
}

void InputWidget::updateNick(QString strNick)
{
    nickLabel->setText(QString("<b>%1</b>").arg(strNick));
}

void InputWidget::inputlineReturnPressed()
{
    // update last active
    QDateTime cdt = QDateTime::currentDateTime();
    int t = (int)cdt.toTime_t(); // seconds that have passed since 1970
    Core::instance()->settings["last_active"] = QString::number(t);

    // disable away
    bool bAway = Core::instance()->settings.value("away") == "on" ? true : false;
    if (bAway)
        Core::instance()->pNetwork->send("AWAY :");

    // text
    QString strText = pInputLine->text().trimmed();
    pasteMultiLine(strText, false);
    pInputLine->clear();
}

void InputWidget::moderButtonClicked()
{
    // update last active
    QDateTime cdt = QDateTime::currentDateTime();
    int t = (int)cdt.toTime_t(); // seconds that have passed since 1970
    Core::instance()->settings["last_active"] = QString::number(t);

    // disable away
    bool bAway = Core::instance()->settings.value("away") == "on" ? true : false;
    if (bAway)
        Core::instance()->pNetwork->send("AWAY :");

    // text
    QString strText = pInputLine->text().trimmed();
    pasteMultiLine(strText, true);
    pInputLine->clear();
}

void InputWidget::showHideToolwidgetClicked()
{
    emit showHideToolwidget();
}

void InputWidget::slotCtrlTabPressed()
{
    emit ctrlTabPressed();
}

void InputWidget::slotCtrlShiftTabPressed()
{
    emit ctrlShiftTabPressed();
}
