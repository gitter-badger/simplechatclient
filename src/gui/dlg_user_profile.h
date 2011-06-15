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

#ifndef DLG_USER_PROFILE_H
#define DLG_USER_PROFILE_H

class QFormLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QTextEdit;
class QToolButton;
class QVBoxLayout;

class Network;
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DlgUserProfile : public QDialog
{
    Q_OBJECT
public:
    DlgUserProfile(QWidget *, Network *);
    virtual ~DlgUserProfile();
    void set_nick(QString);
    inline QString get_nick() { return strNick; }

public slots:
    void set_user_info(QString, QString);

private:
    Network *pNetwork;
    QPixmap avatar;
    QNetworkAccessManager *accessManager;
    QString strNick;

    QVBoxLayout *mainLayout;
    QLabel *label_nick;

    QWidget *toolWidget;
    QVBoxLayout *toolLayout;
    QToolButton *toolButton_zoom;

    QWidget *topWidget;
    QHBoxLayout *topLayout;
    QLabel *label_avatar;
    QTextEdit *textEdit_desc;

    QWidget *agesexWidget;
    QHBoxLayout *agesexLayout;
    QLabel *label_age;
    QLineEdit *lineEdit_age;
    QLabel *label_sex;
    QLineEdit *lineEdit_sex;

    QWidget *moreWidget;
    QFormLayout *moreLayout;
    QLineEdit *lineEdit_birthdate;
    QLineEdit *lineEdit_city;
    QLineEdit *lineEdit_country;
    QPlainTextEdit *plainTextEdit_hobby;
    QLineEdit *lineEdit_type;
    QLabel *label_website_link;

    QWidget *pushButtonWidget;
    QHBoxLayout *pushButtonLayout;
    QPushButton *pushButton_more;
    QPushButton *pushButton_close;

    void create_gui();
    void create_signals();

    void clear_info();
    void show_info();
    QString convert_desc(QString);
    QString convert_sex(QString);
    QString convert_age(QString);
    QString convert_code_to_country(QString);
    QString convert_type(QString);
    void show_avatar(QString);

private slots:
    void avatar_finished(QNetworkReply*);
    void button_zoom();
    void button_more();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);
};

#endif // DLG_USER_PROFILE_H
