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

#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include "network.h"
#include "dlg_channel_list.h"

DlgChannelList::DlgChannelList(QWidget *parent, Network *param1, sChannelList *param2) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Channel list"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    pNetwork = param1;
    stlChannelList = param2;

    ui.pushButton_search->setIcon(QIcon(":/images/oxygen/16x16/edit-find.png"));
    ui.pushButton_clear->setIcon(QIcon(":/images/oxygen/16x16/draw-eraser.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.groupBox_search->setTitle(tr("Search"));
    ui.groupBox_type->setTitle(tr("Type"));
    ui.groupBox_category->setTitle(tr("Category"));

    ui.label_channel_name->setText(tr("Channel name:"));
    ui.pushButton_search->setText(tr("Search"));
    ui.pushButton_clear->setText(tr("Clear"));
    ui.checkBox_hide_empty_channels->setText(tr("Hide empty channels"));
    ui.checkBox_show_adv_options->setText(tr("Show advanced"));

    ui.checkBox_teen->setText(tr("Teen"));
    ui.checkBox_common->setText(tr("Common"));
    ui.checkBox_erotic->setText(tr("Erotic"));
    ui.checkBox_thematic->setText(tr("Thematic"));
    ui.checkBox_regional->setText(tr("Regional"));

    ui.checkBox_wild->setText(tr("Wild"));
    ui.checkBox_tame->setText(tr("Tame"));
    ui.checkBox_with_class->setText(tr("With class"));
    ui.checkBox_cult->setText(tr("Cult"));
    ui.checkBox_moderated->setText(tr("Moderated"));
    ui.checkBox_recommended->setText(tr("Recommended"));

    ui.tabWidget->setTabText(0, tr("All"));
    ui.tabWidget->setTabText(1, tr("Teen"));
    ui.tabWidget->setTabText(2, tr("Common"));
    ui.tabWidget->setTabText(3, tr("Erotic"));
    ui.tabWidget->setTabText(4, tr("Thematic"));
    ui.tabWidget->setTabText(5, tr("Regional"));

    // hide adv options
    ui.groupBox_type->hide();
    ui.groupBox_category->hide();

    QObject::connect(ui.tableWidget_all, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(all_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_teen, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(teen_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_common, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(common_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_erotic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(erotic_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_thematic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(thematic_CellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_regional, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(regional_CellDoubleClicked(int,int)));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    QObject::connect(ui.lineEdit_search, SIGNAL(returnPressed()), this, SLOT(button_search()));
    QObject::connect(ui.pushButton_search, SIGNAL(clicked()), this, SLOT(button_search()));
    QObject::connect(ui.pushButton_clear, SIGNAL(clicked()), this, SLOT(button_clear()));
    QObject::connect(ui.checkBox_hide_empty_channels, SIGNAL(clicked()), this, SLOT(hide_empty_channels()));
    QObject::connect(ui.checkBox_show_adv_options, SIGNAL(clicked()), this, SLOT(show_adv_options()));

    refresh();
    create_list();
    sort();
}

bool DlgChannelList::is_erotic(QString strChannel)
{
    for (int i = 0; i < stlChannelList->size(); i++)
    {
        ChannelList channel = stlChannelList->at(i);
        QString strName = channel.name;
        QString strType = channel.type;

        if (strName == strChannel)
        {
            if (strType == tr("Erotic"))
                return true;
            else
                return false;
        }
    }
    return false;
}

void DlgChannelList::refresh()
{
    button_clear();
    clear();

    for (int i = 0; i < stlChannelList->size(); i++)
    {
        ChannelList channel = stlChannelList->at(i);
        QString strName = channel.name;
        QString strPeople = channel.people;
        QString strCat = channel.cat;
        QString strType = channel.type;

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

void DlgChannelList::show_all_channels()
{
    // all
    for (int i = 0; i < ui.tableWidget_all->rowCount()-1; i++)
        ui.tableWidget_all->showRow(i);

    // teen
    for (int i = 0; i < ui.tableWidget_teen->rowCount()-1; i++)
        ui.tableWidget_teen->showRow(i);

    // common
    for (int i = 0; i < ui.tableWidget_common->rowCount()-1; i++)
        ui.tableWidget_common->showRow(i);

    // erotic
    for (int i = 0; i < ui.tableWidget_erotic->rowCount()-1; i++)
        ui.tableWidget_erotic->showRow(i);

    // thematic
    for (int i = 0; i < ui.tableWidget_thematic->rowCount()-1; i++)
        ui.tableWidget_thematic->showRow(i);

    // regional
    for (int i = 0; i < ui.tableWidget_regional->rowCount()-1; i++)
        ui.tableWidget_regional->showRow(i);
}

void DlgChannelList::apply_checkboxes()
{
    bool bShowTeen;
    bool bShowCommon;
    bool bShowErotic;
    bool bShowThematic;
    bool bShowRegional;

    bool bShowWild;
    bool bShowTame;
    bool bShowWithClass;
    bool bShowCult;
    bool bShowModerated;
    bool bShowRecommended;

    // teen
    if (ui.checkBox_teen->isChecked() == true)
        bShowTeen = true;
    else
        bShowTeen = false;

    // common
    if (ui.checkBox_common->isChecked() == true)
        bShowCommon = true;
    else
        bShowCommon = false;

    // erotic
    if (ui.checkBox_erotic->isChecked() == true)
        bShowErotic = true;
    else
        bShowErotic = false;

    // thematic
    if (ui.checkBox_thematic->isChecked() == true)
        bShowThematic = true;
    else
        bShowThematic = false;

    // regional
    if (ui.checkBox_regional->isChecked() == true)
        bShowRegional = true;
    else
        bShowRegional = false;

    // wild
    if (ui.checkBox_wild->isChecked() == true)
        bShowWild = true;
    else
        bShowWild = false;

    // tame
    if (ui.checkBox_tame->isChecked() == true)
        bShowTame = true;
    else
        bShowTame = false;

    // with class
    if (ui.checkBox_with_class->isChecked() == true)
        bShowWithClass = true;
    else
        bShowWithClass = false;

    // cult
    if (ui.checkBox_cult->isChecked() == true)
        bShowCult = true;
    else
        bShowCult = false;

    // moderated
    if (ui.checkBox_moderated->isChecked() == true)
        bShowModerated = true;
    else
        bShowModerated = false;

    // recommended
    if (ui.checkBox_recommended->isChecked() == true)
        bShowRecommended = true;
    else
        bShowRecommended = false;

    // all
    for (int i = 0; i < ui.tableWidget_all->rowCount(); i++)
    {
        QString strCat = ui.tableWidget_all->item(i, 2)->text();
        QString strType = ui.tableWidget_all->item(i, 3)->text();

        if ((bShowTeen == false) && (strType == tr("Teen")))
            ui.tableWidget_all->hideRow(i);

        if ((bShowCommon == false) && (strType == tr("Common")))
            ui.tableWidget_all->hideRow(i);

        if ((bShowErotic == false) && (strType == tr("Erotic")))
            ui.tableWidget_all->hideRow(i);

        if ((bShowThematic == false) && (strType == tr("Thematic")))
            ui.tableWidget_all->hideRow(i);

        if ((bShowRegional == false) && (strType == tr("Regional")))
            ui.tableWidget_all->hideRow(i);

        if ((bShowWild == false) && (strCat.indexOf(tr("Wild")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_all->showRow(i);
            // rest
            else
                ui.tableWidget_all->hideRow(i);
        }

        if ((bShowTame == false) && (strCat.indexOf(tr("Tame")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_all->showRow(i);
            // rest
            else
                ui.tableWidget_all->hideRow(i);
        }

        if ((bShowWithClass == false) && (strCat.indexOf(tr("With class")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_all->showRow(i);
            // rest
            else
                ui.tableWidget_all->hideRow(i);
        }

        if ((bShowCult == false) && (strCat.indexOf(tr("Cult")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_all->showRow(i);
            // rest
            else
                ui.tableWidget_all->hideRow(i);
        }
    }

    // teen
    for (int i = 0; i < ui.tableWidget_teen->rowCount(); i++)
    {
        QString strCat = ui.tableWidget_teen->item(i, 2)->text();

        if ((bShowWild == false) && (strCat.indexOf(tr("Wild")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_teen->showRow(i);
            // rest
            else
                ui.tableWidget_teen->hideRow(i);
        }

        if ((bShowTame == false) && (strCat.indexOf(tr("Tame")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_teen->showRow(i);
            // rest
            else
                ui.tableWidget_teen->hideRow(i);
        }

        if ((bShowWithClass == false) && (strCat.indexOf(tr("With class")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_teen->showRow(i);
            // rest
            else
                ui.tableWidget_teen->hideRow(i);
        }

        if ((bShowCult == false) && (strCat.indexOf(tr("Cult")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_teen->showRow(i);
            // rest
            else
                ui.tableWidget_teen->hideRow(i);
        }
    }

    // common
    for (int i = 0; i < ui.tableWidget_common->rowCount(); i++)
    {
        QString strCat = ui.tableWidget_common->item(i, 2)->text();

        if ((bShowWild == false) && (strCat.indexOf(tr("Wild")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_common->showRow(i);
            // rest
            else
                ui.tableWidget_common->hideRow(i);
        }

        if ((bShowTame == false) && (strCat.indexOf(tr("Tame")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_common->showRow(i);
            // rest
            else
                ui.tableWidget_common->hideRow(i);
        }

        if ((bShowWithClass == false) && (strCat.indexOf(tr("With class")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_common->showRow(i);
            // rest
            else
                ui.tableWidget_common->hideRow(i);
        }

        if ((bShowCult == false) && (strCat.indexOf(tr("Cult")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_common->showRow(i);
            // rest
            else
                ui.tableWidget_common->hideRow(i);
        }
    }

    // erotic
    for (int i = 0; i < ui.tableWidget_erotic->rowCount(); i++)
    {
        QString strCat = ui.tableWidget_erotic->item(i, 2)->text();

        if ((bShowWild == false) && (strCat.indexOf(tr("Wild")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // rest
            else
                ui.tableWidget_erotic->hideRow(i);
        }

        if ((bShowTame == false) && (strCat.indexOf(tr("Tame")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // rest
            else
                ui.tableWidget_erotic->hideRow(i);
        }

        if ((bShowWithClass == false) && (strCat.indexOf(tr("With class")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // rest
            else
                ui.tableWidget_erotic->hideRow(i);
        }

        if ((bShowCult == false) && (strCat.indexOf(tr("Cult")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_erotic->showRow(i);
            // rest
            else
                ui.tableWidget_erotic->hideRow(i);
        }
    }

    // thematic
    for (int i = 0; i < ui.tableWidget_thematic->rowCount(); i++)
    {
        QString strCat = ui.tableWidget_thematic->item(i, 2)->text();

        if ((bShowWild == false) && (strCat.indexOf(tr("Wild")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // rest
            else
                ui.tableWidget_thematic->hideRow(i);
        }

        if ((bShowTame == false) && (strCat.indexOf(tr("Tame")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // rest
            else
                ui.tableWidget_thematic->hideRow(i);
        }

        if ((bShowWithClass == false) && (strCat.indexOf(tr("With class")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // rest
            else
                ui.tableWidget_thematic->hideRow(i);
        }

        if ((bShowCult == false) && (strCat.indexOf(tr("Cult")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_thematic->showRow(i);
            // rest
            else
                ui.tableWidget_thematic->hideRow(i);
        }
    }

    // regional
    for (int i = 0; i < ui.tableWidget_regional->rowCount(); i++)
    {
        QString strCat = ui.tableWidget_regional->item(i, 2)->text();

        if ((bShowWild == false) && (strCat.indexOf(tr("Wild")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }

        if ((bShowTame == false) && (strCat.indexOf(tr("Tame")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }

        if ((bShowWithClass == false) && (strCat.indexOf(tr("With class")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }

        if ((bShowCult == false) && (strCat.indexOf(tr("Cult")) != -1))
        {
            // moderated
            if ((ui.checkBox_moderated->isChecked() == true) && (strCat.indexOf(tr("Moderated")) != -1))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((ui.checkBox_recommended->isChecked() == true) && (strCat.indexOf(tr("Recommended")) != -1))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }
    }
}

void DlgChannelList::all_CellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_all->item(row, 0)->text();

    if (is_erotic(strChannel) == true)
    {
        QSettings settings;
        if (settings.value("age_check").toString() == "on")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults.")).arg(tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
            {
                settings.setValue("age_check", "off");
                pNetwork->send(QString("JOIN %1").arg(strChannel));
            }
        }
        else
            pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
    else
        pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::teen_CellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_teen->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::common_CellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_common->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::erotic_CellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_erotic->item(row, 0)->text();

    if (is_erotic(strChannel) == true)
    {
        QSettings settings;
        if (settings.value("age_check").toString() == "on")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults.")).arg(tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
            {
                settings.setValue("age_check", "off");
                pNetwork->send(QString("JOIN %1").arg(strChannel));
            }
        }
        else
            pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
    else
        pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::thematic_CellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_thematic->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::regional_CellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_regional->item(row, 0)->text();
    pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::button_close()
{
    this->close();
}

void DlgChannelList::button_search()
{
    // show all channels
    show_all_channels();

    // apply checkboxes - hide selected channels
    apply_checkboxes();

    // find channel
    // all
    for (int i = 0; i < ui.tableWidget_all->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_all->item(i, 0)->text();
        if (strChannelName.toLower().indexOf(ui.lineEdit_search->text().toLower()) == -1)
            ui.tableWidget_all->hideRow(i);
    }

    // teen
    for (int i = 0; i < ui.tableWidget_teen->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_teen->item(i, 0)->text();
        if (strChannelName.toLower().indexOf(ui.lineEdit_search->text().toLower()) == -1)
            ui.tableWidget_teen->hideRow(i);
    }

    // common
    for (int i = 0; i < ui.tableWidget_common->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_common->item(i, 0)->text();
        if (strChannelName.toLower().indexOf(ui.lineEdit_search->text().toLower()) == -1)
            ui.tableWidget_common->hideRow(i);
    }

    // erotic
    for (int i = 0; i < ui.tableWidget_erotic->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_erotic->item(i, 0)->text();
        if (strChannelName.toLower().indexOf(ui.lineEdit_search->text().toLower()) == -1)
            ui.tableWidget_erotic->hideRow(i);
    }

    // thematic
    for (int i = 0; i < ui.tableWidget_thematic->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_thematic->item(i, 0)->text();
        if (strChannelName.toLower().indexOf(ui.lineEdit_search->text().toLower()) == -1)
            ui.tableWidget_thematic->hideRow(i);
    }

    // regional
    for (int i = 0; i < ui.tableWidget_regional->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_regional->item(i, 0)->text();
        if (strChannelName.toLower().indexOf(ui.lineEdit_search->text().toLower()) == -1)
            ui.tableWidget_regional->hideRow(i);
    }
}

void DlgChannelList::button_clear()
{
    ui.lineEdit_search->setText("");

    ui.checkBox_teen->setChecked(true);
    ui.checkBox_common->setChecked(true);
    ui.checkBox_erotic->setChecked(true);
    ui.checkBox_thematic->setChecked(true);
    ui.checkBox_regional->setChecked(true);

    ui.checkBox_wild->setChecked(true);
    ui.checkBox_tame->setChecked(true);
    ui.checkBox_with_class->setChecked(true);
    ui.checkBox_cult->setChecked(true);
    ui.checkBox_moderated->setChecked(true);
    ui.checkBox_recommended->setChecked(true);

    ui.checkBox_hide_empty_channels->setChecked(false);

    show_all_channels();
}

void DlgChannelList::hide_empty_channels()
{
    bool bHide;

    if (ui.checkBox_hide_empty_channels->isChecked() == true)
        bHide = true;
    else
        bHide = false;

    // all
    for (int i = 0; i < ui.tableWidget_all->rowCount(); i++)
    {
        QString strPeople = ui.tableWidget_all->item(i, 1)->text();
        if (strPeople == "0")
        {
            if (bHide == true)
                ui.tableWidget_all->hideRow(i);
            else
                ui.tableWidget_all->showRow(i);
        }
    }

    // teen
    for (int i = 0; i < ui.tableWidget_teen->rowCount(); i++)
    {
        QString strPeople = ui.tableWidget_teen->item(i, 1)->text();
        if (strPeople == "0")
        {
            if (bHide == true)
                ui.tableWidget_teen->hideRow(i);
            else
                ui.tableWidget_teen->showRow(i);
        }
    }

    // common
    for (int i = 0; i < ui.tableWidget_common->rowCount(); i++)
    {
        QString strPeople = ui.tableWidget_common->item(i, 1)->text();
        if (strPeople == "0")
        {
            if (bHide == true)
                ui.tableWidget_common->hideRow(i);
            else
                ui.tableWidget_common->showRow(i);
        }
    }

    // erotic
    for (int i = 0; i < ui.tableWidget_erotic->rowCount(); i++)
    {
        QString strPeople = ui.tableWidget_erotic->item(i, 1)->text();
        if (strPeople == "0")
        {
            if (bHide == true)
                ui.tableWidget_erotic->hideRow(i);
            else
                ui.tableWidget_erotic->showRow(i);
        }
    }

    // thematic
    for (int i = 0; i < ui.tableWidget_thematic->rowCount(); i++)
    {
        QString strPeople = ui.tableWidget_thematic->item(i, 1)->text();
        if (strPeople == "0")
        {
            if (bHide == true)
                ui.tableWidget_thematic->hideRow(i);
            else
                ui.tableWidget_thematic->showRow(i);
        }
    }

    // regional
    for (int i = 0; i < ui.tableWidget_regional->rowCount(); i++)
    {
        QString strPeople = ui.tableWidget_regional->item(i, 1)->text();
        if (strPeople == "0")
        {
            if (bHide == true)
                ui.tableWidget_regional->hideRow(i);
            else
                ui.tableWidget_regional->showRow(i);
        }
    }
}

void DlgChannelList::show_adv_options()
{
    if (ui.checkBox_show_adv_options->isChecked() == true)
    {
        ui.groupBox_type->show();
        ui.groupBox_category->show();
    }
    else
    {
        ui.groupBox_type->hide();
        ui.groupBox_category->hide();
    }
}

void DlgChannelList::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED (event);

    ui.verticalLayoutWidget->setGeometry(QRect(0, 0, this->width(), this->height()));
    ui.tabWidget->setGeometry(QRect(205, 1, this->width()-205, this->height()-30));
    ui.tableWidget_all->setGeometry(QRect(0, 0, ui.tabWidget->width()-10, ui.tabWidget->height()-30));
    ui.tableWidget_teen->setGeometry(QRect(0, 0, ui.tabWidget->width()-10, ui.tabWidget->height()-30));
    ui.tableWidget_common->setGeometry(QRect(0, 0, ui.tabWidget->width()-10, ui.tabWidget->height()-30));
    ui.tableWidget_erotic->setGeometry(QRect(0, 0, ui.tabWidget->width()-10, ui.tabWidget->height()-30));
    ui.tableWidget_thematic->setGeometry(QRect(0, 0, ui.tabWidget->width()-10, ui.tabWidget->height()-30));
    ui.tableWidget_regional->setGeometry(QRect(0, 0, ui.tabWidget->width()-10, ui.tabWidget->height()-30));
}

void DlgChannelList::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            button_search();
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}
