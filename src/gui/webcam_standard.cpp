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
#include "simple_rank_widget.h"
#include "webcam_delegate.h"
#include "webcam_standard.h"

DlgWebcamStandard::DlgWebcamStandard()
{
    ui.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Webcams"));

    ui.listWidget_nicks->setItemDelegate(new WebcamDelegate());

    pSimpleRankWidget = new SimpleRankWidget(this);
    pSimpleRankWidget->show();
    ui.horizontalLayout_toolbuttons->insertWidget(2, pSimpleRankWidget);

    createGui();
    createSignals();
}

DlgWebcamStandard::~DlgWebcamStandard()
{
    delete pSimpleRankWidget;
}

void DlgWebcamStandard::createGui()
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

void DlgWebcamStandard::createSignals()
{
    connect(ui.listWidget_nicks, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(changeUser(QListWidgetItem*)));
    connect(ui.toolButton_vote_minus, SIGNAL(clicked()), this, SLOT(voteMinus()));
    connect(ui.toolButton_vote_plus, SIGNAL(clicked()), this, SLOT(votePlus()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(buttonClose()));
}

void DlgWebcamStandard::updateImage(const QByteArray &b)
{
    ui.label_img->clear();
    QPixmap pixmap;
    pixmap.loadFromData(b);
    ui.label_img->setPixmap(pixmap);
}

void DlgWebcamStandard::updateText(const QString &s)
{
    ui.label_img->setText(s);
}

void DlgWebcamStandard::updateRank(int i)
{
    pSimpleRankWidget->setRank(i);
}

void DlgWebcamStandard::userError(const QString &s)
{
    // desc
    ui.textEdit_desc->clear();
    ui.textEdit_desc->hide();
    // img
    ui.label_img->setText(s);
    // rank
    pSimpleRankWidget->setRank(0);
    // channels
    ui.textEdit_channels->clear();
}

void DlgWebcamStandard::updateStatus(const QString &s)
{
    ui.textEdit_desc->setText(s);
    ui.textEdit_desc->show();
}

void DlgWebcamStandard::voteMinus()
{
    if (!strNick.isEmpty())
        emit networkSend(QString("VOTE %1 -").arg(strNick));
}

void DlgWebcamStandard::votePlus()
{
    if (!strNick.isEmpty())
        emit networkSend(QString("VOTE %1 +").arg(strNick));
}

void DlgWebcamStandard::voteOk()
{
    ui.toolButton_vote_minus->setEnabled(false);
    ui.toolButton_vote_plus->setEnabled(false);

    QTimer::singleShot(1000*3, this, SLOT(enableVote())); // 3 sec
}

void DlgWebcamStandard::enableVote()
{
    ui.toolButton_vote_minus->setEnabled(true);
    ui.toolButton_vote_plus->setEnabled(true);
}

void DlgWebcamStandard::error(const QString &s)
{
    updateText(s + "<br>"+tr("Disconnected from server webcams"));

    // clear
    ui.listWidget_nicks->clear();
    ui.textEdit_desc->clear();
    ui.textEdit_desc->hide();
    pSimpleRankWidget->setRank(0);
    ui.textEdit_channels->clear();
}

void DlgWebcamStandard::addUser(const QString &strNick, int iRank, const QString &strSpectators)
{
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setText(strNick);
    item->setData(Qt::UserRole, iRank);
    item->setData(Qt::UserRole+1, strSpectators);
    item->setData(Qt::UserRole+10, false); // is nicklist
    ui.listWidget_nicks->addItem(item);
}

void DlgWebcamStandard::updateUser(const QString &strNick, int iRank, const QString &strSpectators)
{
    QList<QListWidgetItem*> items = ui.listWidget_nicks->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
    {
        item->setData(Qt::UserRole, iRank);
        item->setData(Qt::UserRole+1, strSpectators);
    }
}

void DlgWebcamStandard::removeUser(const QString &strNick)
{
    QList<QListWidgetItem*> items = ui.listWidget_nicks->findItems(strNick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items)
        ui.listWidget_nicks->takeItem(ui.listWidget_nicks->row(item));
}

void DlgWebcamStandard::clearUsers()
{
    ui.listWidget_nicks->clear();
}

bool DlgWebcamStandard::existUser(QString strNick)
{
    QList<QListWidgetItem*> items = ui.listWidget_nicks->findItems(strNick, Qt::MatchCaseSensitive);

    if (items.size() != 0)
        return true;
    else
        return false;
}

void DlgWebcamStandard::changeUser(QListWidgetItem *item)
{
    // img
    ui.label_img->setText(tr("Downloading image"));

    // clear desc
    ui.textEdit_desc->clear();
    ui.textEdit_desc->hide();

    // clear rank
    pSimpleRankWidget->setRank(0);

    // read nick
    QString strNewNick = item->text();

    // change user
    if (strNick.isEmpty())
        emit networkSend(QString("SUBSCRIBE_BIG * %1").arg(strNewNick));
    else
        emit networkSend(QString("UNSUBSCRIBE_BIG %1").arg(strNick));

    // set nick
    strNick = strNewNick;

    // display nick
    ui.label_nick->setText(strNick);

    // set nick
    emit setUser(strNick);

    // update channels
    //ui.textEdit_channels->setText(QString("<b>%1</b><br><font color=\"#0000ff\">%2</font>").arg(tr("Is on channels:"), mNickChannels[strNick]));
}

void DlgWebcamStandard::buttonClose()
{
    this->close();
}

void DlgWebcamStandard::closeEvent(QCloseEvent *)
{
    emit closeCam();
}
