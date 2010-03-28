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
SOURCES += src/commands.cpp \
    src/config.cpp \
    src/crypt.cpp \
    src/dlg_about.cpp \
    src/dlg_channel_favourites.cpp \
    src/dlg_channel_favourites_ad.cpp \
    src/dlg_channel_homes.cpp \
    src/dlg_channel_homes_ad.cpp \
    src/dlg_channel_list.cpp \
    src/dlg_channel_settings.cpp \
    src/dlg_friends.cpp \
    src/dlg_friends_ad.cpp \
    src/dlg_ignore.cpp \
    src/dlg_ignore_ad.cpp \
    src/dlg_moderation.cpp \
    src/dlg_options.cpp \
    src/dlg_priv.cpp \
    src/dlg_privilege.cpp \
    src/http.cpp \
    src/irc_auth.cpp \
    src/irc_kernel.cpp \
    src/log.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/network.cpp \
    src/qinputline.cpp \
    src/qnicklist.cpp \
    src/tab_container.cpp \
    src/tab_manager.cpp \
    src/tab_widget.cpp
HEADERS += src/commands.h \
    src/config.h \
    src/crypt.h \
    src/dlg_about.h \
    src/dlg_channel_favourites.h \
    src/dlg_channel_favourites_ad.h \
    src/dlg_channel_homes.h \
    src/dlg_channel_homes_ad.h \
    src/dlg_channel_list.h \
    src/dlg_channel_settings.h \
    src/dlg_friends.h \
    src/dlg_friends_ad.h \
    src/dlg_ignore.h \
    src/dlg_ignore_ad.h \
    src/dlg_moderation.h \
    src/dlg_options.h \
    src/dlg_priv.h \
    src/dlg_privilege.h \
    src/http.h \
    src/irc_auth.h \
    src/irc_kernel.h \
    src/log.h \
    src/main.h \
    src/mainwindow.h \
    src/network.h \
    src/qinputline.h \
    src/qnicklist.h \
    src/tab_container.h \
    src/tab_manager.h \
    src/tab_widget.h
FORMS += ui/about.ui \
    ui/channel_favourites.ui \
    ui/channel_favourites_ad.ui \
    ui/channel_homes.ui \
    ui/channel_homes_ad.ui \
    ui/channel_list.ui \
    ui/channel_settings.ui \
    ui/friends.ui \
    ui/friends_ad.ui \
    ui/ignore.ui \
    ui/ignore_ad.ui \
    ui/moderation.ui \
    ui/options.ui \
    ui/priv.ui \
    ui/privilege.ui
