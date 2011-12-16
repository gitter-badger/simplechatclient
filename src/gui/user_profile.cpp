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

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QVBoxLayout>

#include <QDate>
#include <QDesktopWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include "convert.h"
#include "core.h"
#include "user_avatar.h"
#include "user_profile.h"

DlgUserProfile::DlgUserProfile(QWidget *parent, QString _strNick) : QDialog(parent), strNick(_strNick)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Profile"));

    createGui();
    createSignals();

    // max height
    setMinimumSize(285, sizeHint().height());
    setMaximumSize(285, sizeHint().height());

    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    accessManager = new QNetworkAccessManager;
    QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(avatarFinished(QNetworkReply*)));

    // set nick
    Core::instance()->strUserProfile = strNick;
    Core::instance()->mUserProfile.clear();
    Core::instance()->bUserProfile = false;

    //set label
    label_nick->setText(strNick);

    // get new user info
    Core::instance()->pNetwork->send(QString("NS INFO %1").arg(strNick));

    // refresh
    QTimer::singleShot(200, this, SLOT(refreshUserInfo())); // 0.2 sec
}

DlgUserProfile::~DlgUserProfile()
{
    accessManager->deleteLater();
}

void DlgUserProfile::createGui()
{
    toolButton_zoom = new QToolButton();
    toolLayout = new QVBoxLayout();
    toolLayout->setMargin(0);
    toolLayout->addStretch();
    toolLayout->addWidget(toolButton_zoom);
    toolWidget = new QWidget(this);
    toolWidget->setLayout(toolLayout);

    label_avatar = new QLabel();
    label_avatar->setMinimumSize(50,50);
    label_avatar->setWordWrap(true);
    label_avatar->setAlignment(Qt::AlignCenter);
    textEdit_desc = new QTextEdit();
    textEdit_desc->setMinimumSize(190,70);
    textEdit_desc->setMaximumSize(190,70);
    textEdit_desc->setReadOnly(true);
    topLayout = new QHBoxLayout();
    topLayout->setMargin(0);
    topLayout->addWidget(toolWidget);
    topLayout->addWidget(label_avatar);
    topLayout->addWidget(textEdit_desc);
    topWidget = new QWidget(this);
    topWidget->setLayout(topLayout);

    label_age = new QLabel();
    lineEdit_age = new QLineEdit();
    lineEdit_age->setAlignment(Qt::AlignCenter);
    lineEdit_age->setReadOnly(true);
    label_age->setBuddy(label_age);
    label_sex = new QLabel();
    lineEdit_sex = new QLineEdit();
    lineEdit_sex->setAlignment(Qt::AlignCenter);
    lineEdit_sex->setReadOnly(true);
    label_sex->setBuddy(label_sex);
    agesexLayout = new QHBoxLayout();
    agesexLayout->setMargin(0);
    agesexLayout->addWidget(label_age);
    agesexLayout->addWidget(lineEdit_age);
    agesexLayout->addWidget(label_sex);
    agesexLayout->addWidget(lineEdit_sex);
    agesexWidget = new QWidget(this);
    agesexWidget->setLayout(agesexLayout);

    lineEdit_birthdate = new QLineEdit();
    lineEdit_birthdate->setAlignment(Qt::AlignCenter);
    lineEdit_birthdate->setReadOnly(true);
    lineEdit_city = new QLineEdit();
    lineEdit_city->setAlignment(Qt::AlignCenter);
    lineEdit_city->setReadOnly(true);
    lineEdit_country = new QLineEdit();
    lineEdit_country->setAlignment(Qt::AlignCenter);
    lineEdit_country->setReadOnly(true);
    plainTextEdit_hobby = new QPlainTextEdit();
    plainTextEdit_hobby->setReadOnly(true);
    plainTextEdit_hobby->setMaximumHeight(50);
    plainTextEdit_hobby->setMaximumHeight(50);
    lineEdit_type = new QLineEdit();
    lineEdit_type->setAlignment(Qt::AlignCenter);
    lineEdit_type->setReadOnly(true);
    label_website_link = new QLabel();
    label_website_link->setOpenExternalLinks(true);
    moreLayout = new QFormLayout();
    moreLayout->setMargin(0);
    moreLayout->addRow(tr("Birthdate:"), lineEdit_birthdate);
    moreLayout->addRow(tr("City:"), lineEdit_city);
    moreLayout->addRow(tr("Country:"), lineEdit_country);
    moreLayout->addRow(tr("Hobby:"), plainTextEdit_hobby);
    moreLayout->addRow(tr("Type:"), lineEdit_type);
    moreLayout->addRow(tr("Website:"), label_website_link);
    moreWidget = new QWidget(this);
    moreWidget->setLayout(moreLayout);

    pushButton_more = new QPushButton();
    pushButton_close = new QPushButton();
    pushButtonLayout = new QHBoxLayout();
    pushButtonLayout->setMargin(0);
    pushButtonLayout->addWidget(pushButton_more);
    pushButtonLayout->addStretch();
    pushButtonLayout->addWidget(pushButton_close);
    pushButtonWidget = new QWidget(this);
    pushButtonWidget->setLayout(pushButtonLayout);

    label_nick = new QLabel();
    label_nick->setAlignment(Qt::AlignCenter);
    label_nick->setStyleSheet("font-weight:bold;");
    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(5);
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    mainLayout->addWidget(label_nick);
    mainLayout->addWidget(topWidget);
    mainLayout->addWidget(agesexWidget);
    mainLayout->addWidget(moreWidget);
    mainLayout->addWidget(pushButtonWidget);
    this->setLayout(mainLayout);

    toolButton_zoom->setIcon(QIcon(":/images/oxygen/16x16/zoom-in.png"));
    pushButton_more->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    pushButton_close->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    label_sex->setText(tr("Sex:"));
    label_age->setText(tr("Age:"));
    pushButton_more->setText(tr("More..."));
    pushButton_close->setText(tr("Close"));

    moreWidget->setVisible(false);
}

void DlgUserProfile::createSignals()
{
    QObject::connect(toolButton_zoom, SIGNAL(clicked()), this, SLOT(buttonZoom()));
    QObject::connect(pushButton_more, SIGNAL(clicked()), this, SLOT(buttonMore()));
    QObject::connect(pushButton_close, SIGNAL(clicked()), this, SLOT(buttonClose()));
}

void DlgUserProfile::refreshUserInfo()
{
    if (Core::instance()->bUserProfile == false)
    {
        QTimer::singleShot(200, this, SLOT(refreshUserInfo())); // 0.2 sec
        return;
    }

    QMapIterator<QString, QString> i(Core::instance()->mUserProfile);
    while (i.hasNext())
    {
        i.next();
        QString strKey = i.key();
        QString strValue = i.value();

        if (strKey == "avatar")
        {
            if (!strValue.isEmpty())
                showAvatar(strValue);
            else
                label_avatar->setText(tr("No photo available"));
        }
        else if (strKey == "birthdate")
        {
            lineEdit_age->setText(convertAge(strValue));
            lineEdit_birthdate->setText(strValue);
        }
        else if (strKey == "city")
            lineEdit_city->setText(strValue);
        else if (strKey == "country")
            lineEdit_country->setText(convertCodeToCountry(strValue));
        else if (strKey == "longDesc")
            plainTextEdit_hobby->setPlainText(strValue);
        else if (strKey == "sex")
            lineEdit_sex->setText(convertSex(strValue));
        else if (strKey == "shortDesc")
            textEdit_desc->setHtml(convertDesc(strValue));
        else if (strKey == "type")
            lineEdit_type->setText(convertType(strValue));
        else if (strKey == "www")
        {
            QString strShortLink = strValue;
            if (strShortLink.size() > 30) strShortLink = strShortLink.left(15)+"..."+strShortLink.right(15);

            label_website_link->setText(QString("<a href=\"%1\">%2</a>").arg(strValue).arg(strShortLink));
        }
    }
}

void DlgUserProfile::avatarFinished(QNetworkReply *pReply)
{
    pReply->deleteLater();

    // if errors
    if (pReply->error())
        return;

    QByteArray bData = pReply->readAll();

    // show avatar
    if (!bData.isEmpty())
    {
        // display
        avatar.loadFromData(bData);
        label_avatar->setPixmap(avatar.scaled(QSize(50,50)));

        // enable zoom
        toolButton_zoom->setEnabled(true);
    }
}

void DlgUserProfile::buttonZoom()
{
    DlgUserAvatar(Core::instance()->sccWindow(), avatar).exec();
}

void DlgUserProfile::buttonMore()
{
    if (moreWidget->isVisible())
    {
        moreWidget->setVisible(false);
        pushButton_more->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
        pushButton_more->setText(tr("More..."));
    }
    else
    {
        moreWidget->setVisible(true);
        pushButton_more->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
        pushButton_more->setText(tr("Less..."));
    }

    setMinimumSize(285, sizeHint().height());
    setMaximumSize(285, sizeHint().height());
}

void DlgUserProfile::buttonClose()
{
    Core::instance()->strUserProfile.clear();
    Core::instance()->mUserProfile.clear();
    Core::instance()->bUserProfile = false;

    close();
}

QString DlgUserProfile::convertDesc(QString strContent)
{
    // replace
    strContent.replace("&", "&amp;");
    strContent.replace("<", "&lt;");
    strContent.replace(">", "&gt;");

    // convert
    Convert *convertText = new Convert();
    convertText->convertText(strContent);
    delete convertText;

    return QString("<html><body style=\"background-color:white;font-size:12px;font-family:Verdana;\">%1</body></html>").arg(strContent);
}

QString DlgUserProfile::convertSex(QString strSex)
{
    if (strSex == "F")
        strSex = tr("Female");
    else if (strSex == "M")
        strSex = tr("Male");

    return strSex;
}

QString DlgUserProfile::convertAge(QString strDate)
{
    if (strDate.isEmpty()) return QString::null; // empty date
    QStringList lDate = strDate.split("-");

    QString strYear = lDate.at(0);
    QString strMonth = lDate.at(1);
    QString strDay = lDate.at(2);

    QDate dDate = QDate::currentDate();
    QString strCurrentYear = QString::number(dDate.year());
    QString strCurrentMonth = QString::number(dDate.month());
    QString strCurrentDay = QString::number(dDate.day());

    int iAge = strCurrentYear.toInt() - strYear.toInt();

    if (strCurrentMonth.toInt() < strMonth.toInt())
        iAge--;
    else if ((strCurrentMonth.toInt() == strMonth.toInt()) && (strCurrentDay.toInt() < strDay.toInt()))
        iAge--;

    return QString::number(iAge);
}

QString DlgUserProfile::convertCodeToCountry(QString strCountryCode)
{
    QStringList lCodes =  (QStringList() <<
       "AF" << "AL" << "DZ" << "AD" << "AO" << "AI" << "AQ" << "AG" << "AN" <<
       "SA" << "AR" << "AM" << "AW" << "AU" << "AT" << "AZ" << "BS" << "BH" << "BD" <<
       "BB" << "BE" << "BZ" << "BJ" << "BM" << "BT" << "BY" << "BO" << "BA" << "BW" <<
       "BR" << "BN" << "IO" << "VG" << "BG" << "BF" << "BI" << "CL" << "CN" << "HR" <<
       "CY" << "TD" << "CZ" << "DK" << "DM" << "DO" << "DJ" << "EG" << "EC" << "ER" <<
       "EE" << "ET" << "FK" << "RU" << "FL" << "PH" << "FI" << "FR" << "GM" << "GA" <<
       "GS" << "GH" << "GI" << "GR" << "GD" << "GL" << "GE" << "GU" << "GY" << "GF" <<
       "GP" << "GT" << "GN" << "GW" << "GQ" << "HT" << "HM" << "ES" << "NL" << "HN" <<
       "HK" << "IN" << "ID" << "IQ" << "IR" << "IE" << "IS" << "IL" << "JM" << "JP" <<
       "YE" << "JO" << "YU" << "KY" << "KH" << "CM" << "CA" << "QU" << "KZ" << "KE" <<
       "KG" << "KI" << "CO" << "KM" << "CG" << "KR" << "KP" << "CR" << "CU" << "KW" <<
       "LA" << "LS" << "LB" << "LR" << "LY" << "LI" << "LT" << "LU" << "LV" << "MK" <<
       "MG" << "YT" << "MO" << "MW" << "MV" << "MY" << "ML" << "MT" << "MP" << "MA" <<
       "MQ" << "MR" << "MU" << "MX" << "FM" << "MD" << "MC" << "MN" << "MS" << "MZ" <<
       "MM" << "NA" << "NR" << "NP" << "DE" << "NE" << "NG" << "NI" << "NU" << "NO" <<
       "NC" << "NZ" << "OM" << "PK" << "PW" << "PA" << "PG" << "PY" << "PE" << "PN" <<
       "PF" << "PL" << "PR" << "PT" << "ZA" << "CV" << "RE" << "RW" << "RO" << "EH" <<
       "KN" << "LC" << "VC" << "PM" << "SV" << "WS" << "AS" << "SM" << "SN" << "SC" <<
       "SL" << "SG" << "SK" << "SI" << "SO" << "LK" << "US" << "SZ" << "SD" << "SR" <<
       "SJ" << "SY" << "CH" << "SE" << "TJ" << "TH" << "TW" << "TZ" << "TP" << "TG" <<
       "TK" << "TO" << "TT" << "TN" << "TR" << "TM" << "TC" << "TV" << "UG" << "UA" <<
       "UY" << "UZ" << "VU" << "WF" << "VA" << "VE" << "HU" << "GB" << "UK" << "VN" <<
       "IT" << "CI" << "BV" << "CX" << "NF" << "SH" << "CK" << "VI" << "CC" << "MH" <<
       "FO" << "SB" << "ST" << "ZR" << "ZM" << "ZW" << "AE");

    QString strCountries = tr("Afghanistan,Albania,Algeria,Andorra,Angola,Anguilla,Antarctica,Antigua and Barbuda,Netherlands Antilles,Saudi Arabia,Argentina,Armenia,Aruba,Australia,Austria,Azerbaijan,Bahamas,Bahrain,Bangladesh,Barbados,Belgium,Belize,Benin,Bermuda,Bhutan,Belarus,Bolivia,Bosnia and Herzegovina,Botswana,Brazil,Brunei,British Indian Ocean Territory,British Virgin Islands,Bulgaria,Burkina Faso,Burundi,Chile,China,Croatia,Cyprus,Chad,Czech Republic,Denmark,Dominica,Dominican Republic,Djibouti,Egypt,Ecuador,Eritrea,Estonia,Ethiopia,Falkland Islands,Russian Federation,Fiji,Philippines,Finland,France,Gambia,Gaon,South Georgia and South Sandwich Islands,Ghana,Gibraltar,Greece,Grenada,Greenland,Georgia,Guam,Guyana,French Guiana,Guadeloupe,Guatemala,Guinea,Guinea Bissau,Equatorial Guinea,Haiti,Heard,Spain,Holland,Honduras,Hong Kong,India,Indonesia,Iraq,Iran,Ireland,Iceland,Israel,Jamaica,Japan,Yemen,Jordan,Yugoslavia,Cayman Islands,Cambodia,Cameroon,Canada,Qatar,Kazakhstan,Kenya,Kyrgyzstan,Kiribati,Colombia,Comoros,Congo,South Korea,North Korea,Costa Rica,Cuba,Kuwait,Laos,Lesotho,Lebanon,Liberia,Libya,Liechtenstein,Lithuania,Latvia,Luxembourg,Macedonia,Madagascar,Mayotte,Macau,Malawi,Maldives,Malaysia,Mali,Malta,Marshall Islands,Morocco,Martinique,Mauritania,Mauritius,Mexico,Micronesia,Moldova,Monaco,Mongolia,Montserrat,Mozambique,Myanmar,Namibia,Nauru,Nepal,Germany,Niger,Nigeria,Nicaragua,Niue,Norway,New Caledonia,New Zealand,Oman,Pakistan,Palau,Panama,Papua New Guinea,Paraguay,Peru,Pitcairn,French Polynesia,Poland,Puerto Rico,Portugal,South Africa,Cape Verde,Reunion,Rwanda,Romania,Western Sahara,Saint Christopher and Nevis,Saint Lucia,Saint Vincent and the Grenadines,Saint-Perr and Miquelon,Salvador,Samoa,American Samoa,San Marino,Senegal,Seychelles,Sierra Leone,Singapore,Slovakia,Slovenia,Somalia,Sri Lanka,United States,Swaziland,Sudan,Suriname,Svalbard and Jan Mayen island,Syria,Switzerland,Sweden,Tajikistan,Thailand,Taiwan,Tanzania,Timor-Leste,Togo,Tokelau,Tonga,Trinidad and Tobago,Tunisia,Turkey,Turkmenistan,Turks and Caicos Islands,Tuvalu,Uganda,Ukraine,Uruguay,Uzbekistan,Vanuatu,Wallis and Futuna Islands,Vatican City,Venezuela,Hungary,Great Britain,United Kingdom,Vietnam,Italy,Ivory Coast,Bouvet Island,Christmas Island,Norfolk Island,Saint Helena,Cook Islands,U.S. Virgin Islands,Cocos Islands,Marshall Islands,Faroe Islands,Solomon Islands,Sao Tome and Principe,Zaire,Zambia,Zimbabwe,United Arab Emirates");
    QStringList lCountries = strCountries.split(",");

    // replace if found
    for (int i = 0; i < lCodes.size(); i++)
    {
        if (strCountryCode == lCodes.at(i))
        {
            QString strCountry = lCountries.at(i);
            return strCountry;
        }
    }

    return QString::null;
}

QString DlgUserProfile::convertType(QString strType)
{
    if (strType == "0")
        strType = tr("Novice");
    else if (strType == "1")
        strType = tr("Beginner");
    else if (strType == "2")
        strType = tr("Master");
    else if (strType == "3")
        strType = tr("Guru");

    return strType;
}

void DlgUserProfile::showAvatar(QString strUrl)
{
    if (!strUrl.contains(",")) return; // wrong url

    // change url
    QStringList lUrl = strUrl.split(",");
    lUrl[1] = "10"; // 10 = zoom; 0 = get real image
    //lUrl[3] = "0.jpg"; // 0 = get real image
    strUrl = lUrl.join(",");

    // get url
    accessManager->get(QNetworkRequest(QUrl(strUrl)));
}
