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

#ifndef DLG_EMOTICONS_H
#define DLG_EMOTICONS_H

class InputWidget;
class MainWindow;
#include <QDialog>
#include <QThread>
#include "ui_emoticons.h"

class ThreadEmoticonsStandard : public QThread
{
    Q_OBJECT
public:
    ThreadEmoticonsStandard();

protected:
    void run();

signals:
    void insertEmoticonsStandard(QString, QByteArray);
};

class ThreadEmoticonsExtended : public QThread
{
    Q_OBJECT
public:
    ThreadEmoticonsExtended();

protected:
    void run();

signals:
    void insertEmoticonsExtended(QString, QByteArray);
};

class DlgEmoticons : public QDialog
{
    Q_OBJECT
public:
    DlgEmoticons(MainWindow *, InputWidget *);
    ~DlgEmoticons();

private:
    Ui::uiEmoticons ui;
    InputWidget *pInputWidget;
    ThreadEmoticonsStandard pThreadEmoticonsStandard;
    ThreadEmoticonsExtended pThreadEmoticonsExtended;
    bool bDoneStandard;
    bool bDoneExtended;

    void createGui();
    void setDefaultValues();
    void createSignals();

    void getEmoticonsStandard();
    void getEmoticonsExtended();

private slots:
    void insertEmoticonsStandard(QString, QByteArray);
    void insertEmoticonsExtended(QString, QByteArray);
    void currentTabChanged(int);
    void clickedStandard(QModelIndex);
    void clickedExtended(QModelIndex);
    void buttonInsert();
};

#endif // DLG_EMOTICONS_H
