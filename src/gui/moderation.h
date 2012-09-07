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

#ifndef MODERATION_H
#define MODERATION_H

#include <QDialog>
#include "ui_moderation.h"

class DlgModeration : public QDialog
{
    Q_OBJECT
public:
    DlgModeration(const QString &_strCurrentChannel, QWidget *parent = 0);

private:
    Ui::uiModeration ui;
    QString strCurrentChannel;

    void createGui();
    void createSignals();

    void removeSelected();

private slots:
    void refreshMessages();
    void buttonAccept();
    void buttonRemove();
};

#endif // MODERATION_H
