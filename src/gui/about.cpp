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
#include "core.h"
#include "about.h"

DlgAbout::DlgAbout(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("About..."));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    setDefaultValues();
    createSignals();
}

void DlgAbout::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.tabWidget->setTabText(0, tr("&About"));
    ui.tabWidget->setTabText(1, tr("A&uthors"));
    ui.tabWidget->setTabText(2, tr("T&hanks to"));
}

void DlgAbout::setDefaultValues()
{
    QString strCurrentVersion = Core::instance()->settings.value("version");
    QString strVersionStatus = Core::instance()->settings.value("version_status");

    QString strVersion;
    if (strVersionStatus == "uptodate")
        strVersion = tr("Up-to-date version");
    else if (strVersionStatus == "beta")
        strVersion = tr("Unstable version");
    else if (strVersionStatus == "outofdate")
         strVersion = QString("<span style=\"color:#ff0000\">%1</span>").arg(tr("A new version is available"));
    else
        strVersion = QString("%1 %2").arg(tr("Version"), strCurrentVersion);

    QString strTitle = QString("<p style=\"font-size:16px;\"><b>Simple Chat Client</b></p>");
    strTitle += "<p>"+strVersion+"</p>";

    QString strAbout = "<p>"+tr("Simple chat client")+"</p>";
    strAbout += "<p>&copy; 2012 by the Simple Chat Client Project</p>";
    strAbout += "<p><a style=\"text-decoration:none;\" href=\"http://simplechatclien.sf.net/\">"+tr("Home page")+"</a></p>";
    strAbout += "<p>"+tr("License")+" <a style=\"text-decoration:none;\" href=\"http://www.fsf.org/licensing/licenses/gpl.html\">GPL v3</a></p>";

    QString strAuthors = "<dl>";
    strAuthors += "<dt><b>"+tr("Piotr Luczko")+"</b></dt>";
    strAuthors += "<dd><a href=\"mailto:piotr.luczko@gmail.com\">piotr.luczko@gmail.com</a></dd>";
    strAuthors += "<dd>"+tr("Project Founder")+", "+tr("Lead Developer")+"</dd>";
    strAuthors += "<dt><b>Dariusz Markowicz</b></dt>";
    strAuthors += "<dd><a href=\"mailto:dmarkowicz@alari.pl\">dmarkowicz@alari.pl</a></dd>";
    strAuthors += "<dd>"+tr("Developer")+"</dd>";
    strAuthors += "</dl>";

    QString strThanksTo = "<p>"+tr("Thanks to <b>ovo_</b> for creating graphics, to all persons who actively support the development of the project and special thanks to <b>Darom</b>.")+"</p>";

    ui.label_logo->setPixmap(QPixmap(":/images/logo64x64.png"));
    ui.label_version->setText(strTitle);
    ui.label_about->setText(strAbout);
    ui.label_authors->setText(strAuthors);
    ui.label_thanks_to->setText(strThanksTo);
}

void DlgAbout::createSignals()
{
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(close()));
}
