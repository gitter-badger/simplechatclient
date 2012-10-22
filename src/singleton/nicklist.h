/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NICKLIST_H
#define NICKLIST_H

#include <QHash>
#include <QObject>

class Nicklist : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Nicklist)
    static Nicklist *Instance;
public:
    static Nicklist *instance();

    Nicklist();
    void addUser(const QString &strNick, const QString &strChannel, const QString &strModes);
    void delUser(const QString &strNick, const QString &strChannel);
    void renameUser(const QString &strNick, const QString &strNewNick, const QString &strDisplay);
    void quitUser(const QString &strNick, const QString &strDisplay);
    void changeFlag(const QString &strNick, const QString &strChannel, const QString &strFlag);
    void changeFlag(const QString &strNick, const QString &strFlag);
    void clearAllNicklist();
    void setUserAvatar(const QString &strNick, const QString &strValue);
    QString getUserAvatar(const QString &strNick);
    QString getUserModes(const QString &strNick, const QString &strChannel);
    int getUserMaxModes(const QString &strModes);

private:
    QHash<QString,QString> lAvatar;

    void updateUserAvatar(const QString &strNick, const QString &strValue);
};

#endif // NICKLIST_H
