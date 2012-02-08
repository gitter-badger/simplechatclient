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

#include "webcam_mini.h"

DlgWebcamMini::DlgWebcamMini(QString nick)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(nick);
}

void DlgWebcamMini::updateImage(const QByteArray &b)
{
    QPixmap pixmap;
    pixmap.loadFromData(b);
    ui.label_img->setPixmap(pixmap);
}

void DlgWebcamMini::updateText(const QString &s)
{
    ui.label_img->setText(s);
}

void DlgWebcamMini::error(const QString &s)
{
    updateText(s + "<br>"+tr("Disconnected from server webcams"));
}

void DlgWebcamMini::closeEvent(QCloseEvent *)
{
    emit closeCam();
}
