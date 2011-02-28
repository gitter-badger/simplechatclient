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

#include <QDate>
#include <QDesktopWidget>
#include <QPushButton>
#include <QSettings>
#include "convert.h"
#include "network.h"
#include "dlg_my_profile.h"

DlgMyProfile::DlgMyProfile(QWidget *parent, Network *param1, QMap <QString, QByteArray> *param2, QMap <QString, QString> *param3) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowTitle(tr("My profile"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    myparent = parent;
    pNetwork = param1;
    mNickAvatar = param2;
    mMyProfile = param3;

    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.label_sex->setText(tr("Sex:"));
    ui.label_birthdate->setText(tr("Birthdate:"));
    ui.label_city->setText(tr("City:"));
    ui.label_country->setText(tr("Country:"));
    ui.label_hobby->setText(tr("Hobby:"));
    ui.label_www->setText(tr("Website:"));

    // sex
    QStringList strlSex;
    strlSex << "" << tr("Male") << tr("Female");

    for (int i = 0; i < strlSex.count(); i++)
        ui.comboBox_sex->insertItem(i, strlSex.at(i));

    // days
    QStringList strlDays;
    strlDays << "";
    for (int i = 1; i <= 31; i++)
        strlDays << (i < 10 ? "0"+QString::number(i) : QString::number(i));

    for (int i = 0; i < strlDays.count(); i++)
        ui.comboBox_day->insertItem(i, strlDays.at(i));

    // months
    QStringList strlMonths;
    strlMonths << "" << tr("January") << tr("February") << tr("March") << tr("April") << tr("May") << tr("June") << tr("July") << tr("August") << tr("September") << tr("October") << tr("November") << tr("December");
    for (int i = 0; i < strlMonths.count(); i++)
        ui.comboBox_month->insertItem(i, strlMonths.at(i));

    // years
    QStringList strlYears;
    strlYears << "";
    for (int i = 1900; i <= QDate::currentDate().year()-5; i++)
        strlYears << QString::number(i);

    for (int i = 0; i < strlYears.count(); i++)
        ui.comboBox_year->insertItem(i, strlYears.at(i));

    // country
    QString strCountries = tr("Afghanistan,Albania,Algeria,Andorra,Angola,Anguilla,Antarctica,Antigua and Barbuda,Netherlands Antilles,Saudi Arabia,Argentina,Armenia,Aruba,Australia,Austria,Azerbaijan,Bahamas,Bahrain,Bangladesh,Barbados,Belgium,Belize,Benin,Bermuda,Bhutan,Belarus,Bolivia,Bosnia and Herzegovina,Botswana,Brazil,Brunei,British Indian Ocean Territory,British Virgin Islands,Bulgaria,Burkina Faso,Burundi,Chile,China,Croatia,Cyprus,Chad,Czech Republic,Denmark,Dominica,Dominican Republic,Djibouti,Egypt,Ecuador,Eritrea,Estonia,Ethiopia,Falkland Islands,Russian Federation,Fiji,Philippines,Finland,France,Gambia,Gaon,South Georgia and South Sandwich Islands,Ghana,Gibraltar,Greece,Grenada,Greenland,Georgia,Guam,Guyana,French Guiana,Guadeloupe,Guatemala,Guinea,Guinea Bissau,Equatorial Guinea,Haiti,Heard,Spain,Holland,Honduras,Hong Kong,India,Indonesia,Iraq,Iran,Ireland,Iceland,Israel,Jamaica,Japan,Yemen,Jordan,Yugoslavia,Cayman Islands,Cambodia,Cameroon,Canada,Qatar,Kazakhstan,Kenya,Kyrgyzstan,Kiribati,Colombia,Comoros,Congo,South Korea,North Korea,Costa Rica,Cuba,Kuwait,Laos,Lesotho,Lebanon,Liberia,Libya,Liechtenstein,Lithuania,Latvia,Luxembourg,Macedonia,Madagascar,Mayotte,Macau,Malawi,Maldives,Malaysia,Mali,Malta,Marshall Islands,Morocco,Martinique,Mauritania,Mauritius,Mexico,Micronesia,Moldova,Monaco,Mongolia,Montserrat,Mozambique,Myanmar,Namibia,Nauru,Nepal,Germany,Niger,Nigeria,Nicaragua,Niue,Norway,New Caledonia,New Zealand,Oman,Pakistan,Palau,Panama,Papua New Guinea,Paraguay,Peru,Pitcairn,French Polynesia,Poland,Puerto Rico,Portugal,South Africa,Cape Verde,Reunion,Rwanda,Romania,Western Sahara,Saint Christopher and Nevis,Saint Lucia,Saint Vincent and the Grenadines,Saint-Perr and Miquelon,Salvador,Samoa,American Samoa,San Marino,Senegal,Seychelles,Sierra Leone,Singapore,Slovakia,Slovenia,Somalia,Sri Lanka,United States,Swaziland,Sudan,Suriname,Svalbard and Jan Mayen island,Syria,Switzerland,Sweden,Tajikistan,Thailand,Taiwan,Tanzania,Timor-Leste,Togo,Tokelau,Tonga,Trinidad and Tobago,Tunisia,Turkey,Turkmenistan,Turks and Caicos Islands,Tuvalu,Uganda,Ukraine,Uruguay,Uzbekistan,Vanuatu,Wallis and Futuna Islands,Vatican City,Venezuela,Hungary,Great Britain,United Kingdom,Vietnam,Italy,Ivory Coast,Bouvet Island,Christmas Island,Norfolk Island,Saint Helena,Cook Islands,U.S. Virgin Islands,Cocos Islands,Marshall Islands,Faroe Islands,Solomon Islands,Sao Tome and Principe,Zaire,Zambia,Zimbabwe,United Arab Emirates");
    QStringList strlCountries = strCountries.split(",");
    strlCountries.insert(0, "");
    for (int i = 0; i < strlCountries.count(); i++)
        ui.comboBox_country->insertItem(i, strlCountries.at(i));

    // signals
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));

    refresh();
}

void DlgMyProfile::refresh()
{
    QSettings settings;
    QString strMe = settings.value("nick").toString();

    QMap<QString, QString>::const_iterator i = mMyProfile->constBegin();
    while (i != mMyProfile->constEnd())
    {
        QString strKey = i.key();
        QString strValue = i.value();

        if (strKey == "avatar")
        {
            if ((strValue.isEmpty() == false) && (mNickAvatar->contains(strMe)))
            {
                QPixmap avatar;
                avatar.loadFromData(mNickAvatar->value(strMe));
                ui.label_avatar->setPixmap(avatar.scaled(QSize(50,50)));
            }
            else
                ui.label_avatar->setText(tr("No photo available"));
        }
        else if (strKey == "birthdate")
        {
            if (strValue.isEmpty() == false)
            {
                QStringList strlDate = strValue.split("-");
                QString strYear = strlDate.at(0);
                QString strMonth = strlDate.at(1);
                QString strDay = strlDate.at(2);

                strMonth = convert_int_to_month(strMonth);

                ui.comboBox_day->setCurrentIndex(get_index(ui.comboBox_day, strDay));
                ui.comboBox_month->setCurrentIndex(get_index(ui.comboBox_month, strMonth));
                ui.comboBox_year->setCurrentIndex(get_index(ui.comboBox_year, strYear));
            }
            else
            {
                ui.comboBox_day->setCurrentIndex(0);
                ui.comboBox_month->setCurrentIndex(0);
                ui.comboBox_year->setCurrentIndex(0);
            }
        }
        else if (strKey == "shortDesc")
            ui.textEdit_desc->setHtml(convert_desc(strValue));
        else if (strKey == "city")
            ui.lineEdit_city->setText(strValue);
        else if (strKey == "country")
        {
            QString strCountry = convert_code_to_country(strValue);
            ui.comboBox_country->setCurrentIndex(get_index(ui.comboBox_country, strCountry));
        }
        else if (strKey == "longDesc")
            ui.plainTextEdit_hobby->setPlainText(strValue);
        else if (strKey == "sex")
        {
            QString strSex = strValue;
            if (strSex == "")
                ui.comboBox_sex->setCurrentIndex(0);
            else if (strSex == "M")
                ui.comboBox_sex->setCurrentIndex(1);
            else if (strSex == "F")
                ui.comboBox_sex->setCurrentIndex(2);
        }
        else if (strKey == "www")
            ui.lineEdit_www->setText(strValue);

        ++i;
    }
}

int DlgMyProfile::get_index(QComboBox *comboBox, QString strCheck)
{
    for (int i = 0; i < comboBox->count(); i++)
    {
        if (comboBox->itemText(i) == strCheck)
            return i;
    }
    return 0;
}

QString DlgMyProfile::convert_desc(QString strContent)
{
    QString strContentStart = "<html><body style=\"background-color:white;font-size:12px;font-family:Verdana;\">";
    QString strContentEnd = "</body></html>";
    QString strContentLast;

    // replace
    strContent.replace("&", "&amp;");
    strContent.replace("<", "&lt;");
    strContent.replace(">", "&gt;");

    // convert
    Convert *convertText = new Convert();
    convertText->convert_text(&strContent, &strContentLast);
    delete convertText;

#ifdef Q_WS_X11
    strContent.replace("file://", "");
#endif

    // return
    strContent = strContent+strContentLast;
    return (strContentStart+strContent+strContentEnd);
}

QString DlgMyProfile::convert_code_to_country(QString strCountryCode)
{
    QStringList strlCodes =  (QStringList() <<
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
    QStringList strlCountries = strCountries.split(",");

    // replace if found
    for (int i = 0; i < strlCodes.count(); i++)
    {
        if (strCountryCode == strlCodes.at(i))
        {
            QString strCountry = strlCountries.at(i);
            return strCountry;
        }
    }

    return "";
}

QString DlgMyProfile::convert_country_to_code(QString strCountry)
{
    QStringList strlCodes =  (QStringList() <<
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
    QStringList strlCountries = strCountries.split(",");

    // replace if found
    for (int i = 0; i < strlCountries.count(); i++)
    {
        if (strCountry == strlCountries.at(i))
        {
            QString strCountryCode = strlCodes.at(i);
            return strCountryCode;
        }
    }

    return "";
}

QString DlgMyProfile::convert_int_to_month(QString strCheckMonth)
{
    QStringList strlMonths;
    strlMonths << "" << tr("January") << tr("February") << tr("March") << tr("April") << tr("May") << tr("June") << tr("July") << tr("August") << tr("September") << tr("October") << tr("November") << tr("December");
    for (int i = 0; i < strlMonths.count(); i++)
    {
        QString strMonth = (i < 10 ? "0"+QString::number(i) : QString::number(i));
        if (strMonth == strCheckMonth)
            return strlMonths.at(i);
    }

    return "";
}

QString DlgMyProfile::convert_month_to_int(QString strMonth)
{
    QStringList strlMonths;
    strlMonths << "" << tr("January") << tr("February") << tr("March") << tr("April") << tr("May") << tr("June") << tr("July") << tr("August") << tr("September") << tr("October") << tr("November") << tr("December");
    for (int i = 0; i < strlMonths.count(); i++)
    {
        QString strInt = (i < 10 ? "0"+QString::number(i) : QString::number(i));
        if (strInt == "00") strInt = ""; // fix 0 month
        if (strMonth == strlMonths.at(i))
            return strInt;
    }

    return "";
}

void DlgMyProfile::button_ok()
{
    // set sex
    int iSex = ui.comboBox_sex->currentIndex();
    QString strSexChar;
    if (iSex == 1) strSexChar = "M";
    else if (iSex == 2) strSexChar = "F";
    if (strSexChar != mMyProfile->value("sex"))
        pNetwork->send(QString("NS SET sex %1").arg(strSexChar));

    // set birthdate
    QString strDay = ui.comboBox_day->currentText();
    QString strMonth = convert_month_to_int(ui.comboBox_month->currentText());
    QString strYear = ui.comboBox_year->currentText();
    QString strBirthdate;
    if ((strDay.isEmpty() == false) && (strMonth.isEmpty() == false) && (strYear.isEmpty() == false))
        strBirthdate = QString("%1-%2-%3").arg(strYear).arg(strMonth).arg(strDay);
    if (strBirthdate != mMyProfile->value("birthdate"))
        pNetwork->send(QString("NS SET birthdate %1").arg(strBirthdate));

    // set city
    QString strCity = ui.lineEdit_city->text();
    if (strCity != mMyProfile->value("city"))
        pNetwork->send(QString("NS SET city %1").arg(strCity));

    // set country
    QString strCountry = ui.comboBox_country->currentText();
    QString strCountryCode = convert_country_to_code(strCountry);
    if (strCountryCode != mMyProfile->value("country"))
        pNetwork->send(QString("NS SET country %1").arg(strCountryCode));

    // set hobby - longDesc
    QString strHobby = ui.plainTextEdit_hobby->toPlainText();
    if (strHobby != mMyProfile->value("longDesc"))
        pNetwork->send(QString("NS SET longDesc %1").arg(strHobby));

    // set www
    QString strWWW = ui.lineEdit_www->text();
    if (strWWW != mMyProfile->value("www"))
        pNetwork->send(QString("NS SET www %1").arg(strWWW));

    // close
    this->close();
}

void DlgMyProfile::button_close()
{
    this->close();
}
