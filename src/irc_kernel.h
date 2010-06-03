/****************************************************************************
 *                                                                          *
 *   This file is part of Simple Chat Client                                *
 *   Copyright (C) 2010 Piotr �uczko <piotr.luczko@gmail.com>               *
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

#ifndef IRC_KERNEL_H
#define IRC_KERNEL_H

#include <QMessageBox>
#include <QObject>
#include "channel_avatar.h"
#include "config.h"
#include "dlg_channel_favourites.h"
#include "dlg_channel_homes.h"
#include "dlg_channel_list.h"
#include "dlg_channel_settings.h"
#include "dlg_friends.h"
#include "dlg_ignore.h"
#include "dlg_moderation.h"
#include "dlg_priv.h"
#include "irc_auth.h"
#include "network.h"
#include "tab_container.h"

class irc_kernel : public QObject
{
    Q_OBJECT
public:
    irc_kernel(network *, tab_container *, QSettings *, dlg_channel_settings *, dlg_channel_homes *, dlg_channel_list *, dlg_channel_favourites *, dlg_friends *, dlg_ignore *, dlg_moderation *);

public slots:
    void kernel(QString);

private:
    network *pNetwork;
    tab_container *tabc;
    QStringList strDataList;
    QSettings *settings;
    dlg_channel_settings *dlgchannel_settings;
    dlg_channel_homes *dlgchannel_homes;
    dlg_channel_list *dlgchannel_list;
    dlg_channel_favourites *dlgchannel_favourites;
    dlg_friends *dlgfriends;
    dlg_ignore *dlgignore;
    dlg_moderation *dlgmoderation;
    QString strData;
    QList <channel_avatar*> caThreadList;

    void raw_error();
    void raw_ping();
    void raw_join();
    void raw_part();
    void raw_quit();
    void raw_kick();
    void raw_mode();
    void raw_privmsg();
    void raw_notice();
    void raw_invite();
    void raw_topic();
    void raw_invreject();
    void raw_invignore();
    void raw_modermsg();
    void raw_modernotice();
    void raw_moderate();
    void raw_001();
    void raw_109n();
    void raw_121n();
    void raw_122n();
    void raw_131n();
    void raw_132n();
    void raw_141n();
    void raw_142n();
    void raw_151n();
    void raw_152n();
    void raw_160n();
    void raw_161n();
    void raw_162n();
    void raw_163n();
    void raw_164n();
    void raw_165n();
    void raw_220n();
    void raw_221n();
    void raw_230n();
    void raw_231n();
    void raw_240n();
    void raw_241n();
    void raw_250n();
    void raw_251n();
    void raw_252n();
    void raw_253n();
    void raw_254n();
    void raw_255n();
    void raw_256n();
    void raw_257n();
    void raw_258n();
    void raw_259n();
    void raw_260n();
    void raw_261n();
    void raw_301();
    void raw_305();
    void raw_306();
    void raw_307();
    void raw_311();
    void raw_312();
    void raw_313();
    void raw_317();
    void raw_318();
    void raw_319();
    void raw_332();
    void raw_333();
    void raw_341();
    void raw_353();
    void raw_366();
    void raw_372();
    void raw_375();
    void raw_376();
    void raw_396();
    void raw_400n();
    void raw_401();
    void raw_401n();
    void raw_403();
    void raw_404n();
    void raw_406n();
    void raw_407n();
    void raw_408n();
    void raw_420n();
    void raw_421();
    void raw_421n();
    void raw_404();
    void raw_430n();
    void raw_431n();
    void raw_432();
    void raw_433();
    void raw_440n();
    void raw_441n();
    void raw_443();
    void raw_451();
    void raw_452n();
    void raw_454n();
    void raw_458n();
    void raw_459n();
    void raw_461n();
    void raw_463n();
    void raw_467n();
    void raw_468n();
    void raw_472n();
    void raw_473();
    void raw_474();
    void raw_484();
    void raw_530();
    void raw_600();
    void raw_601();
    void raw_602();
    void raw_604();
    void raw_605();
    void raw_801();
    void raw_802();
    void raw_807();
    void raw_808();
    void raw_809();
    void raw_811();
    void raw_812();
    void raw_815();
    void raw_817();
    void raw_818();
    void raw_819();
    void raw_820();
    void raw_821();
    void raw_951();
    QString get_settings_key(QString);

private slots:
    void remove_cathread(channel_avatar*);

};

#endif // IRC_KERNEL_H
