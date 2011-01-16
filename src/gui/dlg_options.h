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

class Notify;
#include <QDialog>
#include "ui_options.h"

class DlgOptions : public QDialog
{
    Q_OBJECT
public:
    DlgOptions(QWidget *, Notify *);

private:
    Ui::uiOptions ui;
    QWidget *myparent;
    Notify *pNotify;
    QString strOpenFolderCommand;

    void set_mainwindow_colors();
    void set_nicklist_colors();
    void save_settings();
    void clear_settings();

private slots:
    void change_page(QTreeWidgetItem *, QTreeWidgetItem *);
    void hide_pass();
    void show_pass();
    void button_register_nick();
    void set_modern_style_avatars();
    void set_modern_style_no_avatars();
    void set_classic_style();
    void language_changed(int);
    void auto_busy();
    void disable_autojoin_favourites();
    void show_zuo();
    void hide_formating();
    void hide_join_part();
    void hide_join_part_200();
    void disable_avatars();
    void disable_emots();
    void disable_replaces();
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
    void disable_sounds();
    void open_logs_folder();
    void disable_logs();
    void button_ok();
    void button_cancel();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);

signals:
    void refresh_colors();
};

#endif // DLG_OPTIONS_H
