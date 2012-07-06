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

#ifndef CHANNEL_HOMES_MODEL_H
#define CHANNEL_HOMES_MODEL_H

#include <QObject>

class ChannelHomes : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ChannelHomes)
    static ChannelHomes *Instance;
public:
    static ChannelHomes *instance();

    ChannelHomes();
    void init();
    void clear();
    void add(const QString &channel);
    QList<QString> get();
    void setReady(bool ready);
    bool getReady();

private:
    QList<QString> lChannelHomes;
    bool bChannelHomesReady;
};

#endif // CHANNEL_HOMES_MODEL_H
