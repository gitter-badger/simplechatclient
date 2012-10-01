/*
 * Simple Chat Client
 *
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
#include <QDebug>
#include <QGraphicsPixmapItem>

#include "avatar_client.h"
#include "avatar_edit.h"
#include "avatar_edit_scene.h"
#include "my_avatar_model.h"

#include "avatar_edit.h"

DlgAvatarEdit::DlgAvatarEdit(QWidget *parent, MyAvatarModel avatar, AvatarClient *avatarClient) :
    QDialog(parent), avatar(avatar), avatarClient(avatarClient)
{
    //avatar.debug("DlgAvatarEdit");

    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Edit Avatar"));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    Q_ASSERT(avatarClient);

    angle = avatar.angle();

    createGui();
    setDefaultValues();
    createSignals();

    connect(avatarClient, SIGNAL(getAvatarReady(const QByteArray &,const QString &,AvatarClient::AvatarType)), this, SLOT(getAvatarReady(const QByteArray &,const QString &,AvatarClient::AvatarType)));
    connect(editScene, SIGNAL(cropChanged(const QRect &)), this, SLOT(cropChanged(const QRect &)));

    avatarClient->requestGetAvatar(avatar.getRawUrl(), AvatarClient::AT_myRaw);
}

DlgAvatarEdit::~DlgAvatarEdit()
{
    delete editScene;
    delete previewScene;
}

void DlgAvatarEdit::createGui()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok.png"));
    ui.buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/oxygen/16x16/dialog-cancel.png"));
    ui.toolButton_rotate_left->setIcon(QIcon(":/images/oxygen/16x16/object_rotate_left.png"));
    ui.toolButton_rotate_right->setIcon(QIcon(":/images/oxygen/16x16/object_rotate_right.png"));

    ui.label_fileName_title->setText(tr("Filename:"));
    ui.label_size_title->setText(tr("Size:"));

    editScene = new AvatarEditScene(this);
    ui.graphicsView_edit->setScene(editScene);
    previewScene = new QGraphicsScene(this);
    cropItem = previewScene->addPixmap(QPixmap());
    ui.graphicsView_preview->setScene(previewScene);
}

void DlgAvatarEdit::setDefaultValues()
{
    ui.label_fileName->setText(avatar.desc());
    ui.label_size->setText(QString("%1x%2 px").arg(QString::number(avatar.width()), QString::number(avatar.height())));
}

void DlgAvatarEdit::createSignals()
{
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(okClicked()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.toolButton_rotate_left, SIGNAL(clicked()), this, SLOT(rotateLeftClicked()));
    connect(ui.toolButton_rotate_right, SIGNAL(clicked()), this, SLOT(rotateRightClicked()));
}

void DlgAvatarEdit::rotateLeftClicked()
{
    avatar.setCrop(MyAvatarModel::scaledCropToString(crop, photo.size(), avatar.size(), avatar.angle(), true));
    avatar.rotateLeft();
    avatarClient->requestGetAvatar(avatar.getRawUrl(), AvatarClient::AT_myRaw);
}

void DlgAvatarEdit::rotateRightClicked()
{
    avatar.setCrop(MyAvatarModel::scaledCropToString(crop, photo.size(), avatar.size(), avatar.angle(), true));
    avatar.rotateRight();
    avatarClient->requestGetAvatar(avatar.getRawUrl(), AvatarClient::AT_myRaw);
}

void DlgAvatarEdit::getAvatarReady(const QByteArray &content, const QString &avatarUrl, AvatarClient::AvatarType type)
{
    if (type != AvatarClient::AT_myRaw)
        return;

    if (!photo.loadFromData(content))
    {
        qWarning() << "Unable to load image from " << avatarUrl;
        return;
    }

    if (avatar.crop().isEmpty())
    {
        int cropSize = qMin(photo.width(), photo.height());
        crop.setRect(0, 0, cropSize, cropSize);
    }
    else
    {
        crop = MyAvatarModel::stringToScaledCrop(avatar.crop(), photo.size(), avatar.size(), avatar.angle(), angle != avatar.angle());
    }
    angle = avatar.angle();

    editScene->setPhoto(photo, crop);
    previewScene->setSceneRect(photo.rect());
    cropItem->setPixmap(photo);
    ui.graphicsView_preview->fitInView(crop);
}

void DlgAvatarEdit::cropChanged(const QRect &crop)
{
    this->crop = crop;
    ui.graphicsView_preview->fitInView(crop);
}

MyAvatarModel DlgAvatarEdit::getAvatar() const
{
    return avatar;
}

void DlgAvatarEdit::okClicked()
{
    avatar.setCrop(MyAvatarModel::scaledCropToString(crop, photo.size(), avatar.size(), avatar.angle()));
    accept();
}

