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

#include <QColorDialog>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QProcess>
#include <QShowEvent>
#include "config.h"
#include "core.h"
#include "profile_manager.h"
#include "notify.h"
#include "options.h"

#ifdef Q_WS_WIN
    #include <QDesktopServices>
#endif

DlgOptions::DlgOptions(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Options"));
    // center screen
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    createGui();
    refreshProfilesList();
    setDefaultValues();
    createSignals();
}

void DlgOptions::createGui()
{
    ui.pushButton_profiles->setIcon(QIcon(":/images/oxygen/16x16/preferences-activities.png"));
    ui.pushButton_mainwindow_restore_default->setIcon(QIcon(":/images/oxygen/16x16/edit-undo.png"));
    ui.pushButton_nicklist_restore_default->setIcon(QIcon(":/images/oxygen/16x16/edit-undo.png"));
    ui.pushButton_play_beep->setIcon(QIcon(":/images/oxygen/16x16/media-playback-start.png"));
    ui.pushButton_play_query->setIcon(QIcon(":/images/oxygen/16x16/media-playback-start.png"));
    ui.pushButton_sound_beep_change->setIcon(QIcon(":/images/oxygen/16x16/document-edit.png"));
    ui.pushButton_sound_query_change->setIcon(QIcon(":/images/oxygen/16x16/document-edit.png"));
    ui.pushButton_logs_open_folder->setIcon(QIcon(":/images/oxygen/16x16/folder-txt.png"));
    ui.pushButton_set_background_image->setIcon(QIcon(":/images/oxygen/16x16/insert-image.png"));
    ui.pushButton_set_winamp->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    // page basic
    ui.groupBox_profiles->setTitle(tr("Profiles"));
    ui.label_profile->setText(tr("Current profile:"));
    ui.pushButton_profiles->setText(tr("Profiles"));
    ui.groupBox_skins->setTitle(tr("Skins"));
    ui.checkBox_show_avatars->setText(tr("Show avatars in nicklist"));
    ui.groupBox_language->setTitle(tr("Language"));

    // page adv
    ui.checkBox_auto_busy->setText(tr("Busy mode after you log in to chat"));
    ui.checkBox_disable_autojoin_favourites->setText(tr("Disable autojoin favourite channels"));
    ui.checkBox_show_zuo_and_ip->setText(tr("Show ZUO and IP"));
    ui.checkBox_hide_formating->setText(tr("Disable font size, color..."));
    ui.checkBox_hide_join_part->setText(tr("Hide join/part"));
    ui.checkBox_hide_join_part_200->setText(tr("Hide join/part on big channels"));
    ui.checkBox_disable_emots->setText(tr("Disable emoticons"));
    ui.checkBox_disable_replaces->setText(tr("Disable replaces"));

    // page colors
    ui.tabWidget->setTabText(0,tr("Main window"));
    ui.tabWidget->setTabText(1,tr("Nicklist"));

    ui.label_background_color->setText(tr("Background color:"));
    ui.label_default_font_color->setText(tr("Default font color:"));
    ui.label_join_font_color->setText(tr("Join color:"));
    ui.label_part_font_color->setText(tr("Part color:"));
    ui.label_quit_font_color->setText(tr("Quit color:"));
    ui.label_kick_font_color->setText(tr("Kick color:"));
    ui.label_mode_font_color->setText(tr("Mode color:"));
    ui.label_notice_font_color->setText(tr("Notice color:"));
    ui.label_info_font_color->setText(tr("Info color:"));
    ui.label_me_font_color->setText(tr("Me color:"));
    ui.label_error_font_color->setText(tr("Error color:"));
    ui.label_channel_font_color->setText(tr("Channel color:"));
    ui.pushButton_mainwindow_restore_default->setText(tr("Restore default"));

    ui.label_nicklist_nick_color->setText(tr("Nick color:"));
    ui.label_nicklist_selected_nick_color->setText(tr("Selected nick color:"));
    ui.label_nicklist_busy_nick_color->setText(tr("Busy nick color:"));
    ui.label_nicklist_gradient_1_color->setText(tr("Gradient 1:"));
    ui.label_nicklist_gradient_2_color->setText(tr("Gradient 2:"));
    ui.pushButton_nicklist_restore_default->setText(tr("Restore default"));

    // page sounds
    ui.groupBox_sounds->setTitle(tr("Sounds"));
    ui.label_sound_beep->setText(tr("Beep"));
    ui.label_sound_query->setText(tr("Query"));
    ui.pushButton_sound_beep_change->setText(tr("Change"));
    ui.pushButton_sound_query_change->setText(tr("Change"));
    ui.checkBox_disable_sounds->setText(tr("Disable sounds"));

    // page logs
    ui.groupBox_logs->setTitle(tr("Logs"));
    ui.label_logs->setText(tr("Default logs folder:"));
    ui.pushButton_logs_open_folder->setText(tr("Open folder"));
    ui.checkBox_save_logs_by_date->setText(tr("Save logs by date"));
    ui.checkBox_disable_logs->setText(tr("Disable logs"));

    // page background image
    ui.groupBox_background_image->setTitle(tr("Background image"));
    ui.label_background_image->setText(tr("Default background image:"));
    ui.pushButton_set_background_image->setText(tr("Set"));
    ui.checkBox_disable_background_image->setText(tr("Disable background image"));

    // page winamp
    ui.groupBox_winamp->setTitle(tr("Winamp"));
    ui.label_winamp_text->setText(tr("Displayed text:"));
    ui.label_winamp_version->setText(tr("$version - Winamp version"));
    ui.label_winamp_song->setText(tr("$song - current song"));
    ui.label_winamp_position->setText(tr("$position - position"));
    ui.label_winamp_length->setText(tr("$length - length"));
    ui.pushButton_set_winamp->setText(tr("Set"));

    // options list
    QTreeWidgetItem *basic = new QTreeWidgetItem(ui.treeWidget_options);
    basic->setIcon(0, QIcon(":/images/oxygen/16x16/view-media-artist.png"));
    basic->setText(0, tr("Basic"));
    basic->setToolTip(0, tr("Basic"));

    QTreeWidgetItem *adv = new QTreeWidgetItem(ui.treeWidget_options);
    adv->setIcon(0, QIcon(":/images/oxygen/16x16/dialog-warning.png"));
    adv->setText(0, tr("Advanced"));
    adv->setToolTip(0, tr("Advanced"));

    QTreeWidgetItem *colors = new QTreeWidgetItem(ui.treeWidget_options);
    colors->setIcon(0, QIcon(":/images/oxygen/16x16/view-media-visualization.png"));
    colors->setText(0, tr("Colors"));
    colors->setToolTip(0, tr("Colors"));

    QTreeWidgetItem *sounds = new QTreeWidgetItem(ui.treeWidget_options);
    sounds->setIcon(0, QIcon(":/images/oxygen/16x16/media-playback-start.png"));
    sounds->setText(0, tr("Sounds"));
    sounds->setToolTip(0, tr("Sounds"));

    QTreeWidgetItem *logs = new QTreeWidgetItem(ui.treeWidget_options);
    logs->setIcon(0, QIcon(":/images/oxygen/16x16/text-field.png"));
    logs->setText(0, tr("Logs"));
    logs->setToolTip(0, tr("Logs"));

    QTreeWidgetItem *background_image = new QTreeWidgetItem(ui.treeWidget_options);
    background_image->setIcon(0, QIcon(":/images/oxygen/16x16/games-config-background.png"));
    background_image->setText(0, tr("Background image"));
    background_image->setToolTip(0, tr("Background image"));

    QTreeWidgetItem *winamp = new QTreeWidgetItem(ui.treeWidget_options);
    winamp->setIcon(0, QIcon(":/images/winamp.png"));
    winamp->setText(0, tr("Winamp"));
    winamp->setToolTip(0, tr("Winamp"));
}

void DlgOptions::setDefaultValues()
{
    // open folder command
#ifdef Q_WS_X11
    if (QFile::exists("/usr/bin/nautilus"))
        strOpenFolderCommand = "nautilus";
    else if (QFile::exists("/usr/bin/dolphin"))
        strOpenFolderCommand = "dolphin";
#else
    strOpenFolderCommand = "explorer.exe";
#endif

    // current option
    ui.treeWidget_options->setCurrentItem(ui.treeWidget_options->itemAt(0,0));

    // language
    QStringList strlLanguage;
    strlLanguage << tr("English") << tr("Polish");
    ui.comboBox_language->clear();
    ui.comboBox_language->addItems(strlLanguage);

    // sound beep
    ui.lineEdit_sound_beep->setText(Core::instance()->settings.value("sound_beep"));
    ui.lineEdit_sound_query->setText(Core::instance()->settings.value("sound_query"));

    // logs
    QString strLogsPath;
#ifdef Q_WS_WIN
    QString path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";

    strLogsPath = path+"/log";
    strLogsPath.replace("/", "\\");
#else
    strLogsPath = QDir::homePath()+"/.scc/log";
#endif
    ui.lineEdit_logs_folder->setText(strLogsPath);

    if (strOpenFolderCommand.isEmpty())
        ui.pushButton_logs_open_folder->setEnabled(false);

    // background image
    ui.lineEdit_background_image->setText(Core::instance()->settings.value("background_image"));

    // default values
    QString strShowAvatars = Core::instance()->settings.value("show_avatars");
    QString strLanguage = Core::instance()->settings.value("language");

    QString strAutoBusy = Core::instance()->settings.value("auto_busy");
    QString strDisableAutojoinFavourites = Core::instance()->settings.value("disable_autojoin_favourites");
    QString strShowZuoAndIp = Core::instance()->settings.value("show_zuo_and_ip");
    QString strHideFormating = Core::instance()->settings.value("hide_formating");
    QString strHideJoinPart = Core::instance()->settings.value("hide_join_part");
    QString strHideJoinPart200 = Core::instance()->settings.value("hide_join_part_200");
    QString strDisableEmots = Core::instance()->settings.value("disable_emots");
    QString strDisableReplaces = Core::instance()->settings.value("disable_replaces");

    QString strSaveLogsByDate = Core::instance()->settings.value("save_logs_by_date");
    QString strDisableLogs = Core::instance()->settings.value("disable_logs");
    QString strDisableSounds = Core::instance()->settings.value("disable_sounds");
    QString strDisableBackgroundImage = Core::instance()->settings.value("disable_background_image");

    QString strWinamp = Core::instance()->settings.value("winamp");

    // set style
    if (strShowAvatars == "on")
        ui.checkBox_show_avatars->setChecked(true);
    else
        ui.checkBox_show_avatars->setChecked(false);

    // language
    if (strLanguage == "en")
        ui.comboBox_language->setCurrentIndex(0);
    else if (strLanguage == "pl")
        ui.comboBox_language->setCurrentIndex(1);
    else
        ui.comboBox_language->setCurrentIndex(1);

    // auto busy
    if (strAutoBusy == "on")
        ui.checkBox_auto_busy->setChecked(true);
    else
        ui.checkBox_auto_busy->setChecked(false);

    // auto busy
    if (strDisableAutojoinFavourites == "on")
        ui.checkBox_disable_autojoin_favourites->setChecked(true);
    else
        ui.checkBox_disable_autojoin_favourites->setChecked(false);

    // show zuo
    if (strShowZuoAndIp == "on")
        ui.checkBox_show_zuo_and_ip->setChecked(true);
    else
        ui.checkBox_show_zuo_and_ip->setChecked(false);

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

    // hide join part
    if (strHideJoinPart200 == "on")
        ui.checkBox_hide_join_part_200->setChecked(true);
    else
        ui.checkBox_hide_join_part_200->setChecked(false);

    // disable emots
    if (strDisableEmots == "on")
        ui.checkBox_disable_emots->setChecked(true);
    else
        ui.checkBox_disable_emots->setChecked(false);

    // disable replaces
    if (strDisableReplaces == "on")
        ui.checkBox_disable_replaces->setChecked(true);
    else
        ui.checkBox_disable_replaces->setChecked(false);

    // save logs by date
    if (strSaveLogsByDate == "on")
        ui.checkBox_save_logs_by_date->setChecked(true);
    else
        ui.checkBox_save_logs_by_date->setChecked(false);

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

    // disable background image
    if (strDisableBackgroundImage == "on")
        ui.checkBox_disable_background_image->setChecked(true);
    else
        ui.checkBox_disable_background_image->setChecked(false);

    // winamp
    ui.lineEdit_winamp->setText(strWinamp);

    // set mainwindow colors
    setMainwindowColors();

    // set nicklist colors
    setNicklistColors();

    // disable change nick if connected
    if (Core::instance()->settings.value("logged") == "on")
    {
        ui.groupBox_profiles->setDisabled(true);
        ui.groupBox_skins->setDisabled(true);
    }
    else
    {
        ui.groupBox_profiles->setDisabled(false);
        ui.groupBox_skins->setDisabled(false);
    }
}

void DlgOptions::createSignals()
{
    QObject::connect(ui.treeWidget_options, SIGNAL(clicked(QModelIndex)), this, SLOT(changePage(QModelIndex)));
    QObject::connect(ui.comboBox_profiles, SIGNAL(activated(int)), this, SLOT(currentProfileChanged(int)));
    QObject::connect(ui.pushButton_profiles, SIGNAL(clicked()), this, SLOT(buttonProfiles()));
    QObject::connect(ui.checkBox_show_avatars, SIGNAL(clicked(bool)), this, SLOT(showAvatars(bool)));
    QObject::connect(ui.comboBox_language, SIGNAL(activated(int)), this, SLOT(languageChanged(int)));
    QObject::connect(ui.checkBox_auto_busy, SIGNAL(clicked(bool)), this, SLOT(autoBusy(bool)));
    QObject::connect(ui.checkBox_disable_autojoin_favourites, SIGNAL(clicked(bool)), this, SLOT(disableAutojoinFavourites(bool)));
    QObject::connect(ui.checkBox_show_zuo_and_ip, SIGNAL(clicked(bool)), this, SLOT(showZuoAndIp(bool)));
    QObject::connect(ui.checkBox_hide_formating, SIGNAL(clicked(bool)), this, SLOT(hideFormating(bool)));
    QObject::connect(ui.checkBox_hide_join_part, SIGNAL(clicked(bool)), this, SLOT(hideJoinPart(bool)));
    QObject::connect(ui.checkBox_hide_join_part_200, SIGNAL(clicked(bool)), this, SLOT(hideJoinPart200(bool)));
    QObject::connect(ui.checkBox_disable_emots, SIGNAL(clicked(bool)), this, SLOT(disableEmots(bool)));
    QObject::connect(ui.checkBox_disable_replaces, SIGNAL(clicked(bool)), this, SLOT(disableReplaces(bool)));
    QObject::connect(ui.pushButton_background_color, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
    QObject::connect(ui.pushButton_default_font_color, SIGNAL(clicked()), this, SLOT(setDefaultFontColor()));
    QObject::connect(ui.pushButton_join_font_color, SIGNAL(clicked()), this, SLOT(setJoinFontColor()));
    QObject::connect(ui.pushButton_part_font_color, SIGNAL(clicked()), this, SLOT(setPartFontColor()));
    QObject::connect(ui.pushButton_quit_font_color, SIGNAL(clicked()), this, SLOT(setQuitFontColor()));
    QObject::connect(ui.pushButton_kick_font_color, SIGNAL(clicked()), this, SLOT(setKickFontColor()));
    QObject::connect(ui.pushButton_mode_font_color, SIGNAL(clicked()), this, SLOT(setModeFontColor()));
    QObject::connect(ui.pushButton_notice_font_color, SIGNAL(clicked()), this, SLOT(setNoticeFontColor()));
    QObject::connect(ui.pushButton_info_font_color, SIGNAL(clicked()), this, SLOT(setInfoFontColor()));
    QObject::connect(ui.pushButton_me_font_color, SIGNAL(clicked()), this, SLOT(setMeFontColor()));
    QObject::connect(ui.pushButton_error_font_color, SIGNAL(clicked()), this, SLOT(setErrorFontColor()));
    QObject::connect(ui.pushButton_channel_font_color, SIGNAL(clicked()), this, SLOT(setChannelFontColor()));
    QObject::connect(ui.pushButton_mainwindow_restore_default, SIGNAL(clicked()), this, SLOT(mainwindowRestoreDefault()));
    QObject::connect(ui.pushButton_nicklist_nick_color, SIGNAL(clicked()), this, SLOT(setNicklistNickColor()));
    QObject::connect(ui.pushButton_nicklist_selected_nick_color, SIGNAL(clicked()), this, SLOT(setNicklistSelectedNickColor()));
    QObject::connect(ui.pushButton_nicklist_busy_nick_color, SIGNAL(clicked()), this, SLOT(setNicklistBusyNickColor()));
    QObject::connect(ui.pushButton_nicklist_gradient_1_color, SIGNAL(clicked()), this, SLOT(setNicklistGradient1Color()));
    QObject::connect(ui.pushButton_nicklist_gradient_2_color, SIGNAL(clicked()), this, SLOT(setNicklistGradient2Color()));
    QObject::connect(ui.pushButton_nicklist_restore_default, SIGNAL(clicked()), this, SLOT(nicklistRestoreDefault()));
    QObject::connect(ui.pushButton_play_beep, SIGNAL(clicked()), this, SLOT(tryPlayBeep()));
    QObject::connect(ui.pushButton_play_query, SIGNAL(clicked()), this, SLOT(tryPlayQuery()));
    QObject::connect(ui.pushButton_sound_beep_change, SIGNAL(clicked()), this, SLOT(setSoundBeep()));
    QObject::connect(ui.pushButton_sound_query_change, SIGNAL(clicked()), this, SLOT(setSoundQuery()));
    QObject::connect(ui.checkBox_disable_sounds, SIGNAL(clicked(bool)), this, SLOT(disableSounds(bool)));
    QObject::connect(ui.pushButton_logs_open_folder, SIGNAL(clicked()), this, SLOT(openLogsFolder()));
    QObject::connect(ui.checkBox_save_logs_by_date, SIGNAL(clicked(bool)), this, SLOT(setSaveLogsByDate(bool)));
    QObject::connect(ui.checkBox_disable_logs, SIGNAL(clicked(bool)), this, SLOT(disableLogs(bool)));
    QObject::connect(ui.pushButton_set_background_image, SIGNAL(clicked()), this, SLOT(setBackgroundImage()));
    QObject::connect(ui.checkBox_disable_background_image, SIGNAL(clicked(bool)), this, SLOT(disableBackgroundImage(bool)));
    QObject::connect(ui.pushButton_set_winamp, SIGNAL(clicked()), this, SLOT(setWinamp()));
    QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

void DlgOptions::refreshProfilesList()
{
    // clear
    ui.comboBox_profiles->clear();

    QString path;
#ifdef Q_WS_WIN
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    path += "/scc";
#else
    path = QDir::homePath()+"/.scc";
#endif

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    path += "/profiles";

    // create dir if not exist
    if (!QDir().exists(path))
        QDir().mkdir(path);

    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    QStringList nameFilters;
    nameFilters << "*.xml";
    QFileInfoList list = dir.entryInfoList(nameFilters);
    for (int i = 0; i < list.size(); ++i)
    {
        QString fileName = list.at(i).fileName();
        fileName.remove(".xml");
        ui.comboBox_profiles->addItem(fileName);
    }

    // set current profile
    ui.comboBox_profiles->setCurrentIndex(ui.comboBox_profiles->findText(Core::instance()->settings.value("current_profile")));
}

void DlgOptions::changePage(QModelIndex index)
{
    ui.stackedWidget->setCurrentIndex(index.row());
}

void DlgOptions::setCurrentProfile(int row)
{
    ui.comboBox_profiles->setCurrentIndex(row);
    currentProfileChanged(row);
}

void DlgOptions::currentProfileChanged(int row)
{
    QString profileName = ui.comboBox_profiles->itemText(row);

    if (profileName.isEmpty())
        return;

    Core::instance()->settings["current_profile"] = profileName; // must be before config !

    Config *pConfig = new Config(false);
    pConfig->setValue("current_profile", profileName);
    delete pConfig;

    // refresh settings
    Core::instance()->createSettings();

    // refresh options
    setDefaultValues();

    // refresh colors
    Core::instance()->refreshColors();

    // refresh background image
    Core::instance()->refreshBackgroundImage();

    // refresh tool widget values
    Core::instance()->refreshToolWidgetValues();
}

void DlgOptions::buttonProfiles()
{
    DlgProfileManager(Core::instance()->sccWindow(), this).exec();
}

void DlgOptions::showAvatars(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("show_avatars", strValue);
    Core::instance()->settings["show_avatars"] = strValue;
    delete pConfig;

    ui.label_skins_warning->setText(tr("Restart program to apply the changes."));
}

void DlgOptions::languageChanged(int index)
{
    Config *pConfig = new Config();
    if (index == 0) // english
    {
        pConfig->setValue("language", "en");
        Core::instance()->settings["language"] = "en";
    }
    else if (index == 1) // polish
    {
        pConfig->setValue("language", "pl");
        Core::instance()->settings["language"] = "pl";
    }
    else // polish
    {
        pConfig->setValue("language", "pl");
        Core::instance()->settings["language"] = "pl";
    }
    delete pConfig;

    ui.label_language_warning->setText(tr("Restart program to apply the changes."));
}

void DlgOptions::autoBusy(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("auto_busy", strValue);
    Core::instance()->settings["auto_busy"] = strValue;
    delete pConfig;
}

void DlgOptions::disableAutojoinFavourites(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("disable_autojoin_favourites", strValue);
    Core::instance()->settings["disable_autojoin_favourites"] = strValue;
    delete pConfig;
}

void DlgOptions::showZuoAndIp(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("show_zuo_and_ip", strValue);
    Core::instance()->settings["show_zuo_and_ip"] = strValue;
    delete pConfig;
}

void DlgOptions::hideFormating(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("hide_formating", strValue);
    Core::instance()->settings["hide_formating"] = strValue;
    delete pConfig;
}

void DlgOptions::hideJoinPart(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("hide_join_part", strValue);
    Core::instance()->settings["hide_join_part"] = strValue;
    delete pConfig;
}

void DlgOptions::hideJoinPart200(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("hide_join_part_200", strValue);
    Core::instance()->settings["hide_join_part_200"] = strValue;
    delete pConfig;
}

void DlgOptions::disableEmots(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("disable_emots", strValue);
    Core::instance()->settings["disable_emots"] = strValue;
    delete pConfig;
}

void DlgOptions::disableReplaces(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("disable_replaces", strValue);
    Core::instance()->settings["disable_replaces"] = strValue;
    delete pConfig;
}

void DlgOptions::setBackgroundColor()
{
    setColor("background_color");
}

void DlgOptions::setDefaultFontColor()
{
    setColor("default_font_color");
}

void DlgOptions::setJoinFontColor()
{
    setColor("font_color_level_1");
}

void DlgOptions::setPartFontColor()
{
    setColor("font_color_level_2");
}

void DlgOptions::setQuitFontColor()
{
    setColor("font_color_level_3");
}

void DlgOptions::setKickFontColor()
{
    setColor("font_color_level_4");
}

void DlgOptions::setModeFontColor()
{
    setColor("font_color_level_5");
}

void DlgOptions::setNoticeFontColor()
{
    setColor("font_color_level_6");
}

void DlgOptions::setInfoFontColor()
{
    setColor("font_color_level_7");
}

void DlgOptions::setMeFontColor()
{
    setColor("font_color_level_8");
}

void DlgOptions::setErrorFontColor()
{
    setColor("font_color_level_9");
}

void DlgOptions::setChannelFontColor()
{
    setColor("channel_font_color");
}

void DlgOptions::mainwindowRestoreDefault()
{
    Config *pConfig = new Config();

    pConfig->setValue("background_color", "#ffffff");
    Core::instance()->settings["background_color"] = "#ffffff";

    pConfig->setValue("default_font_color", "#000000");
    Core::instance()->settings["default_font_color"] = "#000000";

    pConfig->setValue("font_color_level_1", "#009300");
    Core::instance()->settings["font_color_level_1"] = "#009300";

    pConfig->setValue("font_color_level_2", "#4733FF");
    Core::instance()->settings["font_color_level_2"] = "#4733FF";

    pConfig->setValue("font_color_level_3", "#00007F");
    Core::instance()->settings["font_color_level_3"] = "#00007F";

    pConfig->setValue("font_color_level_4", "#00007F");
    Core::instance()->settings["font_color_level_4"] = "#00007F";

    pConfig->setValue("font_color_level_5", "#009300");
    Core::instance()->settings["font_color_level_5"] = "#009300";

    pConfig->setValue("font_color_level_6", "#0066FF");
    Core::instance()->settings["font_color_level_6"] = "#0066FF";

    pConfig->setValue("font_color_level_7", "#666666");
    Core::instance()->settings["font_color_level_7"] = "#666666";

    pConfig->setValue("font_color_level_8", "#800080");
    Core::instance()->settings["font_color_level_8"] = "#800080";

    pConfig->setValue("font_color_level_9", "#ff0000");
    Core::instance()->settings["font_color_level_9"] = "#ff0000";

    pConfig->setValue("channel_font_color", "#0000ff");
    Core::instance()->settings["channel_font_color"] = "#0000ff";

    delete pConfig;

    // set
    setMainwindowColors();

    // refresh tabs
    Core::instance()->refreshColors();
}

void DlgOptions::setNicklistNickColor()
{
    setColor("nicklist_nick_color");
}

void DlgOptions::setNicklistSelectedNickColor()
{
    setColor("nicklist_selected_nick_color");
}

void DlgOptions::setNicklistBusyNickColor()
{
    setColor("nicklist_busy_nick_color");
}

void DlgOptions::setNicklistGradient1Color()
{
    setColor("nicklist_gradient_1_color");
}

void DlgOptions::setNicklistGradient2Color()
{
    setColor("nicklist_gradient_2_color");
}

void DlgOptions::nicklistRestoreDefault()
{
    Config *pConfig = new Config();

    pConfig->setValue("nicklist_nick_color", "#333333");
    Core::instance()->settings["nicklist_nick_color"] = "#333333";

    pConfig->setValue("nicklist_selected_nick_color", "#ffffff");
    Core::instance()->settings["nicklist_selected_nick_color"] = "#ffffff";

    pConfig->setValue("nicklist_busy_nick_color", "#a0a0a4");
    Core::instance()->settings["nicklist_busy_nick_color"] = "#a0a0a4";

    pConfig->setValue("nicklist_gradient_1_color", "#77d5f7");
    Core::instance()->settings["nicklist_gradient_1_color"] = "#77d5f7";

    pConfig->setValue("nicklist_gradient_2_color", "#1b86b7");
    Core::instance()->settings["nicklist_gradient_2_color"] = "#1b86b7";

    delete pConfig;

    // set
    setNicklistColors();
}

void DlgOptions::tryPlayBeep()
{
    Notify::instance()->play(Beep);
}

void DlgOptions::tryPlayQuery()
{
    Notify::instance()->play(Query);
}

void DlgOptions::setSoundBeep()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Audio File"),
                                     ui.lineEdit_sound_beep->text(),
                                     tr("All Files (*);;Mp3 Files (*.mp3);;Wave Files (*.wav)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Config *pConfig = new Config();

        pConfig->setValue("sound_beep", fileName);
        Core::instance()->settings["sound_beep"] = fileName;

        delete pConfig;
        ui.lineEdit_sound_beep->setText(fileName);
    }
}

void DlgOptions::setSoundQuery()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Audio File"),
                                     ui.lineEdit_sound_query->text(),
                                     tr("All Files (*);;Mp3 Files (*.mp3);;Wave Files (*.wav)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Config *pConfig = new Config();

        pConfig->setValue("sound_query", fileName);
        Core::instance()->settings["sound_query"] = fileName;

        delete pConfig;
        ui.lineEdit_sound_query->setText(fileName);
    }
}

void DlgOptions::disableSounds(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("disable_sounds", strValue);
    Core::instance()->settings["disable_sounds"] = strValue;
    delete pConfig;
}

void DlgOptions::openLogsFolder()
{
    QString strLogsPath = ui.lineEdit_logs_folder->text();

    QProcess pProcess;
    pProcess.execute(strOpenFolderCommand+" "+strLogsPath);
}

void DlgOptions::setSaveLogsByDate(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("save_logs_by_date", strValue);
    Core::instance()->settings["save_logs_by_date"] = strValue;
    delete pConfig;
}

void DlgOptions::disableLogs(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("disable_logs", strValue);
    Core::instance()->settings["disable_logs"] = strValue;
    delete pConfig;
}

void DlgOptions::setBackgroundImage()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("Select Image File"),
                                     ui.lineEdit_background_image->text(),
                                     tr("All Files (*);;JPG Files (*.jpg);;PNG Files (*.png);;Bitmap Files (*.bmp)"),
                                     &selectedFilter,
                                     0);
    if (!fileName.isEmpty())
    {
        Config *pConfig = new Config();

        pConfig->setValue("background_image", fileName);
        Core::instance()->settings["background_image"] = fileName;

        delete pConfig;
        ui.lineEdit_background_image->setText(fileName);

        // refresh background image
        Core::instance()->refreshBackgroundImage();
    }
}

void DlgOptions::disableBackgroundImage(bool bValue)
{
    QString strValue = (bValue ? "on" : "off");

    Config *pConfig = new Config();
    pConfig->setValue("disable_background_image", strValue);
    Core::instance()->settings["disable_background_image"] = strValue;
    delete pConfig;

    // refresh background image
    Core::instance()->refreshBackgroundImage();
}

void DlgOptions::setWinamp()
{
    QString strValue = ui.lineEdit_winamp->text();

    Config *pConfig = new Config();
    pConfig->setValue("winamp", strValue);
    Core::instance()->settings["winamp"] = strValue;
    delete pConfig;
}

void DlgOptions::setColor(QString strKey)
{
    // get current value
    QString strDefaultColor = Core::instance()->settings.value(strKey);

    // color dialog
    QColor cColor = QColorDialog::getColor(QColor(strDefaultColor), this);

    if (cColor.isValid())
    {
        // color to name
        QString strColor = cColor.name();

        // save
        Config *pConfig = new Config();
        pConfig->setValue(strKey, strColor);
        Core::instance()->settings[strKey] = strColor;
        delete pConfig;

        // save to pushbutton
        QPixmap color(50,15);
        color.fill(QColor(cColor));

        if (strKey == "background_color")
            ui.pushButton_background_color->setIcon(QIcon(color));
        else if (strKey == "background_color")
            ui.pushButton_background_color->setIcon(QIcon(color));
        else if (strKey == "default_font_color")
            ui.pushButton_default_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_1")
            ui.pushButton_join_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_2")
            ui.pushButton_part_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_3")
            ui.pushButton_quit_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_4")
            ui.pushButton_kick_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_5")
            ui.pushButton_mode_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_6")
            ui.pushButton_notice_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_7")
            ui.pushButton_info_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_8")
            ui.pushButton_me_font_color->setIcon(QIcon(color));
        else if (strKey == "font_color_level_9")
            ui.pushButton_error_font_color->setIcon(QIcon(color));
        else if (strKey == "channel_font_color")
            ui.pushButton_channel_font_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_nick_color")
            ui.pushButton_nicklist_nick_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_selected_nick_color")
            ui.pushButton_nicklist_selected_nick_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_busy_nick_color")
            ui.pushButton_nicklist_busy_nick_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_gradient_1_color")
            ui.pushButton_nicklist_gradient_1_color->setIcon(QIcon(color));
        else if (strKey == "nicklist_gradient_2_color")
            ui.pushButton_nicklist_gradient_2_color->setIcon(QIcon(color));

        // refresh
        Core::instance()->refreshColors();
    }
}

void DlgOptions::setMainwindowColors()
{
    QString strBackgroundColor = Core::instance()->settings.value("background_color");
    QString strDefaultFontColor = Core::instance()->settings.value("default_font_color");
    QString strJoinFontColor = Core::instance()->settings.value("font_color_level_1");
    QString strPartFontColor = Core::instance()->settings.value("font_color_level_2");
    QString strQuitFontColor = Core::instance()->settings.value("font_color_level_3");
    QString strKickFontColor = Core::instance()->settings.value("font_color_level_4");
    QString strModeFontColor = Core::instance()->settings.value("font_color_level_5");
    QString strNoticeFontColor = Core::instance()->settings.value("font_color_level_6");
    QString strInfoFontColor = Core::instance()->settings.value("font_color_level_7");
    QString strMeFontColor = Core::instance()->settings.value("font_color_level_8");
    QString strErrorFontColor = Core::instance()->settings.value("font_color_level_9");
    QString strChannelFontColor = Core::instance()->settings.value("channel_font_color");

    // set background color
    QPixmap bcolor(50,15);
    bcolor.fill(QColor(strBackgroundColor));
    ui.pushButton_background_color->setIcon(QIcon(bcolor));

    // set default font color
    QPixmap dfcolor(50,15);
    dfcolor.fill(QColor(strDefaultFontColor));
    ui.pushButton_default_font_color->setIcon(QIcon(dfcolor));

    // set join font color
    QPixmap jfcolor(50,15);
    jfcolor.fill(QColor(strJoinFontColor));
    ui.pushButton_join_font_color->setIcon(QIcon(jfcolor));

    // set part font color
    QPixmap pfcolor(50,15);
    pfcolor.fill(QColor(strPartFontColor));
    ui.pushButton_part_font_color->setIcon(QIcon(pfcolor));

    // set quit font color
    QPixmap qfcolor(50,15);
    qfcolor.fill(QColor(strQuitFontColor));
    ui.pushButton_quit_font_color->setIcon(QIcon(qfcolor));

    // set kick font color
    QPixmap kfcolor(50,15);
    kfcolor.fill(QColor(strKickFontColor));
    ui.pushButton_kick_font_color->setIcon(QIcon(kfcolor));

    // set mode font color
    QPixmap mfcolor(50,15);
    mfcolor.fill(QColor(strModeFontColor));
    ui.pushButton_mode_font_color->setIcon(QIcon(mfcolor));

    // set notice font color
    QPixmap nfcolor(50,15);
    nfcolor.fill(QColor(strNoticeFontColor));
    ui.pushButton_notice_font_color->setIcon(QIcon(nfcolor));

    // set info font color
    QPixmap ifcolor(50,15);
    ifcolor.fill(QColor(strInfoFontColor));
    ui.pushButton_info_font_color->setIcon(QIcon(ifcolor));

    // set me font color
    QPixmap mecolor(50,15);
    mecolor.fill(QColor(strMeFontColor));
    ui.pushButton_me_font_color->setIcon(QIcon(mecolor));

    // set error font color
    QPixmap efcolor(50,15);
    efcolor.fill(QColor(strErrorFontColor));
    ui.pushButton_error_font_color->setIcon(QIcon(efcolor));

    // set channel font color
    QPixmap cfcolor(50,15);
    cfcolor.fill(QColor(strChannelFontColor));
    ui.pushButton_channel_font_color->setIcon(QIcon(cfcolor));
}

void DlgOptions::setNicklistColors()
{
    QString strNicklistNickColor = Core::instance()->settings.value("nicklist_nick_color");
    QString strNicklistSelectedNickColor = Core::instance()->settings.value("nicklist_selected_nick_color");
    QString strNicklistBusyNickColor = Core::instance()->settings.value("nicklist_busy_nick_color");
    QString strNicklistGradient1Color = Core::instance()->settings.value("nicklist_gradient_1_color");
    QString strNicklistGradient2Color = Core::instance()->settings.value("nicklist_gradient_2_color");

    // set nicklist nick color
    QPixmap nncolor(50,15);
    nncolor.fill(QColor(strNicklistNickColor));
    ui.pushButton_nicklist_nick_color->setIcon(QIcon(nncolor));

    // set nicklist selected nick color
    QPixmap nsncolor(50,15);
    nsncolor.fill(QColor(strNicklistSelectedNickColor));
    ui.pushButton_nicklist_selected_nick_color->setIcon(QIcon(nsncolor));

    // set nicklist busy nick color
    QPixmap nbncolor(50,15);
    nbncolor.fill(QColor(strNicklistBusyNickColor));
    ui.pushButton_nicklist_busy_nick_color->setIcon(QIcon(nbncolor));

    // set nicklist gradient 1 color
    QPixmap ng1color(50,15);
    ng1color.fill(QColor(strNicklistGradient1Color));
    ui.pushButton_nicklist_gradient_1_color->setIcon(QIcon(ng1color));

    // set nicklist gradient 2 color
    QPixmap ng2color(50,15);
    ng2color.fill(QColor(strNicklistGradient2Color));
    ui.pushButton_nicklist_gradient_2_color->setIcon(QIcon(ng2color));
}