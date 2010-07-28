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
    setWindowTitle(tr("Options"));

    settings = param1;

// page basic
    ui.radioButton_nick_t->setText(tr("Unregistered nickname"));
    ui.radioButton_nick_s->setText(tr("Registered nickname"));
    ui.label_nick->setText(tr("Nick:"));
    ui.label_password->setText(tr("Password:"));
    ui.groupBox_skins->setTitle(tr("Skins"));
    ui.radioButton_modern_avatars->setText(tr("Modern"));
    ui.radioButton_modern_no_avatars->setText(tr("Modern without avatars"));
    ui.radioButton_classic->setText(tr("Classic"));
// page adv
    ui.tabWidget_adv->setTabText(0, tr("Advanced"));
    ui.tabWidget_adv->setTabText(1, tr("Skins"));
    ui.tabWidget_adv->setTabText(2, tr("Colors"));
    ui.checkBox_auto_busy->setText(tr("Busy mode after you log in to chat"));
    ui.checkBox_debug->setText(tr("Show messages from the server (debug)"));
    ui.checkBox_show_zuo->setText(tr("Show ZUO"));
    ui.checkBox_hide_formating->setText(tr("Disable formatting messages"));
    ui.checkBox_hide_join_part->setText(tr("Hide join/part"));
    ui.checkBox_disable_avatars->setText(tr("Disable avatars"));
    ui.checkBox_disable_logs->setText(tr("Disable logs"));
    ui.checkBox_disable_sounds->setText(tr("Disable sounds"));
    ui.radioButton_modern_style->setText(tr("Modern"));
    ui.radioButton_classic_style->setText(tr("Classic"));
    ui.label_background_color->setText(tr("Background color:"));

// options list
    QListWidgetItem *basicConfButton = new QListWidgetItem(ui.listWidget_options);
    basicConfButton->setIcon(QIcon(":/images/basic_conf.png"));
    basicConfButton->setText(tr("Basic"));
    basicConfButton->setTextAlignment(Qt::AlignHCenter);
    basicConfButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *advConfButton = new QListWidgetItem(ui.listWidget_options);
    advConfButton->setIcon(QIcon(":/images/adv_conf.png"));
    advConfButton->setText(tr("Advanced"));
    advConfButton->setTextAlignment(Qt::AlignHCenter);
    advConfButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui.listWidget_options->setCurrentRow(0);

// comboBox colors
    ui.comboBox_background_color->setIconSize(QSize(50,10));

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
        ui.comboBox_background_color->insertItem(iComboBoxColors, pixmap, strColor);
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

// set nick staly/tyldowy
    if (strPass.isEmpty() == true)
    {
        ui.radioButton_nick_t->setChecked(true);
        ui.radioButton_nick_s->setChecked(false);
        ui.label_password->hide();
        ui.lineEdit_password->hide();
    }
    else
    {
        ui.radioButton_nick_t->setChecked(false);
        ui.radioButton_nick_s->setChecked(true);
    }

// set nick and pass
    ui.lineEdit_nick->setText(strNick);
    ui.lineEdit_password->setText(strPass);

// set style
    if (strStyle == "modern")
    {
        if (strDisableAvatars == "off")
            ui.radioButton_modern_avatars->setChecked(true);
        else
            ui.radioButton_modern_no_avatars->setChecked(true);
    }
    else if (strStyle == "classic")
        ui.radioButton_classic->setChecked(true);

// auto busy
    if (strAutoBusy == "on")
        ui.checkBox_auto_busy->setChecked(true);
    else
        ui.checkBox_auto_busy->setChecked(false);

// debug
    if (strDebugAll == "on")
        ui.checkBox_debug->setChecked(true);
    else
        ui.checkBox_debug->setChecked(false);

// show zuo
    if (strShowZuo == "on")
        ui.checkBox_show_zuo->setChecked(true);
    else
        ui.checkBox_show_zuo->setChecked(false);

// hide formating
    if (strHideFormating == "on")
        ui.checkBox_hide_formating->setChecked(true);
    else
        ui.checkBox_hide_formating->setChecked(false);

// hide join part
    if (strHideJoinPart == "on")
        ui.checkBox_hide_join_part->setChecked(true);
    else
        ui.checkBox_hide_join_part->setChecked(false);

// disable avatars
    if (strDisableAvatars == "on")
        ui.checkBox_disable_avatars->setChecked(true);
    else
        ui.checkBox_disable_avatars->setChecked(false);

// disable logs
    if (strDisableLogs == "on")
        ui.checkBox_disable_logs->setChecked(true);
    else
        ui.checkBox_disable_logs->setChecked(false);

// disable sounds
    if (strDisableSounds == "on")
        ui.checkBox_disable_sounds->setChecked(true);
    else
        ui.checkBox_disable_sounds->setChecked(false);

// style
    if (strStyle == "modern")
        ui.radioButton_modern_style->setChecked(true);
    else if (strStyle == "classic")
        ui.radioButton_classic_style->setChecked(true);

// set color combobox
    for (int i = 0; i < ui.comboBox_background_color->count(); i++)
    {
        if (ui.comboBox_background_color->itemText(i) == strBackgroundColor)
            ui.comboBox_background_color->setCurrentIndex(i);
    }

    QObject::connect(ui.listWidget_options, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(change_page(QListWidgetItem*,QListWidgetItem*)));
    QObject::connect(ui.radioButton_nick_t, SIGNAL(clicked()), this, SLOT(hide_pass()));
    QObject::connect(ui.radioButton_nick_s, SIGNAL(clicked()), this, SLOT(show_pass()));
    QObject::connect(ui.radioButton_modern_avatars, SIGNAL(clicked()), this, SLOT(set_modern_style_avatars()));
    QObject::connect(ui.radioButton_modern_no_avatars, SIGNAL(clicked()), this, SLOT(set_modern_style_no_avatars()));
    QObject::connect(ui.radioButton_classic, SIGNAL(clicked()), this, SLOT(set_classic_style()));
    QObject::connect(ui.checkBox_auto_busy, SIGNAL(clicked()), this, SLOT(auto_busy()));
    QObject::connect(ui.checkBox_debug, SIGNAL(clicked()), this, SLOT(debug_all()));
    QObject::connect(ui.checkBox_show_zuo, SIGNAL(clicked()), this, SLOT(show_zuo()));
    QObject::connect(ui.checkBox_hide_formating, SIGNAL(clicked()), this, SLOT(hide_formating()));
    QObject::connect(ui.checkBox_hide_join_part, SIGNAL(clicked()), this, SLOT(hide_join_part()));
    QObject::connect(ui.checkBox_disable_avatars, SIGNAL(clicked()), this, SLOT(disable_avatars()));
    QObject::connect(ui.checkBox_disable_logs, SIGNAL(clicked()), this, SLOT(disable_logs()));
    QObject::connect(ui.checkBox_disable_sounds, SIGNAL(clicked()), this, SLOT(disable_sounds()));
    QObject::connect(ui.radioButton_modern_style, SIGNAL(clicked()), this, SLOT(set_modern_style()));
    QObject::connect(ui.radioButton_classic_style, SIGNAL(clicked()), this, SLOT(set_classic_style()));
    QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(button_ok()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(button_cancel()));
}

void DlgOptions::change_page(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui.stackedWidget->setCurrentIndex(ui.listWidget_options->row(current));
}

void DlgOptions::hide_pass()
{
    ui.label_password->hide();
    ui.lineEdit_password->hide();
}

void DlgOptions::show_pass()
{
    ui.label_password->show();
    ui.lineEdit_password->show();
}

void DlgOptions::set_modern_style_avatars()
{
// save settings
    save_settings();

// save style
    Config *pConfig = new Config();
    pConfig->set_value("style", "modern");
    settings->setValue("style", "modern");
    pConfig->set_value("disable_avatars", "off");
    settings->setValue("disable_avatars", "off");
    delete pConfig;

    this->close(); // fix stay on top

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString(tr("Restart program to apply the changes.")));
    msgBox.exec();
}

void DlgOptions::set_modern_style_no_avatars()
{
// save settings
    save_settings();

// save style
    Config *pConfig = new Config();
    pConfig->set_value("style", "modern");
    settings->setValue("style", "modern");
    pConfig->set_value("disable_avatars", "on");
    settings->setValue("disable_avatars", "on");
    delete pConfig;

    this->close(); // fix stay on top

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/images/logo_64.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setText(QString(tr("Restart program to apply the changes.")));
    msgBox.exec();
}

void DlgOptions::auto_busy()
{
    Config *pConfig = new Config();
    if (ui.checkBox_auto_busy->isChecked() == true)
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
    if (ui.checkBox_debug->isChecked() == true)
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
    if (ui.checkBox_show_zuo->isChecked() == true)
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
    if (ui.checkBox_hide_formating->isChecked() == true)
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
    if (ui.checkBox_hide_join_part->isChecked() == true)
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
    if (ui.checkBox_disable_avatars->isChecked() == true)
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
    if (ui.checkBox_disable_logs->isChecked() == true)
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
    if (ui.checkBox_disable_sounds->isChecked() == true)
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
// save settings
    save_settings();

// save style
    Config *pConfig = new Config();
    pConfig->set_value("style", "modern");
    settings->setValue("style", "modern");
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
// save settings
    save_settings();

// save style
    Config *pConfig = new Config();
    pConfig->set_value("style", "classic");
    settings->setValue("style", "classic");
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
    ui.radioButton_nick_t->QObject::disconnect();
    ui.radioButton_nick_s->QObject::disconnect();
    ui.lineEdit_nick->clear();
    ui.lineEdit_password->clear();
    ui.radioButton_modern_avatars->QObject::disconnect();
    ui.radioButton_modern_no_avatars->QObject::disconnect();
    ui.radioButton_classic->QObject::disconnect();
    ui.checkBox_auto_busy->QObject::disconnect();
    ui.checkBox_debug->QObject::disconnect();
    ui.checkBox_show_zuo->QObject::disconnect();
    ui.checkBox_hide_formating->QObject::disconnect();
    ui.checkBox_hide_join_part->QObject::disconnect();
    ui.checkBox_disable_avatars->QObject::disconnect();
    ui.checkBox_disable_logs->QObject::disconnect();
    ui.checkBox_disable_sounds->QObject::disconnect();
    ui.radioButton_modern_style->QObject::disconnect();
    ui.radioButton_classic_style->QObject::disconnect();
    ui.comboBox_background_color->clear();
    ui.buttonBox->QObject::disconnect();
    this->close();
}

void DlgOptions::button_ok()
{
// save
    save_settings();

// clear
    ui.radioButton_nick_t->QObject::disconnect();
    ui.radioButton_nick_s->QObject::disconnect();
    ui.lineEdit_nick->clear();
    ui.lineEdit_password->clear();
    ui.radioButton_modern_avatars->QObject::disconnect();
    ui.radioButton_modern_no_avatars->QObject::disconnect();
    ui.radioButton_classic->QObject::disconnect();
    ui.checkBox_auto_busy->QObject::disconnect();
    ui.checkBox_debug->QObject::disconnect();
    ui.checkBox_show_zuo->QObject::disconnect();
    ui.checkBox_hide_formating->QObject::disconnect();
    ui.checkBox_hide_join_part->QObject::disconnect();
    ui.checkBox_disable_avatars->QObject::disconnect();
    ui.checkBox_disable_logs->QObject::disconnect();
    ui.checkBox_disable_sounds->QObject::disconnect();
    ui.radioButton_modern_style->QObject::disconnect();
    ui.radioButton_classic_style->QObject::disconnect();
    ui.comboBox_background_color->clear();
    ui.buttonBox->QObject::disconnect();
    this->close();
}

void DlgOptions::save_settings()
{
// get values
    QString strNick = ui.lineEdit_nick->text();
    QString strBackgroundColor = ui.comboBox_background_color->currentText();

// check nick
    QString strPass;
    if (ui.radioButton_nick_t->isChecked() == true)
        strPass = "";
    else if (ui.radioButton_nick_s->isChecked() == true)
        strPass = ui.lineEdit_password->text();

// encrypt pass
    if (strPass.isEmpty() == false)
    {
        Crypt *pCrypt = new Crypt();
        strPass = pCrypt->encrypt(strNick, strPass);
        delete pCrypt;
    }

// save values
    Config *pConfig = new Config();
    pConfig->set_value("login-nick", strNick);
    pConfig->set_value("login-pass", strPass);
    pConfig->set_value("background_color", strBackgroundColor);
    delete pConfig;

// save to settings
    settings->setValue("background_color", strBackgroundColor);
}
