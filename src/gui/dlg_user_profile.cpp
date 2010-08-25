/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr Łuczko <piotr.luczko@gmail.com>               *
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

#include "dlg_user_profile.h"

DlgUserProfile::DlgUserProfile(QWidget *parent, Network *param1, QSettings *param2, sNickInfo param3) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Profile"));

    pNetwork = param1;
    settings = param2;
    sCurrentNickInfo = param3;

    ui.label_sex->setText(tr("Sex:"));
    ui.label_birthdate->setText(tr("Birthdate:"));
    ui.label_city->setText(tr("City:"));
    ui.label_country->setText(tr("Country:"));
    ui.label_shortDesc->setText(tr("ShortDesc:"));
    ui.label_type->setText(tr("Type:"));
    ui.label_email->setText(tr("Email:"));
    ui.label_www->setText(tr("Website:"));
}

void DlgUserProfile::show_info()
{
    ui.label_avatar->setText("");
    ui.label_nick->setText(QString("<p style=\"font-weight:bold;\"> %1</p>").arg(sCurrentNickInfo.nick));
    ui.plainTextEdit_longDesc->setPlainText(sCurrentNickInfo.longDesc);
    ui.lineEdit_sex->setText(convert_sex(sCurrentNickInfo.sex));
    ui.lineEdit_birthdate->setText(sCurrentNickInfo.birthdate);
    ui.lineEdit_city->setText(sCurrentNickInfo.city);
    ui.lineEdit_country->setText(convert_country(sCurrentNickInfo.country));
    ui.plainTextEdit_shortDesc->setPlainText(sCurrentNickInfo.shortDesc);
    ui.lineEdit_type->setText(convert_type(sCurrentNickInfo.type));
    ui.lineEdit_email->setText(sCurrentNickInfo.email);
    ui.lineEdit_www->setText(sCurrentNickInfo.www);

    if (sCurrentNickInfo.avatar.isEmpty() == false)
        show_avatar(sCurrentNickInfo.avatar);
}

QString DlgUserProfile::convert_sex(QString strSex)
{
    if (strSex == "F")
        strSex = tr("Female");
    else if (strSex == "M")
        strSex = tr("Male");

    return strSex;
}

QString DlgUserProfile::convert_country(QString strCountry)
{
    QStringList slCodes =  (QStringList() <<
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

    QString strCountries = tr("Afghanistan,Albania,Algeria,Andorra,Angola,Anguilla,Antarctica,Antigua and Barbuda,Netherlands Antilles,Saudi Arabia,Argentina,Armenia,Aruba,Australia,Austria,Azerbaijan,Bahamas,Bahrain,Bangladesh,Barbados,Belgium,Belize,Benin,Bermuda,Bhutan,Belarus,Bolivia,Bosnia and Herzegovina,Botswana,Brazil,Brunei,British Indian Ocean Territory,British Virgin Islands,Bulgaria,Burkina Faso,Burundi,Chile,China,Croatia,Cyprus,Chad,Czech Republic,Denmark,Dominica,Dominican Republic,Djibouti,Egypt,Ecuador,Eritrea,Estonia,Ethiopia,Falkland Islands,Russian Federation,Fiji,Philippines,Finland,France,Gambia,Gaon,South Georgia and South Sandwich Islands,Ghana,Gibraltar,Greece,Grenada,Greenland,Georgia,Guam ,Guyana,French Guiana,Guadeloupe,Guatemala,Guinea,Guinea Bissau,Equatorial Guinea,Haiti,Heard,Spain,Holland,Honduras,Hong Kong,India,Indonesia,Iraq,Iran,Ireland,Iceland,Israel,Jamaica,Japan,Yemen,Jordan,Yugoslavia,Cayman Islands,Cambodia,Cameroon,Canada,Qatar,Kazakhstan,Kenya,Kyrgyzstan,Kiribati,Colombia,Comoros,Congo,South Korea,North Korea,Costa Rica,Cuba,Kuwait,Laos,Lesotho,Lebanon,Liberia,Libya,Liechtenstein,Lithuania,Latvia,Luxembourg,Macedonia,Madagascar,Mayotte,Macau,Malawi,Maldives,Malaysia,Mali,Malta,Marshall Islands,Morocco,Martinique,Mauritania,Mauritius,Mexico,Micronesia,Moldova,Monaco,Mongolia,Montserrat,Mozambique ,Myanmar,Namibia,Nauru,Nepal,Germany,Niger,Nigeria,Nicaragua,Niue,Norway,New Caledonia,New Zealand,Oman,Pakistan,Palau,Panama,Papua New Guinea,Paraguay,Peru,Pitcairn,French Polynesia,Poland ,Puerto Rico,Portugal,South Africa,Cape Verde,Reunion,Rwanda,Romania,Western Sahara,Saint Christopher and Nevis,Saint Lucia,Saint Vincent and the Grenadines,Saint-Perr and Miquelon,Salvador,Samoa,American Samoa,San Marino ,Senegal,Seychelles,Sierra Leone,Singapore,Slovakia,Slovenia,Somalia,Sri Lanka,United States,Swaziland,Sudan,Suriname,Svalbard and Jan Mayen island,Syria,Switzerland,Sweden,Tajikistan,Thailand,Taiwan,Tanzania,Timor-Leste ,Togo,Tokelau,Tonga,Trinidad and Tobago,Tunisia,Turkey,Turkmenistan,Turks and Caicos Islands,Tuvalu,Uganda,Ukraine,Uruguay,Uzbekistan,Vanuatu,Wallis and Futuna Islands,Vatican City,Venezuela,Hungary,Great Britain,United Kingdom,Vietnam ,Italy,Ivory Coast,Bouvet Island,Christmas Island,Norfolk Island,Saint Helena,Cook Islands,U.S. Virgin Islands,Cocos Islands,Marshall Islands,Faroe Islands,Solomon Islands,Sao Tome and Principe,Zaire,Zambia ,Zimbabwe,United Arab Emirates");
    QStringList slCountries = strCountries.split(",");

    // replace if found
    for (int i = 0; i < slCodes.count(); i++)
    {
        if (strCountry == slCodes.at(i))
        {
            strCountry = slCountries.at(i);
            return strCountry;
        }
    }

    return strCountry;
}

QString DlgUserProfile::convert_type(QString strType)
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

void DlgUserProfile::show_avatar(QString strUrl)
{
    if (strUrl.indexOf(",") == -1) return; // wrong url

    // change url
    QStringList lUrl = strUrl.split(",");
    lUrl[1] = "3";
    strUrl = lUrl.join(",");

    // get url
    QNetworkAccessManager accessManager;
    QNetworkReply *pReply;
    QEventLoop eventLoop;
    pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
    QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QByteArray bData = pReply->readAll();

    // show avatar
    QPixmap pixmap;
    pixmap.loadFromData(bData);
    ui.label_avatar->setPixmap(pixmap);
}

void DlgUserProfile::button_close()
{
    this->close();
}

void DlgUserProfile::showEvent(QShowEvent *event)
{
    event->accept();

    show_info();

    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
}
