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

#include <QDate>
#include <QDesktopWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include "core.h"
#include "convert.h"
#include "my_profile.h"

DlgMyProfile::DlgMyProfile(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("My profile"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    createGui();
    setDefaultValues();
    createSignals();

    accessManager = new QNetworkAccessManager;
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(avatarFinished(QNetworkReply*)));

    refresh();
}

DlgMyProfile::~DlgMyProfile()
{
    accessManager->deleteLater();
}

void DlgMyProfile::createGui()
{
    ui.toolButton_bold->setIcon(QIcon(":/images/oxygen/16x16/format-text-bold.png"));
    ui.toolButton_italic->setIcon(QIcon(":/images/oxygen/16x16/format-text-italic.png"));
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));

    ui.label_ltype->setText(tr("Type:"));
    ui.label_sex->setText(tr("Sex:"));
    ui.label_birthdate->setText(tr("Birthdate:"));
    ui.label_city->setText(tr("City:"));
    ui.label_country->setText(tr("Country:"));
    ui.label_hobby->setText(tr("Hobby:"));
    ui.label_www->setText(tr("Website:"));
}

void DlgMyProfile::setDefaultValues()
{
    // font
    QStringList comboBoxFont;
    comboBoxFont << "Arial" << "Times" << "Verdana" << "Tahoma" << "Courier";

    foreach (QString strFont, comboBoxFont)
        ui.comboBox_font->addItem(strFont);

    // color
    ui.comboBox_color->setToolTip(tr("Font color"));
    ui.comboBox_color->setIconSize(QSize(50,10));

    QStringList comboBoxColors;
    comboBoxColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    foreach (QString strColor, comboBoxColors)
    {
        QPixmap pixmap(50,10);
        pixmap.fill(QColor(strColor));
        ui.comboBox_color->addItem(QIcon(pixmap), "");
    }

    // sex
    QStringList lSex;
    lSex << "" << tr("Male") << tr("Female");

    for (int i = 0; i < lSex.size(); i++)
        ui.comboBox_sex->addItem(lSex.at(i));

    // days
    QStringList lDays;
    lDays << "";
    for (int i = 1; i <= 31; i++)
        lDays << (i < 10 ? "0"+QString::number(i) : QString::number(i));

    for (int i = 0; i < lDays.size(); i++)
        ui.comboBox_day->addItem(lDays.at(i));

    // months
    QStringList lMonths;
    lMonths << "" << tr("January") << tr("February") << tr("March") << tr("April") << tr("May") << tr("June") << tr("July") << tr("August") << tr("September") << tr("October") << tr("November") << tr("December");
    for (int i = 0; i < lMonths.size(); i++)
        ui.comboBox_month->addItem(lMonths.at(i));

    // years
    QStringList lYears;
    lYears << "";
    for (int i = 1900; i <= QDate::currentDate().year()-5; i++)
        lYears << QString::number(i);

    for (int i = 0; i < lYears.size(); i++)
        ui.comboBox_year->addItem(lYears.at(i));

    // country
    QString strCountries = tr("Afghanistan,Albania,Algeria,Andorra,Angola,Anguilla,Antarctica,Antigua and Barbuda,Netherlands Antilles,Saudi Arabia,Argentina,Armenia,Aruba,Australia,Austria,Azerbaijan,Bahamas,Bahrain,Bangladesh,Barbados,Belgium,Belize,Benin,Bermuda,Bhutan,Belarus,Bolivia,Bosnia and Herzegovina,Botswana,Brazil,Brunei,British Indian Ocean Territory,British Virgin Islands,Bulgaria,Burkina Faso,Burundi,Chile,China,Croatia,Cyprus,Chad,Czech Republic,Denmark,Dominica,Dominican Republic,Djibouti,Egypt,Ecuador,Eritrea,Estonia,Ethiopia,Falkland Islands,Russian Federation,Fiji,Philippines,Finland,France,Gambia,Gaon,South Georgia and South Sandwich Islands,Ghana,Gibraltar,Greece,Grenada,Greenland,Georgia,Guam,Guyana,French Guiana,Guadeloupe,Guatemala,Guinea,Guinea Bissau,Equatorial Guinea,Haiti,Heard,Spain,Holland,Honduras,Hong Kong,India,Indonesia,Iraq,Iran,Ireland,Iceland,Israel,Jamaica,Japan,Yemen,Jordan,Yugoslavia,Cayman Islands,Cambodia,Cameroon,Canada,Qatar,Kazakhstan,Kenya,Kyrgyzstan,Kiribati,Colombia,Comoros,Congo,South Korea,North Korea,Costa Rica,Cuba,Kuwait,Laos,Lesotho,Lebanon,Liberia,Libya,Liechtenstein,Lithuania,Latvia,Luxembourg,Macedonia,Madagascar,Mayotte,Macau,Malawi,Maldives,Malaysia,Mali,Malta,Marshall Islands,Morocco,Martinique,Mauritania,Mauritius,Mexico,Micronesia,Moldova,Monaco,Mongolia,Montserrat,Mozambique,Myanmar,Namibia,Nauru,Nepal,Germany,Niger,Nigeria,Nicaragua,Niue,Norway,New Caledonia,New Zealand,Oman,Pakistan,Palau,Panama,Papua New Guinea,Paraguay,Peru,Pitcairn,French Polynesia,Poland,Puerto Rico,Portugal,South Africa,Cape Verde,Reunion,Rwanda,Romania,Western Sahara,Saint Christopher and Nevis,Saint Lucia,Saint Vincent and the Grenadines,Saint-Perr and Miquelon,Salvador,Samoa,American Samoa,San Marino,Senegal,Seychelles,Sierra Leone,Singapore,Slovakia,Slovenia,Somalia,Sri Lanka,United States,Swaziland,Sudan,Suriname,Svalbard and Jan Mayen island,Syria,Switzerland,Sweden,Tajikistan,Thailand,Taiwan,Tanzania,Timor-Leste,Togo,Tokelau,Tonga,Trinidad and Tobago,Tunisia,Turkey,Turkmenistan,Turks and Caicos Islands,Tuvalu,Uganda,Ukraine,Uruguay,Uzbekistan,Vanuatu,Wallis and Futuna Islands,Vatican City,Venezuela,Hungary,Great Britain,United Kingdom,Vietnam,Italy,Ivory Coast,Bouvet Island,Christmas Island,Norfolk Island,Saint Helena,Cook Islands,U.S. Virgin Islands,Cocos Islands,Marshall Islands,Faroe Islands,Solomon Islands,Sao Tome and Principe,Zaire,Zambia,Zimbabwe,United Arab Emirates");
    QStringList lCountries = strCountries.split(",");
    lCountries.insert(0, "");
    for (int i = 0; i < lCountries.size(); i++)
        ui.comboBox_country->addItem(lCountries.at(i));
}

void DlgMyProfile::createSignals()
{
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(buttonOk()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgMyProfile::avatarFinished(QNetworkReply *pReply)
{
    pReply->deleteLater();

    // if errors
    if (pReply->error())
        return;

    QByteArray bData = pReply->readAll();

    // show avatar
    if (!bData.isEmpty())
    {
        QPixmap avatar;
        avatar.loadFromData(bData);
        ui.label_avatar->setPixmap(avatar.scaled(QSize(50,50)));
    }
}

void DlgMyProfile::refresh()
{
    QHashIterator <QString, QString> i(Core::instance()->mMyProfile);
    while (i.hasNext())
    {
        i.next();
        QString strKey = i.key();
        QString strValue = i.value();

        if (strKey == "avatar")
        {
            if (!strValue.isEmpty())
                accessManager->get(QNetworkRequest(QUrl(strValue)));
            else
                ui.label_avatar->setText(tr("No photo available"));
        }
        else if (strKey == "birthdate")
        {
            if (!strValue.isEmpty())
            {
                QStringList lDate = strValue.split("-");
                QString strYear = lDate.at(0);
                QString strMonth = lDate.at(1);
                QString strDay = lDate.at(2);

                strMonth = convertIntToMonth(strMonth);

                ui.comboBox_day->setCurrentIndex(getIndex(ui.comboBox_day, strDay));
                ui.comboBox_month->setCurrentIndex(getIndex(ui.comboBox_month, strMonth));
                ui.comboBox_year->setCurrentIndex(getIndex(ui.comboBox_year, strYear));
            }
            else
            {
                ui.comboBox_day->setCurrentIndex(0);
                ui.comboBox_month->setCurrentIndex(0);
                ui.comboBox_year->setCurrentIndex(0);
            }
        }
        else if (strKey == "city")
            ui.lineEdit_city->setText(strValue);
        else if (strKey == "country")
            ui.comboBox_country->setCurrentIndex(getIndex(ui.comboBox_country, convertCodeToCountry(strValue)));
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
        else if (strKey == "shortDesc")
            ui.plainTextEdit_desc->setPlainText(convertTextToDesc(strValue));
        else if (strKey == "type")
            ui.label_type->setText(convertType(strValue));
        else if (strKey == "www")
            ui.lineEdit_www->setText(strValue);
    }
}

int DlgMyProfile::getIndex(QComboBox *comboBox, const QString &strFindText)
{
    return comboBox->findText(strFindText);
}

QString DlgMyProfile::convertTextToDesc(QString strContent)
{
    // convert font
    bool bBold = Convert::isBold(strContent);
    bool bItalic = Convert::isItalic(strContent);
    QString strFont = Convert::getFont(strContent);
    int iColor = Convert::getColor(strContent);

    if (bBold) ui.toolButton_bold->setChecked(true);
    else if (bItalic) ui.toolButton_italic->setChecked(true);

    if (strFont == "arial") ui.comboBox_font->setCurrentIndex(0);
    else if (strFont == "times") ui.comboBox_font->setCurrentIndex(1);
    else if (strFont == "verdana") ui.comboBox_font->setCurrentIndex(2);
    else if (strFont == "tahoma") ui.comboBox_font->setCurrentIndex(3);
    else if (strFont == "courier") ui.comboBox_font->setCurrentIndex(4);
    else ui.comboBox_font->setCurrentIndex(2);

    ui.comboBox_color->setCurrentIndex(iColor);

    // default #000000
    if (ui.comboBox_color->currentIndex() == -1)
        ui.comboBox_color->setCurrentIndex(0);

    // default verdana
    if (ui.comboBox_font->currentIndex() == -1)
        ui.comboBox_font->setCurrentIndex(2);

    // convert emoticons
    Convert::simpleConvert(strContent);

    return strContent;
}

QString DlgMyProfile::convertDescToText(QString strContent)
{
    strContent.remove(QRegExp("(\r|\n)"));
    Convert::simpleReverseConvert(strContent);

    bool bBold = false;
    bool bItalic = false;
    QString strFontName;
    QString strFontColor;
    QString strFontWeight;

    // bold
    if (ui.toolButton_bold->isChecked()) bBold = true;

    // italic
    if (ui.toolButton_italic->isChecked()) bItalic = true;

    // font name
    strFontName = ui.comboBox_font->currentText().toLower();

    // font color
    QStringList lFontColors;
    lFontColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    if (ui.comboBox_color->currentIndex() != -1)
        strFontColor = lFontColors.at(ui.comboBox_color->currentIndex());

    // set topic
    if (bBold) strFontWeight += "b";
    if (bItalic) strFontWeight += "i";

    if (strFontName == "verdana")
        strFontName = QString::null;
    if ((strFontColor != "#000000") && (!strFontColor.isEmpty()))
        strContent = "%C"+strFontColor.right(6)+"%"+strContent;
    if (!strFontName.isEmpty())
        strFontName = ":"+strFontName;
    if ((!strFontWeight.isEmpty()) || (!strFontName.isEmpty()))
        strContent = "%F"+strFontWeight+strFontName+"%"+strContent;

    return strContent;
}

QString DlgMyProfile::convertCodeToCountry(const QString &strCountryCode)
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
            return lCountries.at(i);
    }

    return QString::null;
}

QString DlgMyProfile::convertCountryToCode(const QString &strCountry)
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
    for (int i = 0; i < lCountries.size(); i++)
    {
        if (strCountry == lCountries.at(i))
            return lCodes.at(i);
    }

    return QString::null;
}

QString DlgMyProfile::convertIntToMonth(const QString &strConvertMonth)
{
    QStringList lMonths;
    lMonths << "" << tr("January") << tr("February") << tr("March") << tr("April") << tr("May") << tr("June") << tr("July") << tr("August") << tr("September") << tr("October") << tr("November") << tr("December");
    for (int i = 0; i < lMonths.size(); i++)
    {
        QString strMonth = (i < 10 ? "0"+QString::number(i) : QString::number(i));
        if (strMonth == strConvertMonth)
            return lMonths.at(i);
    }

    return QString::null;
}

QString DlgMyProfile::convertMonthToInt(const QString &strMonth)
{
    QStringList lMonths;
    lMonths << "" << tr("January") << tr("February") << tr("March") << tr("April") << tr("May") << tr("June") << tr("July") << tr("August") << tr("September") << tr("October") << tr("November") << tr("December");
    for (int i = 0; i < lMonths.size(); i++)
    {
        QString strInt = (i < 10 ? "0"+QString::number(i) : QString::number(i));
        if (strInt == "00") strInt = ""; // fix 0 month
        if (strMonth == lMonths.at(i))
            return strInt;
    }

    return QString::null;
}

QString DlgMyProfile::convertType(const QString &strType)
{
    if (strType == "0")
        return tr("Novice");
    else if (strType == "1")
        return tr("Beginner");
    else if (strType == "2")
        return tr("Master");
    else if (strType == "3")
        return tr("Guru");
    else
        return strType;
}

void DlgMyProfile::buttonOk()
{
    // set desc - shortDesc
    QString strDesc = convertDescToText(ui.plainTextEdit_desc->toPlainText());
    if (strDesc != Core::instance()->mMyProfile.value("shortDesc"))
        Core::instance()->pNetwork->send(QString("NS SET shortDesc %1").arg(strDesc));

    // set sex
    int iSex = ui.comboBox_sex->currentIndex();
    QString strSexChar;
    if (iSex == 1) strSexChar = "M";
    else if (iSex == 2) strSexChar = "F";
    if (strSexChar != Core::instance()->mMyProfile.value("sex"))
        Core::instance()->pNetwork->send(QString("NS SET sex %1").arg(strSexChar));

    // set birthdate
    QString strDay = ui.comboBox_day->currentText();
    QString strMonth = convertMonthToInt(ui.comboBox_month->currentText());
    QString strYear = ui.comboBox_year->currentText();
    QString strBirthdate;
    if ((!strDay.isEmpty()) && (!strMonth.isEmpty()) && (!strYear.isEmpty()))
        strBirthdate = QString("%1-%2-%3").arg(strYear, strMonth, strDay);
    if (strBirthdate != Core::instance()->mMyProfile.value("birthdate"))
        Core::instance()->pNetwork->send(QString("NS SET birthdate %1").arg(strBirthdate));

    // set city
    QString strCity = ui.lineEdit_city->text();
    if (strCity != Core::instance()->mMyProfile.value("city"))
        Core::instance()->pNetwork->send(QString("NS SET city %1").arg(strCity));

    // set country
    QString strCountry = ui.comboBox_country->currentText();
    QString strCountryCode = convertCountryToCode(strCountry);
    if (strCountryCode != Core::instance()->mMyProfile.value("country"))
        Core::instance()->pNetwork->send(QString("NS SET country %1").arg(strCountryCode));

    // set hobby - longDesc
    QString strHobby = ui.plainTextEdit_hobby->toPlainText();
    if (strHobby != Core::instance()->mMyProfile.value("longDesc"))
        Core::instance()->pNetwork->send(QString("NS SET longDesc %1").arg(strHobby));

    // set www
    QString strWWW = ui.lineEdit_www->text();
    if (strWWW != Core::instance()->mMyProfile.value("www"))
        Core::instance()->pNetwork->send(QString("NS SET www %1").arg(strWWW));

    // close
    this->close();
}
