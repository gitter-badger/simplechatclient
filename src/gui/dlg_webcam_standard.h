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

#ifndef DLG_WEBCAM_STANDARD_H
#define DLG_WEBCAM_STANDARD_H

class SimpleRankWidget;
#include <QDialog>
#include "ui_webcam_standard.h"

class DlgWebcamStandard : public QDialog
{
    Q_OBJECT
public:
    DlgWebcamStandard();

public slots:
    void update_image(QByteArray);
    void update_text(QString);
    void update_rank(int);
    void user_error(QString);
    void update_status(QString);
    void vote_ok();
    void error(QString);
    void add_user(QString,int,QString);
    void update_user(QString,int,QString);
    void remove_user(QString);
    void clear_users();

private:
    Ui::uiWebcamStandard ui;
    QString strNick;
    SimpleRankWidget *pSimpleRankWidget;

    void create_gui();
    void create_signals();
    bool exist_user(QString);

private slots:
    void change_user(QListWidgetItem *);
    void vote_minus();
    void vote_plus();
    void enable_vote();
    void button_close();

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void close_cam();
    void network_send(QString);
    void set_user(QString);
};

#endif // DLG_WEBCAM_STANDARD_H
