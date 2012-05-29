/*
 * Simple Chat Client
 *
 *   Copyright (C) 2012 Piotr Łuczko <piotr.luczko@gmail.com>
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
#include "core.h"

#include "my_avatar.h"

DlgMyAvatar::DlgMyAvatar(QWidget *parent) : QDialog(parent)
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

DlgMyAvatar::~DlgMyAvatar()
{
    delete avatarClient;
}

void DlgMyAvatar::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));
}

void DlgMyAvatar::setDefaultValues()
{
    // nick
    QString me = Core::instance()->settings.value("nick");
    ui.label_my_nick->setText(me);
}

void DlgMyAvatar::createSignals()
{
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui.avatarListWidget, SIGNAL(avatarSelected(const QString &)), this, SLOT(avatarSelected(const QString &)));
}

void DlgMyAvatar::getAvatarReady(const QByteArray &content, const QString &avatarUrl, AvatarClient::AvatarType type)
{
    QPixmap pixmap;
    if (!pixmap.loadFromData(content))
    {
        qDebug() << "Unable to load image from " << avatarUrl;
        return;
    }

    if (type == AvatarClient::AT_other)
        ui.label_my_avatar->setPixmap(pixmap);
}

void DlgMyAvatar::refreshAvatar()
{
    if (Core::instance()->mMyProfile.contains("avatar"))
    {
        QString avatarUrl = Core::instance()->mMyProfile.value("avatar");
        if (!avatarUrl.isEmpty())
        {
            avatarClient->requestGetAvatar(avatarUrl, AvatarClient::AT_other);
            return;
        }
    }

    ui.label_my_avatar->setText(tr("No photo available"));
}

void DlgMyAvatar::avatarSelected(const QString &avatarUrl)
{
    Core::instance()->pNetwork->send(QString("NS SET AVATAR %1").arg(avatarUrl));
    // FIXME: replace by chat event
    QTimer::singleShot(1000*4, this, SLOT(refreshAvatar())); // 4 sec
}

