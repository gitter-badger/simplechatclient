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
#include <QListWidget>
#include <QPixmap>
#include "mainwindow.h"
#include "inputline_widget.h"
#include "emoticons.h"

DlgEmoticonsTab::DlgEmoticonsTab(QString _strDir, QWidget *parent) : QWidget(parent), strDir(_strDir)
{
    listWidget = new QListWidget(this);
    listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setMovement(QListView::Static);
    listWidget->setResizeMode(QListView::Adjust);
    listWidget->show();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(listWidget);
    setLayout(mainLayout);

    showEmoticons();
    listWidget->setSortingEnabled(true);
}

void DlgEmoticonsTab::showEmoticons()
{
    QDir dEmoticonsDir = strDir;
    QStringList lFiles = dEmoticonsDir.entryList(QStringList("*.gif"), QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < lFiles.size(); i++)
    {
        QString strFileName = lFiles.at(i);
        QString strEmoticon = strFileName;
        strEmoticon.remove(".gif");

        QByteArray bData;
        QFile f(strDir+"/"+strFileName);
        if (f.open(QIODevice::ReadOnly))
        {
            bData = f.readAll();
            f.close();
        }

        QPixmap pix;
        if (!bData.isEmpty())
            pix.loadFromData(bData);

        if (!pix.isNull())
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(pix));
            item->setData(Qt::UserRole, strEmoticon);
            item->setToolTip(strEmoticon);

            listWidget->addItem(item);
        }
    }
}

DlgEmoticons::DlgEmoticons(MainWindow *parent, InputLineWidget *_pInputLineWidget) : QDialog(parent), pInputLineWidget(_pInputLineWidget)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Emoticons"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    setDefaultValues();
    createSignals();
}

void DlgEmoticons::createGui()
{
    ui.pushButton_insert->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_insert->setText(tr("Insert"));
}

void DlgEmoticons::setDefaultValues()
{
    QString path;
#ifdef Q_WS_WIN
    path = QCoreApplication::applicationDirPath();
#else
    path = "/usr/share/scc";
#endif

    QDir dAllEmoticonsDirs = path+"/3rdparty/emoticons";
    QStringList lDirs = dAllEmoticonsDirs.entryList(QStringList("*"), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    for (int i = 0; i < lDirs.size(); i++)
    {
        QString strDir = lDirs[i];
        QString strFullPath = QString("%1/%2").arg(dAllEmoticonsDirs.path()).arg(strDir);
        ui.tabWidget->addTab(new DlgEmoticonsTab(strFullPath), strDir);
    }
}

void DlgEmoticons::createSignals()
{
    QObject::connect(ui.pushButton_insert, SIGNAL(clicked()), this, SLOT(buttonInsert()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    for (int i = 0; i < ui.tabWidget->count(); i++)
    {
        DlgEmoticonsTab *tab = (DlgEmoticonsTab *)ui.tabWidget->widget(i);
        QObject::connect(tab->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(buttonInsert()));
    }
}

void DlgEmoticons::buttonInsert()
{
    DlgEmoticonsTab *tab = (DlgEmoticonsTab *)ui.tabWidget->currentWidget();

    if (!tab->listWidget->selectedItems().isEmpty())
    {
        QString strEmoticon = tab->listWidget->selectedItems().at(0)->data(Qt::UserRole).toString();

        // insert
        pInputLineWidget->insertText("//"+strEmoticon);

        // close
        this->close();
    }
}
