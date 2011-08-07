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

#include <QDateTime>
#include <QDesktopWidget>
#include <QPushButton>
#include <QTimer>
#include "core.h"
#include "dlg_offlinemsg.h"

DlgOfflineMsg::DlgOfflineMsg(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Offline messages"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgOfflineMsg::createGui()
{
    ui.pushButton_read->setIcon(QIcon(":/images/oxygen/16x16/mail-mark-read.png"));
    ui.pushButton_reject->setIcon(QIcon(":/images/oxygen/16x16/mail-mark-junk.png"));
    ui.pushButton_reply->setIcon(QIcon(":/images/oxygen/16x16/mail-send.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_read->setText(tr("Read"));
    ui.pushButton_reject->setText(tr("Reject"));
    ui.pushButton_reply->setText(tr("Reply"));

    ui.listWidget_msg->setEnabled(false);
    ui.lineEdit_reply->setEnabled(false);
    ui.pushButton_reply->setEnabled(false);
}

void DlgOfflineMsg::createSignals()
{
    QObject::connect(ui.pushButton_read, SIGNAL(clicked()), this, SLOT(buttonRead()));
    QObject::connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(buttonReject()));
    QObject::connect(ui.pushButton_reply, SIGNAL(clicked()), this, SLOT(buttonReply()));
    QObject::connect(ui.lineEdit_reply, SIGNAL(returnPressed()), this, SLOT(buttonReply()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(buttonClose()));
}

void DlgOfflineMsg::removeNick(QString strRemoveNick)
{
    // list
    Core::instance()->lOfflineNicks.removeOne(strRemoveNick);

    // widget
    for (int i = 0; i < ui.listWidget_nicks->count(); i++)
    {
        if (ui.listWidget_nicks->item(i)->text() == strRemoveNick)
        {
            ui.listWidget_nicks->takeItem(i);
            break;
        }
    }

    // remove from core
    for (int i = 0; i < Core::instance()->lOfflineMsg.size(); i++)
    {
        OfflineMsg msg = Core::instance()->lOfflineMsg.at(i);
        if (msg.nick == strRemoveNick)
            Core::instance()->lOfflineMsg.takeAt(i);
    }
}

void DlgOfflineMsg::refresh()
{
    for (int i = 0; i < Core::instance()->lOfflineNicks.size(); i++)
        ui.listWidget_nicks->insertItem(ui.listWidget_nicks->count(), Core::instance()->lOfflineNicks.at(i));
}

bool DlgOfflineMsg::existNick()
{
    for (int i = 0; i < Core::instance()->lOfflineMsg.size(); i++)
    {
        OfflineMsg msg = Core::instance()->lOfflineMsg.at(i);
        QString strNick = msg.nick;
        if (strNick == strCurrentNick)
            return true;
    }
    return false;
}

void DlgOfflineMsg::refreshMsg()
{
    if (!existNick())
    {
        QTimer::singleShot(1000*4, this, SLOT(refreshMsg())); // 3 sec
        return;
    }

    ui.listWidget_msg->clear();
    ui.lineEdit_reply->clear();
    ui.listWidget_msg->setEnabled(true);
    ui.lineEdit_reply->setEnabled(true);
    ui.pushButton_reply->setEnabled(true);

    for (int i = 0; i < Core::instance()->lOfflineMsg.size(); i++)
    {
        OfflineMsg msg = Core::instance()->lOfflineMsg.at(i);
        QString strTime = msg.datetime;
        QString strType = msg.type;
        QString strNick = msg.nick;
        QString strMessage = msg.message;

        if (strNick == strCurrentNick)
        {
            QDateTime dt = QDateTime::fromTime_t(strTime.toUInt());
            QString strDT = dt.toString("[dd/MM/yyyy] [hh:mm:ss]");

            strMessage.remove(QRegExp("%C([a-zA-Z0-9]+)%"));
            strMessage.remove(QRegExp("%F([a-zA-Z0-9:]+)%"));
            strMessage.replace(QRegExp("%I([a-zA-Z0-9_-]+)%"),"<\\1>");

            if (strType == "quote")
            {
                if (!messagesQuoted.contains(strNick))
                {
                    QString strDisplay = QString(tr("* You sent offline message to %1:").arg(strNick));
                    ui.listWidget_msg->addItem(strDisplay);
                    messagesQuoted.append(strNick);
                }

                QString strMe = Core::instance()->settings.value("nick");

                strMessage = QString("%1 <%2> %3").arg(strDT).arg(strMe).arg(strMessage);
                ui.listWidget_msg->addItem(strMessage);
            }
            else if (strType == "reply")
            {
                if (!messagesReplied.contains(strNick))
                {
                    QString strDisplay = QString(tr("* User %1 replied:").arg(strNick));
                    ui.listWidget_msg->addItem(strDisplay);
                    messagesReplied.append(strNick);
                }
                strMessage = QString("%1 <%2> %3").arg(strDT).arg(strNick).arg(strMessage);
                ui.listWidget_msg->addItem(strMessage);
            }
            else if (strType == "msg")
            {
                strMessage = QString("%1 <%2> %3").arg(strDT).arg(strNick).arg(strMessage);
                ui.listWidget_msg->addItem(strMessage);
            }
        }
    }
}

void DlgOfflineMsg::buttonRead()
{
    if (ui.listWidget_nicks->selectedItems().size() == 0) return; // nothing selected

    QString strNick = ui.listWidget_nicks->selectedItems().at(0)->text();
    Core::instance()->pNetwork->send(QString("NS OFFLINE GET %1").arg(strNick));

    strCurrentNick = strNick;
    ui.label_nick->setText(QString(tr("Offline messages from %1")).arg(strNick));
    // remove nick
    removeNick(strNick);
    // clear
    ui.listWidget_msg->clear();
    ui.lineEdit_reply->clear();

    QTimer::singleShot(1000*4, this, SLOT(refreshMsg())); // 3 sec
}

void DlgOfflineMsg::buttonReject()
{
    if (ui.listWidget_nicks->selectedItems().size() == 0) return; // nothing selected

    QString strNick = ui.listWidget_nicks->selectedItems().at(0)->text();
    Core::instance()->pNetwork->send(QString("NS OFFLINE REJECT %1").arg(strNick));

    // remove nick
    removeNick(strNick);
}

void DlgOfflineMsg::buttonReply()
{
    if (strCurrentNick.isEmpty()) return; // wrong nick

    QString strNick = strCurrentNick;
    QString strMessage = ui.lineEdit_reply->text();

    // quote
    if (!messagesQuotedToSender.contains(strNick))
    {
        Core::instance()->pNetwork->send(QString("NS OFFLINE QUOTE %1").arg(strNick));
        messagesQuotedToSender.append(strNick);
    }

    // reply
    Core::instance()->pNetwork->send(QString("NS OFFLINE REPLY %1 %2").arg(strNick).arg(strMessage));

    // clear
    ui.lineEdit_reply->clear();

    // show
    QString strMe = Core::instance()->settings.value("nick");
    QDateTime dt = QDateTime::currentDateTime();
    QString strDT = dt.toString("[hh:mm:ss]");
    strMessage = QString("%1 <%2> %3").arg(strDT).arg(strMe).arg(strMessage);
    ui.listWidget_msg->addItem(strMessage);
}

void DlgOfflineMsg::buttonClose()
{
    // hide if no messages
    if (Core::instance()->lOfflineNicks.size() == 0)
        Core::instance()->offlineMsgAct->setVisible(false);

    // close
    this->close();
}
