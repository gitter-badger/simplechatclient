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

#ifndef DLG_MODERATION_H
#define DLG_MODERATION_H

#include "defines.h"
#include <QDialog>
#include "ui_moderation.h"

class DlgModeration : public QDialog
{
    Q_OBJECT
public:
    DlgModeration(QWidget *);
    void setCurrentChannel(QString);
    void addMsg(QString, QString, QString, QString);

private:
    Ui::uiModeration ui;
    QString strCurrentChannel;
    sModerateMsg mModerateMessages;

    void createGui();
    void createSignals();
    void refresh();
    void removeSelected();

private slots:
    void comboChanged(QString);
    void buttonAccept();
    void buttonRemove();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

signals:
    void displayMsg(QString&, QString&, MessageCategory);
};

#endif // DLG_MODERATION_H
