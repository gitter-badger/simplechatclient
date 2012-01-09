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

#ifndef ONET_AUTH_H
#define ONET_AUTH_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkCookieJar;
class QNetworkReply;
class QTimer;
class TabContainer;

/**
 * Authorize class for onet
 */
class OnetAuth : public QObject
{
    Q_OBJECT
public:
    OnetAuth(TabContainer *);
    virtual ~OnetAuth();

public slots:
    void authorize(QString, QString, QString);

private:
    TabContainer *pTabC;
    QNetworkAccessManager *accessManager;
    QNetworkCookieJar *cookieJar;

    QTimer *timerSk;

    QString strNick;
    QString strNickAuth;
    QString strPass;
    QString strNickLen;
    QString strVersionLen;
    bool bRegisteredNick;
    bool bOverride;

    bool bAuthorizing;

    QString strVersion;
    QString strCaptcha;

    void getChat();
    void getDeploy();
    void gotDeploy(QString);
    void getKropka();
    void getKropkaFull();
    void getSk();
    void getSecureKropka();
    void getSecureMlogin();
    void getOverride();
    void getUo();
    void showCaptchaDialog();
    void getCheckCode();

    QString getVersion(QString);
    void saveCookies();
    void requestFinished(QString);

private slots:
    void networkFinished(QNetworkReply*);
    void refreshSk();

};

#endif // ONET_AUTH_H
