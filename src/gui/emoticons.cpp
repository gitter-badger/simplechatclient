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
#include <QDir>
#include <QListWidget>
#include <QPixmap>
#include "core.h" // require by SCC_DATA_DIR
#include "config.h"
#include "mainwindow.h"
#include "inputline_widget.h"
#include "emoticons.h"

DlgEmoticonsThread::DlgEmoticonsThread() {}

void DlgEmoticonsThread::setDir(const QString &_strDir)
{
    strDir = _strDir;
}

void DlgEmoticonsThread::run()
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

        if (!bData.isEmpty())
            emit addEmoticon(strEmoticon, bData);
    }
    emit sortEmoticons();

    exec();
}

DlgEmoticonsTab::DlgEmoticonsTab(const QString &_strDir, QWidget *parent) : QWidget(parent)
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

    connect(&thread, SIGNAL(addEmoticon(const QString&, const QByteArray&)), this, SLOT(addEmoticon(const QString&, const QByteArray&)));
    connect(&thread, SIGNAL(sortEmoticons()), this, SLOT(sortEmoticons()));

    thread.setDir(_strDir);
    thread.start();
}

void DlgEmoticonsTab::addEmoticon(const QString &strEmoticon, const QByteArray &bData)
{
    QPixmap pix;
    if (!bData.isEmpty())
        pix.loadFromData(bData);

    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QIcon(pix));
    item->setData(Qt::UserRole, strEmoticon);
    item->setToolTip(strEmoticon);

    listWidget->addItem(item);
}

void DlgEmoticonsTab::sortEmoticons()
{
    listWidget->setSortingEnabled(true);
}

DlgEmoticons::DlgEmoticons(MainWindow *parent, InputLineWidget *_pInputLineWidget) : QDialog(parent), pInputLineWidget(_pInputLineWidget)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Emoticons"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

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
    path = SCC_DATA_DIR;
#endif

    QDir dAllEmoticonsDirs = path+"/3rdparty/emoticons";
    QStringList lDirs = dAllEmoticonsDirs.entryList(QStringList("*"), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    for (int i = 0; i < lDirs.size(); i++)
    {
        QString strDir = lDirs[i];
        QString strFullPath = QString("%1/%2").arg(dAllEmoticonsDirs.path(), strDir);
        ui.tabWidget->addTab(new DlgEmoticonsTab(strFullPath), strDir);
    }
}

void DlgEmoticons::createSignals()
{
    connect(ui.pushButton_insert, SIGNAL(clicked()), this, SLOT(buttonInsert()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    for (int i = 0; i < ui.tabWidget->count(); i++)
    {
        DlgEmoticonsTab *tab = (DlgEmoticonsTab *)ui.tabWidget->widget(i);
        connect(tab->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(buttonInsert()));
    }
}

DlgEmoticons::~DlgEmoticons()
{
    for (int i = 0; i < ui.tabWidget->count(); i++)
    {
        DlgEmoticonsTab *tab = (DlgEmoticonsTab *)ui.tabWidget->widget(i);
        tab->thread.quit();
        tab->thread.wait();
        tab->thread.deleteLater();
    }
}

void DlgEmoticons::buttonInsert()
{
    if (ui.tabWidget->count() == 0) // not found tabs
        return;

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
