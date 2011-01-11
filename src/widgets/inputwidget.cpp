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

#include "inputwidget.h"

InputWidget::InputWidget(QWidget *parent, Network *param1) : QWidget(parent)
{
    myparent = parent;
    pNetwork = param1;

    nickLabel = new QLabel(myparent);
    nickLabel->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(tr("(Unregistered)")));
    nickLabel->show();

    inputLine = new InputLineWidget(myparent);
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

    showHideToolWidget = new QPushButton(QIcon(":/images/oxygen/16x16/text-frame-unlink.png"), "", myparent);
    showHideToolWidget->setToolTip(tr("Show/Hide tool widget"));
    showHideToolWidget->setMaximumWidth(25);
    showHideToolWidget->setMaximumHeight(25);
    showHideToolWidget->show();

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(nickLabel);
    mainLayout->addWidget(inputLine);
    mainLayout->addWidget(sendButton);
    mainLayout->addWidget(moderSendButton);
    mainLayout->addWidget(showHideToolWidget);
    setLayout(mainLayout);

    // default hidden
    moderSendButton->hide();

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(inputline_return_pressed()));
    QObject::connect(inputLine, SIGNAL(returnPressed()), this, SLOT(inputline_return_pressed()));
    QObject::connect(moderSendButton, SIGNAL(clicked()), this, SLOT(moder_button_clicked()));
    QObject::connect(showHideToolWidget, SIGNAL(clicked()), this, SLOT(show_hide_toolwidget_clicked()));
}

void InputWidget::set_active(QString strName)
{
    strChannel = strName;
}

void InputWidget::insert_text(QString strText)
{
    inputLine->insert_text(strText);
}

void InputWidget::set_font(QFont font)
{
    inputLine->setFont(font);
}

void InputWidget::set_color(QString color)
{
    inputLine->setStyleSheet(QString("color:%1").arg(color));
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

    inputLine->set_userslist(usersList);
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
    strData.replace(QRegExp("(http:|https:)//"), "\\1\\\\"); // fix http https
    strData.replace(QRegExp("//([a-zA-Z0-9_-]+)\\b"), "%I\\1%");
    strData.replace(QRegExp("(http:|https:)\\\\\\\\"), "\\1//"); // fix http https
    return strData;
}

QString InputWidget::replace_emots(QString strData)
{
    Replace *pReplace = new Replace();
    strData = pReplace->replace_emots(strData);
    delete pReplace;

    return strData;
}

void InputWidget::send_message(bool bType)
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

void InputWidget::update_nick(QString strNick)
{
    nickLabel->setText(QString("<b>%1</b>").arg(strNick));
}

void InputWidget::inputline_return_pressed()
{
    send_message(true);
}

void InputWidget::moder_button_clicked()
{
    send_message(false);
}

void InputWidget::show_hide_toolwidget_clicked()
{
    emit show_hide_toolwidget();
}

void InputWidget::keyPressEvent(QKeyEvent *e)
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
