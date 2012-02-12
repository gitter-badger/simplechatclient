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

#ifndef USER_PROFILE_H
#define USER_PROFILE_H

class QFormLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QTextEdit;
class QToolButton;
class QVBoxLayout;

class QNetworkAccessManager;
class QNetworkReply;
#include <QDialog>

class DlgUserProfile : public QDialog
{
    Q_OBJECT
public:
    DlgUserProfile(QWidget *parent, const QString &_strNick);
    virtual ~DlgUserProfile();

private:
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

    void createGui();
    void createSignals();

    QString convertDesc(QString strContent);
    QString convertSex(const QString &strSex);
    QString convertAge(const QString &strDate);
    QString convertCodeToCountry(const QString &strCountryCode);
    QString convertType(const QString &strType);
    void showAvatar(const QString &strUrl);

private slots:
    void refreshUserInfo();
    void avatarFinished(QNetworkReply *pReply);
    void buttonZoom();
    void buttonMore();
    void buttonClose();
};

#endif // USER_PROFILE_H
