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

#ifndef DLG_CHANNEL_LIST_H
#define DLG_CHANNEL_LIST_H

class Network;
#include <QDialog>
#include "ui_channel_list.h"

class DlgChannelList : public QDialog
{
    Q_OBJECT
public:
    DlgChannelList(QWidget *, Network *);

private:
    Ui::uiChannelList ui;
    Network *pNetwork;

    void createGui();
    void setDefaultValues();
    void createSignals();

    bool isErotic(QString);
    void createList();
    void showAllChannels();
    void applyCheckboxes();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private slots:
    void allCellDoubleClicked(int, int);
    void teenCellDoubleClicked(int, int);
    void commonCellDoubleClicked(int, int);
    void eroticCellDoubleClicked(int, int);
    void thematicCellDoubleClicked(int, int);
    void regionalCellDoubleClicked(int, int);
    void buttonSearch();
    void buttonClear();
    void hideEmptyChannels();
    void showAdvOptions();
};

#endif // DLG_CHANNEL_LIST_H
