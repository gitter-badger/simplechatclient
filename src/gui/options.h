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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include "ui_options.h"

class DlgOptions : public QDialog
{
    Q_OBJECT
public:
    DlgOptions(QWidget *parent = 0);
    void setCurrentProfile(int row);
    void refreshProfilesList();

private:
    Ui::uiOptions ui;

    void createGui();
    void setDefaultValues();
    void createSignals();

    void setColor(const QString &strKey);
    void setMainwindowColors();
    void setNicklistColors();

    QString reverseColor(QString strColor);

private slots:
    void changePage(QModelIndex index);
    void currentProfileChanged(int row);
    void buttonProfiles();
    void themesChanged(int index);
    void languageChanged(int index);
    void highlightAdd();
    void highlightRemove();
    void punishReasonAdd();
    void punishReasonRemove();
    void autoBusy(bool bValue);
    void autoAway(bool bValue);
    void disableAutojoinFavourites(bool bValue);
    void minimizeToTray(bool bValue);
    void showZuoAndIp(bool bValue);
    void hideFormating(bool bValue);
    void hideJoinPart(bool bValue);
    void hideJoinPart200(bool bValue);
    void disableEmots(bool bValue);
    void disableReplaces(bool bValue);
    void reverseColors();
    void restoreDefaultColors();
    void setBackgroundColor();
    void setDefaultColor();
    void setJoinColor();
    void setPartColor();
    void setQuitColor();
    void setKickColor();
    void setModeColor();
    void setNoticeColor();
    void setInfoColor();
    void setMeColor();
    void setErrorColor();
    void setChannelColor();
    void setNicklistNickColor();
    void setNicklistSelectedNickColor();
    void setNicklistBusyNickColor();
    void setNicklistGradient1Color();
    void setNicklistGradient2Color();
    void tryPlayBeep();
    void tryPlayQuery();
    void setSoundBeep();
    void setSoundQuery();
    void setSaveLogsByDate(bool bValue);
    void disableSounds(bool bValue);
    void openLogsFolder();
    void disableLogs(bool bValue);
    void setBackgroundImage();
    void disableBackgroundImage(bool bValue);
    void setWinamp();
    void trayMessage(bool);
};

#endif // OPTIONS_H
