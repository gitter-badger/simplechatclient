/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr £uczko <piotr.luczko@gmail.com>               *
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

#include <QDialog>
#include "network.h"
#include "tab_container.h"
#include "ui_channel_list.h"

class dlg_channel_list : public QDialog
{
    Q_OBJECT
public:
    dlg_channel_list(QSettings *, network *, tab_container *);
    void clear();
    void add_channel(QString, QString, QString, QString);
    void create_list();
    void sort();

private:
    Ui::uiChannelList ui;
    QSettings *settings;
    network *pNetwork;
    tab_container *tabc;
    QVector < QVector<QString> > list_1;
    QVector < QVector<QString> > list_2;
    QVector < QVector<QString> > list_3;
    QVector < QVector<QString> > list_4;
    QVector < QVector<QString> > list_5;
    QVector < QVector<QString> > list_6;

    void QuickSort(QVector< QVector<QString> > *, int, int);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void allCellDoubleClicked(int, int);
    void teenCellDoubleClicked(int, int);
    void towarzyskieCellDoubleClicked(int, int);
    void erotyczneCellDoubleClicked(int, int);
    void tematyczneCellDoubleClicked(int, int);
    void regionalneCellDoubleClicked(int, int);
    void button_ok();
    void button_cancel();

};

#endif // DLG_CHANNEL_LIST_H
