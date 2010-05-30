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

#include "dlg_channel_list.h"

dlg_channel_list::dlg_channel_list(QSettings *param1, QTcpSocket *param2, tab_container *param3)
{
    ui.setupUi(this);

    settings = param1;
    socket = param2;
    tabc = param3;

    QObject::connect(ui.tableWidget_1, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(allCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_2, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(teenCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_3, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(towarzyskieCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_4, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(erotyczneCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_5, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tematyczneCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_6, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(regionalneCellDoubleClicked(int,int)));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void dlg_channel_list::clear()
{
    list_1.clear();
    list_2.clear();
    list_3.clear();
    list_4.clear();
    list_5.clear();
    list_6.clear();

    for (int i = ui.tableWidget_1->rowCount()-1; i >= 0; --i)
        ui.tableWidget_1->removeRow(i);

    for (int i = ui.tableWidget_2->rowCount()-1; i >= 0; --i)
        ui.tableWidget_2->removeRow(i);

    for (int i = ui.tableWidget_3->rowCount()-1; i >= 0; --i)
        ui.tableWidget_3->removeRow(i);

    for (int i = ui.tableWidget_4->rowCount()-1; i >= 0; --i)
        ui.tableWidget_4->removeRow(i);

    for (int i = ui.tableWidget_5->rowCount()-1; i >= 0; --i)
        ui.tableWidget_5->removeRow(i);

    for (int i = ui.tableWidget_6->rowCount()-1; i >= 0; --i)
        ui.tableWidget_6->removeRow(i);
}

void dlg_channel_list::add_channel(QString strName, QString strPeople, QString strCat, QString strType)
{
    QList <QString> add;
    add << strName << strPeople << strCat << strType;
    list_1 << add;

    if (strType == "Teen")
        list_2.append(add);
    else if (strType == "Towarzyskie")
        list_3.append(add);
    else if (strType == "Erotyczne")
        list_4.append(add);
    else if (strType == "Tematyczne")
        list_5.append(add);
    else if (strType == "Regionalne")
        list_6.append(add);
}

void dlg_channel_list::create_list()
{
    for (int i = 0; i < list_1.size(); i++)
    {
        QString strName = list_1[i][0];
        QString strPeople = list_1[i][1];
        QString strCat = list_1[i][2];
        QString strType = list_1[i][3];

        ui.tableWidget_1->insertRow(ui.tableWidget_1->rowCount());
        ui.tableWidget_1->setItem(ui.tableWidget_1->rowCount()-1, 0, new QTableWidgetItem(strName));
        ui.tableWidget_1->setItem(ui.tableWidget_1->rowCount()-1, 1, new QTableWidgetItem(strPeople));
        ui.tableWidget_1->setItem(ui.tableWidget_1->rowCount()-1, 2, new QTableWidgetItem(strCat));
        ui.tableWidget_1->setItem(ui.tableWidget_1->rowCount()-1, 3, new QTableWidgetItem(strType));

        if (strType == "Teen")
        {
            ui.tableWidget_2->insertRow(ui.tableWidget_2->rowCount());
            ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount()-1, 0, new QTableWidgetItem(strName));
            ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount()-1, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount()-1, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount()-1, 3, new QTableWidgetItem(strType));
        }
        else if (strType == "Towarzyskie")
        {
            ui.tableWidget_3->insertRow(ui.tableWidget_3->rowCount());
            ui.tableWidget_3->setItem(ui.tableWidget_3->rowCount()-1, 0, new QTableWidgetItem(strName));
            ui.tableWidget_3->setItem(ui.tableWidget_3->rowCount()-1, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_3->setItem(ui.tableWidget_3->rowCount()-1, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_3->setItem(ui.tableWidget_3->rowCount()-1, 3, new QTableWidgetItem(strType));
        }
        else if (strType == "Erotyczne")
        {
            ui.tableWidget_4->insertRow(ui.tableWidget_4->rowCount());
            ui.tableWidget_4->setItem(ui.tableWidget_4->rowCount()-1, 0, new QTableWidgetItem(strName));
            ui.tableWidget_4->setItem(ui.tableWidget_4->rowCount()-1, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_4->setItem(ui.tableWidget_4->rowCount()-1, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_4->setItem(ui.tableWidget_4->rowCount()-1, 3, new QTableWidgetItem(strType));
        }
        else if (strType == "Tematyczne")
        {
            ui.tableWidget_5->insertRow(ui.tableWidget_5->rowCount());
            ui.tableWidget_5->setItem(ui.tableWidget_5->rowCount()-1, 0, new QTableWidgetItem(strName));
            ui.tableWidget_5->setItem(ui.tableWidget_5->rowCount()-1, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_5->setItem(ui.tableWidget_5->rowCount()-1, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_5->setItem(ui.tableWidget_5->rowCount()-1, 3, new QTableWidgetItem(strType));
        }
        else if (strType == "Regionalne")
        {
            ui.tableWidget_6->insertRow(ui.tableWidget_6->rowCount());
            ui.tableWidget_6->setItem(ui.tableWidget_6->rowCount()-1, 0, new QTableWidgetItem(strName));
            ui.tableWidget_6->setItem(ui.tableWidget_6->rowCount()-1, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_6->setItem(ui.tableWidget_6->rowCount()-1, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_6->setItem(ui.tableWidget_6->rowCount()-1, 3, new QTableWidgetItem(strType));
        }
    }
}

void dlg_channel_list::QuickSort(QList< QList<QString> > *T, int Lo, int Hi)
{
    int i;
    int j;
    int x = T->at((Lo+Hi)/2).at(1).toInt();
    i = Lo;
    j = Hi;
    do
    {
        while (T->at(i).at(1).toInt() > x) ++i;
        while (T->at(j).at(1).toInt() < x) --j;
        if (i<=j)
        {
            QString item00 = T->at(i).at(0);
            QString item01 = T->at(i).at(1);
            QString item02 = T->at(i).at(2);
            QString item03 = T->at(i).at(3);

            QString item10 = T->at(j).at(0);
            QString item11 = T->at(j).at(1);
            QString item12 = T->at(j).at(2);
            QString item13 = T->at(j).at(3);

            QList<QString> a;
            a << item10 << item11 << item12 << item13;

            QList<QString> b;
            b << item00 << item01 << item02 << item03;

            T->replace(i,a);
            T->replace(j,b);

            ++i; --j;
        }
    } while (i < j);
    if (Lo < j) QuickSort(T, Lo, j);
    if (Hi > i) QuickSort(T, i, Hi);
}

void dlg_channel_list::sort()
{
    QuickSort(&list_1, 0, list_1.size()-1);
    QuickSort(&list_2, 0, list_2.size()-1);
    QuickSort(&list_3, 0, list_3.size()-1);
    QuickSort(&list_4, 0, list_4.size()-1);
    QuickSort(&list_5, 0, list_5.size()-1);
    QuickSort(&list_6, 0, list_6.size()-1);
    dlg_channel_list::create_list();
}

void dlg_channel_list::allCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_1->item(row, 0)->text();
    dlg_channel_list::send(QString("JOIN %1").arg(strChannel));
}

void dlg_channel_list::teenCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_2->item(row, 0)->text();
    dlg_channel_list::send(QString("JOIN %1").arg(strChannel));
}

void dlg_channel_list::towarzyskieCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_3->item(row, 0)->text();
    dlg_channel_list::send(QString("JOIN %1").arg(strChannel));
}

void dlg_channel_list::erotyczneCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_4->item(row, 0)->text();
    dlg_channel_list::send(QString("JOIN %1").arg(strChannel));
}

void dlg_channel_list::tematyczneCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_5->item(row, 0)->text();
    dlg_channel_list::send(QString("JOIN %1").arg(strChannel));
}

void dlg_channel_list::regionalneCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_6->item(row, 0)->text();
    dlg_channel_list::send(QString("JOIN %1").arg(strChannel));
}

void dlg_channel_list::button_ok()
{
    dlg_channel_list::clear();
    this->hide();
}

void dlg_channel_list::button_cancel()
{
    dlg_channel_list::clear();
    this->hide();
}

// copy of network::send
void dlg_channel_list::send(QString strData)
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

        if (socket->write(qbaData) == -1)
        {
            if (socket->state() == QAbstractSocket::ConnectedState)
                tabc->show_msg_active(QString("Error: Nie uda³o siê wys³aæ danych! [%1]").arg(socket->errorString()), 9);
            else if (socket->state() == QAbstractSocket::UnconnectedState)
                tabc->show_msg_active("Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
        }
    }
    else
        tabc->show_msg("Status", "Error: Nie uda³o siê wys³aæ danych! [Not connected]", 9);
}

void dlg_channel_list::showEvent(QShowEvent *event)
{
    event->accept();

    dlg_channel_list::send("SLIST  R- 0 0 100 null");
}

void dlg_channel_list::resizeEvent(QResizeEvent *event)
{
    ui.verticalLayoutWidget->setGeometry(QRect(0,0,this->width(), this->height()));
    ui.tabWidget->setGeometry(QRect(0,0,this->width(), this->height()));
    ui.tableWidget_1->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_2->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_3->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_4->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_5->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_6->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
}
