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

#ifndef DLG_CHANNEL_LIST_H
#define DLG_CHANNEL_LIST_H

#include <QDialog>
#include "tab_container.h"
#include "ui_channel_list.h"

class DlgChannelList : public QDialog
{
    Q_OBJECT
public:
    DlgChannelList(QSettings *, TabContainer *);
    void clear();
    void add_channel(QString, QString, QString, QString);
    void create_list();
    void sort();

private:
    Ui::uiChannelList ui;
    QSettings *settings;
    TabContainer *tabc;
    QList < QList<QString> > list_all;
    QList < QList<QString> > list_teen;
    QList < QList<QString> > list_towarzyskie;
    QList < QList<QString> > list_erotyczne;
    QList < QList<QString> > list_tematyczne;
    QList < QList<QString> > list_regionalne;

    void quickSort(QList< QList<QString> > *, int, int);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void all_CellDoubleClicked(int, int);
    void teen_CellDoubleClicked(int, int);
    void towarzyskie_CellDoubleClicked(int, int);
    void erotyczne_CellDoubleClicked(int, int);
    void tematyczne_CellDoubleClicked(int, int);
    void regionalne_CellDoubleClicked(int, int);
    void button_ok();
    void button_cancel();

signals:
    void send(QString);

};

#endif // DLG_CHANNEL_LIST_H
