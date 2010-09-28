/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "inputlinewidget.h"

InputLineWidget::InputLineWidget(QWidget *parent, Network *param1) : QWidget(parent)
{
    myparent = parent;
    pNetwork = param1;

    nickLabel = new QLabel(myparent);
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(tr("(Unregistered)")));
    nickLabel->show();

    inputLine = new InputLine(myparent);
    inputLine->setMinimumWidth(350);
    inputLine->setMaxLength(300);
    inputLine->setFont(QFont("Verdana", -1, -1, false));
    inputLine->show();

    sendButton = new QPushButton(QIcon(":/images/oxygen/16x16/go-next.png"), tr("Send"), myparent);
    sendButton->setToolTip(tr("Send"));
    sendButton->setMaximumWidth(85);
    sendButton->setMaximumHeight(25);
    sendButton->show();

    moderSendButton = new QPushButton(QIcon(":/images/oxygen/16x16/view-pim-tasks.png"), tr("Send to moderators"), myparent);
    moderSendButton->setToolTip(tr("Send to moderators"));
    moderSendButton->setMaximumHeight(25);
    moderSendButton->show();

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(nickLabel);
    mainLayout->addWidget(inputLine);
    mainLayout->addWidget(sendButton);
    mainLayout->addWidget(moderSendButton);
    setLayout(mainLayout);

    moderSendButton->hide();

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputline_return_pressed()));
    QObject::connect(inputLine, SIGNAL(returnPressed()), this, SLOT(inputline_return_pressed()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moder_button_clicked()));
}

void InputLineWidget::set_active(QString strName)
{
    strChannel = strName;

    moderSendButton->hide();
    mainLayout->removeWidget(moderSendButton);
}

void InputLineWidget::insert_text(QString strText)
{
    inputLine->insert_text(strText);
}

void InputLineWidget::set_font(QFont font)
{
    inputLine->setFont(font);
}

void InputLineWidget::set_userslist(QTreeWidget *treeWidget)
{
    inputLine->set_userslist(treeWidget);
}

void InputLineWidget::set_style_sheet(QString strStyle)
{
    inputLine->setStyleSheet(strStyle);
}

void InputLineWidget::set_moderation(bool m)
{
    if (m == true)
    {
        moderSendButton->show();
        mainLayout->addWidget(moderSendButton);
    }
    else
    {
        moderSendButton->hide();
        mainLayout->removeWidget(moderSendButton);
    }
}

QString InputLineWidget::convert_emots(QString strData)
{
    strData.replace(QRegExp("(http:|https:)//"), "\\1\\\\"); // fix http https
    strData.replace(QRegExp("//([a-zA-Z0-9_-]+)\\b"), "%I\\1%");
    strData.replace(QRegExp("(http:|https:)\\\\\\\\"), "\\1//"); // fix http https
    return strData;
}

QString InputLineWidget::replace_emots(QString strData)
{
    Replace *pReplace = new Replace();
    strData = pReplace->replace_emots(strData);
    delete pReplace;

    return strData;
}

void InputLineWidget::send_message(bool bType)
{
    QString strTextO = inputLine->text();
    QStringList strTextA = strTextO.split(QRegExp("(\n|\r)"));

    for (int i = 0; i < strTextA.count(); i++)
    {
        QString strText = strTextA[i];
        QString strTextOriginal = strText;
        strLast_msg = strText;

        QSettings settings;
        QString strMe = settings.value("nick").toString();
        QString strCurrentColor = settings.value("my_color").toString();
        QString strFontFamily = settings.value("my_font").toString();
        bool bMyBold = settings.value("my_bold").toString() == "on" ? true : false;
        bool bMyItalic = settings.value("my_italic").toString() == "on" ? true : false;

        if ((strText[0] == '/') && (strText[1] != '/'))
        {
            if (strText[0] == '/')
                strText = strText.right(strText.length()-1);
            strTextOriginal = strText;
            QStringList strTextList = strText.split(" ");

            Commands *pCommands = new Commands(strChannel, strText);
            strText = pCommands->execute();
            delete pCommands;

            if ((strTextList[0] == "help") || (strTextList[0] == "pomoc"))
            {
                QStringList strlHelp = strText.split(";");
                for (int i = 0; i < strlHelp.count(); i++)
                    emit show_msg(strChannel, strlHelp.at(i), 7);
            }
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

                    if (strCurrentColor != "#000000")
                        strTextDisplay = "%C"+strCurrentColor.right(6)+"%"+strTextDisplay;
                    if ((weight != "") || (font != "verdana"))
                        strTextDisplay = "%F"+weight+":"+font+"%"+strTextDisplay;

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

                    emit display_message(strChannel, QString("%1<%2> %3ACTION %4%5").arg(strDT).arg(strMe).arg(QString(QByteArray("\x01"))).arg(strTextDisplay).arg(QString(QByteArray("\x01"))), 0);
                    if (pNetwork->is_connected() == true)
                        pNetwork->send(strTextSend);
                }
            }
            else
            {
                if ((pNetwork->is_connected() == true) && (strText.length() > 0))
                    pNetwork->send(strText);
            }

            inputLine->clear();
        }
        else if (strChannel != "Status")
        {
            if ((pNetwork->is_connected() == true) && (strText.length() > 0))
            {
                QString weight;
                QString font = strFontFamily.toLower();

                if (bMyBold == true) weight += "b";
                if (bMyItalic == true) weight += "i";

                if (strCurrentColor != "#000000")
                    strText = "%C"+strCurrentColor.right(6)+"%"+strText;
                if ((weight != "") || (font != "verdana"))
                    strText = "%F"+weight+":"+font+"%"+strText;

                strText = convert_emots(strText);
                strText = replace_emots(strText);

                QDateTime dt = QDateTime::currentDateTime();
                QString strDT = dt.toString("[hh:mm:ss] ");

                if (bType == true)
                {
                    QSettings settings;
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
                else
                {
                    QSettings settings;
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

                inputLine->clear();
            }
        }
    }
}

void InputLineWidget::update_nick(QString strNick)
{
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(strNick));
}

void InputLineWidget::inputline_return_pressed()
{
    send_message(true);
}

void InputLineWidget::moder_button_clicked()
{
    send_message(false);
}

void InputLineWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up)
    {
        inputLine->clear();
        inputLine->setText(strLast_msg);
        inputLine->setFocus();
    }
    else if (e->key() == Qt::Key_Down)
    {
        inputLine->clear();
        inputLine->setFocus();
    }

    QWidget::keyPressEvent(e);
}
