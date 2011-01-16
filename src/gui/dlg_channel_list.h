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

#include <QDialog>
#include "ui_channel_list.h"

class DlgChannelList : public QDialog
{
    Q_OBJECT
public:
    DlgChannelList(QWidget *);
    void clear();
    void add_channel(QString, QString, QString, QString);
    void create_list();
    void sort();

private:
    Ui::uiChannelList ui;
    QList < QList<QString> > list_all;
    QList < QList<QString> > list_teen;
    QList < QList<QString> > list_common;
    QList < QList<QString> > list_erotic;
    QList < QList<QString> > list_thematic;
    QList < QList<QString> > list_regional;

    void quickSort(QList< QList<QString> > *, int, int);
    void show_all_channels();
    void apply_checkboxes();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void closeEvent(QCloseEvent *);
    virtual void keyPressEvent(QKeyEvent *);

private slots:
    void all_CellDoubleClicked(int, int);
    void teen_CellDoubleClicked(int, int);
    void common_CellDoubleClicked(int, int);
    void erotic_CellDoubleClicked(int, int);
    void thematic_CellDoubleClicked(int, int);
    void regional_CellDoubleClicked(int, int);
    void button_close();
    void button_search();
    void button_clear();
    void hide_empty_channels();
    void show_adv_options();

signals:
    void send(QString);
};

#endif // DLG_CHANNEL_LIST_H
