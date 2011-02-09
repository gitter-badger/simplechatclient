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
#include <QPushButton>
#include "dlg_awaylog.h"

DlgAwaylog::DlgAwaylog(QWidget *parent, QList<QString> *param1) : QDialog(parent)
{
    ui.setupUi(this);
    //setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Awaylog"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    lAwaylog = param1;

    ui.buttonBox->button(QDialogButtonBox::Reset)->setIcon(QIcon(":/images/oxygen/16x16/document-revert.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    QObject::connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttons(QAbstractButton*)));

    refresh();
}

void DlgAwaylog::refresh()
{
    for (int i = 0; i < lAwaylog->count(); i++)
        ui.listWidget->insertItem(ui.listWidget->count(),lAwaylog->at(i));
}

void DlgAwaylog::buttons(QAbstractButton *button)
{
    if (ui.buttonBox->button(QDialogButtonBox::Reset) == button)
        button_reset();
    else if (ui.buttonBox->button(QDialogButtonBox::Close) == button)
        button_close();
}

void DlgAwaylog::button_reset()
{
    lAwaylog->clear();
    ui.listWidget->clear();
}

void DlgAwaylog::button_close()
{
    this->close();
}
