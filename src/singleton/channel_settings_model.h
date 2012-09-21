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

#ifndef CHANNEL_SETTINGS_MODEL_H
#define CHANNEL_SETTINGS_MODEL_H

#include <QHash>
#include <QObject>

class ChannelSettingsModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ChannelSettingsModel)
    static ChannelSettingsModel *Instance;
public:
    static ChannelSettingsModel *instance();

    ChannelSettingsModel();
    void clear();

    QString getChannel();
    void setChannel(const QString &channel);

    QString getInfo(const QString &key);
    QHash<QString,QString> getAllInfo();
    void setInfo(const QString &key, const QString &value);

    QMultiHash<QString,QString> getAllPermission();
    void setPermission(const QString &key, const QString &value);
    bool containsPermission(const QString &key, const QString &value);

    bool getReadyInfo();
    void setReadyInfo(bool bReady);

    QString getStats(const QString &key);
    QHash<QString,QString> getAllStats();
    void setStats(const QString &key, const QString &value);

    bool getReadyStats();
    void setReadyStats(bool bReady);

private:
    QString strChannel;

    QHash<QString,QString> lSettingsInfo;
    QMultiHash<QString,QString> lSettingsPermissions;
    bool bReadyInfo;

    QHash<QString,QString> lSettingsStats;
    bool bReadyStats;
};

#endif // CHANNEL_SETTINGS_MODEL_H
