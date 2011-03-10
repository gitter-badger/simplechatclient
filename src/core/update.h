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

#ifndef UPDATE_H
#define UPDATE_H

#include <QNetworkReply>
#include <QObject>

/**
 * Update class. If there is update it show dialog from dlg_update.
 */
class Update : public QObject
{
    Q_OBJECT
public:
    Update(QWidget *);
    void check_update();

private:
    QWidget *myparent;
    QNetworkReply *pReply;
    QNetworkAccessManager *accessManager;

    void version(QString);

private slots:
    void update_finished();
};

#endif // UPDATE_H
