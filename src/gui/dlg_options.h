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

#ifndef DLG_OPTIONS_H
#define DLG_OPTIONS_H

#include <QDialog>
#include "ui_options.h"

class DlgOptions : public QDialog
{
    Q_OBJECT
public:
    DlgOptions(QWidget *);

private:
    Ui::uiOptions ui;
    QString strOpenFolderCommand;

    void create_gui();
    void set_default_values();
    void create_signals();

    void save_settings();
    void set_color(QString);
    void set_mainwindow_colors();
    void set_nicklist_colors();

private slots:
    void change_page(QTreeWidgetItem *, QTreeWidgetItem *);
    void hide_pass();
    void show_pass();
    void button_register_nick();
    void set_modern_style_avatars();
    void set_modern_style_no_avatars();
    void language_changed(int);
    void auto_busy(bool);
    void disable_autojoin_favourites(bool);
    void show_zuo(bool);
    void hide_formating(bool);
    void hide_join_part(bool);
    void hide_join_part_200(bool);
    void disable_avatars(bool);
    void disable_emots(bool);
    void disable_replaces(bool);
    void set_my_bold(int);
    void set_my_italic(int);
    void set_my_font(QString);
    void set_my_color(int);
    void set_font_size(QString);
    void set_background_color();
    void set_default_font_color();
    void set_join_font_color();
    void set_part_font_color();
    void set_quit_font_color();
    void set_kick_font_color();
    void set_mode_font_color();
    void set_notice_font_color();
    void set_info_font_color();
    void set_me_font_color();
    void set_error_font_color();
    void set_channel_font_color();
    void mainwindow_restore_default();
    void set_nicklist_nick_color();
    void set_nicklist_selected_nick_color();
    void set_nicklist_busy_nick_color();
    void set_nicklist_gradient_1_color();
    void set_nicklist_gradient_2_color();
    void nicklist_restore_default();
    void set_embedded_style();
    void try_play_beep();
    void try_play_query();
    void set_sound_beep();
    void set_sound_query();
    void set_save_logs_by_date(bool);
    void disable_sounds(bool);
    void open_logs_folder();
    void disable_logs(bool);
    void set_background_image();
    void disable_background_image(bool);
    void set_winamp();
    void button_ok();
};

#endif // DLG_OPTIONS_H
