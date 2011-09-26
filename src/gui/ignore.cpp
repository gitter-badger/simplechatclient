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
#include <QInputDialog>
#include <QTimer>
#include "core.h"
#include "ignore.h"

DlgIgnore::DlgIgnore(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Ignore list"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    createSignals();

    refresh();
}

void DlgIgnore::createGui()
{
    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/list-add-user.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove-user.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));
}

void DlgIgnore::createSignals()
{
    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(buttonAdd()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(buttonRemove()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgIgnore::refresh()
{
    ui.listWidget_nicks->clear();

    foreach (QString strNick, Core::instance()->lIgnore)
    {
        if (Core::instance()->mNickAvatar.contains(strNick))
        {
            QPixmap pixmap;
            pixmap.loadFromData(Core::instance()->mNickAvatar.value(strNick));

            SortedListWidgetItem *item = new SortedListWidgetItem();
            item->setData(Qt::UserRole+11, false); // is nicklist
            item->setText(strNick);
            item->setIcon(QIcon(pixmap));

            ui.listWidget_nicks->addItem(item);
        }
        else
        {
            SortedListWidgetItem *item = new SortedListWidgetItem();
            item->setData(Qt::UserRole+11, false); // is nicklist
            item->setText(strNick);
            item->setIcon(QIcon(":/images/oxygen/16x16/meeting-attending-tentative.png"));

            ui.listWidget_nicks->addItem(item);

            if (Core::instance()->settings.value("show_avatars") == "on") // with avatars
                Core::instance()->pNetwork->send(QString("NS INFO %1 s").arg(strNick));
        }
    }
}

void DlgIgnore::buttonAdd()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your ignore list"), tr("Enter a nickname to be added:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("NS IGNORE ADD %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgIgnore::buttonRemove()
{
    QString strSelected;
    if (ui.listWidget_nicks->selectedItems().size() != 0)
        strSelected = ui.listWidget_nicks->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your ignore list"), tr("Enter a nickname for removal:"), QLineEdit::Normal, strSelected, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        Core::instance()->pNetwork->send(QString("NS IGNORE DEL %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}
