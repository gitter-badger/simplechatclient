/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012-2013 Piotr Łuczko <piotr.luczko@gmail.com>
 *   Copyright (C) 2012 Dariusz Markowicz <dmarkowicz@alari.pl>
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

#include <QDesktopWidget>
#include <QPushButton>
#include <QTimer>

#include "avatar_client.h"
#include "avatar_list_widget.h"
#include "my_profile.h"
#include "core.h"
#include "settings.h"

#include "my_avatar_gui.h"

MyAvatarGui::MyAvatarGui(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("My avatar"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center() - rect().center());

    createGui();
    setDefaultValues();
    createSignals();

    avatarClient = new AvatarClient();
    ui.avatarListWidget->initialize(avatarClient);

    connect(avatarClient, SIGNAL(getAvatarReady(const QByteArray &,const QString &,AvatarClient::AvatarType)), this, SLOT(getAvatarReady(const QByteArray &,const QString &,AvatarClient::AvatarType)));

    refreshAvatar();
}

MyAvatarGui::~MyAvatarGui()
{
    delete avatarClient;
}

void MyAvatarGui::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));
}

void MyAvatarGui::setDefaultValues()
{
    ui.label_current_avatar->setText(tr("Current avatar:"));
}

void MyAvatarGui::createSignals()
{
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui.avatarListWidget, SIGNAL(avatarSelected(const QString &)), this, SLOT(avatarSelected(const QString &)));
}

void MyAvatarGui::getAvatarReady(const QByteArray &content, const QString &avatarUrl, AvatarClient::AvatarType type)
{
    QPixmap pixmap;
    if (!pixmap.loadFromData(content))
    {
        if (Settings::instance()->get("debug") == "true")
            qWarning() << "Unable to load image from " << avatarUrl;
        return;
    }

    if (type == AvatarClient::AT_other)
        ui.label_my_avatar->setPixmap(pixmap);
}

void MyAvatarGui::refreshAvatar()
{
    QString avatarUrl = MyProfile::instance()->get("avatar");

    if (!avatarUrl.isEmpty())
        avatarClient->requestGetAvatar(avatarUrl, AvatarClient::AT_other);
    else
        ui.label_my_avatar->setText(tr("No photo available"));
}

void MyAvatarGui::avatarSelected(const QString &avatarUrl)
{
    Core::instance()->network->send(QString("NS SET AVATAR %1").arg(avatarUrl));
    // FIXME: replace by chat event
    QTimer::singleShot(1000*4, this, SLOT(refreshAvatar())); // 4 sec
}

