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
#include <QPixmap>
#include "mainwindow.h"
#include "inputwidget.h"
#include "dlg_emoticons.h"

ThreadEmoticonsStandard::ThreadEmoticonsStandard() {}

void ThreadEmoticonsStandard::run()
{
    QString path = QCoreApplication::applicationDirPath();

    // standard
    QDir dStandardEmoticons = path+"/3rdparty/emoticons";
    QStringList slFiles = dStandardEmoticons.entryList(QStringList("*.gif"), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < slFiles.size(); i++)
    {
        QString strFileName = slFiles.at(i);
        QString strEmoticon = strFileName;
        strEmoticon.remove(".gif");

        QFile f(path+"/3rdparty/emoticons/"+strFileName);
        if (!f.open(QIODevice::ReadOnly))
            break;
        QByteArray bData = f.readAll();
        f.close();

        emit insert_emoticons_standard(strEmoticon, bData);
    }
}

ThreadEmoticonsExtended::ThreadEmoticonsExtended() {}

void ThreadEmoticonsExtended::run()
{
    QString path = QCoreApplication::applicationDirPath();

    // extended
    QDir dExtendedEmoticons = path+"/3rdparty/emoticons_other";
    QStringList slFiles = dExtendedEmoticons.entryList(QStringList("*.gif"), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < slFiles.size(); i++)
    {
        QString strFileName = slFiles.at(i);
        QString strEmoticon = strFileName;
        strEmoticon.remove(".gif");

        QFile f(path+"/3rdparty/emoticons_other/"+strFileName);
        if (!f.open(QIODevice::ReadOnly))
            break;
        QByteArray bData = f.readAll();
        f.close();

        emit insert_emoticons_extended(strEmoticon, bData);
    }
}

DlgEmoticons::DlgEmoticons(MainWindow *parent, InputWidget *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Emoticons"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    pInputWidget = param1;

    create_gui();
    set_default_values();
    create_signals();

    get_emoticons_standard();
}

DlgEmoticons::~DlgEmoticons()
{
    if (pThreadEmoticonsStandard.isRunning())
    {
        pThreadEmoticonsStandard.quit();
        pThreadEmoticonsStandard.wait();
        pThreadEmoticonsStandard.deleteLater();
    }

    if (pThreadEmoticonsExtended.isRunning())
    {
        pThreadEmoticonsExtended.quit();
        pThreadEmoticonsExtended.wait();
        pThreadEmoticonsExtended.deleteLater();
    }
}

void DlgEmoticons::create_gui()
{
    ui.pushButton_insert->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.tabWidget->setTabText(0, tr("Standard"));
    ui.tabWidget->setTabText(1, tr("Extended"));
    ui.pushButton_insert->setText(tr("Insert"));
}

void DlgEmoticons::set_default_values()
{
    ui.listWidget_standard->clear();
    ui.listWidget_extended->clear();
    bDoneStandard = false;
    bDoneExtended = false;
}

void DlgEmoticons::create_signals()
{
    QObject::connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(ui.listWidget_standard, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clicked_standard(QModelIndex)));
    QObject::connect(ui.listWidget_extended, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clicked_extended(QModelIndex)));
    QObject::connect(ui.pushButton_insert, SIGNAL(clicked()), this, SLOT(button_insert()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    QObject::connect(&pThreadEmoticonsStandard, SIGNAL(insert_emoticons_standard(QString, QByteArray)), this, SLOT(insert_emoticons_standard(QString, QByteArray)));
    QObject::connect(&pThreadEmoticonsExtended, SIGNAL(insert_emoticons_extended(QString, QByteArray)), this, SLOT(insert_emoticons_extended(QString, QByteArray)));
}

void DlgEmoticons::get_emoticons_standard()
{
    pThreadEmoticonsStandard.start();

    bDoneStandard = true;
}

void DlgEmoticons::insert_emoticons_standard(QString strEmoticon, QByteArray bData)
{
    QPixmap pix;
    pix.loadFromData(bData);

    QListWidgetItem *item = new QListWidgetItem();
    item->setIcon(QIcon(pix));
    item->setData(Qt::UserRole, strEmoticon);
    item->setToolTip(strEmoticon);

    ui.listWidget_standard->addItem(item);
}

void DlgEmoticons::get_emoticons_extended()
{
    pThreadEmoticonsExtended.start();

    bDoneExtended = true;
}

void DlgEmoticons::insert_emoticons_extended(QString strEmoticon, QByteArray bData)
{
    QPixmap pix;
    pix.loadFromData(bData);

    QListWidgetItem *item = new QListWidgetItem();
    item->setIcon(QIcon(pix));
    item->setData(Qt::UserRole, strEmoticon);
    item->setToolTip(strEmoticon);

    ui.listWidget_extended->addItem(item);
}

void DlgEmoticons::current_tab_changed(int index)
{
    if ((index == 0) && (bDoneStandard == false))
        get_emoticons_standard();
    else if ((index == 1) && (bDoneExtended == false))
        get_emoticons_extended();
}

void DlgEmoticons::clicked_standard(QModelIndex index)
{
    // get emoticon
    QString strEmoticon = ui.listWidget_standard->item(index.row())->data(Qt::UserRole).toString();

    if (strEmoticon.isEmpty() == false)
    {
        // insert emots
        pInputWidget->insert_text("//"+strEmoticon);

        // close
        this->close();
    }
}

void DlgEmoticons::clicked_extended(QModelIndex index)
{
    QString strEmoticon;

    // get emoticon
    strEmoticon = ui.listWidget_extended->item(index.row())->data(Qt::UserRole).toString();

    if (strEmoticon.isEmpty() == false)
    {
        // insert emots
        pInputWidget->insert_text("//"+strEmoticon);

        // close
        this->close();
    }
}

void DlgEmoticons::button_insert()
{
    QString strSelected;

    // get selected
    if (ui.tabWidget->currentIndex() == 0)
    {
        if (ui.listWidget_standard->selectedItems().size() != 0)
            strSelected = ui.listWidget_standard->selectedItems().at(0)->data(Qt::UserRole).toString();
    }
    else if (ui.tabWidget->currentIndex() == 1)
    {
        if (ui.listWidget_extended->selectedItems().size() != 0)
            strSelected = ui.listWidget_extended->selectedItems().at(0)->data(Qt::UserRole).toString();
    }

    if (strSelected.isEmpty() == false)
    {
        // insert emots
        pInputWidget->insert_text("//"+strSelected);

        // close
        this->close();
    }
}

void DlgEmoticons::button_close()
{
    this->close();
}
