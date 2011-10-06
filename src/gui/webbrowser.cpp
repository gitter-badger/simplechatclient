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

#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "webbrowser.h"

DlgWebBrowser::DlgWebBrowser(QWidget *parent, QUrl url) : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("YouTube.com");

    view = new QWebView(this);
    view->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    view->settings()->setAttribute(QWebSettings::AutoLoadImages, true);
    view->show();

    QDialogButtonBox *box = new QDialogButtonBox(this);
    box->setStandardButtons(QDialogButtonBox::Close);
    box->show();

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(view);
    l->addWidget(box);
    this->setLayout(l);

    // close button
    QObject::connect(view, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished()));
    QObject::connect(box, SIGNAL(rejected()), this, SLOT(close()));

    // load
    view->load(url);
}

void DlgWebBrowser::loadFinished()
{
    // set title
    setWindowTitle(view->title());

    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}
