/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef MY_PROFILE_H
#define MY_PROFILE_H

class QNetworkAccessManager;
class QNetworkReply;
#include <QDialog>
#include "ui_my_profile.h"

class DlgMyProfile : public QDialog
{
    Q_OBJECT
public:
    DlgMyProfile(QWidget *);
    virtual ~DlgMyProfile();

private:
    Ui::uiMyProfile ui;
    QNetworkAccessManager *accessManager;

    void createGui();
    void setDefaultValues();
    void createSignals();

    void refresh();
    int getIndex(QComboBox *, const QString&);
    QString convertTextToDesc(QString);
    QString convertDescToText(QString);
    QString convertCodeToCountry(const QString&);
    QString convertCountryToCode(const QString&);
    QString convertIntToMonth(const QString&);
    QString convertMonthToInt(const QString&);
    QString convertType(const QString&);

private slots:
    void avatarFinished(QNetworkReply*);
    void buttonOk();
};

#endif // MY_PROFILE_H
