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

#ifndef UPDATES_H
#define UPDATES_H

class QNetworkAccessManager;
class QNetworkReply;
#include <QObject>

/**
 * Update class. If there is update it show dialog from update.
 */
class Updates : public QObject
{
    Q_OBJECT
public:
    Updates();
    virtual ~Updates();
    void checkUpdate();

private:
    QNetworkAccessManager *accessManager;

    void version(const QString&);

private slots:
    void updateFinished(QNetworkReply*);
};

#endif // UPDATES_H
