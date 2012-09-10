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

#ifndef MODERATION_MODEL_H
#define MODERATION_MODEL_H

#include "defines.h"
#include <QMultiHash>
#include <QObject>

class ModerationModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ModerationModel)
    static ModerationModel *Instance;
public:
    static ModerationModel *instance();

    ModerationModel();
    void init();
    void clear();
    QMultiHash<QString,OnetModerateMessage> getAll();
    void set(const QString &id, const QString &channel, qint64 time, const QString &nick, const QString &message);
    void remove(const QString &id);

private:
    QMultiHash<QString,OnetModerateMessage> moderateMessages;
};

#endif // MODERATION_MODEL_H
