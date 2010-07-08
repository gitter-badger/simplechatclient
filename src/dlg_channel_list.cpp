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

#include "dlg_channel_list.h"

DlgChannelList::DlgChannelList(Network *param1, QSettings *param2, TabContainer *param3)
{
    ui.setupUi(this);

    pNetwork = param1;
    settings = param2;
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

void DlgChannelList::clear()
{
    list_1.clear();
    list_2.clear();
    list_3.clear();
    list_4.clear();
    list_5.clear();
    list_6.clear();

    ui.tableWidget_1->clear();
    ui.tableWidget_2->clear();
    ui.tableWidget_3->clear();
    ui.tableWidget_4->clear();
    ui.tableWidget_5->clear();
    ui.tableWidget_6->clear();

    ui.tableWidget_1->setRowCount(0);
    ui.tableWidget_2->setRowCount(0);
    ui.tableWidget_3->setRowCount(0);
    ui.tableWidget_4->setRowCount(0);
    ui.tableWidget_5->setRowCount(0);
    ui.tableWidget_6->setRowCount(0);

    QStringList strlLabels;
    strlLabels << tr("Channel name") << tr("Number of persons") << tr("Category") << tr("Type");

    ui.tableWidget_1->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_2->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_3->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_4->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_5->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_6->setHorizontalHeaderLabels(strlLabels);
}

void DlgChannelList::add_channel(QString strName, QString strPeople, QString strCat, QString strType)
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

void DlgChannelList::create_list()
{
    // count rows
    int iAllCount = 0;
    int iTeenCount = 0;
    int iTowarzyskieCount = 0;
    int iErotyczneCount = 0;
    int iTematyczneCount = 0;
    int iRegionalneCount = 0;

    for (int i = 0; i < list_1.size(); i++)
    {
        QString strType = list_1[i][3];

        iAllCount++;
        if (strType == "Teen")
            iTeenCount++;
        else if (strType == "Towarzyskie")
            iTowarzyskieCount++;
        else if (strType == "Erotyczne")
            iErotyczneCount++;
        else if (strType == "Tematyczne")
            iTematyczneCount++;
        else if (strType == "Regionalne")
            iRegionalneCount++;
    }

    ui.tableWidget_1->setRowCount(iAllCount);
    ui.tableWidget_2->setRowCount(iTeenCount);
    ui.tableWidget_3->setRowCount(iTowarzyskieCount);
    ui.tableWidget_4->setRowCount(iErotyczneCount);
    ui.tableWidget_5->setRowCount(iTematyczneCount);
    ui.tableWidget_6->setRowCount(iRegionalneCount);

    // table counters
    int iAllRow = 0;
    int iTeenRow = 0;
    int iTowarzyskieRow = 0;
    int iErotyczneRow = 0;
    int iTematyczneRow = 0;
    int iRegionalneRow = 0;

    for (int i = 0; i < list_1.size(); i++)
    {
        QString strName = list_1[i][0];
        QString strPeople = list_1[i][1];
        QString strCat = list_1[i][2];
        QString strType = list_1[i][3];

        ui.tableWidget_1->setItem(iAllRow, 0, new QTableWidgetItem(strName));
        ui.tableWidget_1->setItem(iAllRow, 1, new QTableWidgetItem(strPeople));
        ui.tableWidget_1->setItem(iAllRow, 2, new QTableWidgetItem(strCat));
        ui.tableWidget_1->setItem(iAllRow, 3, new QTableWidgetItem(strType));
        iAllRow++;

        if (strType == "Teen")
        {
            ui.tableWidget_2->setItem(iTeenRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_2->setItem(iTeenRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_2->setItem(iTeenRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_2->setItem(iTeenRow, 3, new QTableWidgetItem(strType));
            iTeenRow++;
        }
        else if (strType == "Towarzyskie")
        {
            ui.tableWidget_3->setItem(iTowarzyskieRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_3->setItem(iTowarzyskieRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_3->setItem(iTowarzyskieRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_3->setItem(iTowarzyskieRow, 3, new QTableWidgetItem(strType));
            iTowarzyskieRow++;
        }
        else if (strType == "Erotyczne")
        {
            ui.tableWidget_4->setItem(iErotyczneRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_4->setItem(iErotyczneRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_4->setItem(iErotyczneRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_4->setItem(iErotyczneRow, 3, new QTableWidgetItem(strType));
            iErotyczneRow++;
        }
        else if (strType == "Tematyczne")
        {
            ui.tableWidget_5->setItem(iTematyczneRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_5->setItem(iTematyczneRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_5->setItem(iTematyczneRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_5->setItem(iTematyczneRow, 3, new QTableWidgetItem(strType));
            iTematyczneRow++;
        }
        else if (strType == "Regionalne")
        {
            ui.tableWidget_6->setItem(iRegionalneRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_6->setItem(iRegionalneRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_6->setItem(iRegionalneRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_6->setItem(iRegionalneRow, 3, new QTableWidgetItem(strType));
            iRegionalneRow++;
        }
    }
}

void DlgChannelList::quickSort(QList< QList<QString> > *T, int Lo, int Hi)
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

            QList <QString> a;
            a << item10 << item11 << item12 << item13;

            QList <QString> b;
            b << item00 << item01 << item02 << item03;

            T->replace(i,a);
            T->replace(j,b);

            ++i; --j;
        }
    } while (i < j);
    if (Lo < j) quickSort(T, Lo, j);
    if (Hi > i) quickSort(T, i, Hi);
}

void DlgChannelList::sort()
{
    quickSort(&list_1, 0, list_1.size()-1);
    quickSort(&list_2, 0, list_2.size()-1);
    quickSort(&list_3, 0, list_3.size()-1);
    quickSort(&list_4, 0, list_4.size()-1);
    quickSort(&list_5, 0, list_5.size()-1);
    quickSort(&list_6, 0, list_6.size()-1);
    create_list();

    ui.tableWidget_1->resizeColumnsToContents();
    ui.tableWidget_2->resizeColumnsToContents();
    ui.tableWidget_3->resizeColumnsToContents();
    ui.tableWidget_4->resizeColumnsToContents();
    ui.tableWidget_5->resizeColumnsToContents();
    ui.tableWidget_6->resizeColumnsToContents();
}

void DlgChannelList::allCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_1->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::teenCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_2->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::towarzyskieCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_3->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::erotyczneCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_4->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::tematyczneCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_5->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::regionalneCellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_6->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::button_ok()
{
    clear();
    this->hide();
}

void DlgChannelList::button_cancel()
{
    clear();
    this->hide();
}

void DlgChannelList::showEvent(QShowEvent *event)
{
    event->accept();

    pNetwork->send("SLIST  R- 0 0 100 null");
}

void DlgChannelList::resizeEvent(QResizeEvent *event)
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
