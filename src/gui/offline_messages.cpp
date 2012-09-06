/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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

#include <QDateTime>
#include <QDesktopWidget>
#include <QPushButton>
#include <QTimer>
#include "core.h"
#include "convert.h"
#include "offline.h"
#include "offline_messages.h"

DlgOfflineMessages::DlgOfflineMessages(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Offline messages"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgOfflineMessages::createGui()
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

void DlgOfflineMessages::createSignals()
{
    connect(ui.pushButton_read, SIGNAL(clicked()), this, SLOT(buttonRead()));
    connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(buttonReject()));
    connect(ui.pushButton_reply, SIGNAL(clicked()), this, SLOT(buttonReply()));
    connect(ui.lineEdit_reply, SIGNAL(returnPressed()), this, SLOT(buttonReply()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgOfflineMessages::removeNick(const QString &strNick)
{
    // list
    Offline::instance()->removeNick(strNick);

    // widget
    QList<QListWidgetItem*> items = ui.listWidget_nicks->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
        ui.listWidget_nicks->takeItem(ui.listWidget_nicks->row(item));
}

void DlgOfflineMessages::refresh()
{
    ui.listWidget_nicks->clear();

    QList<QString> list = Offline::instance()->getNicks();
    foreach (const QString &strOfflineNick, list)
        ui.listWidget_nicks->addItem(strOfflineNick);
}

void DlgOfflineMessages::refreshMessages()
{
    if (Offline::instance()->isEmptyMessages())
    {
        QTimer::singleShot(1000*4, this, SLOT(refreshMessages())); // 4 sec
        return;
    }

    ui.listWidget_msg->clear();
    ui.lineEdit_reply->clear();
    ui.listWidget_msg->setEnabled(true);
    ui.lineEdit_reply->setEnabled(true);
    ui.pushButton_reply->setEnabled(true);

    QList<OnetOfflineMessage> list = Offline::instance()->getMessages();
    foreach (OnetOfflineMessage msg, list)
    {
        qint64 iTime = msg.datetime;
        QString strType = msg.type;
        QString strNick = msg.nick;
        QString strMessage = msg.message;

        if (strNick == strCurrentNick)
        {
            QString strDT = QDateTime::fromMSecsSinceEpoch(iTime).toString("[dd MMM yyyy] [hh:mm:ss]");

            Convert::simpleConvert(strMessage);

            if (strType == "quote")
            {
                if (!messagesQuoted.contains(strNick))
                {
                    QString strDisplay = QString(tr("* You sent offline message to %1:").arg(strNick));
                    ui.listWidget_msg->addItem(strDisplay);
                    messagesQuoted.append(strNick);
                }

                QString strMe = Settings::instance()->get("nick");

                strMessage = QString("%1 <%2> %3").arg(strDT, strMe, strMessage);
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
                strMessage = QString("%1 <%2> %3").arg(strDT, strNick, strMessage);
                ui.listWidget_msg->addItem(strMessage);
            }
            else if (strType == "msg")
            {
                strMessage = QString("%1 <%2> %3").arg(strDT, strNick, strMessage);
                ui.listWidget_msg->addItem(strMessage);
            }
        }
    }
}

void DlgOfflineMessages::buttonRead()
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

    QTimer::singleShot(200, this, SLOT(refreshMessages())); // 0.2 sec
}

void DlgOfflineMessages::buttonReject()
{
    if (ui.listWidget_nicks->selectedItems().size() == 0) return; // nothing selected

    QString strNick = ui.listWidget_nicks->selectedItems().at(0)->text();
    Core::instance()->pNetwork->send(QString("NS OFFLINE REJECT %1").arg(strNick));

    // remove nick
    removeNick(strNick);
}

void DlgOfflineMessages::buttonReply()
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
    Core::instance()->pNetwork->send(QString("NS OFFLINE REPLY %1 %2").arg(strNick, strMessage));

    // clear
    ui.lineEdit_reply->clear();

    // show
    QString strMe = Settings::instance()->get("nick");
    QString strDT = QDateTime::currentDateTime().toString("[hh:mm:ss]");
    strMessage = QString("%1 <%2> %3").arg(strDT, strMe, strMessage);
    ui.listWidget_msg->addItem(strMessage);
}
