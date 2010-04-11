/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#include "qnicklist.h"

qnicklist::qnicklist(QTcpSocket *param1, QSettings *param2, QString param3)
{
    socket = param1;
    settings = param2;
    strChannel = param3;
}

qnicklist::~qnicklist()
{
    strOpenChannels.clear();
}

void qnicklist::set_open_channels(QStringList param1)
{
    strOpenChannels = param1;
}

void qnicklist::priv()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("PRIV %1").arg(strNick));
}

void qnicklist::whois()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("WHOIS %1 %1").arg(strNick));
}

void qnicklist::cam()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    (new dlg_cam(settings, socket, strNick))->show();
}

void qnicklist::friends_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("NS FRIENDS ADD %1").arg(strNick));
}

void qnicklist::friends_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("NS FRIENDS DEL %1").arg(strNick));
}

void qnicklist::ignore_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("NS IGNORE ADD %1").arg(strNick));
}

void qnicklist::ignore_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("NS IGNORE DEL %1").arg(strNick));
}

void qnicklist::kick()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    QString strReason = "Zachowuj siê! Byle jak ale siê zachowuj!";
    qnicklist::send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strReason));
}

void qnicklist::ban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
}

void qnicklist::kban()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    QString strReason = "Zachowuj siê! Byle jak ale siê zachowuj!";
    qnicklist::send(QString("CS BAN %1 ADD %2").arg(strChannel).arg(strNick));
    qnicklist::send(QString("KICK %1 %2 :%3").arg(strChannel).arg(strNick).arg(strReason));
}

void qnicklist::op_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS OP %1 ADD %2").arg(strChannel).arg(strNick));
}

void qnicklist::op_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS OP %1 DEL %2").arg(strChannel).arg(strNick));
}

void qnicklist::halfop_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS HALFOP %1 ADD %2").arg(strChannel).arg(strNick));
}

void qnicklist::halfop_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS HALFOP %1 DEL %2").arg(strChannel).arg(strNick));
}

void qnicklist::moderator_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS MODERATOR %1 ADD %2").arg(strChannel).arg(strNick));
}

void qnicklist::moderator_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS MODERATOR %1 DEL %2").arg(strChannel).arg(strNick));
}

void qnicklist::voice_add()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS VOICE %1 ADD %2").arg(strChannel).arg(strNick));
}

void qnicklist::voice_del()
{
    if (this->selectedItems().count() == 0) return;

    QString strNick = this->selectedItems().at(0)->text();
    qnicklist::send(QString("CS VOICE %1 DEL %2").arg(strChannel).arg(strNick));
}

void qnicklist::invite()
{
    if (this->selectedItems().count() == 0) return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString strInviteChannel = action->data().toString();
        QString strNick = this->selectedItems().at(0)->text();
        qnicklist::send(QString("INVITE %1 %2").arg(strNick).arg(strInviteChannel));
    }
}

// copy of network::send
void qnicklist::send(QString strData)
{
    if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->isWritable() == true))
    {
#ifdef Q_WS_X11
        if (settings->value("debug").toString() == "on")
            qDebug() << "-> " << strData;
#endif
        strData += "\r\n";
        QByteArray qbaData;
        for ( int i = 0; i < strData.size(); i++)
            qbaData.insert(i, strData.at(i));

        socket->write(qbaData);
        if ((socket->state() == QAbstractSocket::ConnectedState) && (socket->waitForBytesWritten() == false))
        {
            int nop;
            nop = 1;
           //tabc->show_msg("Status", QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()));
        }
    }
    //else
        //tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]");
}

void qnicklist::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *minvite = new QMenu("Zapro¶");

    for (int i = 0; i < MaxOpenChannels; ++i)
    {
        openChannelsActs[i] = new QAction(this);
        openChannelsActs[i]->setVisible(false);
        connect(openChannelsActs[i], SIGNAL(triggered()), this, SLOT(invite()));
     }

    for (int i = 0; i < MaxOpenChannels; ++i)
        minvite->addAction(openChannelsActs[i]);

    for (int i = 0; i < strOpenChannels.count(); ++i)
    {
        openChannelsActs[i]->setText(strOpenChannels[i]);
        openChannelsActs[i]->setData(strOpenChannels[i]);
        openChannelsActs[i]->setVisible(true);
    }
    for (int j = strOpenChannels.count(); j < MaxOpenChannels; ++j)
        openChannelsActs[j]->setVisible(false);

    QMenu *friends = new QMenu("Lista przyjació³");
    friends->addAction("Dodaj do listy przyjació³", this, SLOT(friends_add()));
    friends->addAction("Usuñ z listy przyjació³", this, SLOT(friends_del()));

    QMenu *ignore = new QMenu("Lista ignorowanych");
    ignore->addAction("Dodaj do listy ignorowanych", this, SLOT(ignore_add()));
    ignore->addAction("Usuñ z listy ignorowanych", this, SLOT(ignore_del()));

    QMenu *privilege = new QMenu("Uprawnienia");
    privilege->addAction("Nadaj prawa super operatora", this, SLOT(op_add()));
    privilege->addAction("Odbierz prawa super operatora", this, SLOT(op_del()));
    privilege->addSeparator();
    privilege->addAction("Nadaj prawa operatora", this, SLOT(halfop_add()));
    privilege->addAction("Odbierz prawa operatora", this, SLOT(halfop_del()));
    privilege->addSeparator();
    privilege->addAction("Nadaj prawa moderatora", this, SLOT(moderator_add()));
    privilege->addAction("Odbierz prawa moderatora", this, SLOT(moderator_del()));
    privilege->addSeparator();
    privilege->addAction("Nadaj prawa go¶cia", this, SLOT(voice_add()));
    privilege->addAction("Odbierz prawa go¶cia", this, SLOT(voice_del()));

    QMenu *menu = new QMenu(this);

    menu->addAction("Rozmowa prywatna", this, SLOT(priv()));
    menu->addAction("Whois", this, SLOT(whois()));
    menu->addAction("Kamerka", this, SLOT(cam()));
    menu->addMenu(minvite);
    menu->addMenu(friends);
    menu->addMenu(ignore);
    menu->addSeparator();
    menu->addAction("Wyrzuæ", this, SLOT(kick()));
    menu->addAction("Cichy ban", this, SLOT(ban()));
    menu->addAction("Banuj i wyrzuæ", this, SLOT(kban()));
    menu->addSeparator();
    menu->addMenu(privilege);

    menu->popup(e->globalPos());

    QEventLoop loop;
    QObject::connect(menu, SIGNAL(triggered(QAction*)), &loop, SLOT(quit()));
    loop.exec();

    delete menu;
    delete privilege;
    delete ignore;
    delete friends;
    delete minvite;
}
