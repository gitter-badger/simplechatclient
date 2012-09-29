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

#ifndef USER_PROFILE_MODEL_H
#define USER_PROFILE_MODEL_H

#include <QHash>
#include <QObject>

class UserProfileModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(UserProfileModel)
    static UserProfileModel *Instance;
public:
    static UserProfileModel *instance();

    UserProfileModel();
    void clear();
    QString getNick();
    void setNick(const QString &newNick);
    QString get(const QString &key);
    QHash<QString,QString> getAll();
    void set(const QString &key, const QString &value);
    bool getReady();
    void setReady(bool readyState);

private:
    QString nick;
    QHash<QString,QString> userProfile;
    bool ready;
};

#endif // USER_PROFILE_MODEL_H
