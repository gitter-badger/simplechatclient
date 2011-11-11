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
#include <QPushButton>
#include "webbrowser.h"

DlgWebBrowser::DlgWebBrowser(QWidget *parent, QUrl url) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("YouTube");
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    ui.webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    ui.webView->settings()->setAttribute(QWebSettings::AutoLoadImages, true);

    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    // signals
    QObject::connect(ui.webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    // load
    ui.webView->load(url);
}

void DlgWebBrowser::loadFinished()
{
    setWindowTitle(ui.webView->title());
}
