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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include "ui_options.h"

class DlgOptions : public QDialog
{
    Q_OBJECT
public:
    DlgOptions(QWidget *);
    void setCurrentProfile(int);
    void refreshProfilesList();

private:
    Ui::uiOptions ui;

    void createGui();
    void setDefaultValues();
    void createSignals();

    void setColor(QString);
    void setMainwindowColors();
    void setNicklistColors();

    QString reverseColor(QString);

private slots:
    void changePage(QModelIndex);
    void currentProfileChanged(int);
    void buttonProfiles();
    void themesChanged(int);
    void languageChanged(int);
    void highlightAdd();
    void highlightRemove();
    void autoBusy(bool);
    void disableAutojoinFavourites(bool);
    void minimizeToTray(bool);
    void showZuoAndIp(bool);
    void hideFormating(bool);
    void hideJoinPart(bool);
    void hideJoinPart200(bool);
    void disableEmots(bool);
    void disableReplaces(bool);
    void reverseColors();
    void restoreDefaultColors();
    void setBackgroundColor();
    void setDefaultFontColor();
    void setJoinFontColor();
    void setPartFontColor();
    void setQuitFontColor();
    void setKickFontColor();
    void setModeFontColor();
    void setNoticeFontColor();
    void setInfoFontColor();
    void setMeFontColor();
    void setErrorFontColor();
    void setChannelFontColor();
    void setNicklistNickColor();
    void setNicklistSelectedNickColor();
    void setNicklistBusyNickColor();
    void setNicklistGradient1Color();
    void setNicklistGradient2Color();
    void tryPlayBeep();
    void tryPlayQuery();
    void setSoundBeep();
    void setSoundQuery();
    void setSaveLogsByDate(bool);
    void disableSounds(bool);
    void openLogsFolder();
    void disableLogs(bool);
    void setBackgroundImage();
    void disableBackgroundImage(bool);
    void setWinamp();
};

#endif // OPTIONS_H
