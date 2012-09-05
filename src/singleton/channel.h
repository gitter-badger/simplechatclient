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

#ifndef CHANNEL_H
#define CHANNEL_H

#include "defines.h"
#include <QHash>
#include <QObject>

class Channel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Channel)
    static Channel *Instance;
public:
    static Channel *instance();

    Channel();
    void init();

    void add(const QString &channel);
    void remove(const QString &channel);
    QList<QString> get();
    QList<QString> getCleared();
    void move(int from, int to);
    bool contains(const QString &channel);
    QString getFromIndex(int index);
    int getIndex(const QString &channel);
    QString getCurrent();
    QList<CaseIgnoreString> getSorted();
    // avatar
    QString getAvatar(const QString &channel);
    void setAvatar(const QString &channel, const QString &path);
    // priv
    void setPriv(const QString &channel, const QString &name);
    QString getPriv(const QString &channel);
    bool containsPriv(const QString &channel);

private:
    QList<QString> lChannels;
    QHash<QString,QString> lAvatar;
    QHash<QString,QString> lPriv;

};

#endif // CHANNEL_H
