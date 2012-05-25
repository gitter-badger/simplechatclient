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

#ifndef ONET_AUTH_H
#define ONET_AUTH_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkCookieJar;
class QNetworkReply;
class QTimer;

/**
 * Authorize class for onet
 */
class OnetAuth : public QObject
{
    Q_OBJECT
public:
    OnetAuth();
    virtual ~OnetAuth();

public slots:
    void authorize(QString _strNick, QString _strNickAuth, QString _strPass);

private:
    enum AuthType
    {
        AT_chat,
        AT_deploy,
        AT_kropka,
        AT_kropkaFull,
        AT_sk,
        AT_secureKropka,
        AT_secureLogin,
        AT_override,
        AT_checkCode,
        AT_uo,
        AT_refreshSk,
        AT_undefined
    };
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
    void gotDeploy(const QString &strData);
    void getKropka();
    void getKropkaFull();
    void getSk();
    void getSecureKropka();
    void getSecureLogin();
    void getOverride();
    void getUo();
    void showCaptchaDialog();
    void getCheckCode();

    QString getVersion(const QString &strData);
    void saveCookies();
    void requestFinished(const QString &strData);

private slots:
    void networkFinished(QNetworkReply *reply);
    void refreshSk();

};

#endif // ONET_AUTH_H
