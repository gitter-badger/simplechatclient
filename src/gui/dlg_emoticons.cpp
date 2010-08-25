/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "dlg_emoticons.h"

DlgEmoticons::DlgEmoticons(QWidget *parent, Inputline *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Emoticons"));

    inputline = param1;

    ui.tabWidget->setTabText(0, tr("Standard"));
    ui.tabWidget->setTabText(1, tr("Extended"));
    ui.pushButton_insert->setText(tr("Insert"));
}

void DlgEmoticons::get_emoticons()
{
    QString path = QCoreApplication::applicationDirPath();

    // standard
    QDir dStandardEmoticons = path+"/3rdparty/emoticons";
    QFileInfoList filStandardEmoticons = dStandardEmoticons.entryInfoList();
    for (int i = 0; i < filStandardEmoticons.size(); ++i)
    {
        QString strFileName = filStandardEmoticons.at(i).fileName();
        if (strFileName.indexOf("gif") != -1)
        {
            QString strEmoticon = strFileName.remove(".gif");

            ui.listWidget_standard->addItem(new QListWidgetItem(QIcon(path+"/3rdparty/emoticons/"+strFileName), strEmoticon));
        }
    }

    // extended
    QDir dExtendedEmoticons = path+"/3rdparty/emoticons_other";
    QFileInfoList filExtendedEmoticons = dExtendedEmoticons.entryInfoList();
    for (int i = 0; i < filExtendedEmoticons.size(); ++i)
    {
        QString strFileName = filExtendedEmoticons.at(i).fileName();
        if (strFileName.indexOf("gif") != -1)
        {
            QString strEmoticon = strFileName.remove(".gif");

            ui.listWidget_extended->addItem(new QListWidgetItem(QIcon(path+"/3rdparty/emoticons_other/"+strFileName), strEmoticon));
        }
    }
}

void DlgEmoticons::clicked_standard(QModelIndex index)
{
    QString strEmoticon;

    // get emoticon
    strEmoticon = ui.listWidget_standard->item(index.row())->text();

    // insert emots
    if (strEmoticon.isEmpty() == false)
        inputline->insert_text("//"+strEmoticon);

    // close
    this->close();
}

void DlgEmoticons::clicked_extended(QModelIndex index)
{
    QString strEmoticon;

    // get emoticon
    strEmoticon = ui.listWidget_extended->item(index.row())->text();

    // insert emots
    if (strEmoticon.isEmpty() == false)
        inputline->insert_text("//"+strEmoticon);

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
            strSelected = ui.listWidget_standard->selectedItems().at(0)->text();
    }
    else if (ui.tabWidget->currentIndex() == 1)
    {
        if (ui.listWidget_extended->selectedItems().count() != 0)
            strSelected = ui.listWidget_extended->selectedItems().at(0)->text();
    }

    // insert emots
    if (strSelected.isEmpty() == false)
        inputline->insert_text("//"+strSelected);

    // close
    this->close();
}

void DlgEmoticons::button_close()
{
    this->close();
}

void DlgEmoticons::showEvent(QShowEvent *event)
{
    event->accept();

    ui.listWidget_standard->clear();
    ui.listWidget_extended->clear();
    get_emoticons();

    QObject::connect(ui.listWidget_standard, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clicked_standard(QModelIndex)));
    QObject::connect(ui.listWidget_extended, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clicked_extended(QModelIndex)));
    QObject::connect(ui.pushButton_insert, SIGNAL(clicked()), this, SLOT(button_insert()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
}
