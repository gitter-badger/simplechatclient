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
#include "inputwidget.h"
#include "dlg_emoticons.h"

DlgEmoticons::DlgEmoticons(QWidget *parent, InputWidget *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Emoticons"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    inputwidget = param1;

    ui.pushButton_insert->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.tabWidget->setTabText(0, tr("Standard"));
    ui.tabWidget->setTabText(1, tr("Extended"));
    ui.pushButton_insert->setText(tr("Insert"));

    ui.listWidget_standard->clear();
    ui.listWidget_extended->clear();
    bDoneStandard = false;
    bDoneExtended = false;

    QObject::connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(current_tab_changed(int)));
    QObject::connect(ui.listWidget_standard, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clicked_standard(QModelIndex)));
    QObject::connect(ui.listWidget_extended, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clicked_extended(QModelIndex)));
    QObject::connect(ui.pushButton_insert, SIGNAL(clicked()), this, SLOT(button_insert()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    get_emoticons_standard();
}

void DlgEmoticons::get_emoticons_standard()
{
    QString path = QCoreApplication::applicationDirPath();

    // standard
    QDir dStandardEmoticons = path+"/3rdparty/emoticons";
    QStringList slFiles = dStandardEmoticons.entryList(QStringList("*.gif"), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < slFiles.count(); i++)
    {
        QString strFileName = slFiles.at(i);
        QString strEmoticon = strFileName;
        strEmoticon.remove(".gif");

        QPixmap pix = QPixmap(path+"/3rdparty/emoticons/"+strFileName);
        pix.scaled(30,30);

        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon(pix));
        item->setData(Qt::UserRole, strEmoticon);
        item->setToolTip(strEmoticon);

        ui.listWidget_standard->addItem(item);
    }

    bDoneStandard = true;
}

void DlgEmoticons::get_emoticons_extended()
{
    QString path = QCoreApplication::applicationDirPath();

    // extended
    QDir dExtendedEmoticons = path+"/3rdparty/emoticons_other";
    QStringList slFiles = dExtendedEmoticons.entryList(QStringList("*.gif"), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < slFiles.count(); i++)
    {
        QString strFileName = slFiles.at(i);
        QString strEmoticon = strFileName;
        strEmoticon.remove(".gif");

        QPixmap pix = QPixmap(path+"/3rdparty/emoticons_other/"+strFileName);
        pix.scaled(30,30);

        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon(pix));
        item->setData(Qt::UserRole, strEmoticon);
        item->setToolTip(strEmoticon);

        ui.listWidget_extended->addItem(item);
    }

    bDoneExtended = true;
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
    QString strEmoticon;

    // get emoticon
    strEmoticon = ui.listWidget_standard->item(index.row())->data(Qt::UserRole).toString();

    // insert emots
    if (strEmoticon.isEmpty() == false)
        inputwidget->insert_text("//"+strEmoticon);

    // close
    this->close();
}

void DlgEmoticons::clicked_extended(QModelIndex index)
{
    QString strEmoticon;

    // get emoticon
    strEmoticon = ui.listWidget_extended->item(index.row())->data(Qt::UserRole).toString();

    // insert emots
    if (strEmoticon.isEmpty() == false)
        inputwidget->insert_text("//"+strEmoticon);

    // close
    this->close();
}

void DlgEmoticons::button_insert()
{
    QString strSelected;

    // get selected
    if (ui.tabWidget->currentIndex() == 0)
    {
        if (ui.listWidget_standard->selectedItems().count() != 0)
            strSelected = ui.listWidget_standard->selectedItems().at(0)->data(Qt::UserRole).toString();
    }
    else if (ui.tabWidget->currentIndex() == 1)
    {
        if (ui.listWidget_extended->selectedItems().count() != 0)
            strSelected = ui.listWidget_extended->selectedItems().at(0)->data(Qt::UserRole).toString();
    }

    // insert emots
    if (strSelected.isEmpty() == false)
        inputwidget->insert_text("//"+strSelected);

    // close
    this->close();
}

void DlgEmoticons::button_close()
{
    this->close();
}
