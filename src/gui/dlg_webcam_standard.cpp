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

#include <QPushButton>
#include <QTimer>
#include "defines.h"
#include "simplerankwidget.h"
#include "webcamdelegate.h"
#include "dlg_webcam_standard.h"

DlgWebcamStandard::DlgWebcamStandard()
{
    ui.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Webcams"));

    ui.listWidget_nicks->setItemDelegate(new WebcamDelegate());

    pSimpleRankWidget = new SimpleRankWidget(this);
    pSimpleRankWidget->show();
    ui.horizontalLayout_toolbuttons->insertWidget(2, pSimpleRankWidget);

    create_gui();
    create_signals();
}

void DlgWebcamStandard::create_gui()
{
    ui.toolButton_vote_minus->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.toolButton_vote_plus->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.label_list_nick->setText(tr("Nick"));
    ui.label_list_spectators->setText(tr("Spectators"));
    ui.label_list_rank->setText(tr("Rank"));

    //hide desc
    ui.textEdit_desc->clear();
    ui.textEdit_desc->hide();
}

void DlgWebcamStandard::create_signals()
{
    QObject::connect(ui.listWidget_nicks, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(change_user(QListWidgetItem*)));
    QObject::connect(ui.toolButton_vote_minus, SIGNAL(clicked()), this, SLOT(vote_minus()));
    QObject::connect(ui.toolButton_vote_plus, SIGNAL(clicked()), this, SLOT(vote_plus()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
}

void DlgWebcamStandard::update_image(QByteArray b)
{
    ui.label_img->clear();
    QPixmap pixmap;
    pixmap.loadFromData(b);
    ui.label_img->setPixmap(pixmap);
}

void DlgWebcamStandard::update_text(QString s)
{
    ui.label_img->setText(s);
}

void DlgWebcamStandard::update_rank(int i)
{
    pSimpleRankWidget->set_rank(i);
}

void DlgWebcamStandard::user_error(QString s)
{
    // desc
    ui.textEdit_desc->clear();
    ui.textEdit_desc->hide();
    // img
    ui.label_img->setText(s);
    // rank
    pSimpleRankWidget->set_rank(0);
    // channels
    ui.textEdit_channels->clear();
}

void DlgWebcamStandard::update_status(QString s)
{
    ui.textEdit_desc->setText(s);
    ui.textEdit_desc->show();
}

void DlgWebcamStandard::vote_minus()
{
    if (!strNick.isEmpty())
        emit network_send(QString("VOTE %1 -").arg(strNick));
}

void DlgWebcamStandard::vote_plus()
{
    if (!strNick.isEmpty())
        emit network_send(QString("VOTE %1 +").arg(strNick));
}

void DlgWebcamStandard::vote_ok()
{
    ui.toolButton_vote_minus->setEnabled(false);
    ui.toolButton_vote_plus->setEnabled(false);

    QTimer::singleShot(1000*3, this, SLOT(enable_vote())); // 3 sec
}

void DlgWebcamStandard::enable_vote()
{
    ui.toolButton_vote_minus->setEnabled(true);
    ui.toolButton_vote_plus->setEnabled(true);
}

void DlgWebcamStandard::error(QString s)
{
    s += "<br>"+tr("Disconnected from server webcams");
    update_text(s);

    // clear
    ui.listWidget_nicks->clear();
    ui.textEdit_desc->clear();
    ui.textEdit_desc->hide();
    pSimpleRankWidget->set_rank(0);
    ui.textEdit_channels->clear();
}

void DlgWebcamStandard::add_user(QString strNick, int iRank, QString strSpectators)
{
    int row = ui.listWidget_nicks->count();
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setText(strNick);
    item->setData(Qt::UserRole, iRank);
    item->setData(Qt::UserRole+1, strSpectators);
    ui.listWidget_nicks->insertItem(row,item);
}

void DlgWebcamStandard::update_user(QString strNick, int iRank, QString strSpectators)
{
    /* update */
    if (exist_user(strNick))
        remove_user(strNick);

    /* add user */
    int row = ui.listWidget_nicks->count();
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setText(strNick);
    item->setData(Qt::UserRole, iRank);
    item->setData(Qt::UserRole+1, strSpectators);
    ui.listWidget_nicks->insertItem(row,item);
}

void DlgWebcamStandard::remove_user(QString strNick)
{
    for (int i = 0; i < ui.listWidget_nicks->count(); i++)
    {
        if (ui.listWidget_nicks->item(i)->text() == strNick)
        {
            ui.listWidget_nicks->takeItem(i);
            break;
        }
    }
}

void DlgWebcamStandard::clear_users()
{
    ui.listWidget_nicks->clear();
}

bool DlgWebcamStandard::exist_user(QString strNick)
{
    QList<QListWidgetItem*> items = ui.listWidget_nicks->findItems(strNick, Qt::MatchCaseSensitive);

    if (items.size() != 0)
        return true;
    else
        return false;
}

void DlgWebcamStandard::change_user(QListWidgetItem *item)
{
    // img
    ui.label_img->setText(tr("Downloading image"));

    // clear desc
    ui.textEdit_desc->clear();
    ui.textEdit_desc->hide();

    // clear rank
    pSimpleRankWidget->set_rank(0);

    // read nick
    QString strNewNick = item->text();

    // change user
    if (strNick.isEmpty())
        emit network_send(QString("SUBSCRIBE_BIG * %1").arg(strNewNick));
    else
        emit network_send(QString("UNSUBSCRIBE_BIG %1").arg(strNick));

    // set nick
    strNick = strNewNick;

    // display nick
    ui.label_nick->setText(strNick);

    // set nick
    emit set_user(strNick);

    // update channels
    //ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:")).arg(mNickChannels[strNick]));
}

void DlgWebcamStandard::button_close()
{
    this->close();
}

void DlgWebcamStandard::closeEvent(QCloseEvent *e)
{
    Q_UNUSED (e);
    emit close_cam();
}
