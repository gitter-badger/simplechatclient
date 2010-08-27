# -------------------------------------------------
# Project created by QtCreator 2009-08-13T02:50:42
# -------------------------------------------------
CONFIG += qtopia
QT += network \
    svg \
    xml
TARGET = scc
TEMPLATE = app
RESOURCES += scc.qrc
SOURCES += src/avatars/channel_avatar.cpp \
    src/avatars/nick_avatar.cpp \
    src/common/commands.cpp \
    src/common/config.cpp \
    src/common/convert.cpp \
    src/common/crypt.cpp \
    src/common/log.cpp \
    src/common/replace.cpp \
    src/core/debug.cpp \
    src/core/main.cpp \
    src/core/mainwindow.cpp \
    src/core/network.cpp \
    src/core/network_thread.cpp \
    src/core/update.cpp \
    src/core/update_thread.cpp \
    src/gui/dlg_about.cpp \
    src/gui/dlg_cam.cpp \
    src/gui/dlg_channel_favourites.cpp \
    src/gui/dlg_channel_favourites_ad.cpp \
    src/gui/dlg_channel_homes.cpp \
    src/gui/dlg_channel_homes_ad.cpp \
    src/gui/dlg_channel_key.cpp \
    src/gui/dlg_channel_list.cpp \
    src/gui/dlg_channel_settings.cpp \
    src/gui/dlg_email.cpp \
    src/gui/dlg_emoticons.cpp \
    src/gui/dlg_friends.cpp \
    src/gui/dlg_friends_ad.cpp \
    src/gui/dlg_ignore.cpp \
    src/gui/dlg_ignore_ad.cpp \
    src/gui/dlg_invite.cpp \
    src/gui/dlg_kick.cpp \
    src/gui/dlg_moderation.cpp \
    src/gui/dlg_options.cpp \
    src/gui/dlg_privilege.cpp \
    src/gui/dlg_register_nick.cpp \
    src/gui/dlg_update.cpp \
    src/gui/dlg_user_profile.cpp \
    src/notify/notify.cpp \
    src/onet/onet_auth.cpp \
    src/onet/onet_kernel.cpp \
    src/tab/tab_container.cpp \
    src/tab/tab_manager.cpp \
    src/tab/tab_widget.cpp \
    src/widgets/inputline.cpp \
    src/widgets/mainwebview.cpp \
    src/widgets/nicklist.cpp \
    src/widgets/nicklistdelegate.cpp
HEADERS += src/avatars/channel_avatar.h \
    src/avatars/nick_avatar.h \
    src/common/commands.h \
    src/common/config.h \
    src/common/convert.h \
    src/common/crypt.h \
    src/common/log.h \
    src/common/replace.h \
    src/core/debug.h \
    src/core/main.h \
    src/core/mainwindow.h \
    src/core/network.h \
    src/core/network_thread.h \
    src/core/update.h \
    src/core/update_thread.h \
    src/gui/dlg_about.h \
    src/gui/dlg_cam.h \
    src/gui/dlg_channel_favourites.h \
    src/gui/dlg_channel_favourites_ad.h \
    src/gui/dlg_channel_homes.h \
    src/gui/dlg_channel_homes_ad.h \
    src/gui/dlg_channel_key.h \
    src/gui/dlg_channel_list.h \
    src/gui/dlg_channel_settings.h \
    src/gui/dlg_email.h \
    src/gui/dlg_emoticons.h \
    src/gui/dlg_friends.h \
    src/gui/dlg_friends_ad.h \
    src/gui/dlg_ignore.h \
    src/gui/dlg_ignore_ad.h \
    src/gui/dlg_invite.h \
    src/gui/dlg_kick.h \
    src/gui/dlg_moderation.h \
    src/gui/dlg_options.h \
    src/gui/dlg_privilege.h \
    src/gui/dlg_register_nick.h \
    src/gui/dlg_update.h \
    src/gui/dlg_user_profile.h \
    src/notify/notify.h \
    src/onet/onet_auth.h \
    src/onet/onet_kernel.h \
    src/tab/tab_container.h \
    src/tab/tab_manager.h \
    src/tab/tab_widget.h \
    src/widgets/inputline.h \
    src/widgets/mainwebview.h \
    src/widgets/nicklist.h \
    src/widgets/nicklistdelegate.h
FORMS += ui/about.ui \
    ui/cam.ui \
    ui/channel_favourites.ui \
    ui/channel_favourites_ad.ui \
    ui/channel_homes.ui \
    ui/channel_homes_ad.ui \
    ui/channel_key.ui \
    ui/channel_list.ui \
    ui/channel_settings.ui \
    ui/email.ui \
    ui/emoticons.ui \
    ui/friends.ui \
    ui/friends_ad.ui \
    ui/ignore.ui \
    ui/ignore_ad.ui \
    ui/invite.ui \
    ui/kick.ui \
    ui/moderation.ui \
    ui/options.ui \
    ui/privilege.ui \
    ui/register_nick.ui \
    ui/update.ui \
    ui/user_profile.ui
