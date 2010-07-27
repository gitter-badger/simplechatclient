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

DlgChannelList::DlgChannelList(QSettings *param1, TabContainer *param2)
{
    ui.setupUi(this);
    setWindowTitle(tr("Channel list"));

    settings = param1;
    tabc = param2;

    QObject::connect(ui.tableWidget_all, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(all_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_teen, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(teen_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_towarzyskie, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(towarzyskie_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_erotyczne, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(erotyczne_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_tematyczne, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tematyczne_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_regionalne, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(regionalne_CellDoubleClicked(int,int)));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgChannelList::clear()
{
    list_all.clear();
    list_teen.clear();
    list_towarzyskie.clear();
    list_erotyczne.clear();
    list_tematyczne.clear();
    list_regionalne.clear();

    ui.tableWidget_all->clear();
    ui.tableWidget_teen->clear();
    ui.tableWidget_towarzyskie->clear();
    ui.tableWidget_erotyczne->clear();
    ui.tableWidget_tematyczne->clear();
    ui.tableWidget_regionalne->clear();

    ui.tableWidget_all->setRowCount(0);
    ui.tableWidget_teen->setRowCount(0);
    ui.tableWidget_towarzyskie->setRowCount(0);
    ui.tableWidget_erotyczne->setRowCount(0);
    ui.tableWidget_tematyczne->setRowCount(0);
    ui.tableWidget_regionalne->setRowCount(0);

    QStringList strlLabels;
    strlLabels << tr("Channel name") << tr("Number of persons") << tr("Category") << tr("Type");

    ui.tableWidget_all->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_teen->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_towarzyskie->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_erotyczne->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_tematyczne->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_regionalne->setHorizontalHeaderLabels(strlLabels);
}

void DlgChannelList::add_channel(QString strName, QString strPeople, QString strCat, QString strType)
{
    QList <QString> add;
    add << strName << strPeople << strCat << strType;
    list_all << add;

    if (strType == "Teen")
        list_teen.append(add);
    else if (strType == "Towarzyskie")
        list_towarzyskie.append(add);
    else if (strType == "Erotyczne")
        list_erotyczne.append(add);
    else if (strType == "Tematyczne")
        list_tematyczne.append(add);
    else if (strType == "Regionalne")
        list_regionalne.append(add);
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

    for (int i = 0; i < list_all.size(); i++)
    {
        QString strType = list_all[i][3];

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

    ui.tableWidget_all->setRowCount(iAllCount);
    ui.tableWidget_teen->setRowCount(iTeenCount);
    ui.tableWidget_towarzyskie->setRowCount(iTowarzyskieCount);
    ui.tableWidget_erotyczne->setRowCount(iErotyczneCount);
    ui.tableWidget_tematyczne->setRowCount(iTematyczneCount);
    ui.tableWidget_regionalne->setRowCount(iRegionalneCount);

    // table counters
    int iAllRow = 0;
    int iTeenRow = 0;
    int iTowarzyskieRow = 0;
    int iErotyczneRow = 0;
    int iTematyczneRow = 0;
    int iRegionalneRow = 0;

    for (int i = 0; i < list_all.size(); i++)
    {
        QString strName = list_all[i][0];
        QString strPeople = list_all[i][1];
        QString strCat = list_all[i][2];
        QString strType = list_all[i][3];

        ui.tableWidget_all->setItem(iAllRow, 0, new QTableWidgetItem(strName));
        ui.tableWidget_all->setItem(iAllRow, 1, new QTableWidgetItem(strPeople));
        ui.tableWidget_all->setItem(iAllRow, 2, new QTableWidgetItem(strCat));
        ui.tableWidget_all->setItem(iAllRow, 3, new QTableWidgetItem(strType));
        iAllRow++;

        if (strType == "Teen")
        {
            ui.tableWidget_teen->setItem(iTeenRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_teen->setItem(iTeenRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_teen->setItem(iTeenRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_teen->setItem(iTeenRow, 3, new QTableWidgetItem(strType));
            iTeenRow++;
        }
        else if (strType == "Towarzyskie")
        {
            ui.tableWidget_towarzyskie->setItem(iTowarzyskieRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_towarzyskie->setItem(iTowarzyskieRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_towarzyskie->setItem(iTowarzyskieRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_towarzyskie->setItem(iTowarzyskieRow, 3, new QTableWidgetItem(strType));
            iTowarzyskieRow++;
        }
        else if (strType == "Erotyczne")
        {
            ui.tableWidget_erotyczne->setItem(iErotyczneRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_erotyczne->setItem(iErotyczneRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_erotyczne->setItem(iErotyczneRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_erotyczne->setItem(iErotyczneRow, 3, new QTableWidgetItem(strType));
            iErotyczneRow++;
        }
        else if (strType == "Tematyczne")
        {
            ui.tableWidget_tematyczne->setItem(iTematyczneRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_tematyczne->setItem(iTematyczneRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_tematyczne->setItem(iTematyczneRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_tematyczne->setItem(iTematyczneRow, 3, new QTableWidgetItem(strType));
            iTematyczneRow++;
        }
        else if (strType == "Regionalne")
        {
            ui.tableWidget_regionalne->setItem(iRegionalneRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_regionalne->setItem(iRegionalneRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_regionalne->setItem(iRegionalneRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_regionalne->setItem(iRegionalneRow, 3, new QTableWidgetItem(strType));
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
    quickSort(&list_all, 0, list_all.size()-1);
    quickSort(&list_teen, 0, list_teen.size()-1);
    quickSort(&list_towarzyskie, 0, list_towarzyskie.size()-1);
    quickSort(&list_erotyczne, 0, list_erotyczne.size()-1);
    quickSort(&list_tematyczne, 0, list_tematyczne.size()-1);
    quickSort(&list_regionalne, 0, list_regionalne.size()-1);
    create_list();

    ui.tableWidget_all->resizeColumnsToContents();
    ui.tableWidget_teen->resizeColumnsToContents();
    ui.tableWidget_towarzyskie->resizeColumnsToContents();
    ui.tableWidget_erotyczne->resizeColumnsToContents();
    ui.tableWidget_tematyczne->resizeColumnsToContents();
    ui.tableWidget_regionalne->resizeColumnsToContents();
}

void DlgChannelList::all_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_all->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::teen_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_teen->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::towarzyskie_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_towarzyskie->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::erotyczne_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_erotyczne->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::tematyczne_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_tematyczne->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::regionalne_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_regionalne->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
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

    emit send("SLIST  R- 0 0 100 null");
}

void DlgChannelList::resizeEvent(QResizeEvent *event)
{
    ui.verticalLayoutWidget->setGeometry(QRect(0,0,this->width(), this->height()));
    ui.tabWidget->setGeometry(QRect(0,0,this->width(), this->height()));
    ui.tableWidget_all->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_teen->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_towarzyskie->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_erotyczne->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_tematyczne->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_regionalne->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
}
