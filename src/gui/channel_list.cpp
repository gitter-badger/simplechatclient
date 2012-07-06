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
#include <QKeyEvent>
#include <QMessageBox>
#include <QTimer>
#include "core.h"
#include "channel_list_model.h"
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

    ui.tabWidget->setTabIcon(0, QIcon(":/images/oxygen/16x16/accessories-dictionary.png"));
    ui.tabWidget->setTabIcon(1, QIcon(":/images/oxygen/16x16/applications-education-university.png"));
    ui.tabWidget->setTabIcon(2, QIcon(":/images/oxygen/16x16/system-users.png"));
    ui.tabWidget->setTabIcon(3, QIcon(":/images/oxygen/16x16/emblem-favorite.png"));
    ui.tabWidget->setTabIcon(4, QIcon(":/images/oxygen/16x16/karbon.png"));
    ui.tabWidget->setTabIcon(5, QIcon(":/images/oxygen/16x16/applications-education-language.png"));
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

    // need refresh
    int iCheckRefresh = QDateTime::currentDateTime().toTime_t() - ChannelList::instance()->getTime();
    if ((ChannelList::instance()->getReady()) && (iCheckRefresh > 3600))
        Core::instance()->pNetwork->send("SLIST  R- 0 0 100 null");
}

void DlgChannelList::createSignals()
{
    connect(ui.tableWidget_all, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(allCellDoubleClicked(int,int)));
    connect(ui.tableWidget_teen, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(teenCellDoubleClicked(int,int)));
    connect(ui.tableWidget_common, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(commonCellDoubleClicked(int,int)));
    connect(ui.tableWidget_erotic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(eroticCellDoubleClicked(int,int)));
    connect(ui.tableWidget_thematic, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(thematicCellDoubleClicked(int,int)));
    connect(ui.tableWidget_regional, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(regionalCellDoubleClicked(int,int)));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    connect(ui.lineEdit_search, SIGNAL(returnPressed()), this, SLOT(createList()));
    connect(ui.lineEdit_search, SIGNAL(textChanged(QString)), this, SLOT(createList()));
    connect(ui.pushButton_search, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.pushButton_clear, SIGNAL(clicked()), this, SLOT(buttonClear()));
    connect(ui.checkBox_hide_empty_channels, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_show_adv_options, SIGNAL(clicked()), this, SLOT(showAdvOptions()));

    connect(ui.checkBox_teen, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_common, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_erotic, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_thematic, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_regional, SIGNAL(clicked()), this, SLOT(createList()));

    connect(ui.checkBox_wild, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_tame, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_with_class, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_cult, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_moderated, SIGNAL(clicked()), this, SLOT(createList()));
    connect(ui.checkBox_recommended, SIGNAL(clicked()), this, SLOT(createList()));
}

bool DlgChannelList::isErotic(const QString &strChannel)
{
    QList<OnetChannelList> list = ChannelList::instance()->get();
    foreach (OnetChannelList channel, list)
    {
        QString strName = channel.name;
        int iType = channel.type;

        if (strName == strChannel)
        {
            if (iType == 3)
                return true;
            else
                return false;
        }
    }
    return false;
}

QString DlgChannelList::channelTypeToString(int type)
{
    if (type == 1)
        return tr("Teen");
    else if (type == 2)
        return tr("Common");
    else if (type == 3)
        return tr("Erotic");
    else if (type == 4)
        return tr("Thematic");
    else if (type == 5)
        return tr("Regional");
    else
        return QString::null;
}

QString DlgChannelList::channelCatToString(int cat, bool moderated, bool recommended)
{
    QString strResult;

    if (cat == 0)
        strResult = tr("Wild");
    else if (cat == 1)
        strResult = tr("Tame");
    else if (cat == 2)
        strResult = tr("With class");
    else if (cat == 3)
        strResult = tr("Cult");

    if (moderated)
        strResult += " "+tr("Moderated");
    if (recommended)
        strResult += " "+tr("Recommended");

    return strResult;
}

bool DlgChannelList::showChannel(const QString &name, int people, int cat, int type, bool moderated, bool recommended)
{
    if ((people == 0) && (bHideEmpty))
        return false;

    if ((type == 1) && (!bShowTeen))
        return false;
    if ((type == 2) && (!bShowCommon))
        return false;
    if ((type == 3) && (!bShowErotic))
        return false;
    if ((type == 4) && (!bShowThematic))
        return false;
    if ((type == 5) && (!bShowRegional))
        return false;

    if ((cat == 0) && (!bShowWild))
    {
        if (!(((bShowModerated) && (moderated)) || ((bShowRecommended) && (recommended))))
            return false;
    }
    if ((cat == 1) && (!bShowTame))
    {
        if (!(((bShowModerated) && (moderated)) || ((bShowRecommended) && (recommended))))
            return false;
    }
    if ((cat == 2) && (!bShowWithClass))
    {
        if (!(((bShowModerated) && (moderated)) || ((bShowRecommended) && (recommended))))
            return false;
    }
    if ((cat == 3) && (!bShowCult))
    {
        if (!(((bShowModerated) && (moderated)) || ((bShowRecommended) && (recommended))))
            return false;
    }

    if (!strSearch.isEmpty())
    {
        if (name.toLower().contains(strSearch.toLower()))
            return true;
        else
            return false;
    }
    else
        return true;
}

void DlgChannelList::createList()
{
    // ready
    if (!ChannelList::instance()->getReady())
    {
        QTimer::singleShot(1000, this, SLOT(createList())); // 1 sec
        return;
    }

    // focus
    ui.lineEdit_search->setFocus();

    // get options
    getOptions();

    // clear
    ui.tableWidget_all->clear();
    ui.tableWidget_all->setRowCount(0);
    ui.tableWidget_teen->clear();
    ui.tableWidget_teen->setRowCount(0);
    ui.tableWidget_common->clear();
    ui.tableWidget_common->setRowCount(0);
    ui.tableWidget_erotic->clear();
    ui.tableWidget_erotic->setRowCount(0);
    ui.tableWidget_thematic->clear();
    ui.tableWidget_thematic->setRowCount(0);
    ui.tableWidget_regional->clear();
    ui.tableWidget_regional->setRowCount(0);

    // set labels
    QStringList lLabels;
    lLabels << tr("Channel name") << tr("Number of persons") << tr("Category") << tr("Type");

    ui.tableWidget_all->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_teen->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_common->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_erotic->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_thematic->setHorizontalHeaderLabels(lLabels);
    ui.tableWidget_regional->setHorizontalHeaderLabels(lLabels);

    // count rows
    int iAllCount = 0;
    int iTeenCount = 0;
    int iCommonCount = 0;
    int iEroticCount = 0;
    int iThematicCount = 0;
    int iRegionalCount = 0;

    QList<OnetChannelList> list1 = ChannelList::instance()->get();
    foreach (OnetChannelList channel, list1)
    {
        QString strName = channel.name;
        int iPeople = channel.people;
        int iCat = channel.cat;
        int iType = channel.type;
        bool bModerated = channel.moderated;
        bool bRecommended = channel.recommended;

        bool bShow = showChannel(strName, iPeople, iCat, iType, bModerated, bRecommended);

        if (bShow)
        {
            iAllCount++;
            if (iType == 1)
                iTeenCount++;
            else if (iType == 2)
                iCommonCount++;
            else if (iType == 3)
                iEroticCount++;
            else if (iType == 4)
                iThematicCount++;
            else if (iType == 5)
                iRegionalCount++;
        }
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

    QList<OnetChannelList> list2 = ChannelList::instance()->get();
    foreach (OnetChannelList channel, list2)
    {
        QString strName = channel.name;
        int iPeople = channel.people;
        int iCat = channel.cat;
        int iType = channel.type;
        bool bModerated = channel.moderated;
        bool bRecommended = channel.recommended;

        bool bShow = showChannel(strName, iPeople, iCat, iType, bModerated, bRecommended);

        if (bShow)
        {
            ui.tableWidget_all->setItem(iAllRow, 0, new SortedChannelListTableWidgetItem(strName));
            ui.tableWidget_all->setItem(iAllRow, 1, new SortedChannelListTableWidgetItem(QString::number(iPeople)));
            ui.tableWidget_all->setItem(iAllRow, 2, new SortedChannelListTableWidgetItem(channelCatToString(iCat, bModerated, bRecommended)));
            ui.tableWidget_all->setItem(iAllRow, 3, new SortedChannelListTableWidgetItem(channelTypeToString(iType)));
            iAllRow++;

            if (iType == 1)
            {
                ui.tableWidget_teen->setItem(iTeenRow, 0, new SortedChannelListTableWidgetItem(strName));
                ui.tableWidget_teen->setItem(iTeenRow, 1, new SortedChannelListTableWidgetItem(QString::number(iPeople)));
                ui.tableWidget_teen->setItem(iTeenRow, 2, new SortedChannelListTableWidgetItem(channelCatToString(iCat, bModerated, bRecommended)));
                ui.tableWidget_teen->setItem(iTeenRow, 3, new SortedChannelListTableWidgetItem(channelTypeToString(iType)));
                iTeenRow++;
            }
            else if (iType == 2)
            {
                ui.tableWidget_common->setItem(iCommonRow, 0, new SortedChannelListTableWidgetItem(strName));
                ui.tableWidget_common->setItem(iCommonRow, 1, new SortedChannelListTableWidgetItem(QString::number(iPeople)));
                ui.tableWidget_common->setItem(iCommonRow, 2, new SortedChannelListTableWidgetItem(channelCatToString(iCat, bModerated, bRecommended)));
                ui.tableWidget_common->setItem(iCommonRow, 3, new SortedChannelListTableWidgetItem(channelTypeToString(iType)));
                iCommonRow++;
            }
            else if (iType == 3)
            {
                ui.tableWidget_erotic->setItem(iEroticRow, 0, new SortedChannelListTableWidgetItem(strName));
                ui.tableWidget_erotic->setItem(iEroticRow, 1, new SortedChannelListTableWidgetItem(QString::number(iPeople)));
                ui.tableWidget_erotic->setItem(iEroticRow, 2, new SortedChannelListTableWidgetItem(channelCatToString(iCat, bModerated, bRecommended)));
                ui.tableWidget_erotic->setItem(iEroticRow, 3, new SortedChannelListTableWidgetItem(channelTypeToString(iType)));
                iEroticRow++;
            }
            else if (iType == 4)
            {
                ui.tableWidget_thematic->setItem(iThematicRow, 0, new SortedChannelListTableWidgetItem(strName));
                ui.tableWidget_thematic->setItem(iThematicRow, 1, new SortedChannelListTableWidgetItem(QString::number(iPeople)));
                ui.tableWidget_thematic->setItem(iThematicRow, 2, new SortedChannelListTableWidgetItem(channelCatToString(iCat, bModerated, bRecommended)));
                ui.tableWidget_thematic->setItem(iThematicRow, 3, new SortedChannelListTableWidgetItem(channelTypeToString(iType)));
                iThematicRow++;
            }
            else if (iType == 5)
            {
                ui.tableWidget_regional->setItem(iRegionalRow, 0, new SortedChannelListTableWidgetItem(strName));
                ui.tableWidget_regional->setItem(iRegionalRow, 1, new SortedChannelListTableWidgetItem(QString::number(iPeople)));
                ui.tableWidget_regional->setItem(iRegionalRow, 2, new SortedChannelListTableWidgetItem(channelCatToString(iCat, bModerated, bRecommended)));
                ui.tableWidget_regional->setItem(iRegionalRow, 3, new SortedChannelListTableWidgetItem(channelTypeToString(iType)));
                iRegionalRow++;
            }
        }
    }

    ui.tableWidget_all->sortByColumn(1);
    ui.tableWidget_teen->sortByColumn(1);
    ui.tableWidget_common->sortByColumn(1);
    ui.tableWidget_erotic->sortByColumn(1);
    ui.tableWidget_thematic->sortByColumn(1);
    ui.tableWidget_regional->sortByColumn(1);
}

void DlgChannelList::getOptions()
{
    // search
    strSearch = ui.lineEdit_search->text();

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

    // hide empty
    if (ui.checkBox_hide_empty_channels->isChecked())
        bHideEmpty = true;
    else
        bHideEmpty = false;
}

void DlgChannelList::allCellDoubleClicked(int row, int column)
{
    Q_UNUSED (column);

    QString strChannel = ui.tableWidget_all->item(row, 0)->text();

    if (isErotic(strChannel))
    {
        if (Core::instance()->settings.value("age_check") == "true")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo16x16.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults."), tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
                Core::instance()->settings["age_check"] = "false";
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
        if (Core::instance()->settings.value("age_check") == "true")
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0.557, stop:0 rgba(198, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo16x16.png"));
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(QString("%1\n%2").arg(tr("Erotic category may contain content intended only for adults."), tr("To enter you must be over 18 years.")));
            QPushButton *exitButton = msgBox.addButton(tr("Exit"), QMessageBox::AcceptRole);
            exitButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
            QPushButton *enterButton = msgBox.addButton(tr("Enter"), QMessageBox::RejectRole);
            enterButton->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
            msgBox.exec();

            if (msgBox.clickedButton() == enterButton)
            {
                Core::instance()->settings["age_check"] = "false";
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

void DlgChannelList::buttonClear()
{
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

    ui.lineEdit_search->clear();

    createList();
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
            createList();
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}
