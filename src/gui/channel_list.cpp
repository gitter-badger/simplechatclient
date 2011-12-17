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
#include "core.h"
#include "channel_list.h"

DlgChannelList::DlgChannelList(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Channel list"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    setDefaultValues();
    createSignals();

    createList();
}

void DlgChannelList::createGui()
{
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

    QStringList lLabels;
    lLabels << tr("Channel name") << tr("Number of persons") << tr("Category") << tr("Type");

    ui.tableWidget_all->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_teen->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_common->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_erotic->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_thematic->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_regional->setHorizontalHeaderLabels(lLabels);
}

void DlgChannelList::setDefaultValues()
{
    // set checked
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

    // hide adv options
    ui.groupBox_type->hide();
    ui.groupBox_category->hide();
}

void DlgChannelList::createSignals()
{
    QObject::connect(ui.tableWidget_all, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(allCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_teen, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(teenCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_common, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(commonCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_erotic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(eroticCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_thematic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(thematicCellDoubleClicked(int,int)));
    QObject::connect(ui.tableWidget_regional, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(regionalCellDoubleClicked(int,int)));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    QObject::connect(ui.lineEdit_search, SIGNAL(returnPressed()), this, SLOT(buttonSearch()));
    QObject::connect(ui.pushButton_search, SIGNAL(clicked()), this, SLOT(buttonSearch()));
    QObject::connect(ui.pushButton_clear, SIGNAL(clicked()), this, SLOT(buttonClear()));
    QObject::connect(ui.checkBox_hide_empty_channels, SIGNAL(clicked()), this, SLOT(hideEmptyChannels()));
    QObject::connect(ui.checkBox_show_adv_options, SIGNAL(clicked()), this, SLOT(showAdvOptions()));
}

bool DlgChannelList::isErotic(QString strChannel)
{
    for (int i = 0; i < Core::instance()->lChannelList.size(); i++)
    {
        ChannelList channel = Core::instance()->lChannelList.at(i);
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

void DlgChannelList::createList()
{
    // count rows
    int iAllCount = 0;
    int iTeenCount = 0;
    int iCommonCount = 0;
    int iEroticCount = 0;
    int iThematicCount = 0;
    int iRegionalCount = 0;

    for (int i = 0; i < Core::instance()->lChannelList.size(); i++)
    {
        ChannelList channel = Core::instance()->lChannelList.at(i);
        QString strType = channel.type;

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

    for (int i = 0; i < Core::instance()->lChannelList.size(); i++)
    {
        ChannelList channel = Core::instance()->lChannelList.at(i);
        QString strName = channel.name;
        QString strPeople = channel.people;
        QString strCat = channel.cat;
        QString strType = channel.type;

        ui.tableWidget_all->setItem(iAllRow, 0, new SortedChannelListTableWidgetItem(strName));
        ui.tableWidget_all->setItem(iAllRow, 1, new SortedChannelListTableWidgetItem(strPeople));
        ui.tableWidget_all->setItem(iAllRow, 2, new SortedChannelListTableWidgetItem(strCat));
        ui.tableWidget_all->setItem(iAllRow, 3, new SortedChannelListTableWidgetItem(strType));

        iAllRow++;

        if (strType == tr("Teen"))
        {
            ui.tableWidget_teen->setItem(iTeenRow, 0, new SortedChannelListTableWidgetItem(strName));
            ui.tableWidget_teen->setItem(iTeenRow, 1, new SortedChannelListTableWidgetItem(strPeople));
            ui.tableWidget_teen->setItem(iTeenRow, 2, new SortedChannelListTableWidgetItem(strCat));
            ui.tableWidget_teen->setItem(iTeenRow, 3, new SortedChannelListTableWidgetItem(strType));
            iTeenRow++;
        }
        else if (strType == tr("Common"))
        {
            ui.tableWidget_common->setItem(iCommonRow, 0, new SortedChannelListTableWidgetItem(strName));
            ui.tableWidget_common->setItem(iCommonRow, 1, new SortedChannelListTableWidgetItem(strPeople));
            ui.tableWidget_common->setItem(iCommonRow, 2, new SortedChannelListTableWidgetItem(strCat));
            ui.tableWidget_common->setItem(iCommonRow, 3, new SortedChannelListTableWidgetItem(strType));
            iCommonRow++;
        }
        else if (strType == tr("Erotic"))
        {
            ui.tableWidget_erotic->setItem(iEroticRow, 0, new SortedChannelListTableWidgetItem(strName));
            ui.tableWidget_erotic->setItem(iEroticRow, 1, new SortedChannelListTableWidgetItem(strPeople));
            ui.tableWidget_erotic->setItem(iEroticRow, 2, new SortedChannelListTableWidgetItem(strCat));
            ui.tableWidget_erotic->setItem(iEroticRow, 3, new SortedChannelListTableWidgetItem(strType));
            iEroticRow++;
        }
        else if (strType == tr("Thematic"))
        {
            ui.tableWidget_thematic->setItem(iThematicRow, 0, new SortedChannelListTableWidgetItem(strName));
            ui.tableWidget_thematic->setItem(iThematicRow, 1, new SortedChannelListTableWidgetItem(strPeople));
            ui.tableWidget_thematic->setItem(iThematicRow, 2, new SortedChannelListTableWidgetItem(strCat));
            ui.tableWidget_thematic->setItem(iThematicRow, 3, new SortedChannelListTableWidgetItem(strType));
            iThematicRow++;
        }
        else if (strType == tr("Regional"))
        {
            ui.tableWidget_regional->setItem(iRegionalRow, 0, new SortedChannelListTableWidgetItem(strName));
            ui.tableWidget_regional->setItem(iRegionalRow, 1, new SortedChannelListTableWidgetItem(strPeople));
            ui.tableWidget_regional->setItem(iRegionalRow, 2, new SortedChannelListTableWidgetItem(strCat));
            ui.tableWidget_regional->setItem(iRegionalRow, 3, new SortedChannelListTableWidgetItem(strType));
            iRegionalRow++;
        }
    }

    ui.tableWidget_all->sortByColumn(1);
    ui.tableWidget_teen->sortByColumn(1);
    ui.tableWidget_common->sortByColumn(1);
    ui.tableWidget_erotic->sortByColumn(1);
    ui.tableWidget_thematic->sortByColumn(1);
    ui.tableWidget_regional->sortByColumn(1);
}

void DlgChannelList::showAllChannels()
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

void DlgChannelList::applyCheckboxes()
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
    if (ui.checkBox_teen->isChecked())
        bShowTeen = true;
    else
        bShowTeen = false;

    // common
    if (ui.checkBox_common->isChecked())
        bShowCommon = true;
    else
        bShowCommon = false;

    // erotic
    if (ui.checkBox_erotic->isChecked())
        bShowErotic = true;
    else
        bShowErotic = false;

    // thematic
    if (ui.checkBox_thematic->isChecked())
        bShowThematic = true;
    else
        bShowThematic = false;

    // regional
    if (ui.checkBox_regional->isChecked())
        bShowRegional = true;
    else
        bShowRegional = false;

    // wild
    if (ui.checkBox_wild->isChecked())
        bShowWild = true;
    else
        bShowWild = false;

    // tame
    if (ui.checkBox_tame->isChecked())
        bShowTame = true;
    else
        bShowTame = false;

    // with class
    if (ui.checkBox_with_class->isChecked())
        bShowWithClass = true;
    else
        bShowWithClass = false;

    // cult
    if (ui.checkBox_cult->isChecked())
        bShowCult = true;
    else
        bShowCult = false;

    // moderated
    if (ui.checkBox_moderated->isChecked())
        bShowModerated = true;
    else
        bShowModerated = false;

    // recommended
    if (ui.checkBox_recommended->isChecked())
        bShowRecommended = true;
    else
        bShowRecommended = false;

    // all
    for (int i = 0; i < ui.tableWidget_all->rowCount(); i++)
    {
        QString strCat = ui.tableWidget_all->item(i, 2)->text();
        QString strType = ui.tableWidget_all->item(i, 3)->text();

        if ((!bShowTeen) && (strType == tr("Teen")))
            ui.tableWidget_all->hideRow(i);

        if ((!bShowCommon) && (strType == tr("Common")))
            ui.tableWidget_all->hideRow(i);

        if ((!bShowErotic) && (strType == tr("Erotic")))
            ui.tableWidget_all->hideRow(i);

        if ((!bShowThematic) && (strType == tr("Thematic")))
            ui.tableWidget_all->hideRow(i);

        if ((!bShowRegional) && (strType == tr("Regional")))
            ui.tableWidget_all->hideRow(i);

        if ((!bShowWild) && (strCat.contains(tr("Wild"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_all->showRow(i);
            // rest
            else
                ui.tableWidget_all->hideRow(i);
        }

        if ((!bShowTame) && (strCat.contains(tr("Tame"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_all->showRow(i);
            // rest
            else
                ui.tableWidget_all->hideRow(i);
        }

        if ((!bShowWithClass) && (strCat.contains(tr("With class"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_all->showRow(i);
            // rest
            else
                ui.tableWidget_all->hideRow(i);
        }

        if ((!bShowCult) && (strCat.contains(tr("Cult"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_all->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
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

        if ((!bShowWild) && (strCat.contains(tr("Wild"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_teen->showRow(i);
            // rest
            else
                ui.tableWidget_teen->hideRow(i);
        }

        if ((!bShowTame) && (strCat.contains(tr("Tame"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_teen->showRow(i);
            // rest
            else
                ui.tableWidget_teen->hideRow(i);
        }

        if ((!bShowWithClass) && (strCat.contains(tr("With class"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_teen->showRow(i);
            // rest
            else
                ui.tableWidget_teen->hideRow(i);
        }

        if ((!bShowCult) && (strCat.contains(tr("Cult"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_teen->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
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

        if ((!bShowWild) && (strCat.contains(tr("Wild"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_common->showRow(i);
            // rest
            else
                ui.tableWidget_common->hideRow(i);
        }

        if ((!bShowTame) && (strCat.contains(tr("Tame"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_common->showRow(i);
            // rest
            else
                ui.tableWidget_common->hideRow(i);
        }

        if ((!bShowWithClass) && (strCat.contains(tr("With class"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_common->showRow(i);
            // rest
            else
                ui.tableWidget_common->hideRow(i);
        }

        if ((!bShowCult) && (strCat.contains(tr("Cult"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_common->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
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

        if ((!bShowWild) && (strCat.contains(tr("Wild"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_erotic->showRow(i);
            // rest
            else
                ui.tableWidget_erotic->hideRow(i);
        }

        if ((!bShowTame) && (strCat.contains(tr("Tame"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_erotic->showRow(i);
            // rest
            else
                ui.tableWidget_erotic->hideRow(i);
        }

        if ((!bShowWithClass) && (strCat.contains(tr("With class"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_erotic->showRow(i);
            // rest
            else
                ui.tableWidget_erotic->hideRow(i);
        }

        if ((!bShowCult) && (strCat.contains(tr("Cult"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_erotic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
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

        if ((!bShowWild) && (strCat.contains(tr("Wild"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_thematic->showRow(i);
            // rest
            else
                ui.tableWidget_thematic->hideRow(i);
        }

        if ((!bShowTame) && (strCat.contains(tr("Tame"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_thematic->showRow(i);
            // rest
            else
                ui.tableWidget_thematic->hideRow(i);
        }

        if ((!bShowWithClass) && (strCat.contains(tr("With class"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_thematic->showRow(i);
            // rest
            else
                ui.tableWidget_thematic->hideRow(i);
        }

        if ((!bShowCult) && (strCat.contains(tr("Cult"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_thematic->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
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

        if ((!bShowWild) && (strCat.contains(tr("Wild"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }

        if ((!bShowTame) && (strCat.contains(tr("Tame"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }

        if ((!bShowWithClass) && (strCat.contains(tr("With class"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }

        if ((!bShowCult) && (strCat.contains(tr("Cult"))))
        {
            // moderated
            if ((bShowModerated) && (strCat.contains(tr("Moderated"))))
                ui.tableWidget_regional->showRow(i);
            // recommended
            else if ((bShowRecommended) && (strCat.contains(tr("Recommended"))))
                ui.tableWidget_regional->showRow(i);
            // rest
            else
                ui.tableWidget_regional->hideRow(i);
        }
    }
}

void DlgChannelList::allCellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_all->item(row, 0)->text();

    if (isErotic(strChannel))
    {
        if (Core::instance()->settings.value("age_check") == "on")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults."), tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
                Core::instance()->settings["age_check"] = "off";
            else
                return;
        }
    }
    Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::teenCellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_teen->item(row, 0)->text();
    Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::commonCellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_common->item(row, 0)->text();
    Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::eroticCellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_erotic->item(row, 0)->text();

    if (isErotic(strChannel))
    {
        if (Core::instance()->settings.value("age_check") == "on")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults."), tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
            {
                Core::instance()->settings["age_check"] = "off";
                Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
            }
        }
        else
            Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
    }
    else
        Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::thematicCellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_thematic->item(row, 0)->text();
    Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::regionalCellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_regional->item(row, 0)->text();
    Core::instance()->pNetwork->send(QString("JOIN %1").arg(strChannel));
}

void DlgChannelList::buttonSearch()
{
    // show all channels
    showAllChannels();

    // apply checkboxes - hide selected channels
    applyCheckboxes();

    // find channel
    // all
    for (int i = 0; i < ui.tableWidget_all->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_all->item(i, 0)->text();
        if (!strChannelName.toLower().contains(ui.lineEdit_search->text().toLower()))
            ui.tableWidget_all->hideRow(i);
    }

    // teen
    for (int i = 0; i < ui.tableWidget_teen->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_teen->item(i, 0)->text();
        if (!strChannelName.toLower().contains(ui.lineEdit_search->text().toLower()))
            ui.tableWidget_teen->hideRow(i);
    }

    // common
    for (int i = 0; i < ui.tableWidget_common->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_common->item(i, 0)->text();
        if (!strChannelName.toLower().contains(ui.lineEdit_search->text().toLower()))
            ui.tableWidget_common->hideRow(i);
    }

    // erotic
    for (int i = 0; i < ui.tableWidget_erotic->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_erotic->item(i, 0)->text();
        if (!strChannelName.toLower().contains(ui.lineEdit_search->text().toLower()))
            ui.tableWidget_erotic->hideRow(i);
    }

    // thematic
    for (int i = 0; i < ui.tableWidget_thematic->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_thematic->item(i, 0)->text();
        if (!strChannelName.toLower().contains(ui.lineEdit_search->text().toLower()))
            ui.tableWidget_thematic->hideRow(i);
    }

    // regional
    for (int i = 0; i < ui.tableWidget_regional->rowCount(); i++)
    {
        QString strChannelName = ui.tableWidget_regional->item(i, 0)->text();
        if (!strChannelName.toLower().contains(ui.lineEdit_search->text().toLower()))
            ui.tableWidget_regional->hideRow(i);
    }
}

void DlgChannelList::buttonClear()
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

    showAllChannels();
}

void DlgChannelList::hideEmptyChannels()
{
    bool bHide;

    if (ui.checkBox_hide_empty_channels->isChecked())
        bHide = true;
    else
        bHide = false;

    // all
    for (int i = 0; i < ui.tableWidget_all->rowCount(); i++)
    {
        QString strPeople = ui.tableWidget_all->item(i, 1)->text();
        if (strPeople == "0")
        {
            if (bHide)
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
            if (bHide)
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
            if (bHide)
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
            if (bHide)
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
            if (bHide)
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
            if (bHide)
                ui.tableWidget_regional->hideRow(i);
            else
                ui.tableWidget_regional->showRow(i);
        }
    }
}

void DlgChannelList::showAdvOptions()
{
    if (ui.checkBox_show_adv_options->isChecked())
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

void DlgChannelList::resizeEvent(QResizeEvent *)
{
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
            buttonSearch();
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}
