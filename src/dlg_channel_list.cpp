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

DlgChannelList::DlgChannelList(QWidget *parent, QSettings *param1, TabContainer *param2) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Channel list"));

    settings = param1;
    tabc = param2;

    ui.tabWidget->setTabText(0, tr("All"));
    ui.tabWidget->setTabText(1, tr("Teen"));
    ui.tabWidget->setTabText(2, tr("Common"));
    ui.tabWidget->setTabText(3, tr("Erotic"));
    ui.tabWidget->setTabText(4, tr("Thematic"));
    ui.tabWidget->setTabText(5, tr("Regional"));

    QObject::connect(ui.tableWidget_all, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(all_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_teen, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(teen_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_common, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(common_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_erotic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(erotic_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_thematic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(thematic_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_regional, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(regional_CellDoubleClicked(int,int)));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgChannelList::clear()
{
    list_all.clear();
    list_teen.clear();
    list_common.clear();
    list_erotic.clear();
    list_thematic.clear();
    list_regional.clear();

    ui.tableWidget_all->clear();
    ui.tableWidget_teen->clear();
    ui.tableWidget_common->clear();
    ui.tableWidget_erotic->clear();
    ui.tableWidget_thematic->clear();
    ui.tableWidget_regional->clear();

    ui.tableWidget_all->setRowCount(0);
    ui.tableWidget_teen->setRowCount(0);
    ui.tableWidget_common->setRowCount(0);
    ui.tableWidget_erotic->setRowCount(0);
    ui.tableWidget_thematic->setRowCount(0);
    ui.tableWidget_regional->setRowCount(0);

    QStringList strlLabels;
    strlLabels << tr("Channel name") << tr("Number of persons") << tr("Category") << tr("Type");

    ui.tableWidget_all->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_teen->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_common->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_erotic->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_thematic->setHorizontalHeaderLabels(strlLabels);
    ui.tableWidget_regional->setHorizontalHeaderLabels(strlLabels);
}

void DlgChannelList::add_channel(QString strName, QString strPeople, QString strCat, QString strType)
{
    QList <QString> add;
    add << strName << strPeople << strCat << strType;
    list_all << add;

    if (strType == tr("Teen"))
        list_teen.append(add);
    else if (strType == tr("Common"))
        list_common.append(add);
    else if (strType == tr("Erotic"))
        list_erotic.append(add);
    else if (strType == tr("Thematic"))
        list_thematic.append(add);
    else if (strType == tr("Regional"))
        list_regional.append(add);
}

void DlgChannelList::create_list()
{
    // count rows
    int iAllCount = 0;
    int iTeenCount = 0;
    int iCommonCount = 0;
    int iEroticCount = 0;
    int iThematicCount = 0;
    int iRegionalCount = 0;

    for (int i = 0; i < list_all.size(); i++)
    {
        QString strType = list_all[i][3];

        iAllCount++;
        if (strType == tr("Teen"))
            iTeenCount++;
        else if (strType == tr("Common"))
            iCommonCount++;
        else if (strType == tr("Erotic"))
            iEroticCount++;
        else if (strType == tr("Thematic"))
            iThematicCount++;
        else if (strType == tr("Regional"))
            iRegionalCount++;
    }

    ui.tableWidget_all->setRowCount(iAllCount);
    ui.tableWidget_teen->setRowCount(iTeenCount);
    ui.tableWidget_common->setRowCount(iCommonCount);
    ui.tableWidget_erotic->setRowCount(iEroticCount);
    ui.tableWidget_thematic->setRowCount(iThematicCount);
    ui.tableWidget_regional->setRowCount(iRegionalCount);

    // table counters
    int iAllRow = 0;
    int iTeenRow = 0;
    int iCommonRow = 0;
    int iEroticRow = 0;
    int iThematicRow = 0;
    int iRegionalRow = 0;

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

        if (strType == tr("Teen"))
        {
            ui.tableWidget_teen->setItem(iTeenRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_teen->setItem(iTeenRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_teen->setItem(iTeenRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_teen->setItem(iTeenRow, 3, new QTableWidgetItem(strType));
            iTeenRow++;
        }
        else if (strType == tr("Common"))
        {
            ui.tableWidget_common->setItem(iCommonRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_common->setItem(iCommonRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_common->setItem(iCommonRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_common->setItem(iCommonRow, 3, new QTableWidgetItem(strType));
            iCommonRow++;
        }
        else if (strType == tr("Erotic"))
        {
            ui.tableWidget_erotic->setItem(iEroticRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_erotic->setItem(iEroticRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_erotic->setItem(iEroticRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_erotic->setItem(iEroticRow, 3, new QTableWidgetItem(strType));
            iEroticRow++;
        }
        else if (strType == tr("Thematic"))
        {
            ui.tableWidget_thematic->setItem(iThematicRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_thematic->setItem(iThematicRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_thematic->setItem(iThematicRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_thematic->setItem(iThematicRow, 3, new QTableWidgetItem(strType));
            iThematicRow++;
        }
        else if (strType == tr("Regional"))
        {
            ui.tableWidget_regional->setItem(iRegionalRow, 0, new QTableWidgetItem(strName));
            ui.tableWidget_regional->setItem(iRegionalRow, 1, new QTableWidgetItem(strPeople));
            ui.tableWidget_regional->setItem(iRegionalRow, 2, new QTableWidgetItem(strCat));
            ui.tableWidget_regional->setItem(iRegionalRow, 3, new QTableWidgetItem(strType));
            iRegionalRow++;
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
    quickSort(&list_common, 0, list_common.size()-1);
    quickSort(&list_erotic, 0, list_erotic.size()-1);
    quickSort(&list_thematic, 0, list_thematic.size()-1);
    quickSort(&list_regional, 0, list_regional.size()-1);
    create_list();

    ui.tableWidget_all->resizeColumnsToContents();
    ui.tableWidget_teen->resizeColumnsToContents();
    ui.tableWidget_common->resizeColumnsToContents();
    ui.tableWidget_erotic->resizeColumnsToContents();
    ui.tableWidget_thematic->resizeColumnsToContents();
    ui.tableWidget_regional->resizeColumnsToContents();
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

void DlgChannelList::common_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_common->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::erotic_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_erotic->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::thematic_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_thematic->item(row, 0)->text();
    emit send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::regional_CellDoubleClicked(int row, int column)
{
    QString strChannel = ui.tableWidget_regional->item(row, 0)->text();
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
    ui.tableWidget_common->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_erotic->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_thematic->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
    ui.tableWidget_regional->setGeometry(QRect(0,0,this->width()-10, this->height()-65));
}

void DlgChannelList::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
