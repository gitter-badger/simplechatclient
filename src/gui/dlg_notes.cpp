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
#include <QTextStream>
#include "dlg_notes.h"

#ifdef Q_WS_X11
    #include <QDebug>
#else
    #include <QDesktopServices>
#endif

DlgNotes::DlgNotes(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Notes"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();

    // read path
    readPath();
    // read notes
    read();

    createSignals();
}

void DlgNotes::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
}

void DlgNotes::createSignals()
{
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(buttonOk()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgNotes::readPath()
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

    strNotesFile = path+"/notes.txt";
}

void DlgNotes::read()
{
    QFile f(strNotesFile);
    if (f.exists())
    {
        if (!f.open(QIODevice::ReadOnly))
        {
#ifdef Q_WS_X11
            qDebug() << tr("Error: Cannot read notes file!");
#endif
            return;
        }

        // read content
        QTextStream in(&f);
        QString strContent = in.readAll();

        // set content
        ui.plainTextEdit->setPlainText(strContent);
    }

    f.close();
}

void DlgNotes::save()
{
    // get content
    QString strContent = ui.plainTextEdit->toPlainText();

    // save
    QFile f(strNotesFile);
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream out(&f);
    out << strContent << "\r\n";

    f.close();
}

void DlgNotes::buttonOk()
{
    // save notes
    save();

    // close
    this->close();
}

void DlgNotes::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED (e);
    int iWidth = this->width();
    int iHeight = this->height();
    ui.plainTextEdit->setMinimumSize(QSize(iWidth, iHeight-30));
    ui.plainTextEdit->setMaximumSize(QSize(iWidth, iHeight-30));
    ui.buttonBox->setGeometry(0, iHeight-30, iWidth, 30);
}
