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
#include "core.h"
#include "mainwindow.h"
#include "channel_key.h"

DlgChannelKey::DlgChannelKey(MainWindow *parent, QString _strChannel) : QDialog(parent), strChannel(_strChannel)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose); // require by show method - prevent hangup!
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Key required"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    createSignals();
}

void DlgChannelKey::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.label_msg->setText(tr("Enter key:"));
}

void DlgChannelKey::createSignals()
{
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(buttonOk()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgChannelKey::buttonOk()
{
    QString strKey = ui.lineEdit_key->text();
    if (!strKey.isEmpty())
    {
        Core::instance()->pNetwork->send(QString("JOIN %1 %2").arg(strChannel).arg(strKey));
        this->close();
    }
}
