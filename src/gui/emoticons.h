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

#ifndef EMOTICONS_H
#define EMOTICONS_H

class InputLineWidget;
class MainWindow;
class QListWidget;
#include <QDialog>
#include "ui_emoticons.h"

class DlgEmoticonsTab : public QWidget
{
    Q_OBJECT
public:
    DlgEmoticonsTab(QString, QWidget *parent = 0);
    QListWidget *listWidget;

private:
    QString strDir;

    void showEmoticons();
};

class DlgEmoticons : public QDialog
{
    Q_OBJECT
public:
    DlgEmoticons(MainWindow *, InputLineWidget *);

private:
    Ui::uiEmoticons ui;
    InputLineWidget *pInputLineWidget;

    void createGui();
    void setDefaultValues();
    void createSignals();

public slots:
    void buttonInsert();
};

#endif // EMOTICONS_H
