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

#ifndef DLG_CHANNEL_SETTINGS_H
#define DLG_CHANNEL_SETTINGS_H

class SimpleStatsWidget;
#include <QDialog>
#include "ui_channel_settings.h"

class DlgChannelSettings : public QDialog
{
    Q_OBJECT
public:
    DlgChannelSettings(QWidget *);
    void setChannel(QString);
    inline QString getChannel() { return strChannel; }
    void setData(QMap<QString, QString>);
    void setStatsData(QMap<QString, QString>);

    void setTopic(QString);
    void setOwner(QString);
    void setDescription(QString);
    void addOp(QString);
    void addHalfop(QString);
    void addBan(QString, QString, QString, QString);
    void addInvite(QString, QString, QString);

private:
    Ui::uiChannelSettings ui;
    QString strChannel;
    SimpleStatsWidget *pSimpleStatsWidget;

    void createGui();
    void setDefaultValues();
    void createSignals();

    void enableTabs();
    bool existItem(QString, QListWidget *); // for permissions
    void clear();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

private slots:
    void ownerChanged();
    void removeChannelClicked();
    void emailChanged();
    void wwwChanged();
    void topicChanged();
    void descChanged();
    void statusPub();
    void statusPriv();
    void categoryChanged(int);
    void guardianInactive();
    void guardianActive();
    void guardianClicked(int);
    void passwordChanged();
    void limitChanged();
    void moderatedInactive();
    void moderatedActive();
    void auditoriumInactive();
    void auditoriumActive();

    void buttonPermissionAdd();
    void buttonPermissionRemove();
    void changePermissionList(QModelIndex);

    void buttonOk();
    void buttonCancel();
};

#endif // DLG_CHANNEL_SETTINGS_H
