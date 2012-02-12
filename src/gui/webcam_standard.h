/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#ifndef WEBCAM_STANDARD_H
#define WEBCAM_STANDARD_H

class SimpleRankWidget;
#include <QDialog>
#include "ui_webcam_standard.h"

class DlgWebcamStandard : public QDialog
{
    Q_OBJECT
public:
    DlgWebcamStandard();
    virtual ~DlgWebcamStandard();

public slots:
    void updateImage(const QByteArray&);
    void updateText(const QString&);
    void updateRank(int);
    void userError(const QString&);
    void updateStatus(const QString&);
    void voteOk();
    void error(const QString&);
    void addUser(const QString&,int,const QString&);
    void updateUser(const QString&,int,const QString&);
    void removeUser(const QString&);
    void clearUsers();

private:
    Ui::uiWebcamStandard ui;
    QString strNick;
    SimpleRankWidget *pSimpleRankWidget;

    void createGui();
    void createSignals();
    bool existUser(QString);

private slots:
    void changeUser(QListWidgetItem *);
    void voteMinus();
    void votePlus();
    void enableVote();
    void buttonClose();

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void closeCam();
    void networkSend(QString);
    void setUser(QString);
};

#endif // WEBCAM_STANDARD_H
