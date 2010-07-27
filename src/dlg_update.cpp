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

DlgUpdate::DlgUpdate(QString param1)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Update"));

    strVersion = param1;

    QString strDisplay;
    strDisplay += tr("An update is available. Download the newer version is available from the address:")+"<br>";
#ifdef Q_WS_WIN
    strDisplay += "<a href=\"http://sourceforge.net/projects/simplechatclien/files/scc-"+strVersion+".msi/download\">http://sourceforge.net/projects/simplechatclien/files/scc-"+strVersion+".msi/download</a>";
#endif
#ifdef Q_WS_X11
    strDisplay += "<a href=\"http://sourceforge.net/projects/simplechatclien/files/scc-"+strVersion+".tar.bz2/download\">http://sourceforge.net/projects/simplechatclien/files/scc-"+strVersion+".tar.bz2/download</a>";
#endif
    ui.label_msg->setText(strDisplay);

    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
}

void DlgUpdate::button_ok()
{
    this->close();
    exit(0);
}

void DlgUpdate::hideEvent(QHideEvent *event)
{
    event->accept();

    ui.buttonBox->QObject::disconnect();
    exit(0);
}
