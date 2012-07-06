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

#include <QDateTime>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>

#include "avatar_client.h"
#include "avatar_list_widget.h"
#include "convert.h"
#include "core.h"
#include "nicklist.h"
#include "simple_stats_widget.h"
#include "channel_settings.h"

DlgChannelSettings::DlgChannelSettings(QWidget *parent, const QString &_strChannel) : QDialog(parent), strChannel(_strChannel)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(QString("%1 %2").arg(tr("Channel settings"), strChannel));
    // center screen
    move(QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(parent)).center()  - rect().center());

    pSimpleStatsWidget = new SimpleStatsWidget(this);
    pSimpleStatsWidget->show();
    ui.verticalLayout_stats->addWidget(pSimpleStatsWidget);

    createGui();
    setDefaultValues();
    createSignals();

    avatarClient = new AvatarClient();
    connect(avatarClient, SIGNAL(getAvatarReady(const QByteArray &,const QString &,AvatarClient::AvatarType)), this, SLOT(getAvatarReady(const QByteArray &,const QString &,AvatarClient::AvatarType)));

    refreshAll();
}

DlgChannelSettings::~DlgChannelSettings()
{
    delete avatarClient;
}

void DlgChannelSettings::createGui()
{
    ui.pushButton_transfer->setIcon(QIcon(":/images/oxygen/16x16/meeting-participant-request-response.png"));
    ui.pushButton_remove_channel->setIcon(QIcon(":/images/oxygen/16x16/meeting-participant-no-response.png"));
    ui.pushButton_set_website->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_topic->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.toolButton_bold->setIcon(QIcon(":/images/oxygen/16x16/format-text-bold.png"));
    ui.toolButton_italic->setIcon(QIcon(":/images/oxygen/16x16/format-text-italic.png"));
    ui.pushButton_set_desc->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_password->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_set_limit->setIcon(QIcon(":/images/oxygen/16x16/dialog-ok-apply.png"));
    ui.pushButton_permission_add->setIcon(QIcon(":/images/oxygen/16x16/list-add.png"));
    ui.pushButton_permission_remove->setIcon(QIcon(":/images/oxygen/16x16/list-remove.png"));
    ui.buttonBox->button(QDialogButtonBox::Close)->setIcon(QIcon(":/images/oxygen/16x16/dialog-close.png"));

    // summary
    ui.label_created1->setText(tr("Created:"));
    ui.label_website1->setText(tr("Website:"));
    ui.label_topic1->setText(tr("Topic:"));
    ui.label_description1->setText(tr("Description:"));
    ui.label_status1->setText(tr("Status:"));
    ui.label_category1->setText(tr("Category:"));
    ui.label_type1->setText(tr("Type:"));
    ui.label_guardian1->setText(tr("Guardian:"));
    ui.label_moderation1->setText(tr("Moderation:"));

    // general
    ui.label_owner->setText(tr("Owner:"));
    ui.label_website->setText(tr("Website:"));
    ui.pushButton_set_website->setText(tr("Apply"));
    ui.label_topic->setText(tr("Topic:"));
    ui.pushButton_set_topic->setText(tr("Apply"));
    ui.label_desc->setText(tr("Description:"));
    ui.pushButton_set_desc->setText(tr("Apply"));
    ui.label_status->setText(tr("Status:"));
    ui.radioButton_status_pub->setText(tr("Public"));
    ui.radioButton_status_priv->setText(tr("Private"));
    ui.label_category->setText(tr("Category:"));
    ui.comboBox_category->setItemText(0, tr("Teen"));
    ui.comboBox_category->setItemText(1, tr("Common"));
    ui.comboBox_category->setItemText(2, tr("Erotic"));
    ui.comboBox_category->setItemText(3, tr("Thematic"));
    ui.comboBox_category->setItemText(4, tr("Regional"));
    ui.label_guardian->setText(tr("Guardian:"));
    ui.radioButton_guardian_off->setText(tr("Inactive"));
    ui.radioButton_guardian_on->setText(tr("Active"));
    ui.comboBox_guardian_level->setItemText(0, tr("Level 1"));
    ui.comboBox_guardian_level->setItemText(1, tr("Level 2"));
    ui.comboBox_guardian_level->setItemText(2, tr("Level 3"));
    ui.label_moderation->setText(tr("Moderation:"));
    ui.radioButton_moderation_off->setText(tr("Off"));
    ui.radioButton_moderation_on->setText(tr("On"));

    // advanced
    ui.label_channel->setText(tr("Channel:"));
    ui.pushButton_transfer->setText(tr("Transfer"));
    ui.pushButton_remove_channel->setText(tr("Destroy"));
    ui.label_email->setText(tr("Email:"));
    ui.label_password->setText(tr("Password:"));
    ui.pushButton_set_password->setText(tr("Apply"));
    ui.label_limit->setText(tr("Limit:"));
    ui.pushButton_set_limit->setText(tr("Apply"));
    ui.label_auditorium->setText(tr("Auditorium:"));
    ui.radioButton_auditorium_off->setText(tr("Off"));
    ui.radioButton_auditorium_on->setText(tr("On"));

    // permissions
    ui.tabWidget_permissions->addTab(new QListWidget(), QIcon(":/images/op.png"), tr("Operators"));
    ui.tabWidget_permissions->addTab(new QListWidget(), QIcon(":/images/halfop.png"), tr("Half-operators"));
    ui.tabWidget_permissions->addTab(new QListWidget(), QIcon(":/images/oxygen/16x16/im-kick-user.png"), tr("Banned"));
    ui.tabWidget_permissions->addTab(new QListWidget(), QIcon(":/images/oxygen/16x16/im-user.png"), tr("Invited"));

    ui.pushButton_permission_add->setText(tr("Add"));
    ui.pushButton_permission_remove->setText(tr("Remove"));

    // avatar
    ui.label_current_avatar->setText(tr("Current avatar:"));

    // statistics
    ui.groupBox_stats->setTitle(tr("Statistics"));
    ui.label_stats_lwords->setText(tr("Average per day spoken words:"));
    ui.label_stats_lfavourites->setText(tr("Channel added in favourites:"));
    ui.label_stats_lexists_days->setText(tr("Channel exists days:"));

    // channel settings list
    QListWidgetItem *summary = new QListWidgetItem(ui.listWidget_channel_settings);
    summary->setIcon(QIcon(":/images/oxygen/16x16/view-list-details.png"));
    summary->setText(tr("Summary"));
    summary->setToolTip(tr("Summary"));

    QListWidgetItem *general = new QListWidgetItem(ui.listWidget_channel_settings);
    general->setIcon(QIcon(":/images/oxygen/16x16/preferences-other.png"));
    general->setText(tr("General"));
    general->setToolTip(tr("General"));

    QListWidgetItem *advanced = new QListWidgetItem(ui.listWidget_channel_settings);
    advanced->setIcon(QIcon(":/images/oxygen/16x16/dialog-warning.png"));
    advanced->setText(tr("Advanced"));
    advanced->setToolTip(tr("Advanced"));

    QListWidgetItem *permissions = new QListWidgetItem(ui.listWidget_channel_settings);
    permissions->setIcon(QIcon(":/images/oxygen/16x16/irc-operator.png"));
    permissions->setText(tr("Permissions"));
    permissions->setToolTip(tr("Permissions"));

    QListWidgetItem *avatar = new QListWidgetItem(ui.listWidget_channel_settings);
    avatar->setIcon(QIcon(":/images/oxygen/16x16/edit-image-face-detect.png"));
    avatar->setText(tr("Avatar"));
    avatar->setToolTip(tr("Avatar"));

    QListWidgetItem *statistics = new QListWidgetItem(ui.listWidget_channel_settings);
    statistics->setIcon(QIcon(":/images/oxygen/16x16/office-chart-bar.png"));
    statistics->setText(tr("Statistics"));
    statistics->setToolTip(tr("Statistics"));

    // set tabs disabled
    setSettingsTabsStatus(false);
}

void DlgChannelSettings::setDefaultValues()
{
    // current option
    ui.listWidget_channel_settings->setCurrentRow(0);

    // permissions
    ((QListWidget*)ui.tabWidget_permissions->widget(0))->setSortingEnabled(true);
    ((QListWidget*)ui.tabWidget_permissions->widget(1))->setSortingEnabled(true);
    ((QListWidget*)ui.tabWidget_permissions->widget(2))->setSortingEnabled(true);
    ((QListWidget*)ui.tabWidget_permissions->widget(3))->setSortingEnabled(true);

    ((QListWidget*)ui.tabWidget_permissions->widget(0))->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ((QListWidget*)ui.tabWidget_permissions->widget(1))->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ((QListWidget*)ui.tabWidget_permissions->widget(2))->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ((QListWidget*)ui.tabWidget_permissions->widget(3))->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // font
    QStringList comboBoxFont;
    comboBoxFont << "Arial" << "Times" << "Verdana" << "Tahoma" << "Courier";

    foreach (QString strFont, comboBoxFont)
        ui.comboBox_font->addItem(strFont);

    // color
    ui.comboBox_color->setToolTip(tr("Font color"));
    ui.comboBox_color->setIconSize(QSize(50,10));

    QStringList comboBoxColors;
    comboBoxColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    foreach (QString strColor, comboBoxColors)
    {
        QPixmap pixmap(50,10);
        pixmap.fill(QColor(strColor));
        ui.comboBox_color->addItem(QIcon(pixmap), "");
    }
}

void DlgChannelSettings::createSignals()
{
    connect(ui.pushButton_transfer, SIGNAL(clicked()), this, SLOT(ownerChanged()));
    connect(ui.pushButton_remove_channel, SIGNAL(clicked()), this, SLOT(removeChannelClicked()));
    connect(ui.pushButton_set_website, SIGNAL(clicked()), this, SLOT(wwwChanged()));
    connect(ui.pushButton_set_topic, SIGNAL(clicked()), this, SLOT(topicChanged()));
    connect(ui.pushButton_set_desc, SIGNAL(clicked()), this, SLOT(descChanged()));
    connect(ui.radioButton_status_pub, SIGNAL(clicked()), this, SLOT(statusPub()));
    connect(ui.radioButton_status_priv, SIGNAL(clicked()), this, SLOT(statusPriv()));
    connect(ui.comboBox_category, SIGNAL(activated(int)), this, SLOT(categoryChanged(int)));
    connect(ui.radioButton_guardian_off, SIGNAL(clicked()), this, SLOT(guardianInactive()));
    connect(ui.radioButton_guardian_on, SIGNAL(clicked()), this, SLOT(guardianActive()));
    connect(ui.comboBox_guardian_level, SIGNAL(activated(int)), this, SLOT(guardianClicked(int)));
    connect(ui.pushButton_set_password, SIGNAL(clicked()), this, SLOT(passwordChanged()));
    connect(ui.pushButton_set_limit, SIGNAL(clicked()), this, SLOT(limitChanged()));
    connect(ui.radioButton_moderation_off, SIGNAL(clicked()), this, SLOT(moderatedInactive()));
    connect(ui.radioButton_moderation_on, SIGNAL(clicked()), this, SLOT(moderatedActive()));
    connect(ui.radioButton_auditorium_off, SIGNAL(clicked()), this, SLOT(auditoriumInactive()));
    connect(ui.radioButton_auditorium_on, SIGNAL(clicked()), this, SLOT(auditoriumActive()));

    connect(ui.pushButton_permission_add, SIGNAL(clicked()), this, SLOT(buttonPermissionAdd()));
    connect(ui.pushButton_permission_remove, SIGNAL(clicked()), this, SLOT(buttonPermissionRemove()));

    connect(ui.avatarListWidget, SIGNAL(avatarSelected(const QString &)), this, SLOT(avatarSelected(const QString &)));

    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(buttonClose()));

    connect(ui.listWidget_channel_settings, SIGNAL(clicked(QModelIndex)), this, SLOT(changePage(QModelIndex)));
}

void DlgChannelSettings::changePage(QModelIndex modelIndex)
{
    int index = modelIndex.row();

    // is selectable
    if (ui.listWidget_channel_settings->item(index)->isSelected())
    {
        // stacket widget
        ui.stackedWidget_channel_settings->setCurrentIndex(index);

        // avatars
        if (index == 4 && !ui.avatarListWidget->isInitialized())
        {
            // lazy initialization
            ui.avatarListWidget->initialize(avatarClient);
        }
    }
}

void DlgChannelSettings::refreshAll()
{
    // clear
    clear();

    // set channel
    Core::instance()->strChannelSettings = strChannel;

    // get data
    Core::instance()->pNetwork->send(QString("CS INFO %1").arg(strChannel));
    Core::instance()->pNetwork->send(QString("RS INFO %1").arg(strChannel));

    // refresh
    QTimer::singleShot(200, this, SLOT(refreshChannelInfo())); // 0.2 sec
    QTimer::singleShot(200, this, SLOT(refreshChannelStats())); // 0.2 sec
}

void DlgChannelSettings::refreshChannelInfo()
{
    if (Core::instance()->bChannelSettingsInfo == false)
    {
        QTimer::singleShot(200, this, SLOT(refreshChannelInfo())); // 0.2 sec
        return;
    }

    QHashIterator<QString, QString> iSettingsInfo(Core::instance()->mChannelSettingsInfo);
    while (iSettingsInfo.hasNext())
    {
        iSettingsInfo.next();

        QString strKey = iSettingsInfo.key();
        QString strValue = iSettingsInfo.value();

        if (strKey == "auditorium")
        {
            if (strValue.toInt() == 0)
                ui.radioButton_auditorium_off->setChecked(true);
            else if (strValue.toInt() == 1)
                ui.radioButton_auditorium_on->setChecked(true);
        }
        else if (strKey == "avatar")
        {
            avatarUrl = strValue;
            refreshAvatar();
        }
        else if (strKey == "catMajor")
        {
            if (strValue.toInt() == 1) // teen
            {
                ui.label_summary_category->setText(tr("Teen"));
                ui.comboBox_category->setCurrentIndex(0);
            }
            else if (strValue.toInt() == 2) // towarzyskie
            {
                ui.label_summary_category->setText(tr("Common"));
                ui.comboBox_category->setCurrentIndex(1);
            }
            else if (strValue.toInt() == 3) // erotyczne
            {
                ui.label_summary_category->setText(tr("Erotic"));
                ui.comboBox_category->setCurrentIndex(2);
            }
            else if (strValue.toInt() == 4) // tematyczne
            {
                ui.label_summary_category->setText(tr("Thematic"));
                ui.comboBox_category->setCurrentIndex(3);
            }
            else if (strValue.toInt() == 5) // regionalne
            {
                ui.label_summary_category->setText(tr("Regional"));
                ui.comboBox_category->setCurrentIndex(4);
            }
            else
                ui.comboBox_category->setCurrentIndex(-1);
        }
        else if (strKey == "createdDate")
        {
            // label created date/time
            QDateTime dt = QDateTime::fromTime_t(strValue.toInt());
            QString strDT = dt.toString("dd MMM yyyy hh:mm:ss");
            ui.label_summary_datetime->setText(strDT);

            // stats
            int iCreatedDate = strValue.toInt();
            int iCurrentDate = QDateTime::currentDateTime().toTime_t();

            int iSeconds = iCurrentDate - iCreatedDate;
            int iMinutes = iSeconds / 60;
            int iHours = iMinutes / 60;
            int iDays = iHours / 24;

            ui.label_stats_exists_days->setText(QString::number(iDays));
        }
        else if (strKey == "email")
        {
            ui.lineEdit_email->setText(strValue);
        }
        else if (strKey == "guardian")
        {
            if (strValue.toInt() == 0)
            {
                ui.label_summary_guardian->setText(tr("Inactive"));
                ui.radioButton_guardian_off->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(-1);
            }
            else if (strValue.toInt() == 1)
            {
                ui.label_summary_guardian->setText(QString("%1, %2").arg(tr("Active"), tr("Level 1")));
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(0);
            }
            else if (strValue.toInt() == 2)
            {
                ui.label_summary_guardian->setText(QString("%1, %2").arg(tr("Active"), tr("Level 2")));
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(1);
            }
            else if (strValue.toInt() == 3)
            {
                ui.label_summary_guardian->setText(QString("%1, %2").arg(tr("Active"), tr("Level 3")));
                ui.radioButton_guardian_on->setChecked(true);
                ui.comboBox_guardian_level->setCurrentIndex(2);
            }
        }
        else if (strKey == "limit")
        {
            ui.spinBox_limit->setValue(strValue.toInt());
        }
        else if (strKey == "moderated")
        {
            if (strValue.toInt() == 0)
            {
                ui.label_summary_moderation->setText(tr("Off"));
                ui.radioButton_moderation_off->setChecked(true);
            }
            else if (strValue.toInt() == 1)
            {
                ui.label_summary_moderation->setText(tr("On"));
                ui.radioButton_moderation_on->setChecked(true);
            }
        }
        else if (strKey == "password")
        {
            ui.lineEdit_password->setText(strValue);
        }
        else if (strKey == "private")
        {
            if (strValue.toInt() == 1)
            {
                ui.label_summary_status->setText(tr("Private"));
                ui.radioButton_status_priv->setChecked(true);
            }
            else if (strValue.toInt() == 0)
            {
                ui.label_summary_status->setText(tr("Public"));
                ui.radioButton_status_pub->setChecked(true);
            }
        }
        else if (strKey == "type")
        {
            if (strValue.toInt() == 0)
                ui.label_summary_type->setText(tr("Wild"));
            else if (strValue.toInt() == 1)
                ui.label_summary_type->setText(tr("Tame"));
            else if (strValue.toInt() == 2)
                ui.label_summary_type->setText(tr("With class"));
            else if (strValue.toInt() == 3)
                ui.label_summary_type->setText(tr("Cult"));
        }
        else if (strKey == "www")
        {
            if (!strValue.isEmpty())
                ui.label_summary_website->setText(QString("<a href=\"%1\">%1</a>").arg(strValue));
            ui.lineEdit_website->setText(strValue);
        }
        else if (strKey == "desc")
        {
            if (!strValue.isEmpty())
            {
                Convert::simpleConvert(strValue);
                ui.plainTextEdit_summary_desc->setPlainText(strValue);
            }

            // set description
            ui.plainTextEdit_desc->setPlainText(strValue);
        }
        else if (strKey == "topic")
        {
            // convert font
            bool bBold = Convert::isBold(strValue);
            bool bItalic = Convert::isItalic(strValue);
            QString strFont = Convert::getFont(strValue);
            int iColor = Convert::getColor(strValue);

            if (bBold) ui.toolButton_bold->setChecked(true);
            else if (bItalic) ui.toolButton_italic->setChecked(true);

            if (strFont == "arial") ui.comboBox_font->setCurrentIndex(0);
            else if (strFont == "times") ui.comboBox_font->setCurrentIndex(1);
            else if (strFont == "verdana") ui.comboBox_font->setCurrentIndex(2);
            else if (strFont == "tahoma") ui.comboBox_font->setCurrentIndex(3);
            else if (strFont == "courier") ui.comboBox_font->setCurrentIndex(4);
            else ui.comboBox_font->setCurrentIndex(2);

            ui.comboBox_color->setCurrentIndex(iColor);

            // default #000000
            if (ui.comboBox_color->currentIndex() == -1)
                ui.comboBox_color->setCurrentIndex(0);

            // default verdana
            if (ui.comboBox_font->currentIndex() == -1)
                ui.comboBox_font->setCurrentIndex(2);

            // convert emoticons
            Convert::simpleConvert(strValue);

            // set summary topic
            if (!strValue.isEmpty())
                ui.plainTextEdit_summary_topic->setPlainText(strValue);

            // set topic
            ui.plainTextEdit_topic->setPlainText(strValue);
        }
    }

    // permissions
    refreshPermissionList();
}

void DlgChannelSettings::refreshChannelStats()
{
    if (Core::instance()->bChannelSettingsStats == false)
    {
        QTimer::singleShot(200, this, SLOT(refreshChannelStats())); // 0.2 sec
        return;
    }

    QHashIterator<QString, QString> i(Core::instance()->mChannelSettingsStats);
    while (i.hasNext())
    {
        i.next();

        QString strKey = i.key();
        QString strValue = i.value();

        if (strKey == "histWords")
        {
            QStringList lWords = strValue.split(",");
            QList<int> lIWords;

            int iWords = 0;
            for (int i = 0; i < lWords.size(); i++)
            {
                bool ok;
                int iWord = lWords.at(i).toInt(&ok, 16);

                // add to list
                lIWords.append(iWord);

                // add to average words
                iWords += iWord;
            }

            // average words
            iWords /= lWords.size();
            ui.label_stats_words->setText(QString::number(iWords));

            // simple stats widget
            pSimpleStatsWidget->setStats(lIWords);
        }
        else if (strKey == "relationsFavourite")
            ui.label_stats_favourites->setText(strValue);
    }
}

void DlgChannelSettings::setSettingsTabsStatus(bool b)
{
    Qt::ItemFlags flags;
    if (b) flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    else flags = Qt::NoItemFlags;

    ui.listWidget_channel_settings->item(1)->setFlags(flags);
    ui.listWidget_channel_settings->item(2)->setFlags(flags);
    ui.listWidget_channel_settings->item(3)->setFlags(flags);
    ui.listWidget_channel_settings->item(4)->setFlags(flags);
}

void DlgChannelSettings::setOwner(const QString &strNick)
{
    ui.label_owner->show();
    ui.label_owner_nick->show();

    ui.label_owner_nick->setText(strNick);
}

void DlgChannelSettings::addOp(const QString &strNick)
{
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setData(Qt::UserRole+10, false); // is nicklist
    item->setText(strNick);

    ((QListWidget*)ui.tabWidget_permissions->widget(0))->addItem(item);
}

void DlgChannelSettings::addHalfop(const QString &strNick)
{
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setData(Qt::UserRole+10, false); // is nicklist
    item->setText(strNick);

    ((QListWidget*)ui.tabWidget_permissions->widget(1))->addItem(item);
}

void DlgChannelSettings::addBan(const QString &strNick, const QString &strWho, const QString &strDT, const QString &strIPNick)
{
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setData(Qt::UserRole+10, false); // is nicklist
    if (strIPNick.isEmpty())
    {
        item->setText(strNick);
        item->setToolTip(QString("%1: %2 (%4)").arg(tr("Created by"), strWho, strDT));
    }
    else
    {

        item->setText(strIPNick);
        item->setTextColor(QColor("#ff0000")); // set color
        item->setData(Qt::UserRole, strNick); // set original ban mask
        QString strFixedNick = strNick; strFixedNick.remove("*!*@");
        QString strIPHint = tr("IP Mask: %1").arg(strFixedNick);
        item->setToolTip(QString("%1: %2 (%3) [%4]").arg(tr("Created by"), strWho, strDT, strIPHint));
    }

    ((QListWidget*)ui.tabWidget_permissions->widget(2))->addItem(item);
}

void DlgChannelSettings::addInvite(const QString &strNick, const QString &strWho, const QString &strDT)
{
    SortedListWidgetItem *item = new SortedListWidgetItem();
    item->setData(Qt::UserRole+10, false); // is nicklist
    item->setText(strNick);
    item->setToolTip(QString("%1: %2 (%3)").arg(tr("Created by"), strWho, strDT));

    ((QListWidget*)ui.tabWidget_permissions->widget(3))->addItem(item);
}

void DlgChannelSettings::ownerChanged()
{
    QString strMsg = "<p style=\"font-weight:bold;\">"+tr("The owner of the channel can be only one!")+"</p><p>"+tr("Enter the nickname of the new owner:")+"</p>";
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing privileges"), strMsg, QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
        Core::instance()->pNetwork->send(QString("CS TRANSFER %1 %2").arg(strChannel, strText));

    refreshAll();
}

void DlgChannelSettings::removeChannelClicked()
{
    QString strMsg = "<p style=\"font-weight:bold;\">"+tr("The removal of the channel operation is irreversible!")+"</p>";
    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing privileges"), strMsg, QLineEdit::Normal, strChannel, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
        Core::instance()->pNetwork->send(QString("CS DROP %1").arg(strText));

    buttonClose();
}

void DlgChannelSettings::wwwChanged()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 WWW %2").arg(strChannel, ui.lineEdit_website->text()));

    refreshAll();
}

void DlgChannelSettings::topicChanged()
{
    QString strTopic = ui.plainTextEdit_topic->toPlainText();
    strTopic.remove(QRegExp("(\r|\n)"));
    Convert::simpleReverseConvert(strTopic);

    bool bBold = false;
    bool bItalic = false;
    QString strFontName;
    QString strFontColor;
    QString strFontWeight;

    // bold
    if (ui.toolButton_bold->isChecked()) bBold = true;

    // italic
    if (ui.toolButton_italic->isChecked()) bItalic = true;

    // font name
    strFontName = ui.comboBox_font->currentText().toLower();

    // font color
    QStringList lFontColors;
    lFontColors << "#000000" << "#623c00" << "#c86c00" << "#ff6500" << "#ff0000" << "#e40f0f" << "#990033" << "#8800ab" << "#ce00ff" << "#0f2ab1" << "#3030ce" << "#006699" << "#1a866e" << "#008100" << "#959595";

    if (ui.comboBox_color->currentIndex() != -1)
        strFontColor = lFontColors.at(ui.comboBox_color->currentIndex());

    // set topic
    if (bBold) strFontWeight += "b";
    if (bItalic) strFontWeight += "i";

    if (strFontName == "verdana")
        strFontName = QString::null;
    if ((strFontColor != "#000000") && (!strFontColor.isEmpty()))
        strTopic = "%C"+strFontColor.right(6)+"%"+strTopic;
    if (!strFontName.isEmpty())
        strFontName = ":"+strFontName;
    if ((!strFontWeight.isEmpty()) || (!strFontName.isEmpty()))
        strTopic = "%F"+strFontWeight+strFontName+"%"+strTopic;

    Core::instance()->pNetwork->send(QString("CS SET %1 TOPIC %2").arg(strChannel, strTopic));

    refreshAll();
}

void DlgChannelSettings::descChanged()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 LONGDESC %2").arg(strChannel, ui.plainTextEdit_desc->toPlainText().replace(QRegExp("(\r|\n)"), "")));

    refreshAll();
}

void DlgChannelSettings::statusPub()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 PRIVATE OFF").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::statusPriv()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 PRIVATE ON").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::categoryChanged(int index)
{
    QString strCatMajor = QString::number(index+1);
    Core::instance()->pNetwork->send(QString("CS SET %1 CATMAJOR %2").arg(strChannel, strCatMajor));

    refreshAll();
}

void DlgChannelSettings::guardianInactive()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 GUARDIAN 0").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::guardianActive()
{
    if (ui.comboBox_guardian_level->currentIndex() != -1)
    {
        QString strLevel = QString::number(ui.comboBox_guardian_level->currentIndex());
        Core::instance()->pNetwork->send(QString("CS SET %1 GUARDIAN %2").arg(strChannel, strLevel));
    }
    else
        Core::instance()->pNetwork->send(QString("CS SET %1 GUARDIAN 1").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::guardianClicked(int iLevel)
{
    if (ui.radioButton_guardian_on->isChecked())
    {
        QString strLevel = QString::number(iLevel+1);
        Core::instance()->pNetwork->send(QString("CS SET %1 GUARDIAN %2").arg(strChannel, strLevel));
    }

    refreshAll();
}

void DlgChannelSettings::passwordChanged()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 PASSWORD %2").arg(strChannel, ui.lineEdit_password->text()));

    refreshAll();
}

void DlgChannelSettings::limitChanged()
{
    QString strLimit = QString::number(ui.spinBox_limit->value());
    Core::instance()->pNetwork->send(QString("CS SET %1 LIMIT %2").arg(strChannel, strLimit));

    refreshAll();
}

void DlgChannelSettings::moderatedInactive()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 MODERATED OFF").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::moderatedActive()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 MODERATED ON").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::auditoriumInactive()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 AUDITORIUM OFF").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::auditoriumActive()
{
    Core::instance()->pNetwork->send(QString("CS SET %1 AUDITORIUM ON").arg(strChannel));

    refreshAll();
}

void DlgChannelSettings::buttonPermissionAdd()
{
    int index = ui.tabWidget_permissions->currentIndex();
    QString strNick;

    QString strLabel;
    if (index == 0)
        strLabel = tr("Add the super-operator:");
    else if (index == 1)
        strLabel = tr("Add operator:");
    else if (index == 2)
        strLabel = tr("Add ban:");
    else if (index == 3)
        strLabel = tr("Add invitation:");

    bool ok;
    QString strText = QInputDialog::getText(this, tr("Changing privileges"), strLabel, QLineEdit::Normal, QString::null, &ok);
    strText = strText.trimmed();

    if ((ok) && (!strText.isEmpty()))
    {
        if ((index == 2) && (strText.contains("*")))
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowIcon(QIcon(":/images/logo16x16.png"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setText(tr("This ban will ban large numbers of people. Are you sure you want to use this ban?"));
            int iResult = msgBox.exec();

            if (iResult == QMessageBox::Ok)
                strNick = strText;
        }
        else
            strNick = strText;
    }

    // if empty
    if (strNick.isEmpty())
        return;

    // add permission
    if (index == 0)
        Core::instance()->pNetwork->send(QString("CS OP %1 ADD %2").arg(strChannel, strNick));
    else if (index == 1)
        Core::instance()->pNetwork->send(QString("CS HALFOP %1 ADD %2").arg(strChannel, strNick));
    else if (index == 2)
        Core::instance()->pNetwork->send(QString("CS BAN %1 ADD %2").arg(strChannel, strNick));
    else if (index == 3)
        Core::instance()->pNetwork->send(QString("CS INVITE %1 ADD %2").arg(strChannel, strNick));

    // refresh
    refreshAll();
}

void DlgChannelSettings::buttonPermissionRemove()
{
    int index = ui.tabWidget_permissions->currentIndex();
    QList<QListWidgetItem*> lRemoveNicks;

    if (index == 0)
    {
        if (!((QListWidget*)ui.tabWidget_permissions->widget(0))->selectedItems().isEmpty())
            lRemoveNicks = ((QListWidget*)ui.tabWidget_permissions->widget(0))->selectedItems();
    }
    else if (index == 1)
    {
        if (!((QListWidget*)ui.tabWidget_permissions->widget(1))->selectedItems().isEmpty())
            lRemoveNicks = ((QListWidget*)ui.tabWidget_permissions->widget(1))->selectedItems();
    }
    else if (index == 2)
    {
        if (!((QListWidget*)ui.tabWidget_permissions->widget(2))->selectedItems().isEmpty())
            lRemoveNicks = ((QListWidget*)ui.tabWidget_permissions->widget(2))->selectedItems();
    }
    else if (index == 3)
    {
        if (!((QListWidget*)ui.tabWidget_permissions->widget(3))->selectedItems().isEmpty())
            lRemoveNicks = ((QListWidget*)ui.tabWidget_permissions->widget(3))->selectedItems();
    }

    // get nick if empty
    if (lRemoveNicks.size() == 0)
    {
        QString strLabel;
        if (index == 0)
            strLabel = tr("Remove the super-operator:");
        else if (index == 1)
            strLabel = tr("Remove operator:");
        else if (index == 2)
            strLabel = tr("Remove ban:");
        else if (index == 3)
            strLabel = tr("Delete invitation:");

        bool ok;
        QString strText = QInputDialog::getText(this, tr("Changing privileges"), strLabel, QLineEdit::Normal, QString::null, &ok);
        strText = strText.trimmed();

        if ((ok) && (!strText.isEmpty()))
            lRemoveNicks.append(new QListWidgetItem(strText));
    }

    // if empty
    if (lRemoveNicks.size() == 0)
        return;

    // remove permission
    if (index == 0)
    {
        foreach (QListWidgetItem *removeNick, lRemoveNicks)
            Core::instance()->pNetwork->send(QString("CS OP %1 DEL %2").arg(strChannel, removeNick->text()));
    }
    else if (index == 1)
    {
        foreach (QListWidgetItem *removeNick, lRemoveNicks)
            Core::instance()->pNetwork->send(QString("CS HALFOP %1 DEL %2").arg(strChannel, removeNick->text()));
    }
    else if (index == 2)
    {
        foreach (QListWidgetItem *removeNick, lRemoveNicks)
        {
            if (!removeNick->data(Qt::UserRole).isNull())
                Core::instance()->pNetwork->send(QString("CS BANIP %1 DEL %2").arg(strChannel, removeNick->text()));
            else
                Core::instance()->pNetwork->send(QString("CS BAN %1 DEL %2").arg(strChannel, removeNick->text()));
        }
    }
    else if (index == 3)
    {
        foreach (QListWidgetItem *removeNick, lRemoveNicks)
            Core::instance()->pNetwork->send(QString("CS INVITE %1 DEL %2").arg(strChannel, removeNick->text()));
    }

    // refresh
    refreshAll();
}

void DlgChannelSettings::refreshPermissionList()
{
    ((QListWidget*)ui.tabWidget_permissions->widget(0))->clear();
    ((QListWidget*)ui.tabWidget_permissions->widget(1))->clear();
    ((QListWidget*)ui.tabWidget_permissions->widget(2))->clear();
    ((QListWidget*)ui.tabWidget_permissions->widget(3))->clear();

    // enable tabs
    QString strMe = Core::instance()->settings.value("nick");
    QString strSelfModes = Nicklist::instance()->getUserModes(strMe, strChannel);
    int iSelfMaxModes = Nicklist::instance()->getUserMaxModes(strSelfModes);

    if (Core::instance()->mChannelSettingsPermissions.contains("q", strMe) || Core::instance()->mChannelSettingsPermissions.contains("o", strMe) || Core::instance()->mChannelSettingsPermissions.contains("h", strMe) || (iSelfMaxModes > 16))
        setSettingsTabsStatus(true);
    else
    {
        ui.stackedWidget_channel_settings->setCurrentIndex(0);
        ui.listWidget_channel_settings->setCurrentRow(0);
        setSettingsTabsStatus(false);
    }

    // refresh permissions
    QHashIterator<QString, QString> iSettingsPermissions(Core::instance()->mChannelSettingsPermissions);
    while (iSettingsPermissions.hasNext())
    {
        iSettingsPermissions.next();

        QString strKey = iSettingsPermissions.key();
        QString strValue = iSettingsPermissions.value();

        if (strKey == "q")
            setOwner(strValue);
        else if (strKey == "o")
            addOp(strValue);
        else if (strKey == "h")
            addHalfop(strValue);
        else if (strKey == "b")
        {
            QStringList strDataList = strValue.split(";");
            QString strNick = strDataList[0];
            QString strWho = strDataList[1];
            QString strDT = strDataList[2];
            QString strIPNick = strDataList[3];

            addBan(strNick, strWho, strDT, strIPNick);
        }
        else if (strKey == "I")
        {
            QStringList strDataList = strValue.split(";");
            QString strNick = strDataList[0];
            QString strWho = strDataList[1];
            QString strDT = strDataList[2];
            QString strIPNick = strDataList[3];
            Q_UNUSED (strIPNick)

            addInvite(strNick, strWho, strDT);
        }
    }
}

void DlgChannelSettings::clear()
{
    Core::instance()->strChannelSettings.clear();
    Core::instance()->mChannelSettingsInfo.clear();
    Core::instance()->mChannelSettingsPermissions.clear();
    Core::instance()->bChannelSettingsInfo = false;
    Core::instance()->mChannelSettingsStats.clear();
    Core::instance()->bChannelSettingsStats = false;

    // summary
    ui.label_summary_datetime->setText("-");
    ui.label_summary_website->setText("-");
    ui.plainTextEdit_summary_topic->setPlainText("-");
    ui.plainTextEdit_summary_desc->setPlainText("-");
    ui.label_summary_status->setText("-");
    ui.label_summary_category->setText("-");
    ui.label_summary_type->setText("-");
    ui.label_summary_guardian->setText("-");
    ui.label_summary_moderation->setText("-");

    // general
    ui.label_owner_nick->clear();
    ui.lineEdit_website->clear();
    ui.plainTextEdit_topic->clear();
    ui.toolButton_bold->setChecked(false);
    ui.toolButton_italic->setChecked(false);
    ui.comboBox_font->setCurrentIndex(-1);
    ui.comboBox_color->setCurrentIndex(-1);
    ui.plainTextEdit_desc->clear();
    ui.radioButton_status_pub->setChecked(false);
    ui.radioButton_status_priv->setChecked(false);
    ui.comboBox_category->setCurrentIndex(-1);
    ui.comboBox_guardian_level->setCurrentIndex(-1);
    ui.radioButton_guardian_off->setChecked(false);
    ui.radioButton_guardian_on->setChecked(false);
    ui.radioButton_moderation_off->setChecked(false);
    ui.radioButton_moderation_on->setChecked(false);

    // advanced
    ui.lineEdit_email->clear();
    ui.lineEdit_password->clear();
    ui.spinBox_limit->setValue(0);
    ui.radioButton_auditorium_off->setChecked(false);
    ui.radioButton_auditorium_on->setChecked(false);

    // permissions
    ((QListWidget*)ui.tabWidget_permissions->widget(0))->clear();
    ((QListWidget*)ui.tabWidget_permissions->widget(1))->clear();
    ((QListWidget*)ui.tabWidget_permissions->widget(2))->clear();
    ((QListWidget*)ui.tabWidget_permissions->widget(3))->clear();

    // statistics
    ui.label_stats_favourites->setText("-");
    ui.label_stats_words->setText("-");
    ui.label_stats_exists_days->setText("-");
}

void DlgChannelSettings::buttonClose()
{
    Core::instance()->strChannelSettings.clear();
    Core::instance()->mChannelSettingsInfo.clear();
    Core::instance()->mChannelSettingsPermissions.clear();
    Core::instance()->bChannelSettingsInfo = false;
    Core::instance()->mChannelSettingsStats.clear();
    Core::instance()->bChannelSettingsStats = false;

    close();
}

void DlgChannelSettings::getAvatarReady(const QByteArray &content, const QString &avatarUrl, AvatarClient::AvatarType type)
{
    QPixmap pixmap;
    if (!pixmap.loadFromData(content))
    {
        qDebug() << "Unable to load image from " << avatarUrl;
        return;
    }

    if (type == AvatarClient::AT_other)
        ui.label_channel_avatar->setPixmap(pixmap);
}

void DlgChannelSettings::refreshAvatar()
{
    if (!avatarUrl.isEmpty())
    {
        avatarClient->requestGetAvatar(avatarUrl, AvatarClient::AT_other);
        return;
    }

    ui.label_channel_avatar->setText(tr("No photo available"));
}

void DlgChannelSettings::avatarSelected(const QString &avatarUrl)
{
    Core::instance()->pNetwork->send(QString("CS SET %1 AVATAR %2").arg(strChannel, avatarUrl));

    refreshAll();
}
