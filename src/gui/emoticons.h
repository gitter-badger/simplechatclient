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

#ifndef EMOTICONS_H
#define EMOTICONS_H

class InputLineWidget;
class QListWidget;
#include <QDialog>
#include <QListWidgetItem>
#include <QThread>
#include "ui_emoticons.h"

enum EmoticonsRoles
{
    EmoticonNameRole = Qt::UserRole
};

class DlgEmoticonsThread : public QThread
{
    Q_OBJECT
public:
    DlgEmoticonsThread();
    void setDir(const QString &_strDir);

private:
    QString strDir;

protected:
    void run();

signals:
    void addEmoticon(const QString &strEmoticon, const QByteArray &bData);
    void sortEmoticons();
};

class DlgEmoticonsTab : public QWidget
{
    Q_OBJECT
public:
    DlgEmoticonsTab(const QString &_strDir, QWidget *parent = 0);
    QListWidget *listWidget;
    DlgEmoticonsThread thread;

public slots:
    void addEmoticon(const QString &strEmoticon, const QByteArray &bData);
    void sortEmoticons();
};

class DlgEmoticons : public QDialog
{
    Q_OBJECT
public:
    DlgEmoticons(InputLineWidget *_pInputLineWidget, QWidget *parent = 0);
    virtual ~DlgEmoticons();

private:
    Ui::uiEmoticons ui;
    InputLineWidget *pInputLineWidget;
    QList<int> lReadedTabIndex;

    void createGui();
    void setDefaultValues();
    void createSignals();

public slots:
    void tabChanged(int index);
    void buttonInsert();
};

#endif // EMOTICONS_H
