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

#include <QVBoxLayout>
#include "core.h"
#include "tab_widget.h"

TabWidget::TabWidget(QString param1, DlgUserProfile *param2)
{
    strName = param1;
    pDlgUserProfile = param2;

    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
    QString strBackgroundColor = Core::instance()->settings.value("background_color");

    topic = new QLabel(this);
    topic->setWordWrap(true);
    topic->setMinimumHeight(25);
    topic->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    topic->show();

    pChatView = new ChatView(strName, pDlgUserProfile);
    pChatView->setParent(this);
    pChatView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pChatView->show();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->addWidget(topic);
    mainLayout->addWidget(pChatView);
    this->setLayout(mainLayout);

    if (strName[0] == '^')
        topic->hide();
    if ((strName[0] != '^') && (strName[0] != '#'))
        topic->hide();

    // set colors
    if ((strBackgroundColor.toLower() != "#ffffff") && (!strBackgroundColor.isEmpty()))
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);
}

TabWidget::~TabWidget()
{
    delete pChatView;
}

void TabWidget::refreshColors()
{
    // get values
    QString strBackgroundColor = Core::instance()->settings.value("background_color");
    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");

    // this
    if (strBackgroundColor.toLower() != "#ffffff")
        this->setStyleSheet(QString("color:%1;background-color:%2;").arg(strDefaultFontColor).arg(strBackgroundColor));
    else
        this->setStyleSheet(QString::null);
}
