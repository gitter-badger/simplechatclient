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

DlgUserProfile::DlgUserProfile(QWidget *parent, Network *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Profile"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    myparent = parent;
    pNetwork = param1;

    // set width
    iWidth = ui.verticalLayout->sizeHint().width();

    ui.toolButton_zoom->setIcon(QIcon(":/images/oxygen/16x16/page-zoom.png"));
    ui.pushButton_more->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    ui.label_sex->setText(tr("Sex:"));
    ui.label_age->setText(tr("Age:"));
    ui.label_birthdate->setText(tr("Birthdate:"));
    ui.label_city->setText(tr("City:"));
    ui.label_country->setText(tr("Country:"));
    ui.label_hobby->setText(tr("Hobby:"));
    ui.label_type->setText(tr("Type:"));
    ui.label_www->setText(tr("Website:"));
    ui.pushButton_more->setText(tr("More..."));

    QObject::connect(ui.toolButton_zoom, SIGNAL(clicked()), this, SLOT(button_zoom()));
    QObject::connect(ui.pushButton_more, SIGNAL(clicked()), this, SLOT(button_more()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_close()));
}

void DlgUserProfile::set_nick(QString n)
{
    strNick = n;

    // clear
    clear_info();

    //set label
    ui.label_nick->setText(QString("<b>%1</b>").arg(strNick));

    // get new user info
    pNetwork->send(QString("NS INFO %1").arg(strNick));
}

void DlgUserProfile::set_user_info(QString strCheckNick, QString strKey, QString strValue)
{
    if (strCheckNick != strNick) return; // incorrect nick

    if (strKey == "avatar")
    {
        if (strValue.isEmpty() == false)
            show_avatar(strValue);
    }
    else if (strKey == "birthdate")
    {
        ui.lineEdit_age->setText(convert_age(strValue));
        ui.lineEdit_birthdate->setText(strValue);
    }
    else if (strKey == "city")
        ui.lineEdit_city->setText(strValue);
    else if (strKey == "country")
        ui.lineEdit_country->setText(convert_country(strValue));
    else if (strKey == "longDesc")
        ui.plainTextEdit_hobby->setPlainText(strValue);
    else if (strKey == "sex")
        ui.lineEdit_sex->setText(convert_sex(strValue));
    else if (strKey == "shortDesc")
        ui.textEdit_desc->setHtml(convert_desc(strValue));
    else if (strKey == "type")
        ui.lineEdit_type->setText(convert_type(strValue));
    else if (strKey == "www")
        ui.lineEdit_www->setText(strValue);
}

void DlgUserProfile::avatar_finished()
{
    QByteArray bData = pReply->readAll();
    pReply->QObject::disconnect();
    pReply->deleteLater();

    // show avatar
    if (bData.isEmpty() == false)
    {
        // display
        avatar.loadFromData(bData);
        ui.label_avatar->setPixmap(avatar.scaled(QSize(50,50)));

        // enable zoom
        ui.toolButton_zoom->setEnabled(true);
    }
}

void DlgUserProfile::button_zoom()
{
    (new DlgUserAvatar(myparent, avatar))->show();
}

void DlgUserProfile::button_more()
{
    if (ui.widget_more->isHidden() == true)
    {
        ui.widget_more->show();
        ui.pushButton_more->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
        ui.pushButton_more->setText(tr("Less..."));

        int iHeight = ui.verticalLayout->sizeHint().height();
        setMinimumHeight(iHeight);
        setMaximumHeight(iHeight);
        setMinimumWidth(iWidth);
        setMaximumWidth(iWidth);
        resize(iWidth, iHeight);
    }
    else
    {
        ui.widget_more->hide();
        ui.pushButton_more->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
        ui.pushButton_more->setText(tr("More..."));

        int iHeight = ui.verticalLayout->sizeHint().height() - ui.formLayout_more->sizeHint().height();
        setMinimumHeight(iHeight);
        setMaximumHeight(iHeight);
        setMinimumWidth(iWidth);
        setMaximumWidth(iWidth);
        resize(iWidth, iHeight);
    }
}

void DlgUserProfile::clear_info()
{
    ui.label_avatar->clear();
    ui.toolButton_zoom->setEnabled(false);
    ui.label_nick->clear();
    ui.textEdit_desc->clear();
    ui.lineEdit_sex->clear();
    ui.lineEdit_age->clear();
    ui.lineEdit_birthdate->clear();
    ui.lineEdit_city->clear();
    ui.lineEdit_country->clear();
    ui.plainTextEdit_hobby->clear();
    ui.lineEdit_type->clear();
    ui.lineEdit_www->clear();
}

QString DlgUserProfile::convert_desc(QString strContent)
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

QString DlgUserProfile::convert_sex(QString strSex)
{
    if (strSex == "F")
        strSex = tr("Female");
    else if (strSex == "M")
        strSex = tr("Male");

    return strSex;
}

QString DlgUserProfile::convert_age(QString strDate)
{
    if (strDate.isEmpty() == true) return QString::null; // empty date

    strDate.replace(QRegExp("(\\d+)-(\\d+)-(\\d+)"), "\\1");
    QDate dDate = QDate::currentDate();
    QString strCurrentYear = QString::number(dDate.year());

    int iYear = strDate.toInt();
    int iCurrentYear = strCurrentYear.toInt();
    int iAge = iCurrentYear - iYear;

    return QString::number(iAge);
}

QString DlgUserProfile::convert_country(QString strCountry)
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
        if (strCountry == strlCodes.at(i))
        {
            strCountry = strlCountries.at(i);
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
    lUrl[1] = "10";
    strUrl = lUrl.join(",");

    // get url
    pReply = accessManager.get(QNetworkRequest(QUrl(strUrl)));
    QObject::connect(pReply, SIGNAL(finished()), this, SLOT(avatar_finished()));
}

void DlgUserProfile::button_close()
{
    this->hide();
}

void DlgUserProfile::showEvent(QShowEvent *event)
{
    event->accept();
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    // is hidden?
    if (ui.widget_more->isHidden() == false)
    {
        // change text
        ui.widget_more->hide();
        ui.pushButton_more->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
        ui.pushButton_more->setText(tr("More..."));

        // resize
        int iHeight = ui.verticalLayout->sizeHint().height() - ui.formLayout_more->sizeHint().height();
        setMinimumHeight(iHeight);
        setMaximumHeight(iHeight);
        setMinimumWidth(iWidth);
        setMaximumWidth(iWidth);
        resize(iWidth, iHeight);
    }
}

void DlgUserProfile::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
