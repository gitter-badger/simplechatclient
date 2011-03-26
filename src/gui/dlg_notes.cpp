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
#include <QFile>
#include <QPushButton>
#include <QSettings>
#include "dlg_notes.h"

#ifdef Q_WS_X11
#include <QDebug>
#endif

DlgNotes::DlgNotes(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Notes"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    // read path
    read_path();
    // read notes
    read();

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgNotes::read_path()
{
#ifdef Q_WS_X11
    path = QDir::homePath()+"/.scc";
#else
    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
    settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    path = settings.value("Personal").toString();
    path += "/scc";
#endif

    // create dir if not exist
    QDir d(path);
    if (d.exists(path) == false)
        d.mkdir(path);

    strNotesFile = path+"/notes.txt";
}

void DlgNotes::read()
{
    QFile *file = new QFile(strNotesFile);
    if (file->exists() == true)
    {
        if (file->open(QIODevice::ReadWrite) == false)
        {
#ifdef Q_WS_X11
            qDebug() << tr("Error: notes: Cannot read notes file!");
#endif
            return;
        }

        // set content
        QString strContent = file->readAll();
        ui.plainTextEdit->setPlainText(strContent);
    }

    file->close();
    delete file;
}

void DlgNotes::save()
{
    // get content
    QString strContent = ui.plainTextEdit->toPlainText();
    QByteArray baContent = strContent.toAscii();

    // save
    QFile *fs = new QFile(strNotesFile);
    fs->open(QIODevice::WriteOnly | QIODevice::Truncate);
    fs->write(baContent);
    fs->close();
    delete fs;
}

void DlgNotes::button_ok()
{
    // save notes
    save();

    // close
    this->close();
}

void DlgNotes::button_cancel()
{
    this->close();
}
