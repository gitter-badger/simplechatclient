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
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include "core.h"
#include "mainwindow.h"
#include "dlg_options.h"
#include "dlg_profile_add.h"
#include "dlg_profile_edit.h"
#include "dlg_profile_manager.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

DlgProfileManager::DlgProfileManager(MainWindow *parent, DlgOptions *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Profile manager"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    options = param1;

    createGui();
    setDefaultValues();
    createSignals();
}

void DlgProfileManager::createGui()
{
    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/user-group-new.png"));
    ui.pushButton_edit->setIcon(QIcon(":/images/oxygen/16x16/user-group-properties.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/user-group-delete.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_edit->setText(tr("Edit"));
    ui.pushButton_remove->setText(tr("Remove"));

    ui.pushButton_edit->setEnabled(false);
    ui.pushButton_remove->setEnabled(false);
}

void DlgProfileManager::setDefaultValues()
{
    // create path
    createPath();

    // refresh list
    refreshProfilesList();
}

void DlgProfileManager::createSignals()
{
    QObject::connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked()));
    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(addProfile()));
    QObject::connect(ui.pushButton_edit, SIGNAL(clicked()), this, SLOT(editProfile()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(removeProfile()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgProfileManager::refreshAllLists()
{
    refreshProfilesList(); // here
    options->refreshProfilesList(); // in options
}

bool DlgProfileManager::existProfile(QString strExistProfile)
{
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    QStringList nameFilters;
    nameFilters << "*.xml";
    QFileInfoList list = dir.entryInfoList(nameFilters);
    for (int i = 0; i < list.size(); ++i)
    {
        QString profileName = list.at(i).fileName();
        profileName.remove(".xml");

        if (profileName == strExistProfile)
            return true;
    }
    return false;
}

void DlgProfileManager::createPath()
{
#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#endif

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    path += "/profiles";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);
}

void DlgProfileManager::refreshProfilesList()
{
    // clear
    ui.listWidget->clear();

    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    QStringList nameFilters;
    nameFilters << "*.xml";
    QFileInfoList list = dir.entryInfoList(nameFilters);
    for (int i = 0; i < list.size(); ++i)
    {
        QString profileName = list.at(i).fileName();
        profileName.remove(".xml");
        ui.listWidget->addItem(profileName);
    }
}

void DlgProfileManager::itemClicked()
{
    QString strNick = ui.listWidget->selectedItems().at(0)->text();

    if (strNick[0] != '~')
        ui.pushButton_edit->setEnabled(true);
    else
        ui.pushButton_edit->setEnabled(false);

    ui.pushButton_remove->setEnabled(true);
}

void DlgProfileManager::addProfile()
{
    DlgProfileAdd(Core::instance()->sccWindow(), this).exec();
}

void DlgProfileManager::editProfile()
{
    QString strNick = ui.listWidget->selectedItems().at(0)->text();
    DlgProfileEdit(Core::instance()->sccWindow(), strNick).exec();
}

void DlgProfileManager::removeProfile()
{
    if (ui.listWidget->count() <= 1)
    {
        QMessageBox::critical(0, "", tr("Cannot remove last profile"));
        return;
    }

    QString profileName = ui.listWidget->selectedItems().at(0)->text();

    if (QFile::exists(path+"/"+profileName+".xml"))
    {
        QFile::remove(path+"/"+profileName+".xml");

        refreshAllLists();

        QSettings settings;
        if (profileName == settings.value("current_profile").toString())
            options->setCurrentProfile(0);
    }
}
