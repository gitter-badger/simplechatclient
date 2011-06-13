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
#include <QSettings>
#include <QTimer>
#include "core.h"
#include "network.h"
#include "dlg_ignore.h"

DlgIgnore::DlgIgnore(QWidget *parent, Network *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("Ignore list"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    pNetwork = param1;

    create_gui();
    create_signals();

    refresh();
}

void DlgIgnore::create_gui()
{
    ui.pushButton_add->setIcon(QIcon(":/images/oxygen/16x16/list-add-user.png"));
    ui.pushButton_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove-user.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.pushButton_add->setText(tr("Add"));
    ui.pushButton_remove->setText(tr("Remove"));
}

void DlgIgnore::create_signals()
{
    QObject::connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(button_add()));
    QObject::connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(button_remove()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgIgnore::refresh()
{
    ui.listWidget_nicks->clear();

    for (int i = 0; i < Core::instance()->lIgnore.size(); ++i)
    {
        QString strNick = Core::instance()->lIgnore.at(i);

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

            QSettings settings;
            if (settings.value("disable_avatars").toString() == "off") // with avatars
                pNetwork->send(QString("NS INFO %1 s").arg(strNick));
        }
    }
}

void DlgIgnore::button_add()
{
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your ignore list"), tr("Enter a nickname to be added:"), QLineEdit::Normal, QString::null, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        pNetwork->send(QString("NS IGNORE ADD %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}

void DlgIgnore::button_remove()
{
    QString strSelected;
    if (ui.listWidget_nicks->selectedItems().size() != 0)
        strSelected = ui.listWidget_nicks->selectedItems().at(0)->text();

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Change your ignore list"), tr("Enter a nickname for removal:"), QLineEdit::Normal, strSelected, &ok);

    if ((ok) && (!strText.isEmpty()))
    {
        pNetwork->send(QString("NS IGNORE DEL %1").arg(strText));
        QTimer::singleShot(1000*4, this, SLOT(refresh())); // 4 sec
    }
}
