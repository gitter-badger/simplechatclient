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

#include "dlg_update.h"

DlgUpdate::DlgUpdate(QWidget *parent, QString param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Update"));

    strVersion = param1;

    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));

    QString strLink;
#ifdef Q_WS_WIN
    strLink += QString("<a href=\"http://sourceforge.net/projects/simplechatclien/files/scc-"+strVersion+".exe/download\">%1</a>").arg(tr("download"));
#endif
#ifdef Q_WS_X11
    strLink += QString("<a href=\"http://sourceforge.net/projects/simplechatclien/files/scc-"+strVersion+".tar.bz2/download\">%1</a>").arg(tr("download"));
#endif

    QString strDisplay = QString(tr("A new version %1 is available. Please %2 an installer and upgrade.").arg(strVersion).arg(strLink));
    ui.label_msg->setText(strDisplay);

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
}

void DlgUpdate::button_ok()
{
    this->close();
}

void DlgUpdate::hideEvent(QHideEvent *event)
{
    event->accept();

    ui.buttonBox->QObject::disconnect();
}
