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
#include "awaylog_model.h"
#include "core.h"
#include "awaylog.h"

DlgAwaylog::DlgAwaylog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Awaylog"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgAwaylog::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Reset)->setIcon(QIcon(":/images/oxygen/16x16/document-revert.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));
}

void DlgAwaylog::createSignals()
{
    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttons(QAbstractButton*)));
}

void DlgAwaylog::refresh()
{
    foreach (QString strAwaylog, Awaylog::instance()->get())
        ui.listWidget->addItem(strAwaylog);
}

void DlgAwaylog::buttons(QAbstractButton *button)
{
    if (ui.buttonBox->button(QDialogButtonBox::Reset) == button)
        buttonReset();
    else if (ui.buttonBox->button(QDialogButtonBox::Close) == button)
        buttonClose();
}

void DlgAwaylog::buttonReset()
{
    Awaylog::instance()->clear();
    ui.listWidget->clear();
}

void DlgAwaylog::buttonClose()
{
    this->close();
}
