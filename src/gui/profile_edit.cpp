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
#include <QMessageBox>
#include <QPushButton>
#include "config.h"
#include "simple_crypt.h"
#include "mainwindow.h"
#include "profile_edit.h"

DlgProfileEdit::DlgProfileEdit(MainWindow *parent, const QString &_strNick) : QDialog(parent), strNick(_strNick)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Edit profile"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    setDefaultValues();
    createSignals();
}

void DlgProfileEdit::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.label_nick->setText(tr("Nick:"));
    ui.label_password->setText(tr("Password:"));
}

void DlgProfileEdit::setDefaultValues()
{
    Config *pConfig = new Config(true, strNick);
    QString strPassword = pConfig->get("pass");
    delete pConfig;

    // decrypt pass
    if (!strPassword.isEmpty())
    {
        SimpleCrypt *pSimpleCrypt = new SimpleCrypt();
        strPassword = pSimpleCrypt->decrypt(strNick, strPassword);
        delete pSimpleCrypt;
    }

    ui.lineEdit_nick->setText(strNick);
    ui.lineEdit_password->setText(strPassword);
}

void DlgProfileEdit::createSignals()
{
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(buttonOk()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgProfileEdit::buttonOk()
{
    QString strPassword = ui.lineEdit_password->text();

    // empty
    if (strPassword.isEmpty())
    {
        QMessageBox::critical(0, "", tr("Password is empty!"));
        return;
    }

    // encrypt pass
    SimpleCrypt *pSimpleCrypt = new SimpleCrypt();
    strPassword = pSimpleCrypt->encrypt(strNick, strPassword);
    delete pSimpleCrypt;

    Config *pConfig = new Config(true, strNick);
    pConfig->set("pass", strPassword);
    delete pConfig;

    // close
    this->close();
}
