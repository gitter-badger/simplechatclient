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

#ifndef DLG_MY_PROFILE_H
#define DLG_MY_PROFILE_H

class Network;
#include <QDialog>
#include "ui_my_profile.h"

class DlgMyProfile : public QDialog
{
    Q_OBJECT
public:
    DlgMyProfile(QWidget *, Network *, QMap <QString, QByteArray> *, QMap<QString,QString> *);

private:
    Ui::uiMyProfile ui;
    Network *pNetwork;
    QMap <QString, QByteArray> *mNickAvatar;
    QMap<QString,QString> *mMyProfile;

    void refresh();
    int get_index(QComboBox *, QString);
    QString convert_text_to_desc(QString);
    QString convert_desc_to_text(QString);
    QString convert_code_to_country(QString);
    QString convert_country_to_code(QString);
    QString convert_int_to_month(QString);
    QString convert_month_to_int(QString);
    QString convert_type(QString);

private slots:
    void button_ok();
    void button_close();
};

#endif // DLG_MY_PROFILE_H
