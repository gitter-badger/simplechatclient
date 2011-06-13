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
#include <QSettings>
#include "dlg_about.h"

DlgAbout::DlgAbout(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("About..."));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    create_gui();
    set_default_values();
    create_signals();
}

void DlgAbout::create_gui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.tabWidget->setTabText(0, tr("&About"));
    ui.tabWidget->setTabText(1, tr("A&uthors"));
    ui.tabWidget->setTabText(2, tr("T&hanks to"));
}

void DlgAbout::set_default_values()
{
    QSettings settings;
    QString version = settings.value("version").toString();

    QString strVersion = "<p style=\"font-size:16px;\"><b>Simple Chat Client</b></p>";
    strVersion += "<p><b>"+tr("Version")+" "+version+"</b></p>";

    QString strAbout = "<p>"+tr("Simple chat client")+"</p>";
    strAbout += "<p>&copy; 2011 by the Simple Chat Client Project</p>";
    strAbout += "<p><a style=\"text-decoration:none;\" href=\"http://simplechatclien.sf.net/\">"+tr("Home page")+"</a></p>";
    strAbout += "<p>"+tr("License")+" <a style=\"text-decoration:none;\" href=\"http://www.fsf.org/licensing/licenses/gpl.html\">GPL v3</a></p>";

    QString strAuthors = "<dl>";
    strAuthors += "<dt><b>"+tr("Piotr Luczko")+"</b></dt>";
    strAuthors += "<dd><a href=\"mailto:piotr.luczko@gmail.com\">piotr.luczko@gmail.com</a></dd>";
    strAuthors += "<dd>"+tr("Project Founder")+", "+tr("Lead Developer")+"</dd>";
    strAuthors += "</dl>";

    QString strThanksTo = "<p>"+tr("Thanks to <b>ovo_</b> for creating graphics, to all persons who actively support the development of the project and special thanks to <b>Darom</b>.")+"</p>";

    ui.label_logo->setPixmap(QPixmap(":/images/logo.png").scaled(QSize(64,64)));
    ui.label_version->setText(strVersion);
    ui.label_about->setText(strAbout);
    ui.label_authors->setText(strAuthors);
    ui.label_thanks_to->setText(strThanksTo);
}

void DlgAbout::create_signals()
{
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(close()));
}
