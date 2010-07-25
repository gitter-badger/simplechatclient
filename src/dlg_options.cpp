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

#include "dlg_options.h"

DlgOptions::DlgOptions(QWidget *parent, QSettings *param1) : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    settings = param1;

// comboBox colors
    ui.comboBox->setIconSize(QSize(50,10));

    QStringList comboBoxColors;
// default
    comboBoxColors << "#FFFFFF";
// shades of red
    comboBoxColors << "#CD5C5C" << "#F08080" << "#FA8072" << "#E9967A" << "#FFA07A" << "#DC143C" << "#FF0000" << "#B22222" << "#8B0000";
// shades of pink
    comboBoxColors << "#FFC0CB" << "#FFB6C1" << "#FF69B4" << "#FF1493" << "#C71585" << "#DB7093";
// shades of orange
    comboBoxColors << "#FFA07A" << "#FF7F50" << "#FF6347" << "#FF4500" << "#FF8C00" << "#FFA500";
// shades of yellow
    comboBoxColors << "#FFD700" << "#FFFF00" << "#FFFFE0" << "#FFFACD" << "#FAFAD2" << "#FFEFD5" << "#FFE4B5" << "#FFDAB9" << "#EEE8AA" << "#F0E68C" << "#BDB76B";
// shades of purple
    comboBoxColors << "#E6E6FA" << "#D8BFD8" << "#DDA0DD" << "#EE82EE" << "#DA70D6" << "#FF00FF" << "#BA55D3" << "#9370DB" << "#8A2BE2" << "#9400D3" << "#9932CC" << "#8B008B" << "#800080" << "#4B0082" << "#6A5ACD" << "#483D8B" << "#7B68EE";
// shades of green
    comboBoxColors << "#ADFF2F" << "#7FFF00" << "#7CFC00" << "#00FF00" << "#32CD32" << "#98DB98" << "#90EE90" << "#00FA9A" << "#00FF7F" << "#3CB371" << "#2E8B57" << "#228B22" << "#008000" << "#006400" << "#9ACD32" << "#6B8E23" << "#808000" << "#556B2F" << "#66CDAA" << "#8FBC8F" << "#20B2AA" << "#008B8B" << "#008080";
// shades of blue
    comboBoxColors << "#00FFFF" << "#E0FFFF" << "#AFEEEE" << "#7FFFD4" << "#40E0D0" << "#48D1CC" << "#00CED1" << "#5F9EA0" << "#4682B4" << "#B0C4DE" << "#B0E0E6" << "#ADD8E6" << "#87CEEB" << "#87CEFA" << "#00BFFF" << "#1E90FF" << "#6495ED" << "#7B68EE" << "#4169E1" << "#0000FF" << "#0000CD" << "#00008B" << "#000080" << "#191970";
// shades of brown
    comboBoxColors << "#FFF8DC" << "#FFEBCD" << "#FFE4C4" << "#FFDEAD" << "#F5DEB3" << "#DEB887" << "#D2B48C" << "#BC8F8F" << "#F4A460" << "#DAA520" << "#B8860B" << "#CD853F" << "#D2691E" << "#8B4513" << "#A0522D" << "#A52A2A" << "#800000";
// shades of white
    comboBoxColors << "#FFFFFF" << "#FFFAFA" << "#F0FFF0" << "#F5FFFA" << "#F0FFFF" << "#F0F8FF" << "#F8F8FF" << "#F5F5F5" << "#FFF5EE" << "#F5F5DC" << "#FDF5E6" << "#FFFAF0" << "#FFFFF0" << "#FAEBD7" << "#FAF0E6" << "#FFF0F5" << "#FFE4E1";
// shades of gray
    comboBoxColors << "#DCDCDC" << "#D3D3D3" << "#C0C0C0" << "#A9A9A9" << "#808080" << "#696969" << "#778899" << "#708090" << "#2F4F4F" << "#000000";

    int iComboBoxColors = 0;
    foreach (QString strColor, comboBoxColors)
    {
        QPixmap pixmap(50,10);
        pixmap.fill(QColor(strColor));
        ui.comboBox->insertItem(iComboBoxColors, pixmap, strColor);
        iComboBoxColors++;
    }

// config
    Config *pConfig = new Config();
    QString strNick = pConfig->get_value("login-nick");
    QString strPass = pConfig->get_value("login-pass");
    QString strAutoBusy = pConfig->get_value("auto_busy");
    QString strDebugAll = pConfig->get_value("debug_all");
    QString strShowZuo = pConfig->get_value("show_zuo");
    QString strHideFormating = pConfig->get_value("hide_formating");
    QString strHideJoinPart = pConfig->get_value("hide_join_part");
    QString strDisableAvatars = pConfig->get_value("disable_avatars");
    QString strDisableLogs = pConfig->get_value("disable_logs");
    QString strDisableSounds = pConfig->get_value("disable_sounds");
    QString strStyle = pConfig->get_value("style");
    QString strBackgroundColor = pConfig->get_value("background_color");
    delete pConfig;

// decrypt pass
    if (strPass.isEmpty() == false)
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->decrypt(strNick, strPass);
        delete pCrypt;
    }

// set nick and pass
    ui.lineEditNick->setText(strNick);
    ui.lineEditPass->setText(strPass);

// set color combobox
    for (int i = 0; i < ui.comboBox->count(); i++)
    {
        if (ui.comboBox->itemText(i) == strBackgroundColor)
            ui.comboBox->setCurrentIndex(i);
    }

// rest
    if (strAutoBusy == "on")
        ui.checkBox_5->setChecked(true);
    else
        ui.checkBox_5->setChecked(false);

    if (strDebugAll == "on")
        ui.checkBox->setChecked(true);
    else
        ui.checkBox->setChecked(false);

    if (strShowZuo == "on")
        ui.checkBox_2->setChecked(true);
    else
        ui.checkBox_2->setChecked(false);

    if (strHideFormating == "on")
        ui.checkBox_3->setChecked(true);
    else
        ui.checkBox_3->setChecked(false);

    if (strHideJoinPart == "on")
        ui.checkBox_4->setChecked(true);
    else
        ui.checkBox_4->setChecked(false);

    if (strDisableAvatars == "on")
        ui.checkBox_6->setChecked(true);
    else
        ui.checkBox_6->setChecked(false);

    if (strDisableLogs == "on")
        ui.checkBox_7->setChecked(true);
    else
        ui.checkBox_7->setChecked(false);

    if (strDisableSounds == "on")
        ui.checkBox_8->setChecked(true);
    else
        ui.checkBox_8->setChecked(false);

    if (strStyle == "modern")
        ui.radioButton->setChecked(true);
    else
        ui.radioButton_2->setChecked(true);

    QObject::connect(ui.checkBox_5, SIGNAL(clicked()), this, SLOT(auto_busy()));
    QObject::connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(debug_all()));
    QObject::connect(ui.checkBox_2, SIGNAL(clicked()), this, SLOT(show_zuo()));
    QObject::connect(ui.checkBox_3, SIGNAL(clicked()), this, SLOT(hide_formating()));
    QObject::connect(ui.checkBox_4, SIGNAL(clicked()), this, SLOT(hide_join_part()));
    QObject::connect(ui.checkBox_6, SIGNAL(clicked()), this, SLOT(disable_avatars()));
    QObject::connect(ui.checkBox_7, SIGNAL(clicked()), this, SLOT(disable_logs()));
    QObject::connect(ui.checkBox_8, SIGNAL(clicked()), this, SLOT(disable_sounds()));
    QObject::connect(ui.radioButton, SIGNAL(clicked()), this, SLOT(set_modern_style()));
    QObject::connect(ui.radioButton_2, SIGNAL(clicked()), this, SLOT(set_classic_style()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgOptions::auto_busy()
{
    Config *pConfig = new Config();
    if (ui.checkBox_5->isChecked() == true)
    {
        pConfig->set_value("auto_busy", "on");
        settings->setValue("auto_busy", "on");
    }
    else
    {
        pConfig->set_value("auto_busy", "off");
        settings->setValue("auto_busy", "off");
    }
    delete pConfig;
}

void DlgOptions::debug_all()
{
    Config *pConfig = new Config();
    if (ui.checkBox->isChecked() == true)
    {
        pConfig->set_value("debug_all", "on");
        settings->setValue("debug_all", "on");
    }
    else
    {
        pConfig->set_value("debug_all", "off");
        settings->setValue("debug_all", "off");
    }
    delete pConfig;
}

void DlgOptions::show_zuo()
{
    Config *pConfig = new Config();
    if (ui.checkBox_2->isChecked() == true)
    {
        pConfig->set_value("show_zuo", "on");
        settings->setValue("show_zuo", "on");
    }
    else
    {
        pConfig->set_value("show_zuo", "off");
        settings->setValue("show_zuo", "off");
    }
    delete pConfig;
}

void DlgOptions::hide_formating()
{
    Config *pConfig = new Config();
    if (ui.checkBox_3->isChecked() == true)
    {
        pConfig->set_value("hide_formating", "on");
        settings->setValue("hide_formating", "on");
    }
    else
    {
        pConfig->set_value("hide_formating", "off");
        settings->setValue("hide_formating", "off");
    }
    delete pConfig;
}

void DlgOptions::hide_join_part()
{
    Config *pConfig = new Config();
    if (ui.checkBox_4->isChecked() == true)
    {
        pConfig->set_value("hide_join_part", "on");
        settings->setValue("hide_join_part", "on");
    }
    else
    {
        pConfig->set_value("hide_join_part", "off");
        settings->setValue("hide_join_part", "off");
    }
    delete pConfig;
}

void DlgOptions::disable_avatars()
{
    Config *pConfig = new Config();
    if (ui.checkBox_6->isChecked() == true)
    {
        pConfig->set_value("disable_avatars", "on");
        settings->setValue("disable_avatars", "on");
    }
    else
    {
        pConfig->set_value("disable_avatars", "off");
        settings->setValue("disable_avatars", "off");
    }
    delete pConfig;
}

void DlgOptions::disable_logs()
{
    Config *pConfig = new Config();
    if (ui.checkBox_7->isChecked() == true)
    {
        pConfig->set_value("disable_logs", "on");
        settings->setValue("disable_logs", "on");
    }
    else
    {
        pConfig->set_value("disable_logs", "off");
        settings->setValue("disable_logs", "off");
    }
    delete pConfig;
}

void DlgOptions::disable_sounds()
{
    Config *pConfig = new Config();
    if (ui.checkBox_8->isChecked() == true)
    {
        pConfig->set_value("disable_sounds", "on");
        settings->setValue("disable_sounds", "on");
    }
    else
    {
        pConfig->set_value("disable_sounds", "off");
        settings->setValue("disable_sounds", "off");
    }
    delete pConfig;
}

void DlgOptions::set_modern_style()
{
    Config *pConfig = new Config();
    if (ui.radioButton->isChecked() == true)
    {
        pConfig->set_value("style", "modern");
        settings->setValue("style", "modern");
    }
    delete pConfig;

    this->close(); // fix stay on top

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString(tr("Restart program to apply the changes.")));
    msgBox.exec();
}

void DlgOptions::set_classic_style()
{
    Config *pConfig = new Config();
    if (ui.radioButton_2->isChecked() == true)
    {
        pConfig->set_value("style", "classic");
        settings->setValue("style", "classic");
    }
    delete pConfig;

    this->close(); // fix stay on top

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString(tr("Restart program to apply the changes.")));
    msgBox.exec();
}

void DlgOptions::button_cancel()
{
    ui.lineEditNick->clear();
    ui.lineEditPass->clear();
    ui.comboBox->clear();
    ui.buttonBox->QObject::disconnect();
    ui.checkBox->QObject::disconnect();
    ui.checkBox_2->QObject::disconnect();
    ui.checkBox_3->QObject::disconnect();
    ui.checkBox_4->QObject::disconnect();
    ui.checkBox_5->QObject::disconnect();
    ui.checkBox_6->QObject::disconnect();
    ui.checkBox_7->QObject::disconnect();
    ui.checkBox_8->QObject::disconnect();
    ui.radioButton->QObject::disconnect();
    ui.radioButton_2->QObject::disconnect();
    this->close();
}

void DlgOptions::button_ok()
{
    QString strNick = ui.lineEditNick->text();
    QString strPass = ui.lineEditPass->text();
    QString strBackgroundColor = ui.comboBox->currentText();

    if (strPass.isEmpty() == false)
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->encrypt(strNick, strPass);
        delete pCrypt;
    }

    Config *pConfig = new Config();
    pConfig->set_value("login-nick", strNick);
    pConfig->set_value("login-pass", strPass);
    pConfig->set_value("background_color", strBackgroundColor);
    delete pConfig;

    settings->setValue("background_color", strBackgroundColor);

    ui.lineEditNick->clear();
    ui.lineEditPass->clear();
    ui.comboBox->clear();
    ui.buttonBox->QObject::disconnect();
    ui.checkBox->QObject::disconnect();
    ui.checkBox_2->QObject::disconnect();
    ui.checkBox_3->QObject::disconnect();
    ui.checkBox_4->QObject::disconnect();
    ui.checkBox_5->QObject::disconnect();
    ui.checkBox_6->QObject::disconnect();
    ui.checkBox_7->QObject::disconnect();
    ui.checkBox_8->QObject::disconnect();
    ui.radioButton->QObject::disconnect();
    ui.radioButton_2->QObject::disconnect();
    this->close();
}
