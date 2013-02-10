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

#include <QDesktopWidget>
#include <QPushButton>
#include "config.h"
#include "settings.h"
#include "themes_model.h"
#include "themes.h"

DlgThemes::DlgThemes(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Themes"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    setDefaultValues();
    createSignals();
}

void DlgThemes::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    // themes
    QStringList lThemes = ThemesModel::instance()->getAll();
    ui.listWidget->addItems(lThemes);
}

void DlgThemes::setDefaultValues()
{
    QString strCurrentTheme = Settings::instance()->get("themes");

    // themes
    if (strCurrentTheme == "Standard")
        ui.listWidget->setCurrentRow(0);
    else if (strCurrentTheme == "Alhena")
        ui.listWidget->setCurrentRow(1);
    else if (strCurrentTheme == "Origin")
        ui.listWidget->setCurrentRow(2);
    else if (strCurrentTheme == "Adara")
        ui.listWidget->setCurrentRow(3);
    else
        ui.listWidget->setCurrentRow(1);
}

void DlgThemes::createSignals()
{
    connect(ui.listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(changeTheme(QModelIndex)));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgThemes::changeTheme(QModelIndex index)
{
    QString strTheme = "Standard";

    switch (index.row())
    {
        case 0: strTheme = "Standard"; break;
        case 1: strTheme = "Alhena"; break;
        case 2: strTheme = "Origin"; break;
        case 3: strTheme = "Adara"; break;
    }

    // change image
    // TODO

    // save
    Settings::instance()->set("themes", strTheme);
    Config *pConfig = new Config();
    pConfig->set("themes", strTheme);
    delete pConfig;

    // refresh
    ThemesModel::instance()->refreshCurrent();
}
