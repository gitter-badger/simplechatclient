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
    QString strPath;
#ifdef Q_WS_WIN
    strPath = QCoreApplication::applicationDirPath();
#else
    strPath = "/usr/share/scc";
#endif

    // standard
    QDir dStandardEmoticons = strPath+"/3rdparty/emoticons";
    QStringList slFiles = dStandardEmoticons.entryList(QStringList("*.gif"), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < slFiles.size(); i++)
    {
        QString strFileName = slFiles.at(i);
        QString strEmoticon = strFileName;
        strEmoticon.remove(".gif");

        QFile f(strPath+"/3rdparty/emoticons/"+strFileName);
        if (!f.open(QIODevice::ReadOnly))
            break;
        QByteArray bData = f.readAll();
        f.close();

        emit insertEmoticonsStandard(strEmoticon, bData);
    }
    exec();
}

ThreadEmoticonsExtended::ThreadEmoticonsExtended() {}

void ThreadEmoticonsExtended::run()
{
    QString strPath;
#ifdef Q_WS_WIN
    strPath = QCoreApplication::applicationDirPath();
#else
    strPath = "/usr/share/scc";
#endif

    // extended
    QDir dExtendedEmoticons = strPath+"/3rdparty/emoticons_other";
    QStringList slFiles = dExtendedEmoticons.entryList(QStringList("*.gif"), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < slFiles.size(); i++)
    {
        QString strFileName = slFiles.at(i);
        QString strEmoticon = strFileName;
        strEmoticon.remove(".gif");

        QFile f(strPath+"/3rdparty/emoticons_other/"+strFileName);
        if (!f.open(QIODevice::ReadOnly))
            break;
        QByteArray bData = f.readAll();
        f.close();

        emit insertEmoticonsExtended(strEmoticon, bData);
    }
    exec();
}

DlgEmoticons::DlgEmoticons(MainWindow *parent, InputWidget *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Emoticons"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    pInputWidget = param1;

    createGui();
    setDefaultValues();
    createSignals();

    getEmoticonsStandard();
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

void DlgEmoticons::createGui()
{
    ui.pushButton_insert->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.tabWidget->setTabText(0, tr("Standard"));
    ui.tabWidget->setTabText(1, tr("Extended"));
    ui.pushButton_insert->setText(tr("Insert"));
}

void DlgEmoticons::setDefaultValues()
{
    ui.listWidget_standard->clear();
    ui.listWidget_extended->clear();
    bDoneStandard = false;
    bDoneExtended = false;
}

void DlgEmoticons::createSignals()
{
    QObject::connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    QObject::connect(ui.listWidget_standard, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clickedStandard(QModelIndex)));
    QObject::connect(ui.listWidget_extended, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(clickedExtended(QModelIndex)));
    QObject::connect(ui.pushButton_insert, SIGNAL(clicked()), this, SLOT(buttonInsert()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    QObject::connect(&pThreadEmoticonsStandard, SIGNAL(insertEmoticonsStandard(QString, QByteArray)), this, SLOT(insertEmoticonsStandard(QString, QByteArray)));
    QObject::connect(&pThreadEmoticonsExtended, SIGNAL(insertEmoticonsExtended(QString, QByteArray)), this, SLOT(insertEmoticonsExtended(QString, QByteArray)));
}

void DlgEmoticons::getEmoticonsStandard()
{
    pThreadEmoticonsStandard.start();

    bDoneStandard = true;
}

void DlgEmoticons::insertEmoticonsStandard(QString strEmoticon, QByteArray bData)
{
    QPixmap pix;
    pix.loadFromData(bData);

    QListWidgetItem *item = new QListWidgetItem();
    item->setIcon(QIcon(pix));
    item->setData(Qt::UserRole, strEmoticon);
    item->setToolTip(strEmoticon);

    ui.listWidget_standard->addItem(item);
}

void DlgEmoticons::getEmoticonsExtended()
{
    pThreadEmoticonsExtended.start();

    bDoneExtended = true;
}

void DlgEmoticons::insertEmoticonsExtended(QString strEmoticon, QByteArray bData)
{
    QPixmap pix;
    pix.loadFromData(bData);

    QListWidgetItem *item = new QListWidgetItem();
    item->setIcon(QIcon(pix));
    item->setData(Qt::UserRole, strEmoticon);
    item->setToolTip(strEmoticon);

    ui.listWidget_extended->addItem(item);
}

void DlgEmoticons::currentTabChanged(int index)
{
    if ((index == 0) && (!bDoneStandard))
        getEmoticonsStandard();
    else if ((index == 1) && (!bDoneExtended))
        getEmoticonsExtended();
}

void DlgEmoticons::clickedStandard(QModelIndex index)
{
    // get emoticon
    QString strEmoticon = ui.listWidget_standard->item(index.row())->data(Qt::UserRole).toString();

    if (!strEmoticon.isEmpty())
    {
        // insert emots
        pInputWidget->insertText("//"+strEmoticon);

        // close
        this->close();
    }
}

void DlgEmoticons::clickedExtended(QModelIndex index)
{
    QString strEmoticon;

    // get emoticon
    strEmoticon = ui.listWidget_extended->item(index.row())->data(Qt::UserRole).toString();

    if (!strEmoticon.isEmpty())
    {
        // insert emots
        pInputWidget->insertText("//"+strEmoticon);

        // close
        this->close();
    }
}

void DlgEmoticons::buttonInsert()
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

    if (!strSelected.isEmpty())
    {
        // insert emots
        pInputWidget->insertText("//"+strSelected);

        // close
        this->close();
    }
}
